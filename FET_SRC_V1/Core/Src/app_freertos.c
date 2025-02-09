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
#include "main.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "ecocar_can.h"
#include "fdcan.h"
#include "tim.h"
#include "exported_typedef.h"
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
fetData_t fet_data = {.current = {0x00001234, 0x00001234, 0x00001234},
                    .voltage = {0x00005678, 0x00005678}};

const float voltAdcConv = (3.278f / 4096) / (1800.0f / (1800 + 15000));
const float currAdcConv = (3.278f / 4096) / (9100.0f / (9100 + 4700));
const uint8_t currSenseVCC = 5;
const float currZeroOffset = 0.515;
const float currSensitivity = 133.0f / 1000; // V/A

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
  .priority = (osPriority_t) osPriorityAboveNormal1,
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
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for blinkyLed */
osThreadId_t blinkyLedHandle;
uint32_t blinkyLedBuffer[ 512 ];
osStaticThreadDef_t blinkyLedControlBlock;
const osThreadAttr_t blinkyLed_attributes = {
  .name = "blinkyLed",
  .stack_mem = &blinkyLedBuffer[0],
  .stack_size = sizeof(blinkyLedBuffer),
  .cb_mem = &blinkyLedControlBlock,
  .cb_size = sizeof(blinkyLedControlBlock),
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for usbReceive */
osThreadId_t usbReceiveHandle;
uint32_t usbReceiveBuffer[ 512 ];
osStaticThreadDef_t usbReceiveControlBlock;
const osThreadAttr_t usbReceive_attributes = {
  .name = "usbReceive",
  .stack_mem = &usbReceiveBuffer[0],
  .stack_size = sizeof(usbReceiveBuffer),
  .cb_mem = &usbReceiveControlBlock,
  .cb_size = sizeof(usbReceiveControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for canQueRxHeader */
osMessageQueueId_t canQueRxHeaderHandle;
uint8_t canReceiveQueBuffer[ 512 * sizeof( uint32_t ) ];
osStaticMessageQDef_t canReceiveQueControlBlock;
const osMessageQueueAttr_t canQueRxHeader_attributes = {
  .name = "canQueRxHeader",
  .cb_mem = &canReceiveQueControlBlock,
  .cb_size = sizeof(canReceiveQueControlBlock),
  .mq_mem = &canReceiveQueBuffer,
  .mq_size = sizeof(canReceiveQueBuffer)
};
/* Definitions for canQueRxData */
osMessageQueueId_t canQueRxDataHandle;
uint8_t canQueRxDataBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canQueRxDataControlBlock;
const osMessageQueueAttr_t canQueRxData_attributes = {
  .name = "canQueRxData",
  .cb_mem = &canQueRxDataControlBlock,
  .cb_size = sizeof(canQueRxDataControlBlock),
  .mq_mem = &canQueRxDataBuffer,
  .mq_size = sizeof(canQueRxDataBuffer)
};
/* Definitions for usbQueReceive */
osMessageQueueId_t usbQueReceiveHandle;
uint8_t usbQueReceiveBuffer[ 512 * sizeof( char ) ];
osStaticMessageQDef_t usbQueReceiveControlBlock;
const osMessageQueueAttr_t usbQueReceive_attributes = {
  .name = "usbQueReceive",
  .cb_mem = &usbQueReceiveControlBlock,
  .cb_size = sizeof(usbQueReceiveControlBlock),
  .mq_mem = &usbQueReceiveBuffer,
  .mq_size = sizeof(usbQueReceiveBuffer)
};
/* Definitions for usbQueSend */
osMessageQueueId_t usbQueSendHandle;
uint8_t usbQueSendBuffer[ 512 * sizeof( char ) ];
osStaticMessageQDef_t usbQueSendControlBlock;
const osMessageQueueAttr_t usbQueSend_attributes = {
  .name = "usbQueSend",
  .cb_mem = &usbQueSendControlBlock,
  .cb_size = sizeof(usbQueSendControlBlock),
  .mq_mem = &usbQueSendBuffer,
  .mq_size = sizeof(usbQueSendBuffer)
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
void funCTION(void *argument);
float adcToCurr(uint32_t adc_value);
float adcToVolt(uint32_t adc_value);

int _write(int file, char *ptr, int len) {
  UNUSED(file);
  CDC_Transmit_FS((uint8_t*)ptr, len);
  return len;
}

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
void funCTION(void *argument);
float adcToCurr(uint32_t adc_value);
float adcToVolt(uint32_t adc_value);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanReceive(void *argument);
void StartCanSend(void *argument);
void StartAdcConv(void *argument);
void StartBlinky(void *argument);
extern void StartUsbReceive(void *argument);

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
  canQueRxHeaderHandle = osMessageQueueNew (512, sizeof(uint32_t), &canQueRxHeader_attributes);

  /* creation of canQueRxData */
  canQueRxDataHandle = osMessageQueueNew (512, sizeof(uint8_t), &canQueRxData_attributes);

  /* creation of usbQueReceive */
  usbQueReceiveHandle = osMessageQueueNew (512, sizeof(char), &usbQueReceive_attributes);

  /* creation of usbQueSend */
  usbQueSendHandle = osMessageQueueNew (512, sizeof(char), &usbQueSend_attributes);

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

  /* creation of adcConvTask */
  adcConvTaskHandle = osThreadNew(StartAdcConv, NULL, &adcConvTask_attributes);

  /* creation of blinkyLed */
  blinkyLedHandle = osThreadNew(StartBlinky, NULL, &blinkyLed_attributes);

  /* creation of usbReceive */
  usbReceiveHandle = osThreadNew(StartUsbReceive, NULL, &usbReceive_attributes);

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
  fet_state = FET_STBY;

  for (;;) {
    switch (fet_state) {
    case FET_STBY:
      // All pins should be in off state. Capacitors discharge through
      // resistor by default.
      HAL_GPIO_WritePin(GPIOA, CNTRL_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_4_Pin, GPIO_PIN_RESET);
      led_state = STANDBY;
      break;
    case FET_CHRGE:
      // Allow fuel cell power through to main bus, into caps, and shut off
      // resistor
      HAL_GPIO_WritePin(GPIOA, CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA, CNTRL_4_Pin, GPIO_PIN_RESET);
      led_state = CHARGING;
      // if CAPACITOR VOL > some value -> go to RUN
      break;
    case FET_RUN:
      HAL_GPIO_WritePin(GPIOA,
                        CNTRL_1_Pin | CNTRL_2_Pin | CNTRL_3_Pin | CNTRL_4_Pin,
                        GPIO_PIN_SET);
        led_state = RUNNING;
      break;
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
void StartCanReceive(void *argument)
{
  /* USER CODE BEGIN StartCanReceive */
  /**
   * THIS SECTION OF CODE UTILIZES A HIGHER PRIORITY SO NO BLOCKING
   * IS ALLOWED TO BE USED OTHER THAN THE SEMAPHORE
   */
  UNUSED(argument);
  FDCAN_RxHeaderTypeDef myheader = {0};
  uint8_t ret[64] = {0};
  FDCAN_FetPack_t mypack = {0};
  /* Infinite loop */
  for (;;) {
    if (osMessageQueueGet(canQueRxHeaderHandle, &myheader.Identifier, 0,
                          osWaitForever) == osOK) {
      switch (myheader.Identifier) {
      case 0x11:
        if (htim2.Instance->CCR1 == SET_BRIGHTNESS(20)) {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(0);
        } else {
          htim2.Instance->CCR1 = SET_BRIGHTNESS(20);
        }
        osMessageQueueGet(canQueRxHeaderHandle, &myheader.DataLength, 0, 0);
        for (uint32_t i = 0; i < mapDlcToBytes(myheader.DataLength); i++) {
          osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0);
        }
        memcpy(mypack.FDCAN_RawFetPack, ret,
               mapDlcToBytes(myheader.DataLength));
        /*printf("RELAY STATE: %d IN VOLT: %d CAP VOLT: %d CAP CURR: %d RES "*/
        /*       "CURR: %d OUT CURR %d\r\n",*/
        /*       mypack.fet_config, mypack.input_volt, mypack.cap_volt,*/
        /*       mypack.cap_curr, mypack.res_curr, mypack.out_curr);*/
        break;
      case 0x12:
        osMessageQueueGet(canQueRxHeaderHandle, &myheader.DataLength, 0, 0);
        for (uint32_t i = 0; i < mapDlcToBytes(myheader.DataLength); i++) {
          osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0);
        }
        break;
      case 0x13:
        osMessageQueueGet(canQueRxHeaderHandle, &myheader.DataLength, 0, 0);
        for (uint32_t i = 0; i < mapDlcToBytes(myheader.DataLength); i++) {
          osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0);
        }
        break;
      case 0x14:
        osMessageQueueGet(canQueRxHeaderHandle, &myheader.DataLength, 0, 0);
        for (uint32_t i = 0; i < mapDlcToBytes(myheader.DataLength); i++) {
          osMessageQueueGet(canQueRxDataHandle, &ret[i], 0, 0);
        }
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
  FDCAN_TxHeaderTypeDef fet_TxHeader;

  fet_TxHeader.IdType = FDCAN_STANDARD_ID;
  fet_TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  fet_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  fet_TxHeader.BitRateSwitch = FDCAN_BRS_ON;
  fet_TxHeader.FDFormat = FDCAN_FD_CAN;
  fet_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  fet_TxHeader.MessageMarker = 0;
  /* Infinite loop */

  FDCAN_FetPack_t mypack = {0};
  mypack.fet_config = (uint32_t)fet_state;
  mypack.input_volt = (uint32_t)(11.0454389f * 10000);
  mypack.cap_volt = (uint32_t)(22.0454389f * 10000);
  mypack.cap_curr = (uint32_t)(33.0454389f * 10000);
  mypack.res_curr = (uint32_t)(44.0454389f * 10000);
  mypack.out_curr = (uint32_t)(55.0454389f * 10000);


  for (;;) {
    /*fet_TxHeader.Identifier = 0x11;*/
    /*fet_TxHeader.DataLength = FDCAN_DLC_BYTES_24;*/
    /*if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,*/
    /*                                  (uint8_t *)&mypack.FDCAN_RawFetPack) !=*/
    /*    HAL_OK) {*/
    /*  Error_Handler();*/
    /*}*/
    /*osDelay(1);*/
    /*fet_TxHeader.Identifier = 0x12;*/
    /*fet_TxHeader.DataLength = FDCAN_DLC_BYTES_24;*/
    /*if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,*/
    /*                                  (uint8_t *)&mypack.FDCAN_RawFetPack) !=*/
    /*    HAL_OK) {*/
    /*  Error_Handler();*/
    /*}*/
    /*fet_TxHeader.Identifier = 0x13;*/
    /*fet_TxHeader.DataLength = FDCAN_DLC_BYTES_24;*/
    /*if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,*/
    /*                                  (uint8_t *)&mypack.FDCAN_RawFetPack) !=*/
    /*    HAL_OK) {*/
    /*  Error_Handler();*/
    /*}*/
    /*fet_TxHeader.Identifier = 0x14;*/
    /*fet_TxHeader.DataLength = FDCAN_DLC_BYTES_24;*/
    /*if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &fet_TxHeader,*/
    /*                                  (uint8_t *)&mypack.FDCAN_RawFetPack) !=*/
    /*    HAL_OK) {*/
    /*  Error_Handler();*/
    /*}*/
    osDelay(100);
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
void StartAdcConv(void *argument)
{
  /* USER CODE BEGIN StartAdcConv */
  UNUSED(argument);
  uint32_t ADC1_Conversion[4]; // four channels on ADC1
  uint32_t ADC2_Conversion;    // one channel on ADC2
  HAL_ADC_Start_DMA(&hadc1, ADC1_Conversion, 4);
  HAL_ADC_Start_DMA(&hadc2, &ADC2_Conversion, 1);
  /* Infinite loop */
  for (;;) {
    for (int i = 0; i < 3; i++) {
      fet_data.current[i] = adcToCurr(ADC1_Conversion[i]);
    }
    fet_data.voltage[0] = adcToVolt(ADC1_Conversion[3]);
    fet_data.voltage[1] = adcToVolt(ADC2_Conversion);
    osDelay(1);
    printf("IN VOLT: %f | CAP VOLT: %f\r\n", fet_data.voltage[0], fet_data.voltage[1]);
    osDelay(1000);
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
void StartBlinky(void *argument)
{
  /* USER CODE BEGIN StartBlinky */
  UNUSED(argument);
  /* Infinite loop */
  for(;;)
  {
    htim3.Instance->CCR3 = SET_BRIGHTNESS(70);
    osDelay(led_state);
    htim3.Instance->CCR3 = SET_BRIGHTNESS(0);
    osDelay(led_state);
  }
  /* USER CODE END StartBlinky */
}

/* Private application code --------------------------------------------------*/
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

float adcToVolt(uint32_t value) {
  float ret;
  ret = value * voltAdcConv;
  return ret;
}
float adcToCurr(uint32_t value) {
  float ret;
  ret = (value * voltAdcConv - currZeroOffset) / currSensitivity;
  return ret;
}

/* USER CODE END Application */

