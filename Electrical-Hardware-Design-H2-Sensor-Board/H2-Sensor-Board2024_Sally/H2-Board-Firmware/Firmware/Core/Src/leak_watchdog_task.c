/*
 * leak_watchdog_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#include "leak_watchdog_thread.h"

void StartLeakWatchdogTask(void *argument) {
	/* USER CODE BEGIN StartLeakWatchdogTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartLeakWatchdogTask */
}
