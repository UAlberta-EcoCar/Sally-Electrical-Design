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
//this is a test for the programming template
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include <string.h>
#include <stdbool.h>
#include "adc.h"
#include "dac.h"
#include <math.h>
#include "usb_device.h"  // Add this line to include the USB device header
#include "usbd_cdc_if.h" // Add this line to include the USB device header
#include "ecocar_can.h"
#include "fdcan.h"
#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */
typedef struct {
	float current[4];
	float voltage[2];
} boostData_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SET_BRIGHTNESS(x) (uint32_t)(65535 * x / 100)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
FDCAN_BOOSTPack_t boost_data = { 0 };
FDCAN_BOOSTPack2_t boost_data2 = { 0 };
FDCAN_BOOSTPack3_t boost_data3 = {0};

static uint8_t voltage_reached = 0;  // Flag to track first detection

bool lock_state = false;
float SET_VOLT = 48;
const float VOLT_MCU = 3.283;

float en_pin = 0;

uint32_t ADC1_VALUE[4];
uint32_t ADC2_VALUE[2];

char ScreenBuffer[2048];
#define ADC_BUFFER_SIZE 50 

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
/* Definitions for canSendMsg */
osThreadId_t canSendMsgHandle;
uint32_t canSendMsgBuffer[ 512 ];
osStaticThreadDef_t canSendMsgControlBlock;
const osThreadAttr_t canSendMsg_attributes = {
  .name = "canSendMsg",
  .stack_mem = &canSendMsgBuffer[0],
  .stack_size = sizeof(canSendMsgBuffer),
  .cb_mem = &canSendMsgControlBlock,
  .cb_size = sizeof(canSendMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for canRecieveMsg */
osThreadId_t canRecieveMsgHandle;
uint32_t canRecieveMsgBuffer[ 512 ];
osStaticThreadDef_t canRecieveMsgControlBlock;
const osThreadAttr_t canRecieveMsg_attributes = {
  .name = "canRecieveMsg",
  .stack_mem = &canRecieveMsgBuffer[0],
  .stack_size = sizeof(canRecieveMsgBuffer),
  .cb_mem = &canRecieveMsgControlBlock,
  .cb_size = sizeof(canRecieveMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adcConvTask */
osThreadId_t adcConvTaskHandle;
uint32_t adcConvTaskBuffer[ 512 ];
osStaticThreadDef_t adcConvTaskControlBlock;
const osThreadAttr_t adcConvTask_attributes = {
  .name = "adcConvTask",
  .stack_mem = &adcConvTaskBuffer[0],
  .stack_size = sizeof(adcConvTaskBuffer),
  .cb_mem = &adcConvTaskControlBlock,
  .cb_size = sizeof(adcConvTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ScreenPrintTask */
osThreadId_t ScreenPrintTaskHandle;
uint32_t ScreenPrintHandBuffer[ 512 ];
osStaticThreadDef_t ScreenPrintHandControlBlock;
const osThreadAttr_t ScreenPrintTask_attributes = {
  .name = "ScreenPrintTask",
  .stack_mem = &ScreenPrintHandBuffer[0],
  .stack_size = sizeof(ScreenPrintHandBuffer),
  .cb_mem = &ScreenPrintHandControlBlock,
  .cb_size = sizeof(ScreenPrintHandControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TRKPinTask */
osThreadId_t TRKPinTaskHandle;
uint32_t TRKPinTaskBuffer[ 512 ];
osStaticThreadDef_t TRKPinTaskControlBlock;
const osThreadAttr_t TRKPinTask_attributes = {
  .name = "TRKPinTask",
  .stack_mem = &TRKPinTaskBuffer[0],
  .stack_size = sizeof(TRKPinTaskBuffer),
  .cb_mem = &TRKPinTaskControlBlock,
  .cb_size = sizeof(TRKPinTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StatusLEDHandle */
osThreadId_t StatusLEDHandleHandle;
uint32_t StatusLEDHandleBuffer[ 512 ];
osStaticThreadDef_t StatusLEDHandleControlBlock;
const osThreadAttr_t StatusLEDHandle_attributes = {
  .name = "StatusLEDHandle",
  .stack_mem = &StatusLEDHandleBuffer[0],
  .stack_size = sizeof(StatusLEDHandleBuffer),
  .cb_mem = &StatusLEDHandleControlBlock,
  .cb_size = sizeof(StatusLEDHandleControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for canQueueRxHeader */
osMessageQueueId_t canQueueRxHeaderHandle;
uint8_t canQueueRxHeaderBuffer[ 512 * sizeof( uint32_t ) ];
osStaticMessageQDef_t canQueueRxHeaderControlBlock;
const osMessageQueueAttr_t canQueueRxHeader_attributes = {
  .name = "canQueueRxHeader",
  .cb_mem = &canQueueRxHeaderControlBlock,
  .cb_size = sizeof(canQueueRxHeaderControlBlock),
  .mq_mem = &canQueueRxHeaderBuffer,
  .mq_size = sizeof(canQueueRxHeaderBuffer)
};
/* Definitions for canQueueRxData */
osMessageQueueId_t canQueueRxDataHandle;
uint8_t canQueueRxDataBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canQueueRxDataControlBlock;
const osMessageQueueAttr_t canQueueRxData_attributes = {
  .name = "canQueueRxData",
  .cb_mem = &canQueueRxDataControlBlock,
  .cb_size = sizeof(canQueueRxDataControlBlock),
  .mq_mem = &canQueueRxDataBuffer,
  .mq_size = sizeof(canQueueRxDataBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[64];
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData)
				!= HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}
		if (HAL_FDCAN_ActivateNotification(hfdcan,
		FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
		osMessageQueuePut(canQueueRxHeaderHandle, &RxHeader.Identifier, 0, 0);
		osMessageQueuePut(canQueueRxHeaderHandle, &RxHeader.DataLength, 0, 0);
		for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
			osMessageQueuePut(canQueueRxDataHandle, &RxData[i], 0, 0);
		}
	}
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanSend(void *argument);
void StartRecieveMsg(void *argument);
void StartAdcConv(void *argument);
void startScreenPrint(void *argument);
void StartTRKPin(void *argument);
void StartStatusLED(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // LED1
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // LED2
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); // LED3
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); // LED4
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // LED5
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // CAN LED
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
  /* creation of canQueueRxHeader */
  canQueueRxHeaderHandle = osMessageQueueNew (512, sizeof(uint32_t), &canQueueRxHeader_attributes);

  /* creation of canQueueRxData */
  canQueueRxDataHandle = osMessageQueueNew (512, sizeof(uint8_t), &canQueueRxData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canSendMsg */
  canSendMsgHandle = osThreadNew(StartCanSend, NULL, &canSendMsg_attributes);

  /* creation of canRecieveMsg */
  canRecieveMsgHandle = osThreadNew(StartRecieveMsg, NULL, &canRecieveMsg_attributes);

  /* creation of adcConvTask */
  adcConvTaskHandle = osThreadNew(StartAdcConv, NULL, &adcConvTask_attributes);

  /* creation of ScreenPrintTask */
  ScreenPrintTaskHandle = osThreadNew(startScreenPrint, NULL, &ScreenPrintTask_attributes);

  /* creation of TRKPinTask */
  TRKPinTaskHandle = osThreadNew(StartTRKPin, NULL, &TRKPinTask_attributes);

  /* creation of StatusLEDHandle */
  StatusLEDHandleHandle = osThreadNew(StartStatusLED, NULL, &StatusLEDHandle_attributes);

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
	for (;;) {

		// Increase the set output voltage using the push button. Since it is connected to the BOOT0 Pin, will need to push this button with the NRST button each time when flashing
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) { 
		    SET_VOLT++;

		    if (SET_VOLT > 55) {
		        float input_voltage = (float)boost_data.in_volt / FDCAN_FOUR_FLT_PREC;

		        if (input_voltage < 15.0f) {
		            SET_VOLT = 16;  // Ensure minimum voltage is 16V
		        } else {
		            SET_VOLT = input_voltage + 3.0f;  // Set 3V higher than input voltage
		        }
		    }
		}


		if ((float) boost_data.in_volt / FDCAN_FOUR_FLT_PREC > 1.5) {
			if (voltage_reached == 0) { 
				HAL_GPIO_WritePin(GPIOF, ENABLE_Pin, GPIO_PIN_RESET); // Ensure it's OFF before delay
				HAL_Delay(2000);  
				HAL_GPIO_WritePin(GPIOF, ENABLE_Pin, GPIO_PIN_SET); // Turn ON after delay
				voltage_reached = 1;  // Set flag to avoid repeated delays
			} else {
				HAL_GPIO_WritePin(GPIOF, ENABLE_Pin, GPIO_PIN_SET); // Keep it ON
			}
		} else { 
			HAL_GPIO_WritePin(GPIOF, ENABLE_Pin, GPIO_PIN_RESET); // Turn OFF
			voltage_reached = 0;  
		}




		osDelay(200);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartCanSend */
/**
 * @brief Function implementing the canSendMsg thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCanSend */
void StartCanSend(void *argument)
{
  /* USER CODE BEGIN StartCanSend */
	UNUSED(argument);
	FDCAN_TxHeaderTypeDef localTxHeader;
	const uint8_t msg_delay = 100;

	//LED SYNC
	uint8_t can_sync_led = 0;
	uint32_t sync_led_last = 0;
	uint32_t sync_led_this = osKernelGetSysTimerCount();

	localTxHeader.IdType = FDCAN_STANDARD_ID;
	localTxHeader.TxFrameType = FDCAN_DATA_FRAME;
	localTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	localTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	localTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	localTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	localTxHeader.MessageMarker = 0;
	/* Infinite loop */

	for (;;) {

	//Sync LEDs
	// sync_led_this = osKernelGetTickCount();
	// if (sync_led_this - sync_led_last > 500) {
	// 	sync_led_last = sync_led_this;
	// 	localTxHeader.Identifier = FDCAN_SYNCLED_ID;
	// 	localTxHeader.DataLength = FDCAN_DLC_BYTES_1;
	// 	can_sync_led = (can_sync_led == 0) ? 1 : 0;
	// 	if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
	// 	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
	// 										&can_sync_led) != HAL_OK) {
	// 		Error_Handler();
	// 		}

	// 	}
	// }

		// Transmit boost data
		localTxHeader.Identifier = FDCAN_BOOSTPACK_ID;
		localTxHeader.DataLength = FDCAN_DLC_BYTES_8;
		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
					(uint8_t*) &boost_data.FDCAN_RawBOOSTPack) != HAL_OK) {
				Error_Handler();
			}
		} //else {
		  //log_warn("Tx Buffer Full");
		  //}


		localTxHeader.Identifier = FDCAN_BOOSTPACK2_ID;
		localTxHeader.DataLength = FDCAN_DLC_BYTES_8;
		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
					(uint8_t*) &boost_data2.FDCAN_RawBOOSTPack2) != HAL_OK) {
				Error_Handler();
			}
		} //else {
		  //log_warn("Tx Buffer Full");
		  //}

		osDelay(msg_delay);

		localTxHeader.Identifier = FDCAN_BOOSTPACK3_ID;
		localTxHeader.DataLength = FDCAN_DLC_BYTES_8;
		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
					(uint8_t*) &boost_data3.FDCAN_RawBOOSTPack3) != HAL_OK) {
				Error_Handler();
			}
		}
		}	

  /* USER CODE END StartCanSend */
}

/* USER CODE BEGIN Header_StartRecieveMsg */
/**
 * @brief Function implementing the canRecieveMsg thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRecieveMsg */
void StartRecieveMsg(void *argument)
{
  /* USER CODE BEGIN StartRecieveMsg */
	/* Infinite loop */
	UNUSED(argument);
	FDCAN_RxHeaderTypeDef localRxHeader = { 0 };
	uint8_t ret[64] = { 0 };
	/* Infinite loop */
	HAL_GPIO_WritePin(GPIOB, CAN_STBY_Pin, GPIO_PIN_RESET);
	for (;;) {

		if (osMessageQueueGet(canQueueRxHeaderHandle, &localRxHeader.Identifier,
				0,
				osWaitForever) == osOK) {
			if (osMessageQueueGet(canQueueRxHeaderHandle,
					&localRxHeader.DataLength, 0, 0) != osOK) {
				Error_Handler();
			}
			for (uint8_t i = 0; i < mapDlcToBytes(localRxHeader.DataLength);
					i++) {
				if (osMessageQueueGet(canQueueRxDataHandle, &ret[i], 0, 0)
						!= osOK) {
					Error_Handler();
				}
			}

			switch (localRxHeader.Identifier) {
			case FDCAN_H2ALARM_ID:
				// H2 ALARM
				if (ret[0] == 1) {
					lock_state = true;
				} else {
					lock_state = false;
				}
				break;

/*			case FDCAN_SYNCLED_ID:
				// CAN SYNC LED
				if (ret[0] == 1) {
					htim3.Instance->CCR3 = SET_BRIGHTNESS(20); // CAN LED
				} else {
					htim3.Instance->CCR3 = SET_BRIGHTNESS(0); // CAN LED
				}
				break;*/
//
//			case FDCAN_BOOSTPACK_ID:
//				memcpy(&boost_data.FDCAN_RawBOOSTPack, ret,
//					mapDlcToBytes(localRxHeader.DataLength));
//				break;
//
//			case FDCAN_BOOSTPACK2_ID:
//				memcpy(&boost_data2.FDCAN_RawBOOSTPack2, ret,
//					mapDlcToBytes(localRxHeader.DataLength));
//				break;



			default:
				break;
			}
		}
	}
  /* USER CODE END StartRecieveMsg */
}

/* USER CODE BEGIN Header_StartAdcConv */
/**
 * @brief Function implementing the adcConvTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAdcConv */
void StartAdcConv(void *argument)
{
  /* USER CODE BEGIN StartAdcConv */
	UNUSED(argument);

	const float = ;




	HAL_ADC_Start_DMA(&hadc1, ADC1_VALUE, 4);


	/* Infinite loop */
	for (;;) {


		boost_data2.out_volt = ;
		boost_data.in_volt = ;
		boost_data.in_curr = ;
		boost_data2.out_curr = ;



		osDelay(10);
	}
  /* USER CODE END StartAdcConv */
}

/* USER CODE BEGIN Header_startScreenPrint */
/**
 * @brief Function implementing the ScreenPrintTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startScreenPrint */
void startScreenPrint(void *argument)
{
  /* USER CODE BEGIN startScreenPrint */
	/* Infinite loop */
	ssd1306_Init();
	// Display the test bitmap for 2.5 seconds
	ssd1306_TestDrawBitmap();
	ssd1306_UpdateScreen();
	osDelay(2500); // Delay for 2.5 seconds
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();

	ssd1306_TestDrawBitmap2();
	ssd1306_UpdateScreen();
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();
	for (;;) {

	ssd1306_Fill(Black); // Clear the screen before updating

		// Voltages
		ssd1306_SetCursor(0, 5);
		sprintf(ScreenBuffer, "Voltage(V) (%.1f)", SET_VOLT);
		ssd1306_WriteString(ScreenBuffer, Font_7x10, White);



		ssd1306_UpdateScreen(); // Refresh display

		osDelay(100); // Slow down update rate
	}
  /* USER CODE END startScreenPrint */
}

/* USER CODE BEGIN Header_StartTRKPin */
/**
 * @brief Function implementing the TRKPinTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTRKPin */
void StartTRKPin(void *argument)
{
  /* USER CODE BEGIN StartTRKPin */
	uint32_t DAC_VALUE;
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

	/* Infinite loop */
	for (;;) {

	float TRK_VOLT = SET_VOLT / 60; // Desired voltage output

		// Convert to DAC value

		// Set DAC output

		osDelay(1);
	}
  /* USER CODE END StartTRKPin */
}

/* USER CODE BEGIN Header_StartStatusLED */
/**
 * @brief Function implementing the StatusLEDHandle thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartStatusLED */
void StartStatusLED(void *argument)
{
  /* USER CODE BEGIN StartStatusLED */
	/* Infinite loop */
	for (;;) {




		osDelay(1);	
	}
  /* USER CODE END StartStatusLED */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

