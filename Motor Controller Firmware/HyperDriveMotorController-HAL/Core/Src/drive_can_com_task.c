/*
 * drive_can_com_task.c
 *
 *  Created on: Feb 4, 2025
 *      Author: abina
 */

#include "main.h"

#include "FreeRTOS.h"

#include "task.h"

#include "cmsis_os.h"

void StartCANComTask(void *argument) {
	/* USER CODE BEGIN StartCANComTask */
	/* Infinite loop */
	for (;;) {
		HAL_GPIO_TogglePin(GPLED_4_GPIO_Port, GPLED_4_Pin);
		osDelay(100);
	}
	/* USER CODE END StartCANComTask */
}
