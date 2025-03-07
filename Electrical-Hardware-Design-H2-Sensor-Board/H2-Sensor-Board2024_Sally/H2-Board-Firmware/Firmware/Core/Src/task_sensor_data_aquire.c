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


H2_Sensor_Data_t h2_sensor_data;

uint32_t adc1_results[3] = { 0 }; // 0: h2sense1 1:imon12v 2:imon7V
uint32_t adc2_results[2] = { 0 }; // 0: h2sense3 1: h2sense2
uint32_t adc5_results[4] = { 0 }; // 0: h2sense4 1: cputemp 2: vbat 3: vrefint

#define H2_SENSE_1_IDX 0
#define H2_SENSE_2_IDX 1
#define H2_SENSE_3_IDX 0
#define H2_SENSE_4_IDX 0

#define ADC_CONV_CONST 4096.0f / 3.3f

void StartSensorDataAquireTask(void *argument) {
	/* USER CODE BEGIN StartSensorDataAquireTask */

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc5, ADC_SINGLE_ENDED);

	HAL_ADC_Start_DMA(&hadc1, adc1_results, 3);
	HAL_ADC_Start_DMA(&hadc2, adc2_results, 2);
	HAL_ADC_Start_DMA(&hadc5, adc5_results, 4);

	// Since the bme and oled are on different i2c lines we can neglect mutexes since they will never collide

	// initialize bme

	struct bme280_dev dev;

	bme280_init(&dev);

	bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);



	/* Infinite loop */
	for (;;) {

		h2_sensor_data.h2_sense1_mV = (uint32_t) (adc1_results[H2_SENSE_1_IDX]
				* ADC_CONV_CONST);
		h2_sensor_data.h2_sense2_mV = (uint32_t) (adc2_results[H2_SENSE_2_IDX]
				* ADC_CONV_CONST);
		h2_sensor_data.h2_sense3_mV = (uint32_t) (adc2_results[H2_SENSE_3_IDX]
				* ADC_CONV_CONST);
		h2_sensor_data.h2_sense4_mV = (uint32_t) (adc5_results[H2_SENSE_4_IDX]
				* ADC_CONV_CONST);

		h2_sensor_data.IMON_12V_mA = adc1_results[1] * ADC_CONV_CONST;
		h2_sensor_data.IMON_7V_mA = adc1_results[2] * ADC_CONV_CONST;

		h2_sensor_data.vref_mV = adc5_results[3] * ADC_CONV_CONST;
		h2_sensor_data.vbat_mV = adc5_results[2] * ADC_CONV_CONST;

		h2_sensor_data.mcu_temp_C = adc5_results[1] * ADC_CONV_CONST;

		osDelay(1);
	}
	/* USER CODE END StartSensorDataAquireTask */
}
