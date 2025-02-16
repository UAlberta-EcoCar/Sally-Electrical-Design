/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include <string.h>
#include "adc.h"
#include "dac.h"
#include <math.h>
#include "usb_device.h" // Add this line to include the USB device header
#include "usbd_cdc_if.h" // Add this line to include the USB device header

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */
typedef struct
{
  float current[4];
  float voltage[2];

} boostData_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
boostData_t boost_data = {.current = {},
                          .voltage = {}};


uint32_t ADC1_VALUE[4];
uint32_t ADC2_VALUE[2];

const float VOLT_MCU = 3.244;

char ScreenBuffer[32]; 
char USBBuffer[32];


/* USER CODE END Variables */
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
/* Definitions for canSendMsg */
osThreadId_t canSendMsgHandle;
uint32_t canSendMsgBuffer[ 512 ];
osStaticThreadDef_t canSendMsgControlBlock;
const osThreadAttr_t canSendMsg_attributes = {
  .name = "canSendMsg",
  .stack_mem = &canSendMsgBuffer[0],
  .stack_size = sizeof(canSendMsgBuffer),
  .cb_mem = &canSendMsgControlBlock,
  .cb_size = sizeof(canSendMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for canRecieveMsg */
osThreadId_t canRecieveMsgHandle;
uint32_t canRecieveMsgBuffer[ 512 ];
osStaticThreadDef_t canRecieveMsgControlBlock;
const osThreadAttr_t canRecieveMsg_attributes = {
  .name = "canRecieveMsg",
  .stack_mem = &canRecieveMsgBuffer[0],
  .stack_size = sizeof(canRecieveMsgBuffer),
  .cb_mem = &canRecieveMsgControlBlock,
  .cb_size = sizeof(canRecieveMsgControlBlock),
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for adcConvTask */
osThreadId_t adcConvTaskHandle;
uint32_t adcConvTaskBuffer[ 512 ];
osStaticThreadDef_t adcConvTaskControlBlock;
const osThreadAttr_t adcConvTask_attributes = {
  .name = "adcConvTask",
  .stack_mem = &adcConvTaskBuffer[0],
  .stack_size = sizeof(adcConvTaskBuffer),
  .cb_mem = &adcConvTaskControlBlock,
  .cb_size = sizeof(adcConvTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for ScreenPrintTask */
osThreadId_t ScreenPrintTaskHandle;
uint32_t ScreenPrintHandBuffer[ 512 ];
osStaticThreadDef_t ScreenPrintHandControlBlock;
const osThreadAttr_t ScreenPrintTask_attributes = {
  .name = "ScreenPrintTask",
  .stack_mem = &ScreenPrintHandBuffer[0],
  .stack_size = sizeof(ScreenPrintHandBuffer),
  .cb_mem = &ScreenPrintHandControlBlock,
  .cb_size = sizeof(ScreenPrintHandControlBlock),
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for TRKPinTask */
osThreadId_t TRKPinTaskHandle;
uint32_t TRKPinTaskBuffer[ 512 ];
osStaticThreadDef_t TRKPinTaskControlBlock;
const osThreadAttr_t TRKPinTask_attributes = {
  .name = "TRKPinTask",
  .stack_mem = &TRKPinTaskBuffer[0],
  .stack_size = sizeof(TRKPinTaskBuffer),
  .cb_mem = &TRKPinTaskControlBlock,
  .cb_size = sizeof(TRKPinTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal5,
};
/* Definitions for canQueueRxHeader */
osMessageQueueId_t canQueueRxHeaderHandle;
uint8_t canQueueRxHeaderBuffer[ 512 * sizeof( uint32_t ) ];
osStaticMessageQDef_t canQueueRxHeaderControlBlock;
const osMessageQueueAttr_t canQueueRxHeader_attributes = {
  .name = "canQueueRxHeader",
  .cb_mem = &canQueueRxHeaderControlBlock,
  .cb_size = sizeof(canQueueRxHeaderControlBlock),
  .mq_mem = &canQueueRxHeaderBuffer,
  .mq_size = sizeof(canQueueRxHeaderBuffer)
};
/* Definitions for canQueueRxData */
osMessageQueueId_t canQueueRxDataHandle;
uint8_t canQueueRxDataBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t canQueueRxDataControlBlock;
const osMessageQueueAttr_t canQueueRxData_attributes = {
  .name = "canQueueRxData",
  .cb_mem = &canQueueRxDataControlBlock,
  .cb_size = sizeof(canQueueRxDataControlBlock),
  .mq_mem = &canQueueRxDataBuffer,
  .mq_size = sizeof(canQueueRxDataBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCanSend(void *argument);
void StartRecieveMsg(void *argument);
void StartAdcConv(void *argument);
void startScreenPrint(void *argument);
void StartTRKPin(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of canQueueRxHeader */
  canQueueRxHeaderHandle = osMessageQueueNew (512, sizeof(uint32_t), &canQueueRxHeader_attributes);

  /* creation of canQueueRxData */
  canQueueRxDataHandle = osMessageQueueNew (512, sizeof(uint8_t), &canQueueRxData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of canSendMsg */
  canSendMsgHandle = osThreadNew(StartCanSend, NULL, &canSendMsg_attributes);

  /* creation of canRecieveMsg */
  canRecieveMsgHandle = osThreadNew(StartRecieveMsg, NULL, &canRecieveMsg_attributes);

  /* creation of adcConvTask */
  adcConvTaskHandle = osThreadNew(StartAdcConv, NULL, &adcConvTask_attributes);

  /* creation of ScreenPrintTask */
  ScreenPrintTaskHandle = osThreadNew(startScreenPrint, NULL, &ScreenPrintTask_attributes);

  /* creation of TRKPinTask */
  TRKPinTaskHandle = osThreadNew(StartTRKPin, NULL, &TRKPinTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_Device */
  //MX_USB_Device_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    
    for (int i = 0; i < 5; i++)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_LED1_Pin, GPIO_PIN_SET);
      osDelay(50);
      HAL_GPIO_WritePin(GPIOA, GPIO_LED1_Pin, GPIO_PIN_RESET);
      osDelay(50);

      HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_SET);
      osDelay(50);
      HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_RESET);
      osDelay(50);

      HAL_GPIO_WritePin(GPIOB, GPIO_LED3_Pin, GPIO_PIN_SET);
      osDelay(50);
      HAL_GPIO_WritePin(GPIOB, GPIO_LED3_Pin, GPIO_PIN_RESET);
      osDelay(50);

      HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_SET);
      osDelay(50);
      HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_RESET);
      osDelay(50);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_LED3_Pin, GPIO_PIN_SET);
    osDelay(500);

    HAL_GPIO_WritePin(GPIOA, GPIO_LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_LED3_Pin, GPIO_PIN_SET);
    osDelay(500);


	osDelay(50);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartCanSend */
/**
* @brief Function implementing the canSendMsg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanSend */
void StartCanSend(void *argument)
{
  /* USER CODE BEGIN StartCanSend */
  /* Infinite loop */
  for(;;)
  {



    osDelay(1);
  }
  /* USER CODE END StartCanSend */
}

/* USER CODE BEGIN Header_StartRecieveMsg */
/**
* @brief Function implementing the canRecieveMsg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRecieveMsg */
void StartRecieveMsg(void *argument)
{
  /* USER CODE BEGIN StartRecieveMsg */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRecieveMsg */
}

/* USER CODE BEGIN Header_StartAdcConv */
/**
* @brief Function implementing the adcConvTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdcConv */
void StartAdcConv(void *argument)
{
  /* USER CODE BEGIN StartAdcConv */
  UNUSED(argument);

  const float ADC_VOLT_REF = VOLT_MCU / 4096.0;
  const float CURR_TRANSFER1 = 0.666667; // ratio of voltage divider resistors for current sense: 50k/(50k + 200k)
  const float CURR_TRANSFER2 = 0.6875;
  const float VOLT_TRANSFER_IN = 0.1049; // ratio of voltage divider resistors for input voltage res. divider
  const float VOLT_TRANSFER_OUT = 0.05065; // 3600 and 68000
  const float VOLT_TO_CURR_UNI = 0.133; //

  HAL_ADC_Start_DMA(&hadc1, ADC1_VALUE, 4);
  HAL_ADC_Start_DMA(&hadc2, ADC2_VALUE, 2);

    // Variables for moving average of boost_data.current[0]
    float current1_buffer[10] = {0};
    uint8_t current1_index = 0;
    float current1_sum = 0;
    float current1_avg = 0;
  /* Infinite loop */
  for(;;)
  {


    boost_data.current[0] = ((ADC1_VALUE[0] + 61) * ADC_VOLT_REF / CURR_TRANSFER1 - 0.510) / VOLT_TO_CURR_UNI;  //subtract less and see what happens
    boost_data.current[1] = ((ADC1_VALUE[1] + 68) * ADC_VOLT_REF / CURR_TRANSFER2 - 0.512) / VOLT_TO_CURR_UNI;

        // Update moving average for boost_data.current[0]
        current1_sum -= current1_buffer[current1_index];
        current1_buffer[current1_index] = boost_data.current[1];
        current1_sum += boost_data.current[1];
        current1_index = (current1_index + 1) % 10;
        float current1_avg = current1_sum / 10.0;
    

    boost_data.voltage[0] = ADC1_VALUE[3] * ADC_VOLT_REF / VOLT_TRANSFER_OUT;   
    boost_data.voltage[1] = ADC1_VALUE[2] * ADC_VOLT_REF / VOLT_TRANSFER_IN;

//    sprintf(USBBuffer, "OUT CURR: %.3f IN CURR: %.3f IN VOLT: %.1f OUT VOLT: %.1f 7V ILM: %lu 12V ILM: %lu\r\n",
//    current1_avg, boost_data.current[0], boost_data.voltage[1], boost_data.voltage[0],  (uint32_t)ADC2_VALUE[0], (uint32_t)ADC2_VALUE[1]);
//
//    CDC_Transmit_FS((uint8_t *)USBBuffer, strlen(USBBuffer));

    osDelay(500);
  }
  /* USER CODE END StartAdcConv */
}

/* USER CODE BEGIN Header_startScreenPrint */
/**
* @brief Function implementing the ScreenPrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startScreenPrint */
void startScreenPrint(void *argument)
{
  /* USER CODE BEGIN startScreenPrint */
  /* Infinite loop */
	// ssd1306_Init();
  // // Display the test bitmap for 2.5 seconds
  // ssd1306_TestDrawBitmap();
  // ssd1306_UpdateScreen();  
  // osDelay(2500);  // Delay for 2.5 seconds
  // ssd1306_Fill(Black);
  // ssd1306_UpdateScreen();


  // ssd1306_TestDrawBitmap2();
  // ssd1306_UpdateScreen();  
  // ssd1306_Fill(Black);
  // ssd1306_UpdateScreen();

  for(;;)
  {

    // ssd1306_SetCursor(0, 1);  // Adjust Y position as needed
    // sprintf(ScreenBuffer, "    IN     OUT");
    // ssd1306_WriteString(ScreenBuffer, Font_7x10, White);

    // ssd1306_SetCursor(0, 15);  // Adjust Y position as needed
    // sprintf(ScreenBuffer, "C %.2f  %.2f", boost_data.current[0], boost_data.current[1]);
    // ssd1306_WriteString(ScreenBuffer, Font_7x10, White);

    // ssd1306_SetCursor(0, 40);  // Adjust Y position as needed
    // sprintf(ScreenBuffer, "V %.1f %.1f ", boost_data.voltage[1], boost_data.voltage[0]);
    // ssd1306_WriteString(ScreenBuffer, Font_11x18, White);

    // ssd1306_UpdateScreen();  // Update the screen


     osDelay(1);

    
  }
  /* USER CODE END startScreenPrint */
}

/* USER CODE BEGIN Header_StartTRKPin */
/**
* @brief Function implementing the TRKPinTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTRKPin */
void StartTRKPin(void *argument)
{
  /* USER CODE BEGIN StartTRKPin */
  uint32_t DAC_VALUE;
  const float OUT_VOLT = 45;
  const float TRK_VOLT = OUT_VOLT/60;  // Desired voltage output   Equation:  Output = 60*(DAC INPUT)

  // Calibration values (from measurements)
  const float OFFSET = 0.005F;  // 5 mV offset
  const float GAIN = 1.0F;      // Measured gain factor (adjust if needed)

  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

  /* Infinite loop */
  for(;;)
  {
    // Apply correction
    float corrected_voltage = (TRK_VOLT - OFFSET) / GAIN;

    // Convert to DAC value
    DAC_VALUE = corrected_voltage * 4096 / VOLT_MCU;

    // Set DAC output
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_VALUE);

    osDelay(1);
  }
  /* USER CODE END StartTRKPin */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

