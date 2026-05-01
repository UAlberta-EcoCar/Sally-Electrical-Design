/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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

#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	STATE_DISCHARGE_OFF = 0,
	STATE_RES_CHARGE = 1,
	STATE_CHARGE = 2,
	STATE_RUN = 3,
} relay_state_t;

typedef enum {
	MEAS_VOLT_FC_UP = 2, // Channel 6,
	MEAS_VOLT_FC_OUT = 3, // Channel 7,
	MEAS_VOLT_CAP = 4, // Channel 8,
	MEAS_VOLT_RES_HIGH = 5, // Channel 9,
	MEAS_VOLT_RES_LOW = 0, // Channel 1,
	MEAS_VOLT_SYS_7V = 6, // Channel 11,
	MEAS_CURR_SYS_12V = 1, // Channel 5,
	MEAS_MCU_TEMP = 7, // Channel TempSensor,
	MEAS_VBAT = 8, // Channel VBAT,
	MEAS_VREFINT = 9, // Channel VREFINT
} MEAS_ADC1_IDX_t;

typedef enum {
	MEAS_CURR_RES_HIGH = 0, // Channel 5,
	MEAS_CURR_RES_LOW = 1, // Channel 12,
	MEAS_CURR_MTR = 3, // Channel 17,
	MEAS_CURR_SYS_7V = 2, // Channel 15,
} MEAS_ADC2_IDX_t;

typedef enum {
	MEAS_VOLT_MTR = 0, // Channel 3,
	MEAS_VOLT_SYS_12V = 1, // Channel 5,
	MEAS_CURR_FC_OUT = 2, // Channel 7,
	MEAS_CURR_CAP = 3, // Channel 14,
} MEAS_ADC3_IDX_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint16_t ADC1_MEAS[10];
uint16_t ADC2_MEAS[4];
uint16_t ADC3_MEAS[4];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

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
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC1_MEAS, 10);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*) ADC2_MEAS, 4);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*) ADC3_MEAS, 4);
	/* Infinite loop */
	for (;;) {
		printf("Starting RELAY\r\n");
		osDelay(100);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

