/*
 * leak_watchdog_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include "cmsis_os.h"
#include "task_leak_watchdog.h"
#include "comp.h"

void StartLeakWatchdogTask(void *argument) {
	/* USER CODE BEGIN StartLeakWatchdogTask */

	HAL_COMP_Start(&hcomp1);
	HAL_COMP_Start(&hcomp2);
	HAL_COMP_Start(&hcomp6);
	HAL_COMP_Start(&hcomp7);

	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartLeakWatchdogTask */
}

/**
 * @brief  Comparator trigger callback. Called for all comparators.
 * @param  hcomp  COMP handle
 * @retval None
 */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {
	if (hcomp->Instance == hcomp1.Instance) {

	} else if (hcomp->Instance == hcomp2.Instance){

	} else if (hcomp->Instance == hcomp6.Instance){

	} else if (hcomp->Instance == hcomp7.Instance){

	}
}

