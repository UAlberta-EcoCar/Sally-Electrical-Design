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
#include <stdio.h>
#include <stdlib.h>
#include "debug-log.h"

typedef struct {
	//FDCAN_FetPack_t fet_data;
	FDCAN_RelPackMtr_t mtr_data;
	FDCAN_RelPackCap_t cap_data;
	FDCAN_FccPack1_t fc_data1;
	FDCAN_FccPack2_t fc_data2;

} telemetry_data1_t;

typedef struct {
	FDCAN_FccPack3_t fc_data3;
	//FDCAN_H2Pack_t h2_data;
	FDCAN_BOOSTPack_t boost_data1;
	FDCAN_BOOSTPack2_t boost_data2;
	FDCAN_RelPackFc_t RelPackFc;
} telemetry_data2_t;

FDCAN_FccPack1_t fc_dfe;
FDCAN_RelPackFc_t Rel;

extern telemetry_data1_t data1;
extern telemetry_data2_t data2;

char h2[6] = { 0 };

void StartTelemetryTransmitTask(void *argument) {

	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RST_915_GPIO_Port, RST_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RST_24_GPIO_Port, RST_24_Pin, GPIO_PIN_SET);

	fc_dfe.fc_press = 100;
	fc_dfe.fc_temp = 9932;
	Rel.fc_curr = 12;
	Rel.fc_volt = 21;
	int snr = 0, rssi = 0;
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

	rf_set_spread_factor(&rfm95_868);
	rf_set_spread_factor(&rfm95_915);

	FDCAN_FccPack1_t fc = { 0 };
	FDCAN_RelPackFc_t rel = { 0 };
	uint32_t test = 322;
	uint32_t test2 = 0;
	telemetry_data1_t data_rc = { 0 };
	telemetry_data2_t data_rc2 = { 0 };
	char h[] = "Hello";

//	uint8_t normalized_temp = 0;

	for (;;) {

		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//			rf_send(&rfm95_868, &Rel, sizeof(FDCAN_RelPackFc_t));
			rf_send(&rfm95_868, &data1, sizeof(telemetry_data1_t));
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
//			rf_send(&rfm95_915, &fc_dfe, sizeof(FDCAN_FccPack1_t));
			rf_send(&rfm95_915, &data2, sizeof(telemetry_data2_t));
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		}

		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT2_GPIO_Port, SWT2_Pin)) {
			rf_recieve_single(&rfm95_868, &rec_legth);

			if (rec_legth == sizeof(telemetry_data1_t)) {
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
				rf_read_packet(&rfm95_868, rec_legth, &data_rc);
				rf_packet_snr(&rfm95_868, &snr);
				rf_packet_rssi(&rfm95_868, &rssi);
				printf("[868] RSSI: %d SNR: %d Packet Size: %d", rssi, snr,
						rec_legth);
//				printf("%d %d", rel.fc_volt, rel.fc_curr);
				printf("\r\n");
				rec_legth = 0;
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			}
			rec_legth = 0;
			rf_recieve_single(&rfm95_915, &rec_legth1);

			if (rec_legth1 == sizeof(telemetry_data2_t)) {
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
				rf_read_packet(&rfm95_915, rec_legth1, &data_rc2);
				rf_packet_snr(&rfm95_915, &snr);
				rf_packet_rssi(&rfm95_915, &rssi);
				printf("[915] RSSI: %d SNR: %d Packet Size: %d", rssi, snr,
						rec_legth1);
//				printf("%d %d", fc.fc_temp, fc.fc_press);
//				printf("");
				printf("\r\n");
				rec_legth1 = 0;
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			}
			osDelay(5);
		}

	}
}
