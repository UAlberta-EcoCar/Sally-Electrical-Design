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
#include "mq8calibration.h"
#include "dac.h"

#define H2_THRESH_1 33000 // mV of converted sensor read value
#define H2_THRESH_2 33000
#define H2_THRESH_3 33000
#define H2_THRESH_4 33000

#define RLOAD_MQ8 4700 // load on the sensor

extern osMessageQueueId_t CANMessageRecieveQHandle;
extern osSemaphoreId_t H2AlarmSemHandle;
extern osSemaphoreId_t H2tareCurrentEnviormentHandle;
extern osMessageQueueId_t CANMessageTransmitQHandle;

// global state
H2_Alarm_State_t alarm_state = H2_ALARM_DISARMED;

uint32_t dac1[2] = { 0 };
uint32_t dac2[1] = { 0 };
uint32_t dac4[1] = { 0 };

uint8_t sensor_heating = 0;

extern uint32_t clean_air_constant_mV;
// DAC1.OUT1 -> COMP1.Reference
// DAC1.OUT2 -> COMP2.Reference
// DAC2.OUT1 -> COMP6.Reference
// DAC4.OUT1 -> COMP7.Reference
// Each alarm comparator has a different reference so different thresholds can be set
// A default is set upon watchdog start up. All thresholds are the same.
#define TIM_FREQ 170000000
static void Tone(uint32_t Frequency, uint32_t Duration) {
	TIM2->ARR = (1000000UL / Frequency) - 1; // Set The PWM Frequency
	TIM2->CCR1 = (TIM2->ARR >> 1); // Set Duty Cycle 50%
	osDelay(Duration); // Wait For The Tone Duration
}

void StartLeakWatchdogTask(void *argument) {
	/* USER CODE BEGIN StartLeakWatchdogTask */
//	HAL_TIMEx_PWMN_Start(&htim5, TIM_CHANNEL_2);
	// Use the DACs to set comparator voltage
	dac1[0] = (clean_air_constant_mV + H2_THRESH_1) * 4096.0f / 3.3f; // + clean_air_constant_mV;
	dac1[1] = (clean_air_constant_mV + H2_THRESH_2) * 4096.0f / 3.3f; // + clean_air_constant_mV;
	dac2[0] = (clean_air_constant_mV + H2_THRESH_3) * 4096.0f / 3.3f; // + clean_air_constant_mV;
	dac4[0] = (clean_air_constant_mV + H2_THRESH_4) * 4096.0f / 3.3f; // + clean_air_constant_mV;

	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac4, DAC_CHANNEL_1);

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac1[0]);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac1[1]);
	HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac2[0]);
	HAL_DAC_SetValue(&hdac4, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac4[0]);

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

//	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
//	FDCAN_H2Pack_t data1 = { 0 };
//	FDCAN_H2Pack2_t data2 = { 0 };

	float h2_ppm[4] = { 0 };

	osDelay(5000);

	sensor_heating = 1;

//	HAL_COMP_Start(&hcomp1);
//	HAL_COMP_Start(&hcomp2);
//	HAL_COMP_Start(&hcomp6);
//	HAL_COMP_Start(&hcomp7);

	/* Infinite loop */
	for (;;) {
//		Tone(330, 250);
//		h2_ppm[0] = (float) (RL_VALUE * );

		if (H2_ALARM_ARMED == alarm_state) {
			if (H2_THRESH_1 + clean_air_constant_mV
					<= sensor_data.h2_sense1_mV) {
				alarm_state = H2_ALARM_TRIGGERED;
			}

			if (H2_THRESH_2 + clean_air_constant_mV
					<= sensor_data.h2_sense2_mV) {
				alarm_state = H2_ALARM_TRIGGERED;
			}

			if (H2_THRESH_3 + clean_air_constant_mV
					<= sensor_data.h2_sense3_mV) {
				alarm_state = H2_ALARM_TRIGGERED;
			}

			if (H2_THRESH_4 + clean_air_constant_mV
					<= sensor_data.h2_sense4_mV) {
				alarm_state = H2_ALARM_TRIGGERED;
			}
		}
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
	if (H2_ALARM_ARMED == alarm_state) {
		if (COMP1 == hcomp->Instance) {
			// Alarm
			osSemaphoreRelease(H2AlarmSemHandle);
		} else if (COMP2 == hcomp->Instance) {
			osSemaphoreRelease(H2AlarmSemHandle);
		} else if (COMP6 == hcomp->Instance) {
			osSemaphoreRelease(H2AlarmSemHandle);
		} else if (COMP7 == hcomp->Instance) {
			osSemaphoreRelease(H2AlarmSemHandle);
		}
	}
//	if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//		log_critical(
//				"High H2 Concentration Detected. Triggering System Shutdown.");
////		return;
//	}

//	alarm_state = H2_ALARM_TRIGGERED;

}

uint32_t debaunce_btn_tare = 0;
uint32_t debaunce_btn_1 = 0;
uint32_t debaunce_btn_2 = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	switch (GPIO_Pin) {
	case GPBTN1_Pin:
		if ((HAL_GetTick() - debaunce_btn_1) >= 500) {
			osSemaphoreRelease(H2AlarmSemHandle);
			debaunce_btn_1 = HAL_GetTick();
			alarm_state = H2_ALARM_TRIGGERED;
		}
		break;
	case GPBTN2_Pin:
		if ((HAL_GetTick() - debaunce_btn_1) >= 500) {
			debaunce_btn_2 = HAL_GetTick();
			if (H2_ALARM_TRIGGERED != alarm_state)
				alarm_state = H2_ALARM_ARMED;
		}
		break;
	case H2_TARE_Pin:
		if ((HAL_GetTick() - debaunce_btn_tare) >= 500) {
			debaunce_btn_tare = HAL_GetTick();
			osSemaphoreRelease(H2tareCurrentEnviormentHandle);
		}
		break;
	default:
		break;
	}
}

