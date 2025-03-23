/*
 * leak_watchdog_task.c
 *
 * This task monitors h2 levels and sets the alarm.
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
#include "tim.h"

#define H2_THRESH_1 1000 // mV of converted sensor read value
#define H2_THRESH_2 1000
#define H2_THRESH_3 1000
#define H2_THRESH_4 1000

extern osMessageQueueId_t CANMessageRecieveQHandle;
extern osSemaphoreId_t H2AlarmSemHandle;
extern osMessageQueueId_t CANMessageTransmitQHandle;

// global state
H2_Alarm_State_t alarm_state = H2_ALARM_DISARMED;

// DAC1.OUT1 -> COMP1.Reference
// DAC1.OUT2 -> COMP2.Reference
// DAC2.OUT1 -> COMP6.Reference
// DAC4.OUT1 -> COMP7.Reference
// Each alarm comparator has a different reference so different thresholds can be set
// A default is set upon watchdog start up. All thresholds are the same.
#define TIM_FREQ 170000000

int presForFrequency(int frequency) {
	if (frequency == 0)
		return 0;
	return ((TIM_FREQ / (1000 * frequency)) - 1);  // 1 is added in the register
}
void StartLeakWatchdogTask(void *argument) {
	/* USER CODE BEGIN StartLeakWatchdogTask */
//	HAL_TIMEx_PWMN_Start(&htim5, TIM_CHANNEL_2);
	HAL_COMP_Start(&hcomp1);
	HAL_COMP_Start(&hcomp2);
	HAL_COMP_Start(&hcomp6);
	HAL_COMP_Start(&hcomp7);

	FDCAN_TxHeaderTypeDef h2;

	h2.BitRateSwitch = FDCAN_BRS_OFF;
	h2.DataLength = 1;
	h2.ErrorStateIndicator = FDCAN_FLAG_ERROR_PASSIVE;
	h2.FDFormat = FDCAN_FD_CAN;
	h2.IdType = FDCAN_STANDARD_ID;
	h2.Identifier = FDCAN_H2ALARM_ID;
	h2.TxFrameType = FDCAN_CLASSIC_CAN;
	h2.MessageMarker = 0xAA;
	h2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;

	alarm_state = H2_ALARM_DISARMED;

	uint8_t alarm = 1;
//	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
//	FDCAN_H2Pack_t data1 = { 0 };
//	FDCAN_H2Pack2_t data2 = { 0 };

	/* Infinite loop */
	for (;;) {

//		if (H2_THRESH_1 <= sensor_data.h2_sense1_mV) {
//			alarm_state = H2_ALARM_TRIGGERED;
//		}
//
//		if (H2_THRESH_2 <= sensor_data.h2_sense2_mV) {
//			alarm_state = H2_ALARM_TRIGGERED;
//		}
//
//		if (H2_THRESH_3 <= sensor_data.h2_sense3_mV) {
//			alarm_state = H2_ALARM_TRIGGERED;
//		}
//
//		if (H2_THRESH_4 <= sensor_data.h2_sense4_mV) {
//			alarm_state = H2_ALARM_TRIGGERED;
//		}

		// if the comparator releases a semaphore.
		if (osOK == osSemaphoreAcquire(H2AlarmSemHandle, 0)
				|| H2_ALARM_TRIGGERED == alarm_state) {
			// One of the alarms have tripped, figure out which one and respond appropriatly.
//			if (0 == HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {

//			data1.h2_sense1_mV = sensor_data.h2_sense1_mV;
//			data1.h2_sense2_mV = sensor_data.h2_sense2_mV;
//			data1.h2_sense3_mV = sensor_data.h2_sense3_mV;
//			data1.h2_sense4_mV = sensor_data.h2_sense4_mV;

			if (HAL_OK
					== HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &h2, &alarm)) {
				log_info("Successfully transmitted H2 Alarm");
			}

//			htim8.Instance->CCR1 = 10;
//			} else {
//				log_err("Failed to send H2 Alarm signal, fifo full");
//				// this means there was no room in the tx fifo, so give the semaphore again and retry.
//				if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//					log_info("Retrying");
//				}
//			}
//			__HAL_TIM_SET_PRESCALER(&htim5, presForFrequency(1000));
			if (H2_ALARM_ARMED == alarm_state
					|| H2_ALARM_TRIGGERED == alarm_state) {
				alarm_state = H2_ALARM_TRIGGERED;
			}

		}
		osDelay(50);
	}
	/* USER CODE END StartLeakWatchdogTask */
}

/**
 * @brief  Comparator trigger callback. Called for all comparators.
 * @param  hcomp  COMP handle
 * @retval None
 */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {
	if (COMP1 == hcomp->Instance) {
		// Alarm
	} else if (COMP2 == hcomp->Instance) {

	} else if (COMP6 == hcomp->Instance) {

	} else if (COMP7 == hcomp->Instance) {

	}

//	if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//		log_critical(
//				"High H2 Concentration Detected. Triggering System Shutdown.");
////		return;
//	}

//	alarm_state = H2_ALARM_TRIGGERED;

}

uint32_t debaunce_btn = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	switch (GPIO_Pin) {
	case GPBTN1_Pin:

//		debaunce_h2_test_btn = HAL_GetTick();
//
//		if ((HAL_GetTick() - debaunce_h2_test_btn) >= 100) {
//			log_info("Activating H2 Alarm Test");
//			debaunce_h2_test_btn = 0;
		osSemaphoreRelease(H2AlarmSemHandle);
		alarm_state = H2_ALARM_TRIGGERED;
//		}

		break;
	case GPBTN2_Pin:
		if (H2_ALARM_TRIGGERED != alarm_state)
			alarm_state = H2_ALARM_ARMED;
		break;
	default:
		break;
	}
}

