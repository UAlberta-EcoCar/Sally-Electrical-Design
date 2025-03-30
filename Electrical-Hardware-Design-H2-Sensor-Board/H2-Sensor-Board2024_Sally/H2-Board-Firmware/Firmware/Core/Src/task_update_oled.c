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

#define ALARM_SCREEN_FLASH_INTERVAL 200

extern uint32_t clean_air_constant_mV;
extern uint8_t sensor_heating;

extern uint32_t dac1[2];
extern uint32_t dac2[1];
extern uint32_t dac4[1];

void StartUpdateOLEDTask(void *argument) {
	/* USER CODE BEGIN StartUpdateOLEDTask */

	ssd1306_Init();

//	ssd1306_TestAll();
	char screen_text[64] = { 0 };

	int flash_screen_temp = 0;
	SSD1306_COLOR current_color = White;
	uint32_t timer_screen_flash = 0;
	/* Infinite loop */
	for (;;) {

//		ssd1306_Fill(Black);
		if (alarm_state == H2_ALARM_TRIGGERED) {
			current_color = Black;
			flash_screen_temp = !flash_screen_temp;
			if (timer_screen_flash = 0) {
				timer_screen_flash = osKernelGetTickCount();
			}
			if ((osKernelGetTickCount() - timer_screen_flash)
					> ALARM_SCREEN_FLASH_INTERVAL) {
				if (flash_screen_temp) {
					ssd1306_Fill(White);
				} else {
					ssd1306_Fill(Black);
				}

			}

		}

		// Write all data to buffer;

		if (sensor_heating) {
			sprintf(screen_text, "T: %d H: %d P: %d", sensor_data.temprature_C,
					sensor_data.humidity_per, sensor_data.pressure_hPa);

			ssd1306_SetCursor(0, 0);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "H1: %d", sensor_data.h2_sense1_mV);

			ssd1306_SetCursor(0, 10);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "CAC: %d", clean_air_constant_mV);

			ssd1306_SetCursor(70, 10);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "TH: %d", dac1[0] * 3.3 / 4096);

			ssd1306_SetCursor(70, 20);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "H2: %d", sensor_data.h2_sense2_mV);

			ssd1306_SetCursor(0, 20);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "H3: %d", sensor_data.h2_sense3_mV);

			ssd1306_SetCursor(0, 30);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);

			sprintf(screen_text, "H4: %d", sensor_data.h2_sense4_mV);

			ssd1306_SetCursor(0, 40);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);
		} else {
			sprintf(screen_text, "Sensor Heating 5s...");
			ssd1306_SetCursor(0, 0);
			ssd1306_WriteString(screen_text, Font_6x8, current_color);
		}
		ssd1306_SetCursor(0, 50);
		switch (alarm_state) {
		case H2_ALARM_ARMED:
			sprintf(screen_text, "ALARM ARMED   ");
			break;
		case H2_ALARM_DISARMED:
			sprintf(screen_text, "ALARM DISARMED");
			break;
		case H2_ALARM_TRIGGERED:
			sprintf(screen_text, "ALARM TRIGGERED");
			break;
		case H2_ALARM_TEST:
			sprintf(screen_text, "ALARM TEST");
			break;
		}

		ssd1306_WriteString(screen_text, Font_7x10, current_color);

		ssd1306_UpdateScreen();

		osDelay(100);
	}
	/* USER CODE END StartUpdateOLEDTask */
}
