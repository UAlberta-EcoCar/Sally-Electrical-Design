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
#include "tmc6100.h"
#include "tmc6100_driver.h"
#include "spi.h"

void step1();
void step2();
void step3();
void step4();
void step5();
void step6();

uint8_t step = 0;
volatile uint8_t hA = 0, hB = 0, hC = 0;
void StartDriveController(void *argument) {
	/* USER CODE BEGIN StartDriveController */
	hA = HAL_GPIO_ReadPin(HALL_U_GPIO_Port, HALL_U_Pin);
	hB = HAL_GPIO_ReadPin(HALL_V_GPIO_Port, HALL_V_Pin);
	hC = HAL_GPIO_ReadPin(HALL_W_GPIO_Port, HALL_W_Pin);
	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
	EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);
	osDelay(100);
	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
	EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
	uint8_t read_result[4] = { 0 };
	/* Infinite loop */
	for (;;) {

		HAL_GPIO_TogglePin(GPLED_2_GPIO_Port, GPLED_2_Pin);

		HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
				GPIO_PIN_RESET);

		uint8_t prepped_buffer = (uint8_t) TMC6100_IOIN | 0x7f;

		if (!HAL_SPI_Transmit(&hspi3, &prepped_buffer, 1, HAL_MAX_DELAY)
				!= HAL_OK) {
//			LOG_ERROR("Failed to read register 0x%x", reg_addr);
		}

		if (!HAL_SPI_Receive(&hspi3, read_result, 4, HAL_MAX_DELAY) != HAL_OK) {
//			LOG_ERROR("Failed to read register 0x%x", reg_addr);
		}

		HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
				GPIO_PIN_SET);

//		step1();
//		osDelay(100);
//		step2();
//		//if (hU && !hV && !hW) {
//		//	step = 0;
//		osDelay(10);
//		step1();
//		//} else if (hU && hV && !hW) {
//		//	step = 1;
//		osDelay(10);
//		step2();
//		//} else if (!hU && hV && !hW) {
//		//	step = 2;
//		osDelay(10);
//		step3();
//		//} else if (!hU && hV && hW) {
//		//	step = 3;
//		osDelay(10);
//		step4();
//		//} else if (!hU && !hV && hW) {
////		step = 4;
//		osDelay(10);
//		step5();
//		//} else if (hU && !hV && hW) {
//		//	step = 5;
//		osDelay(10);
//		step6();
		//} else {
		///	step = 0;
//		osDelay(10);
//		step1();
		//}
//		osDelay(10);
//		HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port, EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);
//		osDelay(5000);

		osDelay(100);

	}
	/* USER CODE END StartDriveController */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == HALL_U_Pin || GPIO_Pin == HALL_V_Pin
			|| GPIO_Pin == HALL_W_Pin) {

		hA = HAL_GPIO_ReadPin(HALL_U_GPIO_Port, HALL_U_Pin);
		hB = HAL_GPIO_ReadPin(HALL_V_GPIO_Port, HALL_V_Pin);
		hC = HAL_GPIO_ReadPin(HALL_W_GPIO_Port, HALL_W_Pin);

		if (hA && hB && !hC) {
			step = 1;
			step1();
		} else if (!hA && hB && !hC) {
			step = 2;
			step2();
		} else if (!hA && hB && hC) {
			step = 3;
			step3();
		} else if (!hA && !hB && hC) {
			step = 4;
			step4();
		} else if (hA && !hB && hC) {
			step = 5;
			step5();
		} else if (hA && !hB && !hC) {
			step = 6;
			step6();
		}
	}
}

void step1() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step2() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step3() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step4() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step5() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step6() {
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}
