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
#include "spi.h"
#include "rf-rfm95.h"
#include "ecocar_can.h"
#include "fdcan.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
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
/* Definitions for TaskCan */
osThreadId_t TaskCanHandle;
uint32_t CanTaskBuffer[ 256 ];
osStaticThreadDef_t CanTaskControlBlock;
const osThreadAttr_t TaskCan_attributes = {
  .name = "TaskCan",
  .stack_mem = &CanTaskBuffer[0],
  .stack_size = sizeof(CanTaskBuffer),
  .cb_mem = &CanTaskControlBlock,
  .cb_size = sizeof(CanTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task24 */
osThreadId_t Task24Handle;
uint32_t Task24Buffer[ 256 ];
osStaticThreadDef_t Task24ControlBlock;
const osThreadAttr_t Task24_attributes = {
  .name = "Task24",
  .stack_mem = &Task24Buffer[0],
  .stack_size = sizeof(Task24Buffer),
  .cb_mem = &Task24ControlBlock,
  .cb_size = sizeof(Task24ControlBlock),
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task915 */
osThreadId_t Task915Handle;
uint32_t Task915Buffer[ 256 ];
osStaticThreadDef_t Task915ControlBlock;
const osThreadAttr_t Task915_attributes = {
  .name = "Task915",
  .stack_mem = &Task915Buffer[0],
  .stack_size = sizeof(Task915Buffer),
  .cb_mem = &Task915ControlBlock,
  .cb_size = sizeof(Task915ControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task868 */
osThreadId_t Task868Handle;
uint32_t Task868Buffer[ 256 ];
osStaticThreadDef_t Task868ControlBlock;
const osThreadAttr_t Task868_attributes = {
  .name = "Task868",
  .stack_mem = &Task868Buffer[0],
  .stack_size = sizeof(Task868Buffer),
  .cb_mem = &Task868ControlBlock,
  .cb_size = sizeof(Task868ControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskGNSS */
osThreadId_t TaskGNSSHandle;
uint32_t GNSSTaskBuffer[ 256 ];
osStaticThreadDef_t GNSSTaskControlBlock;
const osThreadAttr_t TaskGNSS_attributes = {
  .name = "TaskGNSS",
  .stack_mem = &GNSSTaskBuffer[0],
  .stack_size = sizeof(GNSSTaskBuffer),
  .cb_mem = &GNSSTaskControlBlock,
  .cb_size = sizeof(GNSSTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskSD */
osThreadId_t TaskSDHandle;
uint32_t SDTaskBuffer[ 256 ];
osStaticThreadDef_t SDTaskControlBlock;
const osThreadAttr_t TaskSD_attributes = {
  .name = "TaskSD",
  .stack_mem = &SDTaskBuffer[0],
  .stack_size = sizeof(SDTaskBuffer),
  .cb_mem = &SDTaskControlBlock,
  .cb_size = sizeof(SDTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskAUX */
osThreadId_t TaskAUXHandle;
uint32_t TaskAUXBuffer[ 256 ];
osStaticThreadDef_t TaskAUXControlBlock;
const osThreadAttr_t TaskAUX_attributes = {
  .name = "TaskAUX",
  .stack_mem = &TaskAUXBuffer[0],
  .stack_size = sizeof(TaskAUXBuffer),
  .cb_mem = &TaskAUXControlBlock,
  .cb_size = sizeof(TaskAUXControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
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
void StartCanTask(void *argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TaskCan */
  TaskCanHandle = osThreadNew(StartCanTask, NULL, &TaskCan_attributes);

  /* creation of Task24 */
  Task24Handle = osThreadNew(Start24Task, NULL, &Task24_attributes);

  /* creation of Task915 */
  Task915Handle = osThreadNew(Start915Task, NULL, &Task915_attributes);

  /* creation of Task868 */
  Task868Handle = osThreadNew(Start868Task, NULL, &Task868_attributes);

  /* creation of TaskGNSS */
  TaskGNSSHandle = osThreadNew(StartGNSSTask, NULL, &TaskGNSS_attributes);

  /* creation of TaskSD */
  TaskSDHandle = osThreadNew(StartSDTask, NULL, &TaskSD_attributes);

  /* creation of TaskAUX */
  TaskAUXHandle = osThreadNew(StartAUXTask, NULL, &TaskAUX_attributes);

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
  /* USER CODE BEGIN StartDefaultTask */
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

	//	HAL_GPIO_WritePin(RST_915_GPIO_Port, RST_915_Pin, GPIO_PIN_SET);
	//
	//	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

	rf_handle_t rfm95_868 = { .rf_nreset_port = RST_868_GPIO_Port,
			.rf_nreset_pin = RST_868_Pin, .rf_nss_port = NSS_868_GPIO_Port,
			.rf_nss_pin = NSS_868_Pin, .rf_spi_handle = &hspi1, .rf_delay_func =
					osDelay, .rf_spi_timeout = HAL_MAX_DELAY,
			.rf_carrier_frequency = 868000000 };

	rf_handle_t rfm95_915 = { .rf_nreset_port = RST_915_GPIO_Port,
			.rf_nreset_pin = RST_915_Pin, .rf_nss_port = NSS_915_GPIO_Port,
			.rf_nss_pin = NSS_915_Pin, .rf_spi_handle = &hspi1, .rf_delay_func =
					osDelay, .rf_spi_timeout = 100, .rf_carrier_frequency =
					915000000 };

	rf_initialize_radio(&rfm95_868);

	rf_initialize_radio(&rfm95_915);

	rf_set_frequency(&rfm95_868, 868000000);

	rf_set_frequency(&rfm95_915, 915000000);

	uint8_t data[] = "hello";
	uint8_t rdata[6] = { 0 };
	uint8_t rdata2[6] = { 0 };
	uint8_t rec_legth = 0;
	uint8_t rec_legth2 = 0;

	FDCAN_TxHeaderTypeDef loctx = {0};
	loctx.BitRateSwitch = FDCAN_BRS_ON;
	loctx.DataLength = 6;
	loctx.FDFormat = FDCAN_FRAME_FD_BRS;
	loctx.IdType = FDCAN_STANDARD_ID;
	loctx.Identifier = 0x020;
	loctx.TxFrameType = FDCAN_DATA_FRAME;


	/* Infinite loop */
	for (;;) {


		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			rf_send(&rfm95_868, data, 5);
			rf_send(&rfm95_915, data, 5);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		}
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT2_GPIO_Port, SWT2_Pin)) {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			while (0 == rec_legth) {
				rf_recieve_single(&rfm95_868, &rec_legth);
				osDelay(10);
			}
			if (rec_legth > 0) {
				rf_read_packet(&rfm95_868, rec_legth, rdata);
				rec_legth = 0;
			}

			while (0 == rec_legth2) {
				rf_recieve_single(&rfm95_915, &rec_legth2);
				//osDelay(10);
			}
			if (rec_legth2 > 0) {
				rf_read_packet(&rfm95_915, rec_legth2, rdata2);
				rec_legth2 = 0;
			}
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}

		if (0 != HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
			if (HAL_OK == HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &loctx, &data)) {
				HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
			}
		}
		osDelay(100);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartCanTask */
/**
 * @brief Function implementing the TaskCan thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCanTask */
void StartCanTask(void *argument)
{
  /* USER CODE BEGIN StartCanTask */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END StartCanTask */
}

/* USER CODE BEGIN Header_Start24Task */
/**
 * @brief Function implementing the Task24 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start24Task */
void Start24Task(void *argument)
{
  /* USER CODE BEGIN Start24Task */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END Start24Task */
}

/* USER CODE BEGIN Header_Start915Task */
/**
 * @brief Function implementing the Task915 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start915Task */
void Start915Task(void *argument)
{
  /* USER CODE BEGIN Start915Task */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END Start915Task */
}

/* USER CODE BEGIN Header_Start868Task */
/**
 * @brief Function implementing the Task868 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start868Task */
void Start868Task(void *argument)
{
  /* USER CODE BEGIN Start868Task */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END Start868Task */
}

/* USER CODE BEGIN Header_StartGNSSTask */
/**
 * @brief Function implementing the TaskGNSS thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGNSSTask */
void StartGNSSTask(void *argument)
{
  /* USER CODE BEGIN StartGNSSTask */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END StartGNSSTask */
}

/* USER CODE BEGIN Header_StartSDTask */
/**
 * @brief Function implementing the TaskSD thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSDTask */
void StartSDTask(void *argument)
{
  /* USER CODE BEGIN StartSDTask */
	/* Infinite loop */
	for (;;) {
		osDelay(10);
	}
  /* USER CODE END StartSDTask */
}

/* USER CODE BEGIN Header_StartAUXTask */
/**
 * @brief Function implementing the TaskAUX thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAUXTask */
void StartAUXTask(void *argument)
{
  /* USER CODE BEGIN StartAUXTask */
	/* Infinite loop */

	for (;;) {
		osDelay(10);
	}
  /* USER CODE END StartAUXTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

