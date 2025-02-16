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
#include "tim.h"

#define DUTY_CYCLE 50

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

	driver_t tmc = { 0 };
	tmc.driver_nss_pin = SPI_NSCS_DRIVER_Pin;
	tmc.driver_nss_port = SPI_NSCS_DRIVER_GPIO_Port;
	tmc.hspi = &hspi3;

	driver_initialize(&tmc);

	hA = HAL_GPIO_ReadPin(HALL_U_GPIO_Port, HALL_U_Pin);
	hB = HAL_GPIO_ReadPin(HALL_V_GPIO_Port, HALL_V_Pin);
	hC = HAL_GPIO_ReadPin(HALL_W_GPIO_Port, HALL_W_Pin);
//	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
//	EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);
//	osDelay(100);


	htim1.Instance->CCR1 = DUTY_CYCLE;
	htim1.Instance->CCR2 = 0;
	htim1.Instance->CCR3 = 0;

	// u
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	// v
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	// w
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
		EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_SET);


//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);
//
	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);
//
	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
	uint8_t read_result[4] = { 0 };
	/* Infinite loop */
	for (;;) {
		HAL_GPIO_TogglePin(GPLED_2_GPIO_Port, GPLED_2_Pin);
//		HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
//		EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);
//		osDelay(100);
//		HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
//		EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_SET);
		switch (step) {
		case 1:
			step1();
			break;
		case 2:
			step2();
			break;
		case 3:
			step3();
			break;
		case 4:
			step4();
			break;
		case 5:
			step5();
			break;
		case 6:
			step6();
			break;
		}
//		if (step > 6) {
//			step = 1;
//		}
//		step++;
		osDelay(150);
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
			step = 2;
			//step2();
		} else if (!hA && hB && !hC) {
			step = 3;
			//step3();
		} else if (!hA && hB && hC) {
			step = 4;
			//step4();
		} else if (!hA && !hB && hC) {
			step = 5;
			//step5();
		} else if (hA && !hB && hC) {
			step = 6;
			//step6();
		} else if (hA && !hB && !hC) {
			step = 7;
			//step1();
		}
	}
}

void step1() {
	// z
//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR1 = 0;
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	// l
//	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR2 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	// h
//	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR3 = 0;
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step2() {
	// h
//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR1 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	//l
//	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR2 = 0;
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	//z
	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step3() {
	// h
//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR1 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	//z
	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	//l
//	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR3 = 0;
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step4() {
	// z
	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_RESET);

	//h
//	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR2 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	// l
//	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR3 = 0;
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}

void step5() {
	// l
//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR1 = 0;
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	// h
//	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR2 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_SET);

	// z
	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_RESET);
}

void step6() {
	// l
//	HAL_GPIO_WritePin(PHASE_U_H_GPIO_Port, PHASE_U_H_Pin, GPIO_PIN_RESET);
	htim1.Instance->CCR1 = 0;
	HAL_GPIO_WritePin(PHASE_U_L_GPIO_Port, PHASE_U_L_Pin, GPIO_PIN_SET);

	//z
	HAL_GPIO_WritePin(PHASE_V_H_GPIO_Port, PHASE_V_H_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PHASE_V_L_GPIO_Port, PHASE_V_L_Pin, GPIO_PIN_RESET);

	// h
//	HAL_GPIO_WritePin(PHASE_W_H_GPIO_Port, PHASE_W_H_Pin, GPIO_PIN_SET);
	htim1.Instance->CCR3 = DUTY_CYCLE;
	HAL_GPIO_WritePin(PHASE_W_L_GPIO_Port, PHASE_W_L_Pin, GPIO_PIN_SET);
}
