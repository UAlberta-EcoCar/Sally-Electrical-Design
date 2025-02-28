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
#include "task_leak_watchdog.h"
#include "task_sensor_data_aquire.h"
#include "task_update_oled.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
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
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for sensorDataAquir */
osThreadId_t sensorDataAquirHandle;
uint32_t sensorDataAquirBuffer[ 512 ];
osStaticThreadDef_t sensorDataAquirControlBlock;
const osThreadAttr_t sensorDataAquir_attributes = {
  .name = "sensorDataAquir",
  .stack_mem = &sensorDataAquirBuffer[0],
  .stack_size = sizeof(sensorDataAquirBuffer),
  .cb_mem = &sensorDataAquirControlBlock,
  .cb_size = sizeof(sensorDataAquirControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for updateOLEDTask */
osThreadId_t updateOLEDTaskHandle;
uint32_t updateOLEDTaskBuffer[ 256 ];
osStaticThreadDef_t updateOLEDTaskControlBlock;
const osThreadAttr_t updateOLEDTask_attributes = {
  .name = "updateOLEDTask",
  .stack_mem = &updateOLEDTaskBuffer[0],
  .stack_size = sizeof(updateOLEDTaskBuffer),
  .cb_mem = &updateOLEDTaskControlBlock,
  .cb_size = sizeof(updateOLEDTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for leakWatchdo */
osThreadId_t leakWatchdoHandle;
uint32_t leakWatchdoBuffer[ 512 ];
osStaticThreadDef_t leakWatchdoControlBlock;
const osThreadAttr_t leakWatchdo_attributes = {
  .name = "leakWatchdo",
  .stack_mem = &leakWatchdoBuffer[0],
  .stack_size = sizeof(leakWatchdoBuffer),
  .cb_mem = &leakWatchdoControlBlock,
  .cb_size = sizeof(leakWatchdoControlBlock),
  .priority = (osPriority_t) osPriorityNormal3,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartSensorDataAquireTask(void *argument);
void StartUpdateOLEDTask(void *argument);
void StartLeakWatchdogTask(void *argument);

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

  /* creation of sensorDataAquir */
  sensorDataAquirHandle = osThreadNew(StartSensorDataAquireTask, NULL, &sensorDataAquir_attributes);

  /* creation of updateOLEDTask */
  updateOLEDTaskHandle = osThreadNew(StartUpdateOLEDTask, NULL, &updateOLEDTask_attributes);

  /* creation of leakWatchdo */
  leakWatchdoHandle = osThreadNew(StartLeakWatchdogTask, NULL, &leakWatchdo_attributes);

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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartSensorDataAquireTask */
/**
* @brief Function implementing the sensorDataAquir thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorDataAquireTask */
__weak void StartSensorDataAquireTask(void *argument)
{
  /* USER CODE BEGIN StartSensorDataAquireTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSensorDataAquireTask */
}

/* USER CODE BEGIN Header_StartUpdateOLEDTask */
/**
* @brief Function implementing the updateOLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateOLEDTask */
__weak void StartUpdateOLEDTask(void *argument)
{
  /* USER CODE BEGIN StartUpdateOLEDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUpdateOLEDTask */
}

/* USER CODE BEGIN Header_StartLeakWatchdogTask */
/**
* @brief Function implementing the leakWatchdo thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLeakWatchdogTask */
__weak void StartLeakWatchdogTask(void *argument)
{
  /* USER CODE BEGIN StartLeakWatchdogTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartLeakWatchdogTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

