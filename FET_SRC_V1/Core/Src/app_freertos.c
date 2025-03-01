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
#include "cmsis_os2.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "debug-log.h"
#include "ecocar_can.h"
#include "exported_typedef.h"
#include "fdcan.h"
#include "stm32g4xx_hal_def.h"
#include "stm32g4xx_hal_fdcan.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */
typedef enum {
  STANDBY = 500,
  CHARGING = 100,
  RUNNING = 10,
  ALARM = 1
} ledState_t;

typedef struct {
  float current[3];
  float voltage[2];
} fetData_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FULL_CAP_CHARGE_V 20

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SET_BRIGHTNESS(x) (uint32_t)(65535 * x / 100)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
ledState_t led_state = STANDBY;
fetState_t fet_state = FET_STBY;
bool lock_state = false;

// Local data
FDCAN_FetPack_t fet_data = {0};

// External boards data
FDCAN_FccPack_t fcc_data = {0};

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
    .priority = (osPriority_t)osPriorityAboveNormal1,
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
/* Definitions for adcConvTask */
osThreadId_t adcConvTaskHandle;
uint32_t adcConvTaskBuffer[512];
osStaticThreadDef_t adcConvTaskControlBlock;
const osThreadAttr_t adcConvTask_attributes = {
    .name = "adcConvTask",
    .stack_mem = &adcConvTaskBuffer[0],
    .stack_size = sizeof(adcConvTaskBuffer),
    .cb_mem = &adcConvTaskControlBlock,
    .cb_size = sizeof(adcConvTaskControlBlock),
    .priority = (osPriority_t)osPriorityNormal3,
};
/* Definitions for blinkyLed */
osThreadId_t blinkyLedHandle;
uint32_t blinkyLedBuffer[512];
osStaticThreadDef_t blinkyLedControlBlock;
const osThreadAttr_t blinkyLed_attributes = {
    .name = "blinkyLed",
    .stack_mem = &blinkyLedBuffer[0],
    .stack_size = sizeof(blinkyLedBuffer),
    .cb_mem = &blinkyLedControlBlock,
    .cb_size = sizeof(blinkyLedControlBlock),
    .priority = (osPriority_t)osPriorityNormal4,
};
/* Definitions for usbReceive */
osThreadId_t usbHandle;
uint32_t usbBuffer[1024];
osStaticThreadDef_t usbControlBlock;
const osThreadAttr_t usb_attributes = {
    .name = "usb",
    .stack_mem = &usbBuffer[0],
    .stack_size = sizeof(usbBuffer),
    .cb_mem = &usbControlBlock,
    .cb_size = sizeof(usbControlBlock),
    .priority = (osPriority_t)osPriorityNormal5,
};
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
/* Definitions for usbQueReceive */
osMessageQueueId_t usbQueReceiveHandle;
uint8_t usbQueReceiveBuffer[512 * sizeof(char)];
osStaticMessageQDef_t usbQueReceiveControlBlock;
const osMessageQueueAttr_t usbQueReceive_attributes = {
    .name = "usbQueReceive",
    .cb_mem = &usbQueReceiveControlBlock,
    .cb_size = sizeof(usbQueReceiveControlBlock),
    .mq_mem = &usbQueReceiveBuffer,
    .mq_size = sizeof(usbQueReceiveBuffer)};
