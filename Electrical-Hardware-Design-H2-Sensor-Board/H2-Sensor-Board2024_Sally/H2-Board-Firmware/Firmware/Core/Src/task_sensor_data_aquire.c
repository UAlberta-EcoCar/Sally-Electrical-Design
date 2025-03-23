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
#include "i2c.h"
#include "log/debug-log.h"
#include "ecocar_can.h"

Sensor_Data_t sensor_data;

uint32_t adc1_results[3] = { 0 }; // 0: h2sense1 1:imon12v 2:imon7V
uint32_t adc2_results[2] = { 0 }; // 0: h2sense3 1: h2sense2
uint32_t adc5_results[4] = { 0 }; // 0: h2sense4 1: cputemp 2: vbat 3: vrefint

#define AVERAGE_SLOPE_MCU_TEMP 2.5f
#define VOLTAGE_AT_30C_MCU_TEMP 0.76f // page 168 5.3.24: Temperature sensor characteristics

#define H2_SENSE_1_IDX 0
#define H2_SENSE_2_IDX 1
#define H2_SENSE_3_IDX 0
#define H2_SENSE_4_IDX 0

#define ADC_CONV_CONST 3.3f / 4096.0f

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	if (HAL_I2C_Master_Transmit(&hi2c4, (id << 1), &reg_addr, 1, 10) != HAL_OK)
		return -1;
	if (HAL_I2C_Master_Receive(&hi2c4, (id << 1) | 0x01, data, len, 10)
			!= HAL_OK)
		return -1;

	return 0;
}

void user_delay_ms(uint32_t period) {
	osDelay(period);
}

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	uint8_t buf[len + 1];
	buf[0] = reg_addr;
	memcpy(buf + 1, data, len);
	while (HAL_I2C_Master_Transmit(&hi2c4, (id << 1), (uint8_t*) buf, len + 1,
	HAL_MAX_DELAY) != HAL_OK)
		printf("I2C Error\r\n");
//	int8_t *buf;
//	buf = malloc(len + 1);
//	buf[0] = reg_addr;
//	memcpy(buf + 1, data, len);
//
//	if (HAL_I2C_Master_Transmit(&hi2c1, (id << 1), (uint8_t*) buf, len + 1,
//	HAL_MAX_DELAY) != HAL_OK)
//		return -1;
//
//	free(buf);
	return 0;
}

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
	struct bme280_data comp_data = { 0 };
	int8_t rslt;

//	HAL_I2C_IsDeviceReady(&hi2c4, BME280_I2C_ADDR_PRIM, 5, 100);

	dev.dev_id = BME280_I2C_ADDR_SEC;
	dev.intf = BME280_I2C_INTF;
	dev.read = user_i2c_read;
	dev.write = user_i2c_write;
	dev.delay_ms = user_delay_ms;
	rslt = bme280_init(&dev);

	dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	dev.settings.filter = BME280_FILTER_COEFF_16;
	rslt = bme280_set_sensor_settings(
			BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL
					| BME280_FILTER_SEL, &dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);

	float vbat= 0, vref = 0, temp = 0, vsense = 0;

	/* Infinite loop */
	for (;;) {

		// Thread Heart Beat

//		HAL_GPIO_WritePin(GPLED1_GPIO_Port, GPLED1_Pin, GPIO_PIN_SET);

		if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c4, BME280_I2C_ADDR_SEC, 1,
		HAL_MAX_DELAY)) {
			log_info("Device Found");
		}

		sensor_data.h2_sense1_mV = (uint32_t) (adc1_results[H2_SENSE_1_IDX]
				* ADC_CONV_CONST * FDCAN_FOUR_FLT_PREC);
		sensor_data.h2_sense2_mV = (uint32_t) (adc2_results[H2_SENSE_2_IDX]
				* ADC_CONV_CONST * FDCAN_FOUR_FLT_PREC);
		sensor_data.h2_sense3_mV = (uint32_t) (adc2_results[H2_SENSE_3_IDX]
				* ADC_CONV_CONST * FDCAN_FOUR_FLT_PREC);
		sensor_data.h2_sense4_mV = (uint32_t) (adc5_results[H2_SENSE_4_IDX]
				* ADC_CONV_CONST * FDCAN_FOUR_FLT_PREC);

		sensor_data.IMON_12V_mA = (uint32_t) (adc1_results[1]
				* ADC_CONV_CONST * FDCAN_FOUR_FLT_PREC);
		sensor_data.IMON_7V_mA = (uint32_t) (adc1_results[2] * ADC_CONV_CONST
				* FDCAN_FOUR_FLT_PREC);

		sensor_data.vref_mV = (uint32_t) (adc5_results[3] * ADC_CONV_CONST
				* FDCAN_FOUR_FLT_PREC);
		sensor_data.vbat_mV = (uint32_t) (adc5_results[2] * ADC_CONV_CONST
				* FDCAN_FOUR_FLT_PREC);

		// temp equation
		// [V_30 - V_Sense] / AVERAGE_SLOPE + 25

		vsense = adc5_results[1] * ADC_CONV_CONST;
		temp = (((VOLTAGE_AT_30C_MCU_TEMP - vsense) * 1000.0f) / AVERAGE_SLOPE_MCU_TEMP) + 30;
		sensor_data.mcu_temp_C = (uint32_t) temp;

		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

		if (rslt == BME280_OK) {
			sensor_data.temprature_C = comp_data.temperature / 100.0; /* °C  */
			sensor_data.humidity_per = comp_data.humidity / 1024.0; /* %   */
			sensor_data.pressure_hPa = comp_data.pressure / 10000.0; /* hPa */
		}
		osDelay(5);
//		HAL_GPIO_WritePin(GPLED1_GPIO_Port, GPLED1_Pin, GPIO_PIN_RESET);
		osDelay(5);
	}
	/* USER CODE END StartSensorDataAquireTask */
}
