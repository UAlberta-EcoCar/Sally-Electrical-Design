/*
 * leak_watchdog_task.c
 *
 *  Created on: Feb 26, 2025
 *      Author: abina
 */

#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include "cmsis_os.h"
#include "task_leak_watchdog.h"
#include "comp.h"
#include "log/debug-log.h"
#include "ecocar_can.h"
#include "task_sensor_data_aquire.h"
#include "fdcan.h"

#define H2_THRESH_1 1000 // mV of converted sensor read value
#define H2_THRESH_2 1000
#define H2_THRESH_3 1000
#define H2_THRESH_4 1000

// DAC1.OUT1 -> COMP1.Reference
// DAC1.OUT2 -> COMP2.Reference
// DAC2.OUT1 -> COMP6.Reference
// DAC4.OUT1 -> COMP7.Reference
// Each alarm comparator has a different reference so different thresholds can be set
// A default is set upon watchdog start up. All thresholds are the same.

void StartLeakWatchdogTask(void *argument) {
	/* USER CODE BEGIN StartLeakWatchdogTask */

	HAL_COMP_Start(&hcomp1);
	HAL_COMP_Start(&hcomp2);
	HAL_COMP_Start(&hcomp6);
	HAL_COMP_Start(&hcomp7);

	FDCAN_TxHeaderTypeDef h2;

	h2.BitRateSwitch = FDCAN_BRS_ON;
	h2.DataLength = 2;
	h2.ErrorStateIndicator = FDCAN_FLAG_ERROR_PASSIVE;
	h2.FDFormat = FDCAN_FD_CAN;
	h2.IdType = FDCAN_EXTENDED_ID;
	h2.Identifier = FDCAN_H2ALARM_ID;
	h2.TxFrameType = FDCAN_DATA_FRAME;
	h2.MessageMarker = 0xAA;
	h2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;


	/* Infinite loop */
	for (;;) {


//		if (H2_THRESH_1 <= h2_sensor_data.h2_sense1_mV)


		if (osOK == osSemaphoreAcquire(H2AlarmSemHandle, osWaitForever)) {
			// One of the alarms have tripped, figure out which one and respond appropriatly.
			if (0 != HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
				if (HAL_OK == HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &h2, &h2_sensor_data)) {
					log_info_success("Successfully transmitted H2 Alarm");
				}
			} else {
				log_err("Failed to send H2 Alarm signal, fifo full");
				// this means there was no room in the tx fifo, so give the semaphore again and retry.
				if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
					log_info("Retrying");
				}
			}
		}
		osDelay(10);
	}
	/* USER CODE END StartLeakWatchdogTask */
}

/**
 * @brief  Comparator trigger callback. Called for all comparators.
 * @param  hcomp  COMP handle
 * @retval None
 */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {
//	if (hcomp->Instance == hcomp1.Instance) {
//		// Alarm
//	} else if (hcomp->Instance == hcomp2.Instance){
//
//	} else if (hcomp->Instance == hcomp6.Instance){
//
//	} else if (hcomp->Instance == hcomp7.Instance){
//
//	}

	if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//		return;
	}

}

