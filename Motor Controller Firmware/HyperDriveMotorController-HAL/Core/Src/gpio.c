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
     PC2   ------> S_TIM20_CH2
     PB1   ------> S_TIM3_CH4
     PB2   ------> S_TIM5_CH1
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

  /*Configure GPIO pins : GPLED_3_Pin GPLED_2_Pin GPLED_1_Pin Controller_A_ENI_Pin
                           Controller_A_DIR_Pin */
  GPIO_InitStruct.Pin = GPLED_3_Pin|GPLED_2_Pin|GPLED_1_Pin|Controller_A_ENI_Pin
                          |Controller_A_DIR_Pin;
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

  /*Configure GPIO pin : GPBTN_1_Pin */
  GPIO_InitStruct.Pin = GPBTN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPBTN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Controller_A_PWM_I_Pin */
  GPIO_InitStruct.Pin = Controller_A_PWM_I_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM20;
  HAL_GPIO_Init(Controller_A_PWM_I_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : HALL_U_Pin HALL_V_Pin */
  GPIO_InitStruct.Pin = HALL_U_Pin|HALL_V_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : HALL_W_Pin GATE_DRIVER_FAULT_IN_Pin */
  GPIO_InitStruct.Pin = HALL_W_Pin|GATE_DRIVER_FAULT_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUZZER_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : POWER_STAGE_ENABLE_Pin */
  GPIO_InitStruct.Pin = POWER_STAGE_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  HAL_GPIO_Init(POWER_STAGE_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CONTROLLER_B_STATUS_OUT_Pin SPI_MEM_CS_Pin SPI_NSCS_CONTROLLER_A_Pin SPI_NSCS_DRIVER_Pin
                           GPLED_4_Pin */
  GPIO_InitStruct.Pin = CONTROLLER_B_STATUS_OUT_Pin|SPI_MEM_CS_Pin|SPI_NSCS_CONTROLLER_A_Pin|SPI_NSCS_DRIVER_Pin
                          |GPLED_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PHASE_U_L_Pin PHASE_V_L_Pin PHASE_W_L_Pin */
  GPIO_InitStruct.Pin = PHASE_U_L_Pin|PHASE_V_L_Pin|PHASE_W_L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : CONTROLLER_A_STATUS_IN_Pin */
  GPIO_InitStruct.Pin = CONTROLLER_A_STATUS_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CONTROLLER_A_STATUS_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EXT_DRIVER_EN_MCU_OUT_Pin */
  GPIO_InitStruct.Pin = EXT_DRIVER_EN_MCU_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXT_DRIVER_EN_MCU_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPBTN_2_Pin */
  GPIO_InitStruct.Pin = GPBTN_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPBTN_2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
