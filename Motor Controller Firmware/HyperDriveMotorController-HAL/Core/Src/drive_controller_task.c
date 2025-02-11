/*
 * drive_controller_task.c
 *
 *  Created on: Feb 4, 2025
 *      Author: abina
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

void step1();
void step2();
void step3();
void step4();
void step5();
void step6();

uint8_t step = 0;
volatile uint8_t hU = 0, hV = 0, hW = 0;
void StartDriveController(void *argument) {
	/* USER CODE BEGIN StartDriveController */
	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
	EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_SET);
	/* Infinite loop */
	for (;;) {

		HAL_GPIO_TogglePin(GPLED_2_GPIO_Port, GPLED_2_Pin);
		//if (hU && !hV && !hW) {
		//	step = 0;
		osDelay(10);
		step1();
		//} else if (hU && hV && !hW) {
		//	step = 1;
		osDelay(10);
		step2();
		//} else if (!hU && hV && !hW) {
		//	step = 2;
		osDelay(10);
		step3();
		//} else if (!hU && hV && hW) {
		//	step = 3;
		osDelay(10);
		step4();
		//} else if (!hU && !hV && hW) {
		step = 4;
		osDelay(10);
		step5();
		//} else if (hU && !hV && hW) {
		//	step = 5;
		osDelay(10);
		step6();
		//} else {
		///	step = 0;
		osDelay(10);
		step1();
		//}
		osDelay(10);
//		HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port, EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);
//		osDelay(5000);
	}
	/* USER CODE END StartDriveController */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == HALL_U_Pin || GPIO_Pin == HALL_V_Pin
			|| GPIO_Pin == HALL_W_Pin) {

		hU = HAL_GPIO_ReadPin(HALL_U_GPIO_Port, HALL_U_Pin);
		hV = HAL_GPIO_ReadPin(HALL_V_GPIO_Port, HALL_V_Pin);
		hW = HAL_GPIO_ReadPin(HALL_W_GPIO_Port, HALL_W_Pin);

		if (hU && !hV && !hW) {
			step = 0;
			step1();
		} else if (hU && hV && !hW) {
			step = 1;
			step2();
		} else if (!hU && hV && !hW) {
			step = 2;
			step3();
		} else if (!hU && hV && hW) {
			step = 3;
			step4();
		} else if (!hU && !hV && hW) {
			step = 4;
			step5();
		} else if (hU && !hV && hW) {
			step = 5;
			step6();
		} else {
			step = 0;
			step1();
		}
	}
}

void step1() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step2() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step3() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step4() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step5() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step6() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}
