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
#include "stm32g4xx.h"
#include "stm32g4xx_hal_fdcan.h"
#include "stm32g4xx_hal_gpio.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADS1115.h"
#include "debug-log.h"
#include "ecocar_can.h"
#include "exported_typedef.h"
#include "fdcan.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <math.h>
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADS1115_ADR1 0x48
#define TACH_TIMER_INTERVAL 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t TachTracker[4] = {0};
FDCAN_FccPack_t fc_data = {0};
FDCAN_FetPack_t fet_data = {0};

uint8_t button_flags = 0x00;

fetState_t fet_state = FET_STBY;

// Default Purge values
volatile uint32_t purgeDelay_ms = 15000; // time delay between purge is ms
volatile uint32_t purgeTime_ms = 1000;   // purge duration

char ScreenBuffer[32];

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
/* Definitions for valveControl */
osThreadId_t valveControlHandle;
uint32_t valveControlBuffer[512];
osStaticThreadDef_t valveControlControlBlock;
const osThreadAttr_t valveControl_attributes = {
    .name = "valveControl",
    .stack_mem = &valveControlBuffer[0],
    .stack_size = sizeof(valveControlBuffer),
    .cb_mem = &valveControlControlBlock,
    .cb_size = sizeof(valveControlControlBlock),
    .priority = (osPriority_t)osPriorityNormal2,
};
/* Definitions for fuelCellData */
osThreadId_t fuelCellDataHandle;
uint32_t fuelCellDataBuffer[512];
osStaticThreadDef_t fuelCellDataControlBlock;
const osThreadAttr_t fuelCellData_attributes = {
    .name = "fuelCellData",
    .stack_mem = &fuelCellDataBuffer[0],
    .stack_size = sizeof(fuelCellDataBuffer),
    .cb_mem = &fuelCellDataControlBlock,
    .cb_size = sizeof(fuelCellDataControlBlock),
    .priority = (osPriority_t)osPriorityNormal3,
};
/* Definitions for RxHeaderQue */
osMessageQueueId_t canQueRxHeaderHandle;
uint8_t RxHeaderQueBuffer[512 * sizeof(uint32_t)];
osStaticMessageQDef_t RxHeaderQueControlBlock;
const osMessageQueueAttr_t RxHeaderQue_attributes = {
    .name = "RxHeaderQue",
    .cb_mem = &RxHeaderQueControlBlock,
    .cb_size = sizeof(RxHeaderQueControlBlock),
    .mq_mem = &RxHeaderQueBuffer,
    .mq_size = sizeof(RxHeaderQueBuffer)};
/* Definitions for RxDataQue */
osMessageQueueId_t canQueRxDataHandle;
uint8_t RxDataQueBuffer[512 * sizeof(uint8_t)];
osStaticMessageQDef_t RxDataQueControlBlock;
const osMessageQueueAttr_t RxDataQue_attributes = {
    .name = "RxDataQue",
    .cb_mem = &RxDataQueControlBlock,
    .cb_size = sizeof(RxDataQueControlBlock),
    .mq_mem = &RxDataQueBuffer,
    .mq_size = sizeof(RxDataQueBuffer)};
/* Definitions for purgetimer */
osTimerId_t purgetimerHandle;
osStaticTimerDef_t purgetimerControlBlock;
const osTimerAttr_t purgetimer_attributes = {
    .name = "purgetimer",
    .cb_mem = &purgetimerControlBlock,
    .cb_size = sizeof(purgetimerControlBlock),
};

/* Private function prototypes -----------------------------------------------*/

/* Definitions for usbReceive */
osThreadId_t usbHandle;
uint32_t usbBuffer[512];
osStaticThreadDef_t usbControlBlock;
const osThreadAttr_t usb_attributes = {
    .name = "usbReceive",
    .stack_mem = &usbBuffer[0],
    .stack_size = sizeof(usbBuffer),
    .cb_mem = &usbControlBlock,
    .cb_size = sizeof(usbControlBlock),
    .priority = (osPriority_t)osPriorityNormal5,
};

