/*
 * sensor_data_aquire_task.h
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#ifndef INC_TASK_SENSOR_DATA_AQUIRE_H_
#define INC_TASK_SENSOR_DATA_AQUIRE_H_

typedef struct {
	uint32_t h2_sense1_ppm;
	uint32_t h2_sense2_ppm;
	uint32_t h2_sense3_ppm;
	uint32_t h2_sense4_ppm;

	uint32_t h2_sense1_mV;
	uint32_t h2_sense2_mV;
	uint32_t h2_sense3_mV;
	uint32_t h2_sense4_mV;

	uint32_t IMON_7V_mA;
	uint32_t IMON_12V_mA;

	uint32_t humidity_per;
	uint32_t temprature_C;
	uint32_t pressure_hPa;

	uint32_t mcu_temp_C;
	uint32_t vbat_mV;
	uint32_t vref_mV;
} H2_Sensor_Data_t;

extern H2_Sensor_Data_t h2_sensor_data;

void StartSensorDataAquireTask(void *argument);

#endif /* INC_TASK_SENSOR_DATA_AQUIRE_H_ */
