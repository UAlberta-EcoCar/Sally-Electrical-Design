/*
 * task_telemtry_transmit.c
 *
 *  Created on: Mar 15, 2025
 *      Author: abina
 */
#include "main.h"
#include "FreeRTOS.h"
#include "rf-rfm95.h"
#include "spi.h"
#include "cmsis_os2.h"
#include "ecocar_can.h"
#include "debug-log.h"
extern FDCAN_FccPack1_t fc_data1;
extern FDCAN_RelPackFc_t RelPackFc;

//struct {
//	FDCAN_FccPack1_t _fc1;
//	FDCAN_RelPackFc_t _RelPackFc;
//} data;

void StartTelemetryTransmitTask(void *argument) {

//		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	//
	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_Pin, GPIO_PIN_SET);
	//
	//	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
	//
	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_SET);
	//
	//	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	//	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

//	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_GPIO_Port, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_GPIO_Port, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(RST_915_GPIO_Port, RST_915_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(RST_24_GPIO_Port, RST_24_Pin, GPIO_PIN_RESET);

	FDCAN_FccPack1_t recpac = { 0 };
	int32_t temp = 0;
	uint8_t temp8[4] = { 0 };
	uint8_t rec_legth = 0;
	uint8_t rec_legth1 = 0;
	rf_handle_t rfm95_868 = { .rf_carrier_frequency = 868000000,
			.rf_delay_func = osDelay, .rf_module_identifier = 0,
			.rf_nreset_port =
			RST_868_GPIO_Port, .rf_nreset_pin = RST_868_Pin, .rf_nss_port =
			NSS_868_GPIO_Port, .rf_spi_handle = &hspi1, .rf_nss_pin =
			NSS_868_Pin, .rf_spi_timeout = HAL_MAX_DELAY };

	rf_handle_t rfm95_915 = { .rf_carrier_frequency = 915000000,
			.rf_delay_func = osDelay, .rf_module_identifier = 1,
			.rf_nreset_port =
			RST_915_GPIO_Port, .rf_nreset_pin = RST_915_Pin, .rf_nss_port =
			NSS_915_GPIO_Port, .rf_spi_handle = &hspi1, .rf_nss_pin =
			NSS_915_Pin, .rf_spi_timeout = HAL_MAX_DELAY };

	rf_initialize_radio(&rfm95_868);
	rf_initialize_radio(&rfm95_915);
	FDCAN_FccPack1_t fc;
	FDCAN_RelPackFc_t rel;
//	uint8_t normalized_temp = 0;

	for (;;) {
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {
//			rf_send(&rf_868, &fc_data1, sizeof(fc_data1));
//		} else {
//			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
//			rf_recieve_single(&rf_868, &packLength);
//			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
//			rf_read_packet(&rf_868, packLength, &fc_data1.FDCAN_RawFccPack);
//
////			log_info("FC_TEMP %d", fc_data1.);
//
//		}
//		uint8_t prep_buffer[4] = { fc_data1.fc_temp >> (8 * 3), fc_data1.fc_temp
//				>> (8 * 2), fc_data1.fc_temp >> (8 * 1), fc_data1.fc_temp };
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//			normalized_temp = abs(fc_data1.fc_temp) / 1000;
			rf_send(&rfm95_868, &fc_data1, sizeof(FDCAN_FccPack1_t));
			rf_send(&rfm95_915, &RelPackFc, sizeof(FDCAN_RelPackFc_t));
//			rf_send(&rfm95_868, &RelPackFc, sizeof(FDCAN_RelPackFc_t));
//			rf_send(&rfm95_915, data, 5);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		}

		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT2_GPIO_Port, SWT2_Pin)) {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin, GPIO_PIN_SET);

			while (0 == (rec_legth = rf_available(&rfm95_868))) {
				rf_recieve_single(&rfm95_868, &rec_legth);
				osDelay(10);
			}
			if (rec_legth > 0) {
				rf_read_packet(&rfm95_868, rec_legth, &fc);

//				temp = temp8[3] << (8 * 3) | temp8[2] << (8 * 2)
//						| temp8[1] << (8 * 1) | temp8[0];

				rec_legth = 0;
			}

			while (0 == (rec_legth1 = rf_available(&rfm95_915))) {
				rf_recieve_single(&rfm95_915, &rec_legth1);
				osDelay(10);
			}
			if (rec_legth1 > 0) {
				rf_read_packet(&rfm95_915, rec_legth1, &rel);

//				temp = temp8[3] << (8 * 3) | temp8[2] << (8 * 2)
//						| temp8[1] << (8 * 1) | temp8[0];

				rec_legth1 = 0;
			}

//			memcpy(temp, temp8, 4);

			printf("FC Temp: %u FC Pres: %u\r\n", fc.fc_temp,
					fc.fc_press);

			printf("C: %u V: %u\r\n", rel.fc_curr, rel.fc_volt);
//			while (0 == rec_legth2) {
//				rf_recieve_single(&rfm95_915, &rec_legth2);
//				//osDelay(10);
//			}
//			if (rec_legth2 > 0) {
//				rf_read_packet(&rfm95_915, rec_legth2, rdata2);
//				rec_legth2 = 0;
//			}
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		}

//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		osDelay(100);
	}

}
