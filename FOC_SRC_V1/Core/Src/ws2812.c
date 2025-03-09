/*
 * WS2812 Driver
 */

#include "ws2812.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_def.h"
#include "stm32g4xx_hal_tim.h"
#include <stdint.h>

ws2812Colors_t ws2812_color_data[WS2812_NUM_LEDS];
uint8_t ws2812_dma_buffer[WS2812_NUM_LEDS];
volatile uint8_t ws2812_dma_complete_flag;

HAL_StatusTypeDef WS2812_Init(void) {

  // Initialize timer just incase
  HAL_StatusTypeDef hal_ok = HAL_TIM_PWM_Init(&WS2812_TIM);

  // Clear buffers
  for (uint32_t bufIndex = 0; bufIndex < WS2812_DMA_BUFF_LEN; bufIndex++) {
    ws2812_dma_buffer[bufIndex] = 0;
  }

  // Set DMA transfer ready flag
  ws2812_dma_complete_flag = 1;

  return hal_ok;
}

HAL_StatusTypeDef WS2812_Update(void) {

  if (ws2812_dma_complete_flag != 1) {
    return HAL_BUSY;
  }

  // Loop through RGB LED Data and check if bits are set
  uint16_t bufIndex = 0;
  for (uint8_t ledIndex = 0; ledIndex < WS2812_NUM_LEDS; ledIndex++) {
    for (uint8_t bitIndex = 0; bitIndex < WS2812_LED_BITS; bitIndex++) {
      if ((ws2812_color_data[ledIndex].rgb_data >> bitIndex) & 0x01) {
        ws2812_dma_buffer[bufIndex] = WS2812_HI_VAL;
      } else {
        ws2812_dma_buffer[bufIndex] = WS2812_LOW_VAL;
      }
      bufIndex++;
    }
  }

  // Note: Remaining bits are set to zero as they are set in init function

  HAL_StatusTypeDef hal_ok = HAL_TIM_PWM_Start_DMA(
      &WS2812_TIM, WS2812_TIM_CHANNEL, (uint32_t *)&ws2812_dma_buffer,
      WS2812_DMA_BUFF_LEN);

  if (hal_ok == HAL_OK) {
    // DMA Transfer began successfully
    ws2812_dma_complete_flag = 0;
  }

  return hal_ok;
}

void WS2812_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {

  ws2812_color_data[index].r = r;
  ws2812_color_data[index].g = g;
  ws2812_color_data[index].b = b;
}

/* Call in HAL_TIM_PWM_PulseFinishedCallback() */
void WS2812_Callback(void) {
  HAL_TIM_PWM_Stop_DMA(&WS2812_TIM, WS2812_TIM_CHANNEL);
  ws2812_dma_complete_flag = 1;
}
