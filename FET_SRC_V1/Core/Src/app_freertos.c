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
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "main.h"
#include "stm32g4xx_hal_fdcan.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "fdcan.h"
#include "tim.h"
#include "tusb.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */
typedef enum {
  ALL_FET_OFF = 0x00,
  FUELCELL_FET = 0x01,
  CAP_FET = 0x02,
  RES_FET = 0x04,
  OUT_FET = 0x08,
} relayBit_t;

typedef enum {
  FET_STBY = ALL_FET_OFF,
  FET_CHRGE = FUELCELL_FET | CAP_FET | RES_FET,
  FET_RUN = FUELCELL_FET | CAP_FET | RES_FET | OUT_FET,
} rbState_t;

typedef struct {
  float current[3];
  float voltage[2];
} rbData_t;

typedef struct {
  uint8_t H2_OK;
} canData_t;
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
rbState_t rb_state = FET_STBY;
rbData_t rb_data;

FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

uint8_t RxData[64];
uint8_t TxData[64];

const float voltAdcConv = (3.278f / 4096) / (1800.0f / (1800 + 15000));
const float currAdcConv = (3.278f / 4096) / (9100.0f / (9100 + 4700));
const uint8_t currSenseVCC = 5;
const float currZeroOffset = 0.515;
const float currSensitivity = 133.0f / 1000; // V/A

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
/* Definitions for canReceiveQue */
osMessageQueueId_t canReceiveQueHandle;
uint8_t canReceiveQueBuffer[512 * sizeof(uint32_t)];
osStaticMessageQDef_t canReceiveQueControlBlock;
const osMessageQueueAttr_t canReceiveQue_attributes = {
    .name = "canReceiveQue",
    .cb_mem = &canReceiveQueControlBlock,
    .cb_size = sizeof(canReceiveQueControlBlock),
    .mq_mem = &canReceiveQueBuffer,
    .mq_size = sizeof(canReceiveQueBuffer)};
/* Definitions for canSendQue */
osMessageQueueId_t canSendQueHandle;
uint8_t canSendQueBuffer[512 * sizeof(uint8_t)];
osStaticMessageQDef_t canSendQueControlBlock;
const osMessageQueueAttr_t canSendQue_attributes = {
    .name = "canSendQue",
    .cb_mem = &canSendQueControlBlock,
    .cb_size = sizeof(canSendQueControlBlock),
    .mq_mem = &canSendQueBuffer,
    .mq_size = sizeof(canSendQueBuffer)};
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
int _write(int file, char *ptr, int len) {
  UNUSED(file);
  CDC_Transmit_FS((uint8_t *)ptr, (uint16_t)len);
  return len;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
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
    osMessageQueuePut(canReceiveQueHandle, &RxHeader.Identifier, 0, 0);
    if (RxHeader.DataLength != FDCAN_DLC_BYTES_0) {
      osMessageQueuePut(canReceiveQueHandle, &RxHeader.DataLength, 0, 0);
      for (uint32_t i = 0; i < RxHeader.DataLength; i++) {
        osMessageQueuePut(canReceiveQueHandle, &RxData[i], 0, 0);
      }
    }
    // osSemaphoreRelease(canSemaphoreHandle);
  }
}
void funCTION(void *argument);
float adcToCurr(uint32_t adc_value);
float adcToVolt(uint32_t adc_value);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanReceive(void *argument);
void StartCanSend(void *argument);
void StartAdcConv(void *argument);

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
  /* creation of canReceiveQue */
  canReceiveQueHandle =
      osMessageQueueNew(512, sizeof(uint32_t), &canReceiveQue_attributes);

  /* creation of canSendQue */
  canSendQueHandle =
      osMessageQueueNew(512, sizeof(uint8_t), &canSendQue_attributes);

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

  // tusb_rhport_init_t dev_init = {
  //   .role = TUSB_ROLE_DEVICE,
  //   .speed = TUSB_SPEED_AUTO
  // };
  // if (tusb_rhport_init(0, &dev_init) != true) {
  //   Error_Handler();
  // };

  /* USER CODE BEGIN StartDefaultTask */
  UNUSED(argument);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // THIRD YELLOW CHANNEL LED1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); // SECOND RED CHANNEL LED2
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // TOP RED CHANNEL LED4
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // BOTTOM GREEN CHANNEL LED3

