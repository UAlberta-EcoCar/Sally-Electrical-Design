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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CAN_SYNC_Pin GPIO_PIN_1
#define CAN_SYNC_GPIO_Port GPIOA
#define ADC_VA_Pin GPIO_PIN_3
#define ADC_VA_GPIO_Port GPIOA
#define PWM1A_Pin GPIO_PIN_6
#define PWM1A_GPIO_Port GPIOA
#define PWM1B_Pin GPIO_PIN_7
#define PWM1B_GPIO_Port GPIOA
#define ADC_VB_Pin GPIO_PIN_0
#define ADC_VB_GPIO_Port GPIOB
#define PHA_Pin GPIO_PIN_8
#define PHA_GPIO_Port GPIOA
#define PHB_Pin GPIO_PIN_9
#define PHB_GPIO_Port GPIOA
#define NSTBY_Pin GPIO_PIN_10
#define NSTBY_GPIO_Port GPIOA
#define ENB_Pin GPIO_PIN_3
#define ENB_GPIO_Port GPIOB
#define ENA_Pin GPIO_PIN_4
#define ENA_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
