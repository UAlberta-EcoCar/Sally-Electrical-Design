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
#include "stm32g4xx_hal_flash_ramfunc.h"
#include "stm32g4xx_hal_tim.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "ws2812.h"
#include <math.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
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
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == htim2.Instance) {
    WS2812_Callback();
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
  /* Infinite loop */
  HAL_StatusTypeDef hal_stat;
  uint8_t r[WS2812_NUM_LEDS] = {0};
  uint8_t g[WS2812_NUM_LEDS] = {0};
  uint8_t b[WS2812_NUM_LEDS] = {0};
  uint8_t offSet = floor(256.0F / WS2812_NUM_LEDS);
  const uint8_t ws2812_color_time = 250;

  if (WS2812_Init() != HAL_OK) {
    Error_Handler();
  }


  // Start each led off at a different color point
  for (uint8_t ledIndex = 0; ledIndex < WS2812_NUM_LEDS; ledIndex++) {
    r[ledIndex] = ledIndex * offSet;
    g[ledIndex] = ledIndex * offSet;
    b[ledIndex] = ledIndex * offSet;
  }

  for (;;) {
    for (uint8_t ledIndex = 0; ledIndex < WS2812_NUM_LEDS; ledIndex++) {
      WS2812_SetColor(ledIndex, r[ledIndex]++, g[ledIndex]++, b[ledIndex]++);
    }
    do {
      hal_stat = WS2812_Update();
    } while (hal_stat != HAL_OK);
    osDelay(ws2812_color_time);
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
  /* Infinite loop */
  for (;;) {
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
void StartCanSend(void *argument) {
  /* USER CODE BEGIN StartCanSend */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartCanSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
