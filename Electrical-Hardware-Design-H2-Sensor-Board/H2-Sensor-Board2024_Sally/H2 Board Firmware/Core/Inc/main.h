/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum {
	H2_SYSTEM_STATE_INACTIVE,
	H2_SYSTEM_STATE_ALARM_ACTIVE,
	H2_SYSTEM_STATE_ALARM_TEST,
	H2_SYSTEM_STATE_ALARM_TEST_SILENT,
	H2_SYSTEM_STATE_CALIBRATE,
	H2_SYSTEM_STATE_ALARM_TRIPPED
} H2_SYSTEM_STATE;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern bool h2_leak;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define H2_SENSE_ADC_Pin GPIO_PIN_3
#define H2_SENSE_ADC_GPIO_Port GPIOA
#define H2_TARE_Pin GPIO_PIN_4
#define H2_TARE_GPIO_Port GPIOA
#define H2_TARE_EXTI_IRQn EXTI4_IRQn
#define BEEPER_Pin GPIO_PIN_5
#define BEEPER_GPIO_Port GPIOA
#define CAN_STBY_Pin GPIO_PIN_15
#define CAN_STBY_GPIO_Port GPIOA
#define LED_D1_PWM_Pin GPIO_PIN_3
#define LED_D1_PWM_GPIO_Port GPIOB
#define LED_D2_PWM_Pin GPIO_PIN_4
#define LED_D2_PWM_GPIO_Port GPIOB
#define LED_D3_PWM_Pin GPIO_PIN_5
#define LED_D3_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define HARD_FAULT_LED_DELAY 500000
#define SAMPLE_COUNT  UINT8_C(50)
#define ADC_BUF_LEN 40
#define ADC_CONST 0.80f
/* Define configASSERT() to call vAssertCalled() if the assertion fails.  The assertion
 has failed if the value of the parameter passed into configASSERT() equals zero. */
#define configASSERT ( x )     if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
