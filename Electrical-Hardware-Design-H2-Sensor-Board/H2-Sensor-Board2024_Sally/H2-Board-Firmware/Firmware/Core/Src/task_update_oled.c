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
#include "ssd1306_fonts.h"

void StartUpdateOLEDTask(void *argument) {
	/* USER CODE BEGIN StartUpdateOLEDTask */

	ssd1306_Init();

//	ssd1306_TestAll();
	char screen_text[64] = { 0 };

	/* Infinite loop */
	for (;;) {

		// Write all data to buffer;
		sprintf(screen_text, "T: %d H: %d P: %d", sensor_data.temprature_C,
				sensor_data.humidity_per, sensor_data.pressure_hPa);

		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString(screen_text, Font_6x8, White);

		ssd1306_UpdateScreen();

		osDelay(1000);
	}
	/* USER CODE END StartUpdateOLEDTask */
}
