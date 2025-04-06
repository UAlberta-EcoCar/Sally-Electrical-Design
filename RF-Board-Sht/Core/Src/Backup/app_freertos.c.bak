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
/* Definitions for telemetryTransm */
osThreadId_t telemetryTransmHandle;
uint32_t telemetryTransmBuffer[ 2048 ];
osStaticThreadDef_t telemetryTransmControlBlock;
const osThreadAttr_t telemetryTransm_attributes = {
  .name = "telemetryTransm",
  .stack_mem = &telemetryTransmBuffer[0],
  .stack_size = sizeof(telemetryTransmBuffer),
  .cb_mem = &telemetryTransmControlBlock,
  .cb_size = sizeof(telemetryTransmControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for auxilary */
osThreadId_t auxilaryHandle;
uint32_t auxilaryBuffer[ 1024 ];
osStaticThreadDef_t auxilaryControlBlock;
const osThreadAttr_t auxilary_attributes = {
  .name = "auxilary",
  .stack_mem = &auxilaryBuffer[0],
  .stack_size = sizeof(auxilaryBuffer),
  .cb_mem = &auxilaryControlBlock,
  .cb_size = sizeof(auxilaryControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CANSend */
osThreadId_t CANSendHandle;
uint32_t CANSendBuffer[ 512 ];
osStaticThreadDef_t CANSendControlBlock;
const osThreadAttr_t CANSend_attributes = {
  .name = "CANSend",
  .stack_mem = &CANSendBuffer[0],
  .stack_size = sizeof(CANSendBuffer),
  .cb_mem = &CANSendControlBlock,
  .cb_size = sizeof(CANSendControlBlock),
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for CANReceive */
osThreadId_t CANReceiveHandle;
uint32_t CANReceiveBuffer[ 512 ];
osStaticThreadDef_t CANReceiveControlBlock;
const osThreadAttr_t CANReceive_attributes = {
  .name = "CANReceive",
  .stack_mem = &CANReceiveBuffer[0],
  .stack_size = sizeof(CANReceiveBuffer),
  .cb_mem = &CANReceiveControlBlock,
  .cb_size = sizeof(CANReceiveControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for canQueRxHeader */
osMessageQueueId_t canQueRxHeaderHandle;
uint8_t canQueRxHeaderBuffer[ 512 * sizeof( uint32_t ) ];
osStaticMessageQDef_t canQueRxHeaderControlBlock;
const osMessageQueueAttr_t canQueRxHeader_attributes = {
  .name = "canQueRxHeader",
  .cb_mem = &canQueRxHeaderControlBlock,
  .cb_size = sizeof(canQueRxHeaderControlBlock),
  .mq_mem = &canQueRxHeaderBuffer,
  .mq_size = sizeof(canQueRxHeaderBuffer)
};
/* Definitions for canQueRxData */
osMessageQueueId_t canQueRxDataHandle;
uint8_t canQueRxDataBuffer[ 1024 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canQueRxDataControlBlock;
const osMessageQueueAttr_t canQueRxData_attributes = {
  .name = "canQueRxData",
  .cb_mem = &canQueRxDataControlBlock,
  .cb_size = sizeof(canQueRxDataControlBlock),
  .mq_mem = &canQueRxDataBuffer,
  .mq_size = sizeof(canQueRxDataBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTelemetryTransmitTask(void *argument);
void StartAuxilaryTask(void *argument);
void StartCanSend(void *argument);
void StartCanReceive(void *argument);

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
  canQueRxDataHandle = osMessageQueueNew (1024, sizeof(uint8_t), &canQueRxData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of telemetryTransm */
  telemetryTransmHandle = osThreadNew(StartTelemetryTransmitTask, NULL, &telemetryTransm_attributes);

  /* creation of auxilary */
  auxilaryHandle = osThreadNew(StartAuxilaryTask, NULL, &auxilary_attributes);

  /* creation of CANSend */
  CANSendHandle = osThreadNew(StartCanSend, NULL, &CANSend_attributes);

  /* creation of CANReceive */
  CANReceiveHandle = osThreadNew(StartCanReceive, NULL, &CANReceive_attributes);

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
__weak void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTelemetryTransmitTask */
/**
* @brief Function implementing the telemetryTransm thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTelemetryTransmitTask */
__weak void StartTelemetryTransmitTask(void *argument)
{
  /* USER CODE BEGIN StartTelemetryTransmitTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTelemetryTransmitTask */
}

/* USER CODE BEGIN Header_StartAuxilaryTask */
/**
* @brief Function implementing the auxilary thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAuxilaryTask */
__weak void StartAuxilaryTask(void *argument)
{
  /* USER CODE BEGIN StartAuxilaryTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartAuxilaryTask */
}

/* USER CODE BEGIN Header_StartCanSend */
/**
* @brief Function implementing the CANSend thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanSend */
__weak void StartCanSend(void *argument)
{
  /* USER CODE BEGIN StartCanSend */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCanSend */
}

/* USER CODE BEGIN Header_StartCanReceive */
/**
* @brief Function implementing the CANReceive thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanReceive */
__weak void StartCanReceive(void *argument)
{
  /* USER CODE BEGIN StartCanReceive */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCanReceive */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

