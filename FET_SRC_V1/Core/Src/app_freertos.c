/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "usb_device.h"
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
/* Definitions for canReceiveMsg */
osThreadId_t canReceiveMsgHandle;
uint32_t CanReceiveMsgBuffer[ 512 ];
osStaticThreadDef_t CanReceiveMsgControlBlock;
const osThreadAttr_t canReceiveMsg_attributes = {
  .name = "canReceiveMsg",
  .stack_mem = &CanReceiveMsgBuffer[0],
  .stack_size = sizeof(CanReceiveMsgBuffer),
  .cb_mem = &CanReceiveMsgControlBlock,
  .cb_size = sizeof(CanReceiveMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for canSendMsg */
osThreadId_t canSendMsgHandle;
uint32_t CanSendMsgBuffer[ 512 ];
osStaticThreadDef_t CanSendMsgControlBlock;
const osThreadAttr_t canSendMsg_attributes = {
  .name = "canSendMsg",
  .stack_mem = &CanSendMsgBuffer[0],
  .stack_size = sizeof(CanSendMsgBuffer),
  .cb_mem = &CanSendMsgControlBlock,
  .cb_size = sizeof(CanSendMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for canReceiveQue */
osMessageQueueId_t canReceiveQueHandle;
uint8_t canReceiveQueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canReceiveQueControlBlock;
const osMessageQueueAttr_t canReceiveQue_attributes = {
  .name = "canReceiveQue",
  .cb_mem = &canReceiveQueControlBlock,
  .cb_size = sizeof(canReceiveQueControlBlock),
  .mq_mem = &canReceiveQueBuffer,
  .mq_size = sizeof(canReceiveQueBuffer)
};
/* Definitions for canSendQue */
osMessageQueueId_t canSendQueHandle;
uint8_t canSendQueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canSendQueControlBlock;
const osMessageQueueAttr_t canSendQue_attributes = {
  .name = "canSendQue",
  .cb_mem = &canSendQueControlBlock,
  .cb_size = sizeof(canSendQueControlBlock),
  .mq_mem = &canSendQueBuffer,
  .mq_size = sizeof(canSendQueBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanReceive(void *argument);
void StartCanSend(void *argument);

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
  /* creation of canReceiveQue */
  canReceiveQueHandle = osMessageQueueNew (512, sizeof(uint8_t), &canReceiveQue_attributes);

  /* creation of canSendQue */
  canSendQueHandle = osMessageQueueNew (512, sizeof(uint8_t), &canSendQue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canReceiveMsg */
  canReceiveMsgHandle = osThreadNew(StartCanReceive, NULL, &canReceiveMsg_attributes);

  /* creation of canSendMsg */
  canSendMsgHandle = osThreadNew(StartCanSend, NULL, &canSendMsg_attributes);

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
  UNUSED(argument);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartCanReceive */
/**
* @brief Function implementing the canReceiveMsg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanReceive */
void StartCanReceive(void *argument)
{
  /* USER CODE BEGIN StartCanReceive */
  UNUSED(argument);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCanReceive */
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCanSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

