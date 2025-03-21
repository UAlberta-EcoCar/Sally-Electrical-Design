/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include "ecocar_can.h"
#include "i2c.h"
//#include "usb_device.h"  // Add this line to include the USB device header
//#include "usbd_cdc_if.h" // Add this line to include the USB device header
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t adc1_results[5] = { 0 };
uint32_t adc2_results[4] = { 0 };
const float RESOLUTION = 4096.0;
const float ref_voltage = 3.3;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 512 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for canReceive */
osThreadId_t canReceiveHandle;
uint32_t canReceiveBuffer[ 512 ];
osStaticThreadDef_t canReceiveControlBlock;
const osThreadAttr_t canReceive_attributes = {
  .name = "canReceive",
  .stack_mem = &canReceiveBuffer[0],
  .stack_size = sizeof(canReceiveBuffer),
  .cb_mem = &canReceiveControlBlock,
  .cb_size = sizeof(canReceiveControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for canSend */
osThreadId_t canSendHandle;
uint32_t canSendBuffer[ 512 ];
osStaticThreadDef_t canSendControlBlock;
const osThreadAttr_t canSend_attributes = {
  .name = "canSend",
  .stack_mem = &canSendBuffer[0],
  .stack_size = sizeof(canSendBuffer),
  .cb_mem = &canSendControlBlock,
  .cb_size = sizeof(canSendControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for canQueRxHeader */
osMessageQueueId_t canQueRxHeaderHandle;
uint8_t canQueRXHeaderBuffer[ 512 * sizeof( uint32_t ) ];
osStaticMessageQDef_t canQueRXHeaderControlBlock;
const osMessageQueueAttr_t canQueRxHeader_attributes = {
  .name = "canQueRxHeader",
  .cb_mem = &canQueRXHeaderControlBlock,
  .cb_size = sizeof(canQueRXHeaderControlBlock),
  .mq_mem = &canQueRXHeaderBuffer,
  .mq_size = sizeof(canQueRXHeaderBuffer)
};
/* Definitions for canQueRxData */
osMessageQueueId_t canQueRxDataHandle;
uint8_t canQueRXDataBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canQueRXDataControlBlock;
const osMessageQueueAttr_t canQueRxData_attributes = {
  .name = "canQueRxData",
  .cb_mem = &canQueRXDataControlBlock,
  .cb_size = sizeof(canQueRXDataControlBlock),
  .mq_mem = &canQueRXDataBuffer,
  .mq_size = sizeof(canQueRXDataBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[64];
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retrieve Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData)
				!= HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}
		if (osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0, 0)
				!= osOK
				|| osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.DataLength,
						0, 0) != osOK) {
			Error_Handler();
		}
		for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
			if (osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0)
					!= osOK) {
				Error_Handler();
			}
		}
	}
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskReceive(void *argument);
void StartTaskSend(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of canQueRxHeader */
  canQueRxHeaderHandle = osMessageQueueNew (512, sizeof(uint32_t), &canQueRxHeader_attributes);

  /* creation of canQueRxData */
  canQueRxDataHandle = osMessageQueueNew (512, sizeof(uint8_t), &canQueRxData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canReceive */
  canReceiveHandle = osThreadNew(StartTaskReceive, NULL, &canReceive_attributes);

  /* creation of canSend */
  canSendHandle = osThreadNew(StartTaskSend, NULL, &canSend_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_Device */
  MX_USB_Device_Init();
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */

	// Calibrate and start the leds

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, &adc1_results, 5);

	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc2, &adc2_results, 4);

	// Init the screen
	ssd1306_Init();

	for (;;) {

		float Buc_temp_sense_Voltage = ((((adc1_results[0] * ref_voltage) / RESOLUTION)) - 0.193);  //buc voltage
		float Op_temp_sense_Voltage = ((((adc1_results[1] * ref_voltage) / RESOLUTION)) - 0.13); //op voltage
		float Cntrl_temp_sense_Voltage = ((((adc1_results[2] * ref_voltage) / RESOLUTION)) + 0.26); //cntrl voltage
		float Battery_Input_Voltage = adc1_results[3] * ref_voltage / RESOLUTION / 0.0909 ; // VOltage divider: R2 = 4.7k R1 = 47K
		float Battery_Input_Current = (adc2_results[3] * ref_voltage / RESOLUTION - 2.5) / 0.400;
		float Volt_Output_12V = (adc2_results[2] * ref_voltage / RESOLUTION) / 0.2655; // Voltage Divider R2: 4.7k R1 = 13k
		float Curr_Output_12V = (adc1_results[4] * ref_voltage / RESOLUTION - 2.5) / 0.400;
		float Volt_Output_7V =  (adc2_results[1] * ref_voltage / RESOLUTION) / (4.7/17.7); // VOltage Divider R2:4.7k R1: 13k
		float Curr_Output_7V =  (adc2_results[0] * ref_voltage / RESOLUTION - 2.5) / 0.400; // VOltage Divider R2:4.7k R1: 13k


		float Buc_temp = (Buc_temp_sense_Voltage - 0.5) / (0.01);  //buc temp
		float Op_temp = (Op_temp_sense_Voltage - 0.5) / (0.01);  //op temp
		float Cntrl_temp = (Cntrl_temp_sense_Voltage - 0.5) / (0.01); //cntrl temp
		float Batt_Input = Battery_Input_Voltage ; // Batter Input Volt
		float Batt_Current = Battery_Input_Current ; // Battery Input Current
		float Output_12V = Volt_Output_12V ; // 12 Volt Output
		float Output_12V_Curr = Curr_Output_12V ;
		float Output_7V = Volt_Output_7V ; // 7 Volt Output
		float Output_7V_Curr = Curr_Output_7V ;

	    printf(
	             "BATT CURR: %.2f BATT VOLT: %.3f 12V VOLT: %.2f 12V CURR: %.2f 7V VOLT: %.2f 7V CURR: %.2f \r\n",
				 Batt_Input,
				 Batt_Current,
				 Output_12V,
				 Output_12V_Curr,
				 Output_7V,
				 Output_7V_Curr

	    );




		char buc_temp[100];
		char op_temp[100];
		char cntrl_temp[100];
		char batt_volt[100];
		char batt_current[100];
		char output_12V [100];
		char output_curr_12V [100];
		char output_7V [100];
		char output_curr_7V [100];

		sprintf(buc_temp, "BUCK T:%.2f C", Buc_temp);
		sprintf(op_temp, "OP T: %.2f C", Op_temp);
		sprintf(cntrl_temp, "CNTRL T: %.2f C", Cntrl_temp);
		sprintf(batt_volt, "Batt Volt: %.2f V", Batt_Input);
		sprintf(batt_current, "Batt Curr: %.2f A", Batt_Current);
		sprintf(output_12V, "12V Out: %.2f V", Output_12V);
		sprintf(output_curr_12V, "12V Out: %.2f A", Output_12V_Curr);
		sprintf(output_7V, "7V Out: %.2f V", Output_7V);
		sprintf(output_curr_7V, "7V Out: %.2f A", Output_7V_Curr);


		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString(buc_temp, Font_7x10, White);
		ssd1306_UpdateScreen();

		ssd1306_SetCursor(0, 15);
		ssd1306_WriteString(op_temp, Font_7x10, White);
		ssd1306_UpdateScreen();

		ssd1306_SetCursor(0, 30);
		ssd1306_WriteString(cntrl_temp, Font_7x10, White);
		ssd1306_UpdateScreen();

		ssd1306_SetCursor(0, 45);
		ssd1306_WriteString(batt_current, Font_7x10, White);
		ssd1306_UpdateScreen();

		osDelay(1);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskReceive */
/**
 * @brief Function implementing the canReceive thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskReceive */
void StartTaskReceive(void *argument)
{
  /* USER CODE BEGIN StartTaskReceive */
	FDCAN_RxHeaderTypeDef localRxHeader = { 0 };
	uint8_t ret[64] = { 0 };

	for (;;) {
		if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.Identifier,
				0,
				osWaitForever) == osOK) {

			if (osMessageQueueGet(canQueRxHeaderHandle,
					&localRxHeader.DataLength, 0, 0) != osOK) {
				Error_Handler();
			}

			for (uint8_t i = 0; i < mapDlcToBytes(localRxHeader.DataLength);
					i++) {
				if (osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0)
						!= osOK) {
					Error_Handler();
				}
			}

			switch (localRxHeader.Identifier) {
			case FDCAN_H2ALARM_ID:
				// H2 ALARM
				if (ret[0] == 1) {
					// Do something?
				}
				break;
			case FDCAN_SYNCLED_ID:
				// CAN SYNC LED
				//HAL_GPIO_WritePin(GPLED1_GPIO_Port, GPLED1_Pin, ret[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPLED2_GPIO_Port, GPLED2_Pin, ret[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
				//HAL_GPIO_WritePin(GPLED3_GPIO_Port, GPLED3_Pin, ret[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
			default:
				//log_info("Received CANID 0x%x but did nothing with it!",
				//		localRxHeader.Identifier);
				break;
			}

		} else {
			//log_err("Something strange is happening!");
		}
	}
  /* USER CODE END StartTaskReceive */
}

