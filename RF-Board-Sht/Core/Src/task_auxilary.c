/*
 * task_telemtry_transmit.c
 *
 *  Created on: Mar 15, 2025
 *      Author: abina
 */

#include "main.h"
#include "FreeRTOS.h"
#include "i2c.h"
#include "math.h"
#include "ansi-codes.h"

#include "task_telemetry_transmit.h"

uint16_t ublox_ReadLength(void) {
	uint8_t data[2];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, (0x42 << 1), 0xFD, 1,
			data, sizeof(data), HAL_MAX_DELAY);
	if (status != HAL_OK)
		printf("ublox_ReadLength failed %d %08X\n", status, hi2c2.ErrorCode);
	return (((uint16_t) data[0] << 8) + (uint16_t) data[1]); // Big Endian
}

//****************************************************************************

void ublox_SendCommand(int size, uint8_t *command) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, (0x42 << 1), 0xFF, 1,
			command, size, HAL_MAX_DELAY);
	if (status != HAL_OK)
		printf("ublox_SendCommand failed %d %08X\n", status, hi2c2.ErrorCode);
}

//****************************************************************************

void ublox_TestI2C(void) {
	int i;
	HAL_StatusTypeDef status;
	uint8_t ubx_mon_ver[] = { 0xB5, 0x62, 0x0A, 0x04, 0x00, 0x00, 0x0E, 0x34 };
	ublox_SendCommand(sizeof(ubx_mon_ver), ubx_mon_ver);
	for (i = 0; i < 10; i++) {
		uint16_t length = ublox_ReadLength();
		printf("uBlox Length %5d %04X\n", length, length);
		if (length) {
			static uint8_t buffer[1000];
			uint16_t len = (length < sizeof(buffer)) ? length : sizeof(buffer);
			status = HAL_I2C_Mem_Read(&hi2c2, (0x42 << 1), 0xFF, 1, buffer, len,
			HAL_MAX_DELAY);
//			if (status == HAL_OK)
////				DumpData(len, buffer);
//			else
////				puts("Read Fail");
		} else
			HAL_Delay(1000);
	}
}

extern telemetry_data1_t data_recieved_1;
extern telemetry_data2_t data_recieved_2;

extern telemetry_data3_t data_recieved_3;

void StartAuxilaryTask(void *argument) {
	/* USER CODE BEGIN StartAuxilaryTask */

	HAL_StatusTypeDef status = 0;
	uint8_t data[10] = { 0 };

	uint8_t buffer[64] = { 0 };

	/* Infinite loop */
	for (;;) {

		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT2_GPIO_Port, SWT2_Pin)) {
			if (data_recieved_1.fc_data1.fc_temp != 0) {
				printf(">CC:%d\n>CV:%d\n", data_recieved_1.cap_data.cap_curr,
						data_recieved_1.cap_data.cap_volt);
				printf(">FCP:%d\n>FCT:%d\n", data_recieved_1.fc_data1.fc_press,
						data_recieved_1.fc_data1.fc_temp);
				printf(">FCF1:%d\n>FCF2:%d\n",
						data_recieved_1.fc_data2.fan_rpm1,
						data_recieved_1.fc_data2.fan_rpm2);
				printf(">MC:%d\n>MV:%d\n", data_recieved_1.mtr_data.mtr_curr,
						data_recieved_1.mtr_data.mtr_volt);
				printf(">FCC:%d\n>FCV:%d\n", data_recieved_2.RelPackFc.fc_curr,
						data_recieved_2.RelPackFc.fc_volt);
				printf(">BCI:%d\n>BVI:%d\n",
						data_recieved_2.boost_data1.in_curr,
						data_recieved_2.boost_data1.in_volt);
				printf(">BCO:%d\n>BVO:%d\n",
						data_recieved_2.boost_data2.out_curr,
						data_recieved_2.boost_data2.out_volt);
				printf(">FCBMEHUM:%d\n>FCBMET:%d\n",
						data_recieved_2.fc_data3.bme_humid,
						data_recieved_2.fc_data3.bme_temp);
				printf(">H2SNS1:%d\n", data_recieved_3.h2_data.h2_sense_1);
				printf(">H2SNS2:%d\n", data_recieved_3.h2_data.h2_sense_2);
				printf(">H2SNS3:%d\n", data_recieved_3.h2_data.h2_sense_3);
				printf(">H2SNS4:%d\n", data_recieved_3.h2_data.h2_sense_4);
			}
			osDelay(5);
		}
	}
	/* USER CODE END StartAuxilaryTask */
}
