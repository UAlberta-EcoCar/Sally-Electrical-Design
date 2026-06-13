/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define NRST_Pin GPIO_PIN_10
#define NRST_GPIO_Port GPIOG
#define VOLT_MEAS_BUF_FC_UP_Pin GPIO_PIN_0
#define VOLT_MEAS_BUF_FC_UP_GPIO_Port GPIOC
#define VOLT_MEAS_BUF_FC_OUT_Pin GPIO_PIN_1
#define VOLT_MEAS_BUF_FC_OUT_GPIO_Port GPIOC
#define VOLT_MEAS_BUF_CAP_Pin GPIO_PIN_2
#define VOLT_MEAS_BUF_CAP_GPIO_Port GPIOC
#define VOLT_MEAS_BUF_RES_HIGH_Pin GPIO_PIN_3
#define VOLT_MEAS_BUF_RES_HIGH_GPIO_Port GPIOC
#define VOLT_MEAS_BUF_RES_LOW_Pin GPIO_PIN_0
#define VOLT_MEAS_BUF_RES_LOW_GPIO_Port GPIOA
#define MTR_RLY_RELAY_EN_Pin GPIO_PIN_1
#define MTR_RLY_RELAY_EN_GPIO_Port GPIOA
#define MTR_RLY_COIL_CUR_Pin GPIO_PIN_2
#define MTR_RLY_COIL_CUR_GPIO_Port GPIOA
#define MTR_RLY_STATUS_Pin GPIO_PIN_3
#define MTR_RLY_STATUS_GPIO_Port GPIOA
#define MTR_RLY_CUR_SENSE_Pin GPIO_PIN_4
#define MTR_RLY_CUR_SENSE_GPIO_Port GPIOA
#define RES_HIGH_RLY_RELAY_EN_Pin GPIO_PIN_5
#define RES_HIGH_RLY_RELAY_EN_GPIO_Port GPIOA
#define RES_HIGH_RLY_COIL_CUR_Pin GPIO_PIN_6
#define RES_HIGH_RLY_COIL_CUR_GPIO_Port GPIOA
#define RES_HIGH_RLY_STATUS_Pin GPIO_PIN_7
#define RES_HIGH_RLY_STATUS_GPIO_Port GPIOA
#define RES_HIGH_RLY_CUR_SENSE_Pin GPIO_PIN_4
#define RES_HIGH_RLY_CUR_SENSE_GPIO_Port GPIOC
#define RES_LOW_RLY_RELAY_EN_Pin GPIO_PIN_5
#define RES_LOW_RLY_RELAY_EN_GPIO_Port GPIOC
#define RES_LOW_RLY_COIL_CUR_Pin GPIO_PIN_0
#define RES_LOW_RLY_COIL_CUR_GPIO_Port GPIOB
#define RES_LOW_RLY_STATUS_Pin GPIO_PIN_1
#define RES_LOW_RLY_STATUS_GPIO_Port GPIOB
#define RES_LOW_RLY_CUR_SENSE_Pin GPIO_PIN_2
#define RES_LOW_RLY_CUR_SENSE_GPIO_Port GPIOB
#define CAP_RLY_RELAY_EN_Pin GPIO_PIN_7
#define CAP_RLY_RELAY_EN_GPIO_Port GPIOE
#define CAP_RLY_COIL_CUR_Pin GPIO_PIN_8
#define CAP_RLY_COIL_CUR_GPIO_Port GPIOE
#define CAP_RLY_STATUS_Pin GPIO_PIN_9
#define CAP_RLY_STATUS_GPIO_Port GPIOE
#define CAP_RLY_CUR_SENSE_Pin GPIO_PIN_10
#define CAP_RLY_CUR_SENSE_GPIO_Port GPIOE
#define TEMP_BOARD_VIN_DIODE_Pin GPIO_PIN_11
#define TEMP_BOARD_VIN_DIODE_GPIO_Port GPIOE
#define TEMP_BOARD_CENTER_Pin GPIO_PIN_12
#define TEMP_BOARD_CENTER_GPIO_Port GPIOE
#define VOLT_MEAS_BUF_MTR_OUT_Pin GPIO_PIN_13
#define VOLT_MEAS_BUF_MTR_OUT_GPIO_Port GPIOE
#define LED1B_Pin GPIO_PIN_14
#define LED1B_GPIO_Port GPIOE
#define SYS_7V_VOLT_Pin GPIO_PIN_12
#define SYS_7V_VOLT_GPIO_Port GPIOB
#define SYS_12V_VOLT_Pin GPIO_PIN_13
#define SYS_12V_VOLT_GPIO_Port GPIOB
#define SYS_12V_CUR_Pin GPIO_PIN_14
#define SYS_12V_CUR_GPIO_Port GPIOB
#define SYS_7V_CUR_Pin GPIO_PIN_15
#define SYS_7V_CUR_GPIO_Port GPIOB
#define FC_RLY_FC_EN_Pin GPIO_PIN_9
#define FC_RLY_FC_EN_GPIO_Port GPIOD
#define FC_RLY_FC_CUR_Pin GPIO_PIN_10
#define FC_RLY_FC_CUR_GPIO_Port GPIOD
#define GPBTN1_Pin GPIO_PIN_10
#define GPBTN1_GPIO_Port GPIOA
#define LED2R_Pin GPIO_PIN_15
#define LED2R_GPIO_Port GPIOA
#define GPBTN3_Pin GPIO_PIN_5
#define GPBTN3_GPIO_Port GPIOB
#define GPBTN2_Pin GPIO_PIN_6
#define GPBTN2_GPIO_Port GPIOB
#define BOOT0_Pin GPIO_PIN_8
#define BOOT0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