/* Definitions for usbQueSend */
osMessageQueueId_t usbQueSendHandle;
uint8_t usbQueSendBuffer[512 * sizeof(char)];
osStaticMessageQDef_t usbQueSendControlBlock;
const osMessageQueueAttr_t usbQueSend_attributes = {
    .name = "usbQueSend",
    .cb_mem = &usbQueSendControlBlock,
    .cb_size = sizeof(usbQueSendControlBlock),
    .mq_mem = &usbQueSendBuffer,
    .mq_size = sizeof(usbQueSendBuffer)};
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
void alarmLEDS(void);
float adcToCurr(uint32_t adc_value);
float adcToVolt(uint32_t adc_value);

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[64];
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
    /* Retreive Rx messages from RX FIFO0 */
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
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0, 0);
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.DataLength, 0, 0);
    for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
      osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0);
    }
  }
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanReceive(void *argument);
void StartCanSend(void *argument);
void StartAdcConv(void *argument);
void StartBlinky(void *argument);
extern void StartUsb(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  // Initing these before any threads or interrupts can be called
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // THIRD YELLOW CHANNEL LED1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); // SECOND RED CHANNEL LED2
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // TOP RED CHANNEL LED4
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // BOTTOM GREEN CHANNEL LED3
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

  /* Create the queue(s) */
  /* creation of canQueRxHeader */
  canQueRxHeaderHandle =
      osMessageQueueNew(512, sizeof(uint32_t), &canQueRxHeader_attributes);

  /* creation of canQueRxData */
  canQueRxDataHandle =
      osMessageQueueNew(512, sizeof(uint8_t), &canQueRxData_attributes);

  /* creation of usbQueReceive */
  usbQueReceiveHandle =
      osMessageQueueNew(512, sizeof(char), &usbQueReceive_attributes);

  /* creation of usbQueSend */
  usbQueSendHandle =
      osMessageQueueNew(512, sizeof(char), &usbQueSend_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
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

  /* creation of adcConvTask */
  adcConvTaskHandle = osThreadNew(StartAdcConv, NULL, &adcConvTask_attributes);

  /* creation of blinkyLed */
  blinkyLedHandle = osThreadNew(StartBlinky, NULL, &blinkyLed_attributes);

  /* creation of usbReceive */
  usbHandle = osThreadNew(StartUsb, NULL, &usb_attributes);

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
  /* USER CODE BEGIN StartDefaultTask */
  UNUSED(argument);

  /* Infinite loop */
  for (;;) {
    if (lock_state == true) {
      HAL_GPIO_WritePin(GPIOA,
                        CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                        GPIO_PIN_RESET);
      led_state = ALARM;
    } else {
      switch (fet_state) {
      case FET_STBY:
        // All pins should be in off state. Capacitors discharge through
        // resistor by default.
        HAL_GPIO_WritePin(GPIOA,
                          CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                          GPIO_PIN_RESET);
        led_state = STANDBY;
        fet_data.fet_config = FET_STBY;
        break;
      case FET_CHRGE:
        // Allow fuel cell power through to main bus, into caps, and shut off
        // resistor
        HAL_GPIO_WritePin(GPIOA, CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin,
                          GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, CNTRL_4_Pin, GPIO_PIN_RESET);

        led_state = CHARGING;
        fet_data.fet_config = FET_CHRGE;

        // if CAPACITOR VOL > some value -> go to RUN
        if ((fet_data.cap_volt / FDCAN_FOUR_FLT_PREC) >= FULL_CAP_CHARGE_V) {
          fet_state = FET_RUN;
        }
        break;
      case FET_RUN:
        HAL_GPIO_WritePin(GPIOA,
                          CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                          GPIO_PIN_SET);
        led_state = RUNNING;
        fet_data.fet_config = FET_RUN;
        break;
      }
    }
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
void StartCanReceive(void *argument) {
  /* USER CODE BEGIN StartCanReceive */
  /**
   * THIS SECTION OF CODE UTILIZES A HIGHER PRIORITY SO NO BLOCKING
   * IS ALLOWED TO BE USED OTHER THAN THE SEMAPHORE
   */
  UNUSED(argument);
  FDCAN_RxHeaderTypeDef localRxHeader = {0};
  uint8_t ret[64] = {0};
  /* Infinite loop */
  for (;;) {
    if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.Identifier, 0,
                          osWaitForever) == osOK) {
      if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.DataLength, 0,
                            0) != osOK) {
        Error_Handler();
      }
      for (uint8_t i = 0; i < mapDlcToBytes(localRxHeader.DataLength); i++) {
        if (osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0) != osOK) {
          Error_Handler();
        }
      }
      switch (localRxHeader.Identifier) {
      case FDCAN_H2ALARM_ID:
        // H2 ALARM
        if (ret[0] == 1) {
          lock_state = true;
          log_info("H2 Alarm received!");
        }
        break;
      case FDCAN_SYNCLED_ID:
        // CAN SYNC LED
        if (ret[0] == 1) {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(20);
        } else {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(0);
        }
        break;
      case FDCAN_FCCPACK_ID:
        memcpy(&fcc_data, ret, mapDlcToBytes(localRxHeader.DataLength));
        break;
      case FDCAN_UPDATESTATE_ID:
        fet_state = ret[0];
        log_info("FDCAN_UPDATESTATE_ID Received: 0x%x", fet_state);
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
void StartCanSend(void *argument) {
  /* USER CODE BEGIN StartCanSend */
  UNUSED(argument);
  FDCAN_TxHeaderTypeDef localTxHeader;
  const uint8_t msg_delay = 10;
  uint8_t can_sync_led = 0;
  uint32_t sync_led_last = 0;
  uint32_t sync_led_this = osKernelGetSysTimerCount();

  localTxHeader.IdType = FDCAN_STANDARD_ID;
  localTxHeader.TxFrameType = FDCAN_DATA_FRAME;
  localTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  localTxHeader.BitRateSwitch = FDCAN_BRS_ON;
  localTxHeader.FDFormat = FDCAN_FD_CAN;
  localTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  localTxHeader.MessageMarker = 0;
  /* Infinite loop */
  for (;;) {
    // Sync LEDs
    sync_led_this = osKernelGetTickCount();
    if (sync_led_this - sync_led_last > 500) {
      sync_led_last = sync_led_this;
      localTxHeader.Identifier = FDCAN_SYNCLED_ID;
      localTxHeader.DataLength = FDCAN_DLC_BYTES_1;
      can_sync_led = (can_sync_led == 0) ? 1 : 0;
      if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
                                          &can_sync_led) != HAL_OK) {
          Error_Handler();
        }
        if (can_sync_led == 1) {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(20);
        } else {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(0);
        }

      } else {
        log_warn("Tx Buffer Full");
      }
    }

    // Transmit data
    localTxHeader.Identifier = FDCAN_FETPACK_ID;
    localTxHeader.DataLength = FDCAN_DLC_BYTES_24;
    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
      if (HAL_FDCAN_AddMessageToTxFifoQ(
              &hfdcan2, &localTxHeader,
              (uint8_t *)&fet_data.FDCAN_RawFetPack) != HAL_OK) {
        Error_Handler();
      }
    } else {
      log_warn("Tx Buffer Full");
    }
    osDelay(msg_delay);
  }
  /* USER CODE END StartCanSend */
}

