/*
 * drive_telemetry_task.c
 *
 *  Created on: Feb 4, 2025
 *      Author: abina
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "dma.h"
#include "adc.h"

#define ADC_RESOLUTION 4096
#define ADC_VREF 3.3
#define ADC_CONSTANT 0.8f // 3.3V / 4096 = V/step -> *1000 = mV / step //ADC_VREF / ADC_RESOLUTION

/* Individual ADC channel Offsets. */
#define ADC_1_IN_14_OFFSET 0 //(140 - 51)

/* ADC ELEMENT INDEXES */

//ADC1
#define CUR_SNS_LOW_SIDE_ADC1 1
#define VOLT_SNS_U_ADC1 0
#define CNTRL_TMP_SNS_ADC1 2

// ADC2


uint32_t temp = 0;
uint32_t ADC1_RESULTS[3] = { 0 };

/* TPS2663 Parameters */
// A/A
#define TPS2663_GAIN_IMON 27.9f
// Ohms
#define TPS2663_R_IMON 71.5f
float load = 0;
void StartDriveTelemetry(void *argument) {
	/* USER CODE BEGIN StartDriveTelemetry */

	// Calibrate The ADC On Power-Up For Better Accuracy
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC1_RESULTS, 3);

	/* Infinite loop */
	for (;;) {

		/* Low side current draw; read from IMON pin on TPS2663 */
		// V(IMON) = I(OUT) * GAIN(IMON) * R(IMON) -> I(OUT) = V(IMON) / GAIN(IMON) / R(IMON) mA
		// Start ADC Conversion
		// Pass (The ADC Instance, Result Buffer Address, Buffer Length)
//		HAL_Delay(1);
		static float ave[40] = { 0.0f };

		static int buf_i = 0;

		static float averaged = 0;

		if (buf_i >= 40) {
			buf_i = 0;
			for (int i = 0; i < 40; i++) {
				averaged += ave[i];
			}
			averaged = averaged / 40;
		}

		ave[buf_i] = (float) ADC1_RESULTS[1];
		buf_i = buf_i + 1;
		load = (averaged * ADC_CONSTANT + ADC_1_IN_14_OFFSET)
				/ TPS2663_GAIN_IMON / TPS2663_R_IMON * 1000; // mA
		HAL_GPIO_TogglePin(GPLED_3_GPIO_Port, GPLED_3_Pin);
		HAL_GPIO_TogglePin(GPLED_5_GPIO_Port, GPLED_5_Pin);
		osDelay(100);
	}
	/* USER CODE END StartDriveTelemetry */
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
//
//	//temp = ((AD_RES[3]) * ADC_CONSTANT + ADC_1_IN_14_OFFSET);
//}
