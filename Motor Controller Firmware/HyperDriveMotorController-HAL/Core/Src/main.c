/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ctype.h>
#include <stdio.h>
#include <tmc6100_driver.h>
#include "tmc6100.h"
//#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t TxBuffer[] =
		"Hello World! From STM32 USB CDC Device To Virtual COM Port\r\n";
uint8_t TxBufferLen = sizeof(TxBuffer);

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

//int _write(int file, char *ptr, int len) {
//
////	while (CDC_Transmit_FS(ptr, len) != USBD_OK)
////		;
//	return len;
//}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC4_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_SPI3_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(EXT_DRIVER_EN_MCU_OUT_GPIO_Port,
			EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_SET);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

//		driver_t drv = { 0 };
//		drv.driver_nss_pin = SPI_NSCS_DRIVER_Pin;
//		drv.driver_nss_port = SPI_NSCS_CONTROLLER_A_GPIO_Port;
//		drv.hspi = &hspi3;
//		tmc6100_ioin_t ioin = { 0 };
//		driver_spi_read(&drv, TMC6100_GSTAT, &ioin.reg_rawa_ioin);
		HAL_Delay(10);
		step1();
		HAL_Delay(10);
		step2();
		HAL_Delay(10);
		step3();
		HAL_Delay(10);
		step4();
		HAL_Delay(10);
		step5();
		HAL_Delay(10);
		step6();
		HAL_Delay(10);
		HAL_GPIO_TogglePin(GPLED_1_GPIO_Port, GPLED_1_Pin);

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
