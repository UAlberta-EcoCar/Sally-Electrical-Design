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
/* Definitions for driveController */
osThreadId_t driveControllerHandle;
uint32_t driveControllerBuffer[ 512 ];
osStaticThreadDef_t driveControllerControlBlock;
const osThreadAttr_t driveController_attributes = {
  .name = "driveController",
  .stack_mem = &driveControllerBuffer[0],
  .stack_size = sizeof(driveControllerBuffer),
  .cb_mem = &driveControllerControlBlock,
  .cb_size = sizeof(driveControllerControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for driveTelemetry */
osThreadId_t driveTelemetryHandle;
uint32_t driveTelemetryBuffer[ 256 ];
osStaticThreadDef_t driveTelemetryControlBlock;
const osThreadAttr_t driveTelemetry_attributes = {
  .name = "driveTelemetry",
  .stack_mem = &driveTelemetryBuffer[0],
  .stack_size = sizeof(driveTelemetryBuffer),
  .cb_mem = &driveTelemetryControlBlock,
  .cb_size = sizeof(driveTelemetryControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for driveCANCom */
osThreadId_t driveCANComHandle;
uint32_t driveCANComBuffer[ 256 ];
osStaticThreadDef_t driveCANComControlBlock;
const osThreadAttr_t driveCANCom_attributes = {
  .name = "driveCANCom",
  .stack_mem = &driveCANComBuffer[0],
  .stack_size = sizeof(driveCANComBuffer),
  .cb_mem = &driveCANComControlBlock,
  .cb_size = sizeof(driveCANComControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for driveTelemetryO */
osThreadId_t driveTelemetryOHandle;
uint32_t driveTelemetryOBuffer[ 256 ];
osStaticThreadDef_t driveTelemetryOControlBlock;
const osThreadAttr_t driveTelemetryO_attributes = {
  .name = "driveTelemetryO",
  .stack_mem = &driveTelemetryOBuffer[0],
  .stack_size = sizeof(driveTelemetryOBuffer),
  .cb_mem = &driveTelemetryOControlBlock,
  .cb_size = sizeof(driveTelemetryOControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void StartDriveController(void *argument);
extern void StartDriveTelemetry(void *argument);
extern void StartCANComTask(void *argument);
extern void StartDriveTelemetryOutput(void *argument);

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

  /* creation of driveController */
  driveControllerHandle = osThreadNew(StartDriveController, NULL, &driveController_attributes);

  /* creation of driveTelemetry */
  driveTelemetryHandle = osThreadNew(StartDriveTelemetry, NULL, &driveTelemetry_attributes);

  /* creation of driveCANCom */
  driveCANComHandle = osThreadNew(StartCANComTask, NULL, &driveCANCom_attributes);

  /* creation of driveTelemetryO */
  driveTelemetryOHandle = osThreadNew(StartDriveTelemetryOutput, NULL, &driveTelemetryO_attributes);

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
	for (;;) {
		HAL_GPIO_TogglePin(GPLED_1_GPIO_Port, GPLED_1_Pin);
		osDelay(100);
	}
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

