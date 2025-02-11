/*
 * drive_telemetry_output_task.c
 *
 *  Created on: Feb 9, 2025
 *      Author: abina
 */

#include <ssd1306_config.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

void StartDriveTelemetryOutput(void *argument) {

	ssd1306_Init();

//	ssd1306_Fill(White);

	ssd1306_TestDrawBitmap();

	ssd1306_UpdateScreen();

	for (;;) {

		osDelay(1000);
	}
}
