/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* ── GPIOB (PB15) ──────────────────────────────────────────────────────── */
#define GPIOB_BASE_A    0x48000400UL
#define GPIOB_MODER     (*(volatile uint32_t*)(GPIOB_BASE_A + 0x00))
#define GPIOB_OTYPER    (*(volatile uint32_t*)(GPIOB_BASE_A + 0x04))
#define GPIOB_OSPEEDR   (*(volatile uint32_t*)(GPIOB_BASE_A + 0x08))
#define GPIOB_PUPDR     (*(volatile uint32_t*)(GPIOB_BASE_A + 0x0C))
#define GPIOB_BSRR      (*(volatile uint32_t*)(GPIOB_BASE_A + 0x18))
/* ── DWT ────────────────────────────────────────────────────────────────── */
#define DWT_CYCCNT      (*(volatile uint32_t*)0xE0001004UL)
/* ── WS2812B ────────────────────────────────────────────────────────────── */
#define NUM_LEDS  12
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
typedef struct { uint8_t r, g, b; } Color;
static void ws2812b_init(void);
static void ws2812b_send_frame(const Color *leds, int n);
static Color hsv(uint8_t h, uint8_t s, uint8_t v);
static void delay_ms(uint32_t ms);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void ws2812b_init(void)
{
    /* Enable DWT cycle counter */
    *(volatile uint32_t*)0xE000EDFC |= (1U << 24);
    *(volatile uint32_t*)0xE0001004  = 0;
    *(volatile uint32_t*)0xE0001000 |= (1U << 0);

    /* PB15: GPIO output, push-pull, very high speed (GPIOB clock already on) */
    GPIOB_MODER  &= ~(0x3U << 30);  GPIOB_MODER  |= (0x1U << 30);
    GPIOB_OTYPER &= ~(1U   << 15);
    GPIOB_OSPEEDR|=  (0x3U << 30);
    GPIOB_PUPDR  &= ~(0x3U << 30);
    GPIOB_BSRR    = (1U << 31);   /* start LOW */
}

/* Bit-bang one frame. Timing identical to the confirmed-working send_solid. */
static void ws2812b_send_frame(const Color *leds, int n)
{
    GPIOB_BSRR = (1U << 31);
    uint32_t t = DWT_CYCCNT;
    while (DWT_CYCCNT - t < 960U);   /* 60 µs reset */

    __disable_irq();

    for (int led = 0; led < n; led++) {
        uint8_t grb[3] = { leds[led].g, leds[led].r, leds[led].b };
        for (int j = 0; j < 3; j++) {
            for (int k = 7; k >= 0; k--) {
                uint32_t s = DWT_CYCCNT;
                if ((grb[j] >> k) & 1) {
                    GPIOB_BSRR = (1U << 15);
                    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
                    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
                    GPIOB_BSRR = (1U << 31);
                } else {
                    GPIOB_BSRR = (1U << 15);
                    __NOP(); __NOP();
                    GPIOB_BSRR = (1U << 31);
                }
                while (DWT_CYCCNT - s < 20U);
            }
        }
    }

    __enable_irq();
}

static Color hsv(uint8_t h, uint8_t s, uint8_t v)
{
    if (s == 0) return (Color){ v, v, v };
    uint8_t region    = h / 43;
    uint8_t remainder = (h - region * 43) * 6;
    uint8_t p  = (uint16_t)v * (255 - s) >> 8;
    uint8_t q  = (uint16_t)v * (255 - ((uint16_t)s * remainder >> 8)) >> 8;
    uint8_t tv = (uint16_t)v * (255 - ((uint16_t)s * (255 - remainder) >> 8)) >> 8;
    switch (region) {
        case 0:  return (Color){ v,  tv, p  };
        case 1:  return (Color){ q,  v,  p  };
        case 2:  return (Color){ p,  v,  tv };
        case 3:  return (Color){ p,  q,  v  };
        case 4:  return (Color){ tv, p,  v  };
        default: return (Color){ v,  p,  q  };
    }
}

static void delay_ms(uint32_t ms)
{
    uint32_t start = DWT_CYCCNT;
    while ((DWT_CYCCNT - start) < 16000U * ms);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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
  /* USER CODE BEGIN 2 */
  ws2812b_init();
  /* USER CODE END 2 */

  /* Infinite loop */

      #define NUM_GROUPS 5
      #define FADE_STEPS 25

      const uint8_t group_leds[NUM_GROUPS][3] = {
          { 2, 3, 4  },
          { 5, 1, 255},
          { 6, 0, 255},
          { 7, 11, 255},
          { 8, 9, 10 },
      };
      const uint8_t group_size[NUM_GROUPS] = { 3, 2, 2, 2, 3 };

      Color leds[NUM_LEDS];

      while (1)
      {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = (Color){ .r = 255, .g = 255, .b = 255 };
        }
        ws2812b_send_frame(leds, NUM_LEDS);
        delay_ms(100);

        /* USER CODE END 3 */
      }
    }
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pins : PC13 PC14 PC15 PC7
                           PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
