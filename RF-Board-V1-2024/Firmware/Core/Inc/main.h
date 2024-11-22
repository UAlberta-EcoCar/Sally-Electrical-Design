/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define VDDA_APPLi 3300
#define NSS_915_Pin GPIO_PIN_13
#define NSS_915_GPIO_Port GPIOC
#define NSS_868_Pin GPIO_PIN_14
#define NSS_868_GPIO_Port GPIOC
#define XCLK_Pin GPIO_PIN_0
#define XCLK_GPIO_Port GPIOF
#define NSS_915C4_Pin GPIO_PIN_4
#define NSS_915C4_GPIO_Port GPIOC
#define NSS_868C5_Pin GPIO_PIN_5
#define NSS_868C5_GPIO_Port GPIOC
#define NSS_24_Pin GPIO_PIN_0
#define NSS_24_GPIO_Port GPIOB
#define RST_GNSS_Pin GPIO_PIN_1
#define RST_GNSS_GPIO_Port GPIOB
#define EXTINT_GNSS_Pin GPIO_PIN_2
#define EXTINT_GNSS_GPIO_Port GPIOB
#define CAN_STBY_Pin GPIO_PIN_11
#define CAN_STBY_GPIO_Port GPIOB
#define TXEN_24_Pin GPIO_PIN_14
#define TXEN_24_GPIO_Port GPIOB
#define RXEN_24_Pin GPIO_PIN_15
#define RXEN_24_GPIO_Port GPIOB
#define DIO3_24_Pin GPIO_PIN_6
#define DIO3_24_GPIO_Port GPIOC
#define DIO2_24_Pin GPIO_PIN_7
#define DIO2_24_GPIO_Port GPIOC
#define DIO1_24_Pin GPIO_PIN_8
#define DIO1_24_GPIO_Port GPIOC
#define BUSY_24_Pin GPIO_PIN_9
#define BUSY_24_GPIO_Port GPIOC
#define RST_24_Pin GPIO_PIN_10
#define RST_24_GPIO_Port GPIOA
#define RST_868_Pin GPIO_PIN_13
#define RST_868_GPIO_Port GPIOA
#define DIO0_868_Pin GPIO_PIN_14
#define DIO0_868_GPIO_Port GPIOA
#define DIO1_868_Pin GPIO_PIN_15
#define DIO1_868_GPIO_Port GPIOA
#define DIO2_868_Pin GPIO_PIN_10
#define DIO2_868_GPIO_Port GPIOC
#define DIO3_868_Pin GPIO_PIN_11
#define DIO3_868_GPIO_Port GPIOC
#define DIO4_868_Pin GPIO_PIN_12
#define DIO4_868_GPIO_Port GPIOC
#define DIO5_868_Pin GPIO_PIN_2
#define DIO5_868_GPIO_Port GPIOD
#define RST_915_Pin GPIO_PIN_3
#define RST_915_GPIO_Port GPIOB
#define DIO0_915_Pin GPIO_PIN_4
#define DIO0_915_GPIO_Port GPIOB
#define DIO1_915_Pin GPIO_PIN_5
#define DIO1_915_GPIO_Port GPIOB
#define DIO2_915_Pin GPIO_PIN_6
#define DIO2_915_GPIO_Port GPIOB
#define DIO3_915_Pin GPIO_PIN_7
#define DIO3_915_GPIO_Port GPIOB
#define DIO4_915_Pin GPIO_PIN_8
#define DIO4_915_GPIO_Port GPIOB
#define DIO5_915_Pin GPIO_PIN_9
#define DIO5_915_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
