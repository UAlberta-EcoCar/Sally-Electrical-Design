/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
     PC8   ------> I2C3_SCL
     PC9   ------> I2C3_SDA
     PB3   ------> FDCAN3_RX
     PB4   ------> FDCAN3_TX
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPLED_3_Pin|GPLED_2_Pin|GPLED_1_Pin|Controller_A_ENI_Pin
                          |Controller_A_DIR_Pin|EXT_DRIVER_EN_MCU_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPLED_5_GPIO_Port, GPLED_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CONTROLLER_B_STATUS_OUT_Pin|PHASE_U_L_Pin|PHASE_V_L_Pin|PHASE_W_L_Pin
                          |SPI_MEM_CS_Pin|SPI_NSCS_CONTROLLER_A_Pin|SPI_NSCS_DRIVER_Pin|GPLED_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PHASE_U_H_Pin|PHASE_V_H_Pin|PHASE_W_H_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPLED_3_Pin GPLED_2_Pin GPLED_1_Pin Controller_A_ENI_Pin
                           Controller_A_DIR_Pin EXT_DRIVER_EN_MCU_OUT_Pin */
  GPIO_InitStruct.Pin = GPLED_3_Pin|GPLED_2_Pin|GPLED_1_Pin|Controller_A_ENI_Pin
                          |Controller_A_DIR_Pin|EXT_DRIVER_EN_MCU_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : GPLED_5_Pin */
  GPIO_InitStruct.Pin = GPLED_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPLED_5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GPBTN_1_Pin CONTROLLER_A_STATUS_IN_Pin */
  GPIO_InitStruct.Pin = GPBTN_1_Pin|CONTROLLER_A_STATUS_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CONTROLLER_B_STATUS_OUT_Pin PHASE_U_L_Pin PHASE_V_L_Pin PHASE_W_L_Pin
                           SPI_MEM_CS_Pin SPI_NSCS_CONTROLLER_A_Pin SPI_NSCS_DRIVER_Pin GPLED_4_Pin */
  GPIO_InitStruct.Pin = CONTROLLER_B_STATUS_OUT_Pin|PHASE_U_L_Pin|PHASE_V_L_Pin|PHASE_W_L_Pin
                          |SPI_MEM_CS_Pin|SPI_NSCS_CONTROLLER_A_Pin|SPI_NSCS_DRIVER_Pin|GPLED_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_I2C3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PHASE_U_H_Pin PHASE_V_H_Pin PHASE_W_H_Pin */
  GPIO_InitStruct.Pin = PHASE_U_H_Pin|PHASE_V_H_Pin|PHASE_W_H_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : GATE_DRIVER_FAULT_IN_Pin */
  GPIO_InitStruct.Pin = GATE_DRIVER_FAULT_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GATE_DRIVER_FAULT_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPBTN_2_Pin */
  GPIO_InitStruct.Pin = GPBTN_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPBTN_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
