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
#include "usart.h"
#include "task_telemetry_transmit.h"
#include "app_fatfs.h"

#include "eeprom_emul.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

uint16_t ublox_ReadLength(void) {
	uint8_t data[2];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, (0x42 << 1), 0xFD, 1,
			data, sizeof(data), 0);
	if (status != HAL_OK)
		printf("ublox_ReadLength failed %d %08X\n", status, hi2c2.ErrorCode);
	return (((uint16_t) data[0] << 8) + (uint16_t) data[1]); // Big Endian
}

//****************************************************************************

void ublox_SendCommand(int size, uint8_t *command) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, (0x42 << 1), 0xFF, 1,
			command, size, 0);
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
					0);
//			if (status == HAL_OK)
////				DumpData(len, buffer);
//			else
////				puts("Read Fail");
		} else
			osDelay(1000);
	}
}

void UART_Printf(const char *fmt, ...) {
	char buff[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buff, sizeof(buff), fmt, args);
	HAL_UART_Transmit(&huart1, (uint8_t*) buff, strlen(buff), HAL_MAX_DELAY);
	va_end(args);
}

void myprintf(const char *fmt, ...) {
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {

		static char buffer[256];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);

		int len = strlen(buffer);
		HAL_UART_Transmit(&huart1, (uint8_t*) buffer, len, -1);
	}
}

#define RUN_NUMBER_INDEX 1

extern telemetry_data1_t data_recieved_1;
extern telemetry_data2_t data_recieved_2;

extern telemetry_data3_t data_recieved_3;

extern telemetry_data1_t data1;
extern telemetry_data2_t data2;
extern telemetry_data3_t data3;

uint32_t runNumber = 0;

uint32_t Index = 1;
__IO uint32_t ErasingOnGoing = 0;
uint32_t a_VarDataTab[NB_OF_VARIABLES] = { 0 };
uint32_t VarValue = 0;
EE_Status ee_status = EE_OK;

