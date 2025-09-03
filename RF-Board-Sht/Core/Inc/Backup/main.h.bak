/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPBTN1_Pin GPIO_PIN_13
#define GPBTN1_GPIO_Port GPIOC
#define GPBTN1_EXTI_IRQn EXTI15_10_IRQn
#define GPBTN2_RPM_Pin GPIO_PIN_14
#define GPBTN2_RPM_GPIO_Port GPIOC
#define GPBTN2_RPM_EXTI_IRQn EXTI15_10_IRQn
#define SD_CS_Pin GPIO_PIN_2
#define SD_CS_GPIO_Port GPIOC
#define SWT1_Pin GPIO_PIN_3
#define SWT1_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOA
#define SWT2_Pin GPIO_PIN_4
#define SWT2_GPIO_Port GPIOA
#define NSS_915_Pin GPIO_PIN_4
#define NSS_915_GPIO_Port GPIOC
#define NSS_868_Pin GPIO_PIN_5
#define NSS_868_GPIO_Port GPIOC
#define NSS_24_Pin GPIO_PIN_0
#define NSS_24_GPIO_Port GPIOB
#define NRST_GNSS_Pin GPIO_PIN_1
#define NRST_GNSS_GPIO_Port GPIOB
#define GNSS_EXTINT_Pin GPIO_PIN_2
#define GNSS_EXTINT_GPIO_Port GPIOB
#define RST_24_Pin GPIO_PIN_10
#define RST_24_GPIO_Port GPIOA
#define RST_868_Pin GPIO_PIN_10
#define RST_868_GPIO_Port GPIOC
#define RST_915_Pin GPIO_PIN_3
#define RST_915_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define HARDFAULT_TIMER 500000
#define SD_SPI_HANDLE hspi2
#define SPI2_CS_Pin SD_CS_Pin
#define SPI3_CS_GPIO_Port SD_CS_GPIO_Port
//typedef struct {
//	//FDCAN_FetPack_t fet_data;
//	FDCAN_RelPackMtr_t mtr_data;
//	FDCAN_RelPackCap_t cap_data;
//	FDCAN_FccPack1_t fc_data1;
//	FDCAN_FccPack2_t fc_data2;
//} telemetry_data1_t;
//
//typedef struct {
//	FDCAN_FccPack3_t fc_data3;
//	//FDCAN_H2Pack_t h2_data;
//	FDCAN_BOOSTPack_t boost_data1;
//	FDCAN_BOOSTPack2_t boost_data2;
//	FDCAN_RelPackFc_t RelPackFc;
//} telemetry_data2_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
