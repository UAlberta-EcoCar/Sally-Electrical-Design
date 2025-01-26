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
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_device.h"
#include "usbd_cdc_if.h"
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

FDCAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[64];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[512];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_mem = &defaultTaskBuffer[0],
    .stack_size = sizeof(defaultTaskBuffer),
    .cb_mem = &defaultTaskControlBlock,
    .cb_size = sizeof(defaultTaskControlBlock),
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for canReceive */
osThreadId_t canReceiveHandle;
uint32_t canReceiveBuffer[512];
osStaticThreadDef_t canReceiveControlBlock;
const osThreadAttr_t canReceive_attributes = {
    .name = "canReceive",
    .stack_mem = &canReceiveBuffer[0],
    .stack_size = sizeof(canReceiveBuffer),
    .cb_mem = &canReceiveControlBlock,
    .cb_size = sizeof(canReceiveControlBlock),
    .priority = (osPriority_t)osPriorityAboveNormal,
};
/* Definitions for canSend */
osThreadId_t canSendHandle;
uint32_t canSendBuffer[512];
osStaticThreadDef_t canSendControlBlock;
const osThreadAttr_t canSend_attributes = {
    .name = "canSend",
    .stack_mem = &canSendBuffer[0],
    .stack_size = sizeof(canSendBuffer),
    .cb_mem = &canSendControlBlock,
    .cb_size = sizeof(canSendControlBlock),
    .priority = (osPriority_t)osPriorityNormal1,
};
/* Definitions for RxHeaderQue */
osMessageQueueId_t RxHeaderQueHandle;
uint8_t RxHeaderQueBuffer[512 * sizeof(uint32_t)];
osStaticMessageQDef_t RxHeaderQueControlBlock;
const osMessageQueueAttr_t RxHeaderQue_attributes = {
    .name = "RxHeaderQue",
    .cb_mem = &RxHeaderQueControlBlock,
    .cb_size = sizeof(RxHeaderQueControlBlock),
    .mq_mem = &RxHeaderQueBuffer,
    .mq_size = sizeof(RxHeaderQueBuffer)};
/* Definitions for RxDataQue */
osMessageQueueId_t RxDataQueHandle;
uint8_t RxDataQueBuffer[512 * sizeof(uint8_t)];
osStaticMessageQDef_t RxDataQueControlBlock;
const osMessageQueueAttr_t RxDataQue_attributes = {
    .name = "RxDataQue",
    .cb_mem = &RxDataQueControlBlock,
    .cb_size = sizeof(RxDataQueControlBlock),
    .mq_mem = &RxDataQueBuffer,
    .mq_size = sizeof(RxDataQueBuffer)};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t mapDlcToBytes(uint32_t DLC);

int _write(int file, char *ptr, int len) {
  UNUSED(file);
  CDC_Transmit_FS((uint8_t *)ptr, (uint16_t)len);
  return len;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
    /* Retrieve Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) !=
        HAL_OK) {
      /* Reception Error */
      Error_Handler();
    }
    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
                                       0) != HAL_OK) {
      /* Notification Error */
      Error_Handler();
    }
    osMessageQueuePut(RxHeaderQueHandle, &RxHeader.Identifier, 0, 0);
    if (RxHeader.DataLength != FDCAN_DLC_BYTES_0) {
      osMessageQueuePut(RxHeaderQueHandle, &RxHeader.DataLength, 0, 0);
      for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
        osMessageQueuePut(RxDataQueHandle, &RxData[i], 0, 0);
      }
    }
    // osSemaphoreRelease(canSemaphoreHandle);
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
  /* creation of RxHeaderQue */
  RxHeaderQueHandle =
      osMessageQueueNew(512, sizeof(uint32_t), &RxHeaderQue_attributes);

  /* creation of RxDataQue */
  RxDataQueHandle =
      osMessageQueueNew(512, sizeof(uint8_t), &RxDataQue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canReceive */
  canReceiveHandle =
      osThreadNew(StartTaskReceive, NULL, &canReceive_attributes);

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
void StartDefaultTask(void *argument) {
  /* init code for USB_Device */
  MX_USB_Device_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;) {
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
void StartTaskReceive(void *argument) {
  /* USER CODE BEGIN StartTaskReceive */
  /* Infinite loop */
  uint32_t RXID;
  for (;;) {
    if (osMessageQueueGet(RxHeaderQueHandle, &RXID, 0, osWaitForever) ==
        osOK) {
      switch (RXID) {
      case 0x11:
        break;
      case 0x12:
        break;
      case 0x13:
        break;
      case 0x14:
        break;
      default:
        printf("ERROR: NO CANID DEFINED 0x%lx\r\n", RxHeader.Identifier);
        break;
      }
      osMessageQueueReset(RxHeaderQueHandle);
      osMessageQueueReset(RxDataQueHandle);
    }
    osDelay(1);
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
void StartTaskSend(void *argument) {
  /* USER CODE BEGIN StartTaskSend */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
uint32_t mapDlcToBytes(uint32_t DLC) {
  uint32_t bytes;
  if (DLC <= 0x08) {
    return DLC;
  } else {
    switch (DLC) {
    case 0x09:
      bytes = 12;
      break;
    case 0x0A:
      bytes = 16;
      break;
    case 0x0B:
      bytes = 20;
      break;
    case 0x0C:
      bytes = 24;
      break;
    case 0x0D:
      bytes = 32;
      break;
    case 0x0E:
      bytes = 48;
      break;
    case 0x0F:
      bytes = 64;
      break;
    }
  }
  return bytes;
}

/* USER CODE END Application */
