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
	h2.Identifier = 0x001;
	h2.TxFrameType = FDCAN_DATA_FRAME;
	h2.MessageMarker = 0xAA;
	h2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;


	/* Infinite loop */
	for (;;) {
		if (osOK == osSemaphoreAcquire(H2AlarmSemHandle, osWaitForever)) {

		}
		osDelay(1);
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



}