// Not using cubemx to implement this timer
osTimerId_t tachTimerHandle;
osStaticTimerDef_t tachTimerControlBlock;
const osTimerAttr_t tachTimer_attributes = {
    .name = "tachTimer",
    .cb_mem = &tachTimerControlBlock,
    .cb_size = sizeof(tachTimerControlBlock),
};

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

/* USER CODE BEGIN FunctionPrototypes */

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[64];
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
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0, 0);
    osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.DataLength, 0, 0);
    for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
      osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0);
    }
  }
}

uint32_t lastTicks = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  /* Prevent unused argument(s) compilation warning */
  uint32_t ticks = osKernelGetTickCount();
  switch (GPIO_Pin) {
  case TACH1_Pin:
    TachTracker[0]++;
    break;
  case TACH2_Pin:
    TachTracker[1]++;
    break;
  case TACH3_Pin:
    TachTracker[2]++;
    break;
  case TACH4_Pin:
    TachTracker[3]++;
    break;

  case BTN1_Pin: // GPA
    if (ticks - lastTicks >= 1000) {
      lastTicks = ticks;

      // Trigger a FET state change
      SET_BIT(button_flags, 0x01);
    }
    break;
  case BTN2_Pin: // GPB
    //"confirms" the purge timers
    // purgeDelay_ms = adc_delayreading
    // purgeTime_ms = adc_timereading

    break;

  default:
    break;
  }
  /* NOTE: This function should not be modified, when the callback is needed,
   the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}

void calcTachRpmTimer(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskReceive(void *argument);
void StartTaskSend(void *argument);
void StartValveControl(void *argument);
void StartFuelCellData(void *argument);
void purgeValveTimer(void *argument);
extern void StartUsb(void *argument);

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

  /* Create the timer(s) */
  /* creation of purgetimer */
  purgetimerHandle = osTimerNew(purgeValveTimer, osTimerPeriodic, NULL,
                                &purgetimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  tachTimerHandle = osTimerNew(calcTachRpmTimer, osTimerPeriodic, NULL,
                               &tachTimer_attributes);
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of RxHeaderQue */
  canQueRxHeaderHandle =
      osMessageQueueNew(512, sizeof(uint32_t), &RxHeaderQue_attributes);

  /* creation of RxDataQue */
  canQueRxDataHandle =
      osMessageQueueNew(512, sizeof(uint8_t), &RxDataQue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* creation of usbQueReceive */
  usbQueReceiveHandle =
      osMessageQueueNew(512, sizeof(char), &usbQueReceive_attributes);

  /* creation of usbQueSend */
  usbQueSendHandle =
      osMessageQueueNew(512, sizeof(char), &usbQueSend_attributes);

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

  /* creation of valveControl */
  valveControlHandle =
      osThreadNew(StartValveControl, NULL, &valveControl_attributes);

  /* creation of fuelCellData */
  fuelCellDataHandle =
      osThreadNew(StartFuelCellData, NULL, &fuelCellData_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  usbHandle = osThreadNew(StartUsb, NULL, &usb_attributes);
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

  /* TODO: More screen stuff */

  for (;;) {
    // Used for non-essential peripheral control; OLED,POTS,Encoder,

    ssd1306_SetCursor(0, 1); // Adjust Y position as needed
    sprintf(ScreenBuffer, "    IN     OUT");
    ssd1306_WriteString(ScreenBuffer, Font_7x10, White);

    ssd1306_SetCursor(0, 15); // Adjust Y position as needed
    sprintf(ScreenBuffer, "Line 1 TEST");
    ssd1306_WriteString(ScreenBuffer, Font_7x10, White);

    ssd1306_SetCursor(0, 40); // Adjust Y position as needed
    sprintf(ScreenBuffer, "Line 2 TEST ");
    ssd1306_WriteString(ScreenBuffer, Font_11x18, White);

    ssd1306_UpdateScreen(); // Update the screen

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
  FDCAN_RxHeaderTypeDef localRxHeader = {0};
  uint8_t ret[64] = {0};

  /* TODO: Implement receive CAN stuff */

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
          // Once this flag is raised, system reset needs to occur
          // to clear
          SET_BIT(button_flags, 0x80);
        }
        break;
      case FDCAN_SYNCLED_ID:
        // CAN SYNC LED
        if (ret[0] == 1) {
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        } else {
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
        log_info("Syncing LED!");
        break;
      case FDCAN_FETPACK_ID:
        memcpy(&fet_data.FDCAN_RawFetPack, ret,
               mapDlcToBytes(localRxHeader.DataLength));

        // It is possible for the fet state to be updated via usb so
        // we should ensure that this board state aligns with it.
        if (fet_state != fet_data.fet_config) {
          log_info("Updating FCC (0x%x) state to align with FET (0x%x)",
                   fet_state, (unsigned int)fet_data.fet_config);
          fet_state = fet_data.fet_config;
        }
      default:
        break;
      }
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
  UNUSED(argument);

  FDCAN_TxHeaderTypeDef localTxHeader;
  const uint8_t msg_delay = 10;
  uint8_t updateState;


  localTxHeader.IdType = FDCAN_STANDARD_ID;
  localTxHeader.TxFrameType = FDCAN_DATA_FRAME;
  localTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  localTxHeader.BitRateSwitch = FDCAN_BRS_ON;
  localTxHeader.FDFormat = FDCAN_FD_CAN;
  localTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  localTxHeader.MessageMarker = 0;

  for (;;) {
    localTxHeader.Identifier = FDCAN_FCCPACK_ID;
    localTxHeader.DataLength = FDCAN_DLC_BYTES_24;
    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {
      if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
                                        (uint8_t *)&fc_data.FDCAN_RawFccPack) !=
          HAL_OK) {
        Error_Handler();
      }
    } else {
      log_warn("Tx Buffer Full");
    }
    osDelay(msg_delay);

    // If the appropriate button flag is raised and bit 8 is not raised (h2
    // alarm) try to send a new state to FET board
    if (READ_BIT(button_flags, 0x01) == 1 &&
        READ_BIT(button_flags, 0x80) == 0) {

      localTxHeader.Identifier = FDCAN_UPDATESTATE_ID;
      localTxHeader.DataLength = FDCAN_DLC_BYTES_4;

      // If in STBY go to CHRGE otherwise goto STBY
      updateState = (fet_state == FET_STBY) ? FET_CHRGE : FET_STBY;

      if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) != 0) {

        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &localTxHeader,
                                          (uint8_t *)&updateState) != HAL_OK) {
          Error_Handler();
        }

        // Clear the button flag
        CLEAR_BIT(button_flags, 0x01);
      } else {
        // Button flag not cleared
        log_warn("Tx Buffer Full: button flag 0x01 not cleared");
      }
    }
    osDelay(msg_delay);
    /* USER CODE END StartTaskSend */
  }
}

