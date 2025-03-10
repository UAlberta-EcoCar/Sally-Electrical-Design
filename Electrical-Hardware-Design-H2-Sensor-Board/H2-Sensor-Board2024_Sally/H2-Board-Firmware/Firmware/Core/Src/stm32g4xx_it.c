/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g4xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern COMP_HandleTypeDef hcomp1;
extern COMP_HandleTypeDef hcomp2;
extern COMP_HandleTypeDef hcomp6;
extern COMP_HandleTypeDef hcomp7;
extern TIM_HandleTypeDef htim20;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1) {
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
		HAL_GPIO_WritePin(GPLED1_GPIO_Port, GPLED1_Pin, GPIO_PIN_SET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED2_GPIO_Port, GPLED2_Pin, GPIO_PIN_SET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED3_GPIO_Port, GPLED3_Pin, GPIO_PIN_SET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED4_GPIO_Port, GPLED4_Pin, GPIO_PIN_SET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED5_GPIO_Port, GPLED5_Pin, GPIO_PIN_SET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;

		HAL_GPIO_WritePin(GPLED1_GPIO_Port, GPLED1_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED2_GPIO_Port, GPLED2_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED3_GPIO_Port, GPLED3_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED4_GPIO_Port, GPLED4_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
		HAL_GPIO_WritePin(GPLED5_GPIO_Port, GPLED5_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < HARDFAULT_TIMER; i++)
			;
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USB low priority interrupt remap.
  */
void USB_LP_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_IRQn 0 */

  /* USER CODE END USB_LP_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_IRQn 1 */

  /* USER CODE END USB_LP_IRQn 1 */
}

/**
  * @brief This function handles COMP1, COMP2 and COMP3 interrupts through EXTI lines 21, 22 and 29.
  */
void COMP1_2_3_IRQHandler(void)
{
  /* USER CODE BEGIN COMP1_2_3_IRQn 0 */

  /* USER CODE END COMP1_2_3_IRQn 0 */
  HAL_COMP_IRQHandler(&hcomp1);
  HAL_COMP_IRQHandler(&hcomp2);
  /* USER CODE BEGIN COMP1_2_3_IRQn 1 */

  /* USER CODE END COMP1_2_3_IRQn 1 */
}

/**
  * @brief This function handles COMP4, COMP5 and COMP6 interrupts through EXTI lines 30, 31 and 32.
  */
void COMP4_5_6_IRQHandler(void)
{
  /* USER CODE BEGIN COMP4_5_6_IRQn 0 */

  /* USER CODE END COMP4_5_6_IRQn 0 */
  HAL_COMP_IRQHandler(&hcomp6);
  /* USER CODE BEGIN COMP4_5_6_IRQn 1 */

  /* USER CODE END COMP4_5_6_IRQn 1 */
}

/**
  * @brief This function handles COMP7 interrupt through EXTI line 33.
  */
void COMP7_IRQHandler(void)
{
  /* USER CODE BEGIN COMP7_IRQn 0 */

  /* USER CODE END COMP7_IRQn 0 */
  HAL_COMP_IRQHandler(&hcomp7);
  /* USER CODE BEGIN COMP7_IRQn 1 */

  /* USER CODE END COMP7_IRQn 1 */
}

/**
  * @brief This function handles TIM20 update interrupt.
  */
void TIM20_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM20_UP_IRQn 0 */

  /* USER CODE END TIM20_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim20);
  /* USER CODE BEGIN TIM20_UP_IRQn 1 */

  /* USER CODE END TIM20_UP_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