/* USER CODE BEGIN Header_StartTaskSend */
/**
 * @brief Function implementing the canSend thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskSend */
void StartTaskSend(void *argument)
{
  /* USER CODE BEGIN StartTaskSend */

	UNUSED(argument);
	FDCAN_TxHeaderTypeDef localTxHeader;
	const uint8_t msg_delay = 100;

	localTxHeader.IdType = FDCAN_STANDARD_ID;
	localTxHeader.TxFrameType = FDCAN_DATA_FRAME;
	localTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	localTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	localTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	localTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	localTxHeader.MessageMarker = 0;
	/* Infinite loop */
	for (;;) {
		// Transmit boost data
//		localTxHeader.Identifier = FDCAN_BOOSTPACK_ID;
//		localTxHeader.DataLength = FDCAN_DLC_BYTES_8;
//		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
//			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
//					(uint8_t*) &boost_data.FDCAN_RawBOOSTPack) != HAL_OK) {
//				Error_Handler();
//			}
//		} //else {
		  //log_warn("Tx Buffer Full");
		  //}


//		localTxHeader.Identifier = FDCAN_BOOSTPACK2_ID;
//		localTxHeader.DataLength = FDCAN_DLC_BYTES_8;
//		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
//			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
//					(uint8_t*) &boost_data2.FDCAN_RawBOOSTPack2) != HAL_OK) {
//				Error_Handler();
//			}
//		} //else {
//		  //log_warn("Tx Buffer Full");
//		  //}
		osDelay(1);
	}
  /* USER CODE END StartTaskSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

