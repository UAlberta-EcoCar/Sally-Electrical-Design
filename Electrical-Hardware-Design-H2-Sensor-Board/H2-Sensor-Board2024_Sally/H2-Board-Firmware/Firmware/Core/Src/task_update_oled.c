/*
 * update_oled_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */
#include <task_update_oled.h>
#include <FreeRTOS.h>
#include <task.h>
#include "cmsis_os.h"
#include "ssd1306_config.h"
#include "main.h"
#include "ssd1306.h"
#include "i2c.h"


void StartUpdateOLEDTask(void *argument) {
	/* USER CODE BEGIN StartUpdateOLEDTask */

	ssd1306_Init();


	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartUpdateOLEDTask */
}
