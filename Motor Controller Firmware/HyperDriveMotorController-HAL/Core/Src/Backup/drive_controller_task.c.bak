/*
 * drive_controller_task.c
 *
 *  Created on: Feb 4, 2025
 *      Author: abina
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

void StartDriveController(void *argument) {
	/* USER CODE BEGIN StartDriveController */
	/* Infinite loop */
	for (;;) {
		HAL_GPIO_TogglePin(GPLED_2_GPIO_Port, GPLED_2_Pin);
		osDelay(100);
	}
	/* USER CODE END StartDriveController */
}