void StartAuxilaryTask(void *argument) {
	/* USER CODE BEGIN StartAuxilaryTask */

	HAL_StatusTypeDef status = 0;
	uint8_t data[10] = { 0 };

	uint8_t buffer[64] = { 0 };

	//some variables for FatFs
	FATFS FatFs; 	//Fatfs handle
	FIL fil; 		//File handle
	FRESULT fres; //Result after operations

	char filename[32];

	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {

//		printf("beginning new log of run at:");

		HAL_NVIC_SetPriority(FLASH_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(FLASH_IRQn);

		/* Unlock the Flash Program Erase controller */
		HAL_FLASH_Unlock();

		ee_status = EE_Init(EE_CONDITIONAL_ERASE);
		if (ee_status != EE_OK) {
			Error_Handler();
		}

		while (ErasingOnGoing == 1) {
			osDelay(1);
		}

//	ee_status = EE_WriteVariable32bits(Index, 123);
//	ee_status |= EE_ReadVariable32bits(Index, &VarValue);
//	if (Index * VarValue != a_VarDataTab[Index - 1]) {
//		Error_Handler();
//	}

		/* Start cleanup IT mode, if cleanup is needed */
		if ((ee_status & EE_STATUSMASK_CLEANUP ) == EE_STATUSMASK_CLEANUP) {
			ErasingOnGoing = 1;
			ee_status |= EE_CleanUp_IT();
		}
		if ((ee_status & EE_STATUSMASK_ERROR ) == EE_STATUSMASK_ERROR) {
			Error_Handler();
		}
		VarValue = 0;
		/* Read all the variables */
		ee_status = EE_ReadVariable32bits(RUN_NUMBER_INDEX, &runNumber);

		if (ee_status == EE_NO_DATA) {
//			Error_Handler();
			ee_status = EE_WriteVariable32bits(RUN_NUMBER_INDEX, 0);
			if (ee_status != EE_OK) {
				Error_Handler();
			}
		}

		if (ee_status != EE_OK) {
			Error_Handler();
		}

		runNumber = runNumber + 1;

//		printf("%d\r\n", runNumber);

		ee_status = EE_WriteVariable32bits(RUN_NUMBER_INDEX, runNumber);

		/* Test is completed successfully */
		/* Lock the Flash Program Erase controller */
		HAL_FLASH_Lock();

		osDelay(1000);

		//Open the file system
		fres = f_mount(&FatFs, "", 1); //1=mount now
		if (fres != FR_OK) {
			myprintf("f_mount error (%i)\r\n", fres);
			while (1)
				;
		}

		//Let's get some statistics from the SD card
		DWORD free_clusters, free_sectors, total_sectors;

		FATFS *getFreeFs;

		fres = f_getfree("", &free_clusters, &getFreeFs);
		if (fres != FR_OK) {
			myprintf("f_getfree error (%i)\r\n", fres);
			while (1)
				;
		}

		//Formula comes from ChaN's documentation
		total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
		free_sectors = free_clusters * getFreeFs->csize;

		printf(
				"SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n",
				total_sectors / 2, free_sectors / 2);

		BYTE readBuf[256];
		runNumber = 69999;
		sprintf(filename, "log-%d.txt", runNumber);

		fres = f_open(&fil, filename,
		FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

//		fres = f_open(&fil, "log.txt",
//		FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
		if (fres == FR_OK) {
			printf("I was able to open 'write.txt' for writing\r\n");
		} else {
			printf("f_open error (%i)\r\n", fres);
		}

		f_printf(&fil,
				"StartNewRun;\r\nCC,CV,FCP,FCT,fcf1,fcf2,mc,mv,fcc,fcv,bci,bvi,bco,bvo,fcbmehum,fcbmetemp,h21,h22,h23,h24\r\n");

		//Be a tidy kiwi - don't forget to close your file!
		f_close(&fil);
	}
	//We're done, so de-mount the drive
//	f_mount(NULL, "", 0);

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
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(SWT1_GPIO_Port, SWT1_Pin)) {

			printf("Writing to file: %s", filename);

//		else {
			fres = f_open(&fil, filename,
			FA_WRITE | FA_OPEN_APPEND
//			| FA_CREATE_ALWAYS
					);
			if (fres == FR_OK) {
				myprintf("I was able to open 'write.txt' for writing\r\n");
			} else {
				myprintf("f_open error (%i)\r\n", fres);
			}

//		f_printf(&fil, "tesst %d", 1);

			f_printf(&fil, "%d,%d,", data1.cap_data.cap_curr,
					data1.cap_data.cap_volt);
			f_printf(&fil, "%d,%d,", data1.fc_data1.fc_press,
					data1.fc_data1.fc_temp);
			f_printf(&fil, "%d,%d,", data1.fc_data2.fan_rpm1,
					data1.fc_data2.fan_rpm2);
			f_printf(&fil, "%d,%d,", data1.mtr_data.mtr_curr,
					data1.mtr_data.mtr_volt);
			f_printf(&fil, "%d,%d,", data2.RelPackFc.fc_curr,
					data2.RelPackFc.fc_volt);
			f_printf(&fil, "%d,%d,", data2.boost_data1.in_curr,
					data2.boost_data1.in_volt);
			f_printf(&fil, "%d,%d,", data2.boost_data2.out_curr,
					data2.boost_data2.out_volt);
			f_printf(&fil, "%d,%d,", data2.fc_data3.bme_humid,
					data2.fc_data3.bme_temp);
			f_printf(&fil, "%d,", data3.h2_data.h2_sense_1);
			f_printf(&fil, "%d,", data3.h2_data.h2_sense_2);
			f_printf(&fil, "%d,", data3.h2_data.h2_sense_3);
			f_printf(&fil, "%d,\r\n", data3.h2_data.h2_sense_4);

			f_close(&fil);

			printf(">CC:%d\n\r>CV:%d\n\r", data_recieved_1.cap_data.cap_curr,
					data_recieved_1.cap_data.cap_volt);
			printf(">FCP:%d\n\r>FCT:%d\n\r", data_recieved_1.fc_data1.fc_press,
					data_recieved_1.fc_data1.fc_temp);
			printf(">FCF1:%d\n\r>FCF2:%d\n\r",
					data_recieved_1.fc_data2.fan_rpm1,
					data_recieved_1.fc_data2.fan_rpm2);
			printf(">MC:%d\n\r>MV:%d\n\r", data_recieved_1.mtr_data.mtr_curr,
					data_recieved_1.mtr_data.mtr_volt);
			printf(">FCC:%d\n\r>FCV:%d\n\r", data_recieved_2.RelPackFc.fc_curr,
					data_recieved_2.RelPackFc.fc_volt);
			printf(">BCI:%d\n\r>BVI:%d\n\r",
					data_recieved_2.boost_data1.in_curr,
					data_recieved_2.boost_data1.in_volt);
			printf(">BCO:%d\n\r>BVO:%d\n\r",
					data_recieved_2.boost_data2.out_curr,
					data_recieved_2.boost_data2.out_volt);
			printf(">FCBMEHUM:%d\n\r>FCBMET:%d\n\r",
					data_recieved_2.fc_data3.bme_humid,
					data_recieved_2.fc_data3.bme_temp);
			printf(">H2SNS1:%d\n\r", data_recieved_3.h2_data.h2_sense_1);
			printf(">H2SNS2:%d\n\r", data_recieved_3.h2_data.h2_sense_2);
			printf(">H2SNS3:%d\n\r", data_recieved_3.h2_data.h2_sense_3);
			printf(">H2SNS4:%d\n\r", data_recieved_3.h2_data.h2_sense_4);
			ublox_TestI2C();
			osDelay(100);

		}

//		ublox_TestI2C();

	}
	/* USER CODE END StartAuxilaryTask */
}