/* USER CODE BEGIN Header_valveContrl */
/**
 * @brief Function implementing the valveControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_valveContrl */
void StartValveControl(void *argument) {
  /* USER CODE BEGIN valveContrl */
  /* Infinite loop */
  uint8_t status;
  uint8_t startupPurge = 0;
  for (;;) {
    // returns 1 or 0 depending on status
    status = osTimerIsRunning(purgetimerHandle);

    // Switch through valid solenoid states
    if (fet_state == FET_STBY) {
      HAL_GPIO_WritePin(SUPPLYvlve_GPIO_Port, SUPPLYvlve_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PURGEvlve_GPIO_Port, PURGEvlve_Pin, GPIO_PIN_RESET);

      if (status == 1) {
        osTimerStop(purgetimerHandle); // kill purge cycle
      }

      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

      startupPurge = 0;

    } else {
      // If either FET_CHRGE or FET_RUN this code is what we need

      HAL_GPIO_WritePin(SUPPLYvlve_GPIO_Port, SUPPLYvlve_Pin, GPIO_PIN_SET);
      if (startupPurge == 0) {
        HAL_GPIO_WritePin(PURGEvlve_GPIO_Port, PURGEvlve_Pin, GPIO_PIN_SET);
        osDelay(purgeTime_ms);
        HAL_GPIO_WritePin(PURGEvlve_GPIO_Port, PURGEvlve_Pin, GPIO_PIN_RESET);
        startupPurge = 1;
      }

      if (status == 0) {
        osTimerStart(purgetimerHandle, purgeDelay_ms);
      }

      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    }
    osDelay(1);
  }
  /* USER CODE END valveContrl */
}

