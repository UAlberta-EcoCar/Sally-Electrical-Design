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
#include "task_telemetry_transmit.h"

#define MAX_TELEMTRY_PACKET_SIZE 63 // Not including packet ID.

//typedef struct {
//	//FDCAN_FetPack_t fet_data;
//	FDCAN_RelPackMtr_t mtr_data;
//	FDCAN_RelPackCap_t cap_data;
//	FDCAN_FccPack1_t fc_data1;
//	FDCAN_FccPack2_t fc_data2;
//
//} telemetry_data1_t;
//
//typedef struct {
//	FDCAN_FccPack3_t fc_data3;
//	//FDCAN_H2Pack_t h2_data;
//	FDCAN_BOOSTPack_t boost_data1;
//	FDCAN_BOOSTPack2_t boost_data2;
//	FDCAN_RelPackFc_t RelPackFc;
//} telemetry_data2_t;

typedef struct {
	uint8_t latitude;
	uint8_t longitude;
} telemetry_gps_t;

typedef enum {
	BASIC_DATA_1_SIZE = sizeof(telemetry_data1_t),
	BASIC_DATA_2_SIZE = sizeof(telemetry_data2_t),
	BASIC_DATA_3_SIZE = sizeof(telemetry_data3_t),
	GPS_DATA_1_SIZE = sizeof(telemetry_gps_t),
	NATHAN_DATA_SIZE = 8,
} packet_id_size_t;

typedef enum {
	BASIC_DATA_1_ID = 0x01,
	BASIC_DATA_2_ID = 0x02,
	GPS_DATA_1_ID = 0x03,
	NATHAN_DATA_ID = 0x04,
	BASIC_DATA_3_ID = 0x05,

} packet_id_t; // must be 1 byte

typedef struct {
	packet_id_t packet_id;
	uint8_t packet_data[MAX_TELEMTRY_PACKET_SIZE];
} telemetry_packet_t;

FDCAN_FccPack1_t fc_dfe;
FDCAN_RelPackFc_t Rel;

extern telemetry_data1_t data1;
extern telemetry_data2_t data2;
extern telemetry_data3_t data3;

telemetry_data1_t data_recieved_1 = { 0 };
telemetry_data2_t data_recieved_2 = { 0 };
telemetry_data2_t data_recieved_3 = { 0 };

telemetry_packet_t recieved_data;

extern uint8_t nathan[8];

void StartTelemetryTransmitTask(void *argument) {

	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RST_915_GPIO_Port, RST_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RST_24_GPIO_Port, RST_24_Pin, GPIO_PIN_SET);

	int snr = 0, rssi = 0;
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

	rf_set_spread_factor(&rfm95_868, 10);
	rf_set_spread_factor(&rfm95_915, 10);

	rf_set_tx_power(&rfm95_868, 20);
	rf_set_tx_power(&rfm95_915, 20);

	rf_set_bandwidth(&rfm95_868, RF_BW_500K);
	rf_set_bandwidth(&rfm95_915, RF_BW_500K);

	rf_set_ocp(&rfm95_868, 240);
	rf_set_ocp(&rfm95_915, 240);

	rf_set_coding_rate(&rfm95_868, 8);
	rf_set_coding_rate(&rfm95_915, 8);

	rf_enable_crc(&rfm95_868);
	rf_enable_crc(&rfm95_915);

	telemetry_packet_t dat1 = { 0 };
	telemetry_packet_t dat2 = { 0 };
	telemetry_packet_t dat3 = { 0 };

	dat1.packet_id = BASIC_DATA_1_ID;
	dat2.packet_id = BASIC_DATA_2_ID;
	dat3.packet_id = BASIC_DATA_3_ID;

	memcpy(&dat1.packet_data, &data1, BASIC_DATA_1_SIZE);

	memcpy(&dat2.packet_data, &data2, BASIC_DATA_2_SIZE);
	memcpy(&dat3.packet_data, &data3, BASIC_DATA_3_SIZE);

	for (;;) {
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {
//			memcpy(&gps_pack.packet_data, gps_can, 8);
//
//			data1.mtr_data.mtr_curr += 1;
//
//			data2.fc_data3.bme_temp += 1;
//
//			data3.h2_data.h2_sense_1 += 1;

			memcpy(dat1.packet_data, &data1, sizeof(telemetry_data1_t));

			memcpy(dat2.packet_data, &data2, sizeof(telemetry_data2_t));
			memcpy(dat3.packet_data, &data3, sizeof(telemetry_data3_t));

//			memcpy(nathan_data.packet_data, nathan, 8);

			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

			if (0
					!= rf_send(&rfm95_868, &dat1,
							1 + sizeof(telemetry_data1_t))) {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
			osDelay(5);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			if (0
					!= rf_send(&rfm95_868, &dat2,
							1 + sizeof(telemetry_data2_t))) {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			osDelay(5);
			if (0
					!= rf_send(&rfm95_868, &dat3,
							1 + sizeof(telemetry_data3_t))) {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}

			osDelay(5);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			if (0
					!= rf_send(&rfm95_915, &dat1,
							1 + sizeof(telemetry_data1_t))) {
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
			osDelay(5);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			if (0
					!= rf_send(&rfm95_915, &dat2,
							1 + sizeof(telemetry_data2_t))) {
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
			osDelay(10);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			if (0
					!= rf_send(&rfm95_915, &dat3,
							1 + sizeof(telemetry_data3_t))) {
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
			osDelay(5);

		}

		// reciever
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT2_GPIO_Port, SWT2_Pin)) {

			rf_recieve_single(&rfm95_868, &rec_legth);
			if (rec_legth != 0) {
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
				rf_read_packet(&rfm95_868, rec_legth, &recieved_data);
				rf_packet_snr(&rfm95_868, &snr);
				rf_packet_rssi(&rfm95_868, &rssi);

				switch (recieved_data.packet_id) {

				case BASIC_DATA_1_ID:

					memcpy(&data_recieved_1, recieved_data.packet_data,
							BASIC_DATA_1_SIZE);

					break;
				case BASIC_DATA_2_ID:

					memcpy(&data_recieved_2, recieved_data.packet_data,
							BASIC_DATA_2_SIZE);

					break;
				case BASIC_DATA_3_ID:

					memcpy(&data_recieved_3, recieved_data.packet_data,
							BASIC_DATA_3_SIZE);

					break;
				default:
					break;
				}

				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
//				rec_legth = 0;
			}
			rec_legth = 0;

			rf_recieve_single(&rfm95_915, &rec_legth);
			if (rec_legth != 0) {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
				rf_read_packet(&rfm95_915, rec_legth, &recieved_data);
				rf_packet_snr(&rfm95_915, &snr);
				rf_packet_rssi(&rfm95_915, &rssi);

				switch (recieved_data.packet_id) {
				case BASIC_DATA_1_ID:
					memcpy(&data_recieved_1, recieved_data.packet_data,
							BASIC_DATA_1_SIZE);
					break;
				case BASIC_DATA_2_ID:
					memcpy(&data_recieved_2, recieved_data.packet_data,
							BASIC_DATA_2_SIZE);
					break;
				case BASIC_DATA_3_ID:

					memcpy(&data_recieved_3, recieved_data.packet_data,
							BASIC_DATA_3_SIZE);

					break;
				case GPS_DATA_1_ID:
					log_info("[915]GOT GPS 1");
					break;
				default:
					break;
				}
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				rec_legth = 0;
			}
			rec_legth = 0;
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			osDelay(5);
		}

//		osDelay(5);
	}
}