#define DELAY pdMS_TO_TICKS(500)
  /* Infinite loop */
  rb_state = FET_STBY;
  for (;;) {
    switch (rb_state) {
    case FET_STBY:
      // All pins should be in off state. Capacitors discharge through resistor
      // by default.
      HAL_GPIO_WritePin(GPIOA, CNTRL_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                        GPIO_PIN_RESET);
      break;
    case FET_CHRGE:
      // Allow fuel cell power through to main bus, into caps, and shut off
      // resistor
      HAL_GPIO_WritePin(GPIOA, CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_4_Pin, GPIO_PIN_RESET);
      // if CAPACITOR VOL > some value -> go to RUN
      break;
    case FET_RUN:
      HAL_GPIO_WritePin(GPIOA,
                        CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                        GPIO_PIN_SET);
      break;
    }
    osDelay(10);
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
   *
   */
  UNUSED(argument);
  uint32_t RXID, DLC;
  /* Infinite loop */
  for (;;) {
    // if (osSemaphoreAcquire(canSemaphoreHandle, osWaitForever) == osOK) {
    if (osMessageQueueGet(canReceiveQueHandle, &RXID, 0, osWaitForever) ==
        osOK) {
      switch (RXID) {
      case 0x11:
        if (htim2.Instance->CCR1 == SET_BRIGHTNESS(20)) {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(0);
        } else {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(20);
        }
        break;
      case 0x12:
        if (htim1.Instance->CCR3 == SET_BRIGHTNESS(20)) {
          htim1.Instance->CCR3 = SET_BRIGHTNESS(0);
        } else {
          htim1.Instance->CCR3 = SET_BRIGHTNESS(20);
        }
        break;
      case 0x13:
        if (htim1.Instance->CCR2 == SET_BRIGHTNESS(30)) {
          htim1.Instance->CCR2 = SET_BRIGHTNESS(0);
        } else {
          htim1.Instance->CCR2 = SET_BRIGHTNESS(30);
        }
        break;
      case 0x14:
        if (htim3.Instance->CCR3 == SET_BRIGHTNESS(70)) {
          htim3.Instance->CCR3 = SET_BRIGHTNESS(0);
        } else {
          htim3.Instance->CCR3 = SET_BRIGHTNESS(70);
        }
        break;
      default:
        printf("ERROR: NO CANID DEFINED 0x%x\r\n", RxHeader.Identifier);
        break;
      }
      osMessageQueueReset(canReceiveQueHandle);
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
#define wait 1
  UNUSED(argument);
  FDCAN_TxHeaderTypeDef fet_TxHeader;
  uint8_t fet_TxData[64] = {0};

  fet_TxHeader.IdType = FDCAN_STANDARD_ID;
  fet_TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  fet_TxHeader.DataLength = FDCAN_DLC_BYTES_64;
  fet_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  fet_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  fet_TxHeader.FDFormat = FDCAN_FD_CAN;
  fet_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  fet_TxHeader.MessageMarker = 0;
  /* Infinite loop */

  for (;;) {
    fet_TxHeader.Identifier = 0x11;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader, fet_TxData) !=
        HAL_OK) {
      Error_Handler();
    }
    osDelay(wait);

    fet_TxHeader.Identifier = 0x12;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,
                                      (uint8_t *)&rb_state) != HAL_OK) {
      Error_Handler();
    }
    osDelay(wait);

    fet_TxHeader.Identifier = 0x13;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,
                                      (uint8_t *)&rb_state) != HAL_OK) {
      Error_Handler();
    }
    osDelay(wait);

    fet_TxHeader.Identifier = 0x14;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,
                                      (uint8_t *)&rb_state) != HAL_OK) {
      Error_Handler();
    }
    osDelay(wait);
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
  /* Infinite loop */
  for (;;) {
    for (int i = 0; i < 3; i++) {
      rb_data.current[i] = adcToCurr(ADC1_Conversion[i]);
    }
    rb_data.voltage[0] = adcToVolt(ADC1_Conversion[3]);
    rb_data.voltage[1] = adcToVolt(ADC2_Conversion);
    printf("ADC Conversion Values: C:%u, C:%u, C:%u, V:%u, V:%u\r\n",
           ADC1_Conversion[0], ADC1_Conversion[1], ADC1_Conversion[2],
           ADC1_Conversion[3], ADC2_Conversion);
    osDelay(1);
    printf("CURRENT VALUES: C:%f, C:%f, C:%f\r\n", rb_data.current[0],
           rb_data.current[1], rb_data.current[2]);
    osDelay(1);
    printf("VOLTAGE VALUES: V_INPUT:%f, V_OUTPUT:%f\r\n", rb_data.voltage[0],
           rb_data.voltage[1]);
    osDelay(1000);
  }
  /* USER CODE END StartAdcConv */
}

/* Private application code --------------------------------------------------*/
float adcToCurr(uint32_t adc_value) {
  float value;
  value = (adc_value * currAdcConv - currZeroOffset) / currSensitivity;
  return value;
}
float adcToVolt(uint32_t adc_value) {
  float value;
  value = adc_value * voltAdcConv;
  return value;
}
/* USER CODE BEGIN Application */
void funCTION(void *argument) {
  UNUSED(argument);
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
/* USER CODE END Application */