/* USER CODE BEGIN Header_StartFuelCellData */
/**
 * @brief Function implementing the fuelCellData thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFuelCellData */
void StartFuelCellData(void *argument) {
  /* USER CODE BEGIN StartFuelCellData */
  // Following for
#define DELAY_FOR_CHANNEL_SWITCH 20
#define B 3950.0f
#define VOLT_2_TEMP(x)                                                         \
  (B * 298.15f /                                                               \
   (298.15f * logf(100.0f / (100.0f * (3.3f / x - 1.0f))) + B)) -              \
      273.15f
#define VOLT_2_PRES(x) (x - 2.3555F) / 0.1038F

  const float VOLT_CONVERSION = 4.094F / 32768.0F;
  const float TRANSFER_FUNC_P = 0.657F;

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  osTimerStart(tachTimerHandle, TACH_TIMER_INTERVAL);
  // Used to see in between steps of ADC
  uint16_t step;
  float step2;

  /* Infinite loop */
  for (;;) {
    // Gather ADC Values for temp & pressure from fuel cell over I2C
    configReg.channel = CHANNEL_AIN0_GND;
    ADS1115_updateConfig(pADS_1, configReg);
    osDelay(DELAY_FOR_CHANNEL_SWITCH);
    step = ADS1115_getData(pADS_1);
    step2 = step * VOLT_CONVERSION;
    fc_data.fc_temp = (uint32_t)(VOLT_2_TEMP(step2) * FDCAN_FOUR_FLT_PREC);

    configReg.channel = CHANNEL_AIN1_GND;
    ADS1115_updateConfig(pADS_1, configReg);
    osDelay(DELAY_FOR_CHANNEL_SWITCH);
    step = ADS1115_getData(pADS_1);
    step2 = step * VOLT_CONVERSION;
    fc_data.fc_press =
        (uint32_t)(VOLT_2_PRES(step2 / TRANSFER_FUNC_P) * FDCAN_FOUR_FLT_PREC);

    // TODO: Implement fan control
    htim2.Instance->CCR2 = 50;
    htim3.Instance->CCR1 = 50;

    osDelay(10);
  }
  /* USER CODE END StartFuelCellData */
}

/* Callback01 function */
void purgeValveTimer(void *argument) {
  /* USER CODE BEGIN Callback01 */
  // This is the PURGE TIMER callback. Name should probably be changed to
  // better reflect that
  HAL_GPIO_WritePin(PURGEvlve_GPIO_Port, PURGEvlve_Pin, GPIO_PIN_SET);
  osDelay(purgeTime_ms); // Replace with purgeDelay
  HAL_GPIO_WritePin(PURGEvlve_GPIO_Port, PURGEvlve_Pin, GPIO_PIN_RESET);
  osDelay(1);
  /* USER CODE END Callback01 */
}

void calcTachRpmTimer(void *argument) {
  // TACH_TIMER_INTERVAL is in ms and there are two pulses per period
  fc_data.fan_rpm1 = (TachTracker[0] / (TACH_TIMER_INTERVAL / 1000 * 2));
  fc_data.fan_rpm2 = (TachTracker[1] / (TACH_TIMER_INTERVAL / 1000 * 2));
  TachTracker[0] = TachTracker[1] = 0;

  // Currently these are unused
  TachTracker[2] = TachTracker[3] = 0;
}

/* Private application code
 * --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */
