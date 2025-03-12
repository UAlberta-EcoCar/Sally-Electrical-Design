/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rf-rfm95.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan2;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 512 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskCan */
osThreadId_t TaskCanHandle;
uint32_t CanTaskBuffer[ 256 ];
osStaticThreadDef_t CanTaskControlBlock;
const osThreadAttr_t TaskCan_attributes = {
  .name = "TaskCan",
  .stack_mem = &CanTaskBuffer[0],
  .stack_size = sizeof(CanTaskBuffer),
  .cb_mem = &CanTaskControlBlock,
  .cb_size = sizeof(CanTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task24 */
osThreadId_t Task24Handle;
uint32_t Task24Buffer[ 256 ];
osStaticThreadDef_t Task24ControlBlock;
const osThreadAttr_t Task24_attributes = {
  .name = "Task24",
  .stack_mem = &Task24Buffer[0],
  .stack_size = sizeof(Task24Buffer),
  .cb_mem = &Task24ControlBlock,
  .cb_size = sizeof(Task24ControlBlock),
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task915 */
osThreadId_t Task915Handle;
uint32_t Task915Buffer[ 256 ];
osStaticThreadDef_t Task915ControlBlock;
const osThreadAttr_t Task915_attributes = {
  .name = "Task915",
  .stack_mem = &Task915Buffer[0],
  .stack_size = sizeof(Task915Buffer),
  .cb_mem = &Task915ControlBlock,
  .cb_size = sizeof(Task915ControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task868 */
osThreadId_t Task868Handle;
uint32_t Task868Buffer[ 256 ];
osStaticThreadDef_t Task868ControlBlock;
const osThreadAttr_t Task868_attributes = {
  .name = "Task868",
  .stack_mem = &Task868Buffer[0],
  .stack_size = sizeof(Task868Buffer),
  .cb_mem = &Task868ControlBlock,
  .cb_size = sizeof(Task868ControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskGNSS */
osThreadId_t TaskGNSSHandle;
uint32_t GNSSTaskBuffer[ 256 ];
osStaticThreadDef_t GNSSTaskControlBlock;
const osThreadAttr_t TaskGNSS_attributes = {
  .name = "TaskGNSS",
  .stack_mem = &GNSSTaskBuffer[0],
  .stack_size = sizeof(GNSSTaskBuffer),
  .cb_mem = &GNSSTaskControlBlock,
  .cb_size = sizeof(GNSSTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskSD */
osThreadId_t TaskSDHandle;
uint32_t SDTaskBuffer[ 256 ];
osStaticThreadDef_t SDTaskControlBlock;
const osThreadAttr_t TaskSD_attributes = {
  .name = "TaskSD",
  .stack_mem = &SDTaskBuffer[0],
  .stack_size = sizeof(SDTaskBuffer),
  .cb_mem = &SDTaskControlBlock,
  .cb_size = sizeof(SDTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskAUX */
osThreadId_t TaskAUXHandle;
uint32_t TaskAUXBuffer[ 256 ];
osStaticThreadDef_t TaskAUXControlBlock;
const osThreadAttr_t TaskAUX_attributes = {
  .name = "TaskAUX",
  .stack_mem = &TaskAUXBuffer[0],
  .stack_size = sizeof(TaskAUXBuffer),
  .cb_mem = &TaskAUXControlBlock,
  .cb_size = sizeof(TaskAUXControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
void StartDefaultTask(void *argument);
void StartCanTask(void *argument);
void Start24Task(void *argument);
void Start915Task(void *argument);
void Start868Task(void *argument);
void StartGNSSTask(void *argument);
void StartSDTask(void *argument);
void StartAUXTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_FDCAN2_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TaskCan */
  TaskCanHandle = osThreadNew(StartCanTask, NULL, &TaskCan_attributes);

  /* creation of Task24 */
  Task24Handle = osThreadNew(Start24Task, NULL, &Task24_attributes);

  /* creation of Task915 */
  Task915Handle = osThreadNew(Start915Task, NULL, &Task915_attributes);

  /* creation of Task868 */
  Task868Handle = osThreadNew(Start868Task, NULL, &Task868_attributes);

  /* creation of TaskGNSS */
  TaskGNSSHandle = osThreadNew(StartGNSSTask, NULL, &TaskGNSS_attributes);

  /* creation of TaskSD */
  TaskSDHandle = osThreadNew(StartSDTask, NULL, &TaskSD_attributes);

  /* creation of TaskAUX */
  TaskAUXHandle = osThreadNew(StartAUXTask, NULL, &TaskAUX_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 75;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 16;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 1;
  hfdcan2.Init.NominalTimeSeg2 = 1;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 1;
  hfdcan2.Init.DataTimeSeg2 = 1;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x30D293D6;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BTN1_Pin|BTN2_Pin|LED6_Pin|TXEN_24_Pin
                          |RXEN_24_Pin|SPI2_NSS_Pin|SWT1_Pin|NSS_915_Pin
                          |NSS_868_Pin|DIO2_24_Pin|DIO1_24_Pin|RST_868_Pin
                          |DIO0_868_Pin|DIO4_868_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |SWT2_Pin|RST_24_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, NSS_24_Pin|RST_GNSS_Pin|EXTINT_GNSS_Pin|DIO0_915_Pin
                          |RST_915_Pin|DIO5_915_Pin|DIO4_915_Pin|LED7_Pin
                          |LED8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin LED6_Pin TXEN_24_Pin
                           RXEN_24_Pin SPI2_NSS_Pin SWT1_Pin NSS_915_Pin
                           NSS_868_Pin DIO2_24_Pin DIO1_24_Pin DIO0_868_Pin
                           DIO4_868_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin|LED6_Pin|TXEN_24_Pin
                          |RXEN_24_Pin|SPI2_NSS_Pin|SWT1_Pin|NSS_915_Pin
                          |NSS_868_Pin|DIO2_24_Pin|DIO1_24_Pin|DIO0_868_Pin
                          |DIO4_868_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           SWT2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |SWT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_24_Pin EXTINT_GNSS_Pin DIO0_915_Pin DIO5_915_Pin
                           DIO4_915_Pin LED7_Pin LED8_Pin */
  GPIO_InitStruct.Pin = NSS_24_Pin|EXTINT_GNSS_Pin|DIO0_915_Pin|DIO5_915_Pin
                          |DIO4_915_Pin|LED7_Pin|LED8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RST_GNSS_Pin RST_915_Pin */
  GPIO_InitStruct.Pin = RST_GNSS_Pin|RST_915_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IMON_Pin */
  GPIO_InitStruct.Pin = IMON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IMON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO3_24_Pin */
  GPIO_InitStruct.Pin = DIO3_24_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO3_24_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_24_Pin */
  GPIO_InitStruct.Pin = BUSY_24_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_24_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_24_Pin */
  GPIO_InitStruct.Pin = RST_24_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(RST_24_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED5_Pin */
  GPIO_InitStruct.Pin = LED5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LED5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_868_Pin */
  GPIO_InitStruct.Pin = RST_868_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(RST_868_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO5_868_Pin */
  GPIO_InitStruct.Pin = DIO5_868_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO5_868_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(NSS_24_GPIO_Port, NSS_24_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_915_GPIO_Port, NSS_915_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NSS_868_GPIO_Port, NSS_868_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RST_868_GPIO_Port, RST_868_Pin, GPIO_PIN_SET);

//	HAL_GPIO_WritePin(RST_915_GPIO_Port, RST_915_Pin, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

	rf_handle_t rfm95_868 = { .rf_nreset_port = RST_868_GPIO_Port,
			.rf_nreset_pin = RST_868_Pin, .rf_nss_port = NSS_868_GPIO_Port,
			.rf_nss_pin = NSS_868_Pin, .rf_spi_handle = &hspi1,
			.rf_delay_func = osDelay, .rf_spi_timeout = HAL_MAX_DELAY,
			.rf_carrier_frequency = 868000000 };

//	rf_handle_t rfm95_915 = { .rf_nreset_port = RST_915_GPIO_Port,
//			.rf_nreset_pin = RST_915_Pin, .rf_nss_port = NSS_915_GPIO_Port,
//			.rf_nss_pin = NSS_915_GPIO_Port, .rf_spi_handle = &hspi1,
//			.rf_delay_func = osDelay, .rf_spi_timeout = 100,
//			.rf_carrier_frequency = 915000000 };

//	rf_initialize_radio(&rfm95_868);




//	rf_initialize_radio(&rfm95_915);

//	rf_set_frequency(&rfm95_868, 868000000);

//	rf_set_frequency(&rfm95_915, 915000000);

	/* Infinite loop */
	for (;;) {

		rf_initialize_radio(&rfm95_868);

//		rf_initialize_radio(&rfm95_915);

//		rf_set_frequency(&rfm95_868, 868000000);

//		rf_set_frequency(&rfm95_915, 915000000);

		osDelay(10);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartCanTask */
/**
 * @brief Function implementing the CanTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCanTask */
void StartCanTask(void *argument)
{
  /* USER CODE BEGIN StartCanTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartCanTask */
}

/* USER CODE BEGIN Header_Start24Task */
/**
 * @brief Function implementing the Task24 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start24Task */
void Start24Task(void *argument)
{
  /* USER CODE BEGIN Start24Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END Start24Task */
}

/* USER CODE BEGIN Header_Start915Task */
/**
 * @brief Function implementing the Task915 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start915Task */
void Start915Task(void *argument)
{
  /* USER CODE BEGIN Start915Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END Start915Task */
}

/* USER CODE BEGIN Header_Start868Task */
/**
 * @brief Function implementing the Task868 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start868Task */
void Start868Task(void *argument)
{
  /* USER CODE BEGIN Start868Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END Start868Task */
}

/* USER CODE BEGIN Header_StartGNSSTask */
/**
 * @brief Function implementing the GNSSTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGNSSTask */
void StartGNSSTask(void *argument)
{
  /* USER CODE BEGIN StartGNSSTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartGNSSTask */
}

/* USER CODE BEGIN Header_StartSDTask */
/**
 * @brief Function implementing the SDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSDTask */
void StartSDTask(void *argument)
{
  /* USER CODE BEGIN StartSDTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartSDTask */
}

/* USER CODE BEGIN Header_StartAUXTask */
/**
 * @brief Function implementing the TaskAUX thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAUXTask */
void StartAUXTask(void *argument)
{
  /* USER CODE BEGIN StartAUXTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartAUXTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
