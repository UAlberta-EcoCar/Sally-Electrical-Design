/*
 * sensor_data_aquire_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#include "main.h"
#include "task_sensor_data_aquire.h"
#include "adc.h"

uint32_t adc1_results[3] = {0};
uint32_t adc2_results[2] = {0};
uint32_t adc5_results[4] = {0};

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