/* USER CODE BEGIN Header_StartAdcConv */
/**
 * @brief Function implementing the adcConvTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAdcConv */
void StartAdcConv(void *argument) {
  /* USER CODE BEGIN StartAdcConv */
  UNUSED(argument);
  uint32_t ADC1_Conversion[4]; // four channels on ADC1
  uint32_t ADC2_Conversion;    // one channel on ADC2
  HAL_ADC_Start_DMA(&hadc1, ADC1_Conversion, 4);
  HAL_ADC_Start_DMA(&hadc2, &ADC2_Conversion, 1);

#define PRINT_FLOAT(x) (float)x / FDCAN_FOUR_FLT_PREC

  uint32_t this_print, last_print = 0;
  /* Infinite loop */
  for (;;) {
    fet_data.cap_curr =
        (uint32_t) (adcToCurr(ADC1_Conversion[0]) * FDCAN_FOUR_FLT_PREC);
    fet_data.res_curr =
        (uint32_t) (adcToCurr(ADC1_Conversion[1]) * FDCAN_FOUR_FLT_PREC);
    fet_data.out_curr =
        (uint32_t) (adcToCurr(ADC1_Conversion[2]) * FDCAN_FOUR_FLT_PREC);
    fet_data.input_volt =
        (uint32_t) (adcToVolt(ADC1_Conversion[3]) * FDCAN_FOUR_FLT_PREC);
    fet_data.cap_volt =
        (uint32_t) (adcToVolt(ADC2_Conversion) * FDCAN_FOUR_FLT_PREC);

    this_print = osKernelGetTickCount();
    if (this_print - last_print >= 1000) {
      last_print = this_print;
      log_info("Cap Curr: %fA\tRes Curr: %fA\tOut Curr: %fA\tFC Volt: %fV\tCap "
               "Volt: %fV\t",
               PRINT_FLOAT(fet_data.cap_curr), PRINT_FLOAT(fet_data.res_curr),
               PRINT_FLOAT(fet_data.out_curr), PRINT_FLOAT(fet_data.input_volt),
               PRINT_FLOAT(fet_data.cap_volt));
    }

    osDelay(10);
  }
  /* USER CODE END StartAdcConv */
}

/* USER CODE BEGIN Header_StartBlinky */
/**
 * @brief Function implementing the blinkyLed thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartBlinky */
void StartBlinky(void *argument) {
  /* USER CODE BEGIN StartBlinky */
  UNUSED(argument);
  /* Infinite loop */
  for (;;) {
    if (led_state != ALARM) {
      htim3.Instance->CCR3 = SET_BRIGHTNESS(70);
      osDelay(led_state);
      htim3.Instance->CCR3 = SET_BRIGHTNESS(0);
      osDelay(led_state);
    } else {
      alarmLEDS();
    }
    osDelay(1);
  }
  /* USER CODE END StartBlinky */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void alarmLEDS(void) {
  htim2.Instance->CCR1 = SET_BRIGHTNESS(20);
  osDelay(50);
  htim1.Instance->CCR3 = SET_BRIGHTNESS(20);
  osDelay(50);
  htim1.Instance->CCR2 = SET_BRIGHTNESS(30);
  osDelay(50);
  htim3.Instance->CCR3 = SET_BRIGHTNESS(70);
  osDelay(50);
  htim2.Instance->CCR1 = SET_BRIGHTNESS(0);
  osDelay(50);
  htim1.Instance->CCR3 = SET_BRIGHTNESS(0);
  osDelay(50);
  htim1.Instance->CCR2 = SET_BRIGHTNESS(0);
  osDelay(50);
  htim3.Instance->CCR3 = SET_BRIGHTNESS(0);
  osDelay(50);
}

float adcToVolt(uint32_t value) {
  const float voltAdcConv = (3.278 / 4096) / (1800.0 / (1800 + 15000));
  return value * voltAdcConv;
}

float adcToCurr(uint32_t value) {
  const float voltAdcConv = (3.278 / 4096) / (4700.0 / (4700 + 9100));
  const float currZeroOffset = 0.8;
  const float currSensitivity = 133.0f / 1000; // V/A
  return (value * voltAdcConv - currZeroOffset) / currSensitivity;
}

/* USER CODE END Application */
