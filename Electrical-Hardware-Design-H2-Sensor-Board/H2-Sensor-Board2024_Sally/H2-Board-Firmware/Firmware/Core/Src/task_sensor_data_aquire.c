/*
 * sensor_data_aquire_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#include "main.h"
#include "task_sensor_data_aquire.h"
#include "adc.h"
#include <FreeRTOS.h>
#include <task.h>
#include "cmsis_os.h"
#include "bme280.h"
#include "bme280_defs.h"

typedef struct {
	float h2_sense1;
	float h2_sense2;
	float h2_sense3;
	float h2_sense4;

	float IMON_7V;
	float IMON_12V;

	float humidity;
	float temprature;
	float pressure;

	float mcu_temp;
	float vbat;
	float vref;
} H2_Sensor_Data_t;

uint32_t adc1_results[3] = { 0 }; // 0: h2sense1 2:imon12v 3:imon
uint32_t adc2_results[2] = { 0 }; // 0: h2sense3 1: h2sense2
uint32_t adc5_results[4] = { 0 }; // 0: h2sense4 1: cputemp 2: vbat 3: vrefint

void StartSensorDataAquireTask(void *argument) {
	/* USER CODE BEGIN StartSensorDataAquireTask */

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc5, ADC_SINGLE_ENDED);

	HAL_ADC_Start_DMA(&hadc1, adc1_results, 3);
	HAL_ADC_Start_DMA(&hadc2, adc2_results, 2);
	HAL_ADC_Start_DMA(&hadc5, adc5_results, 4);

	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartSensorDataAquireTask */
}
