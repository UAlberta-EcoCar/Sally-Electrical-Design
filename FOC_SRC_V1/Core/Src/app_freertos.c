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
#include "cmsis_os2.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ecocar_can.h"
#include "exported_typedef.h"
#include "fdcan.h"
#include "tim.h"
#include "ws2812.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
FDCAN_FccPack_t fcc_data;
FDCAN_FetPack_t fet_data;
FDCAN_H2Pack_t h2_data;
FDCAN_BOOSTPack_t boost_data;
uint8_t can_sync_led;

rbState_t relay_state;
FDCAN_RelPackFc_t relay_fc_pack;
FDCAN_RelPackCap_t relay_cap_pack;
FDCAN_RelPackMtr_t relay_motor_pack;
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
/* Definitions for canReceiveMsg */
osThreadId_t canReceiveMsgHandle;
uint32_t CanReceiveMsgBuffer[512];
osStaticThreadDef_t CanReceiveMsgControlBlock;
const osThreadAttr_t canReceiveMsg_attributes = {
    .name = "canReceiveMsg",
    .stack_mem = &CanReceiveMsgBuffer[0],
    .stack_size = sizeof(CanReceiveMsgBuffer),
    .cb_mem = &CanReceiveMsgControlBlock,
    .cb_size = sizeof(CanReceiveMsgControlBlock),
    .priority = (osPriority_t)osPriorityNormal1,
};
/* Definitions for canSendMsg */
osThreadId_t canSendMsgHandle;
uint32_t CanSendMsgBuffer[512];
osStaticThreadDef_t CanSendMsgControlBlock;
const osThreadAttr_t canSendMsg_attributes = {
    .name = "canSendMsg",
    .stack_mem = &CanSendMsgBuffer[0],
    .stack_size = sizeof(CanSendMsgBuffer),
    .cb_mem = &CanSendMsgControlBlock,
    .cb_size = sizeof(CanSendMsgControlBlock),
    .priority = (osPriority_t)osPriorityNormal2,
};
/* Definitions for canSemaphore */
osSemaphoreId_t canSemaphoreHandle;
osStaticSemaphoreDef_t canSemaphoreControlBlock;
const osSemaphoreAttr_t canSemaphore_attributes = {
    .name = "canSemaphore",
    .cb_mem = &canSemaphoreControlBlock,
    .cb_size = sizeof(canSemaphoreControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* Definitions for canQueRxHeader */
osMessageQueueId_t canQueRxHeaderHandle;
uint8_t canReceiveQueBuffer[512 * sizeof(uint32_t)];
osStaticMessageQDef_t canReceiveQueControlBlock;
const osMessageQueueAttr_t canQueRxHeader_attributes = {
    .name = "canQueRxHeader",
    .cb_mem = &canReceiveQueControlBlock,
    .cb_size = sizeof(canReceiveQueControlBlock),
    .mq_mem = &canReceiveQueBuffer,
    .mq_size = sizeof(canReceiveQueBuffer)};
/* Definitions for canQueRxData */
osMessageQueueId_t canQueRxDataHandle;
uint8_t canQueRxDataBuffer[512 * sizeof(uint8_t)];
osStaticMessageQDef_t canQueRxDataControlBlock;
const osMessageQueueAttr_t canQueRxData_attributes = {
    .name = "canQueRxData",
    .cb_mem = &canQueRxDataControlBlock,
    .cb_size = sizeof(canQueRxDataControlBlock),
    .mq_mem = &canQueRxDataBuffer,
    .mq_size = sizeof(canQueRxDataBuffer)};

// Callback needed for LEDs
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim2.Instance)
  {
    WS2812_Callback();
  }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs)
{
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[64];
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    /* Retreive Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) !=
        HAL_OK)
    {
      /* Reception Error */
      Error_Handler();
    }
    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
                                       0) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0, 0);
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.DataLength, 0, 0);
    for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++)
    {
      osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0);
    }
  }
}
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
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of canSemaphore */
  canSemaphoreHandle = osSemaphoreNew(1, 0, &canSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  canQueRxHeaderHandle =
      osMessageQueueNew(512, sizeof(uint32_t), &canQueRxHeader_attributes);

  /* creation of canQueRxData */
  canQueRxDataHandle =
      osMessageQueueNew(512, sizeof(uint8_t), &canQueRxData_attributes);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canReceiveMsg */
  canReceiveMsgHandle =
      osThreadNew(StartCanReceive, NULL, &canReceiveMsg_attributes);

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
  /* USER CODE BEGIN StartDefaultTask */
  if (WS2812_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /* Infinite loop */
  HAL_StatusTypeDef hal_stat;
  for (;;)
  {
    if (can_sync_led == 1)
    {
      switch (relay_state)
      {
      case RELAY_STBY:
        hal_stat = WS2812_Idle_Animation(200);
        break;
      case RELAY_CHRGE:
        hal_stat = WS2812_Charging_Animation(200);
        break;
      case RELAY_RUN:
        hal_stat = WS2812_Driving_Animation(200);
        break;
      case RELAY_STRTP:
        hal_stat = WS2812_Stop_Animation(200);
        break;
      }
    }
    else
    {
      // hal_stat = WS2812_Charging_Animation(300);
      // hal_stat = WS2812_Driving_Animation(300);
      hal_stat = WS2812_Idle_Animation(200);
      // hal_stat = WS2812_Stop_Animation(200);
    }
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
  /**
   * THIS SECTION OF CODE UTILIZES A HIGHER PRIORITY SO NO BLOCKING
   * IS ALLOWED TO BE USED OTHER THAN THE SEMAPHORE
   */
  UNUSED(argument);
  FDCAN_RxHeaderTypeDef localRxHeader = {0};
  uint8_t ret[64] = {0};
  /* Infinite loop */
  for (;;)
  {
    if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.Identifier, 0,
                          osWaitForever) == osOK)
    {
      if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.DataLength, 0,
                            0) != osOK)
      {
        Error_Handler();
      }
      for (uint8_t i = 0; i < mapDlcToBytes(localRxHeader.DataLength); i++)
      {
        if (osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0) != osOK)
        {
          Error_Handler();
        }
      }
      switch (localRxHeader.Identifier)
      {
      case FDCAN_H2ALARM_ID:
        // H2 ALARM
        if (ret[0] == 1)
        {
        }
        break;
      case FDCAN_SYNCLED_ID:
        // CAN SYNC LED
        can_sync_led = ret[0];
        break;
      case FDCAN_FCCPACK_ID:
        memcpy(&fcc_data, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_FETPACK_ID:
        memcpy(&fet_data, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_H2PACK_ID:
        memcpy(&h2_data, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_BOOSTPACK_ID:
        memcpy(&boost_data, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_RELSTATE_ID:
        memcpy(&relay_state, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_RELPACKFC_ID:
        memcpy(&relay_fc_pack, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_RELPACKCAP_ID:
        memcpy(&relay_cap_pack, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_RELPACKMTR_ID:
        memcpy(&relay_motor_pack, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      default:
        break;
      }
    }
  }
  osDelay(1);
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
  FDCAN_TxHeaderTypeDef localTxHeader;
  const uint8_t msg_delay = 10;

  localTxHeader.IdType = FDCAN_STANDARD_ID;
  localTxHeader.TxFrameType = FDCAN_DATA_FRAME;
  localTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  localTxHeader.BitRateSwitch = FDCAN_BRS_ON;
  localTxHeader.FDFormat = FDCAN_FD_CAN;
  localTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  localTxHeader.MessageMarker = 0;
  /* Infinite loop */
  for (;;)
  {
    osDelay(msg_delay);
  }
  /* USER CODE END StartCanSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
