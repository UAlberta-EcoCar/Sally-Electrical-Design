/*
 * WS2812 Led Driver
 */

#ifndef WS2812_RGB_LED_H
#define WS2812_RGB_LED_H

// Includes
#include "stm32g4xx_hal.h"
#include "tim.h"

// Defines
#define WS2812_NUM_LEDS       5
#define WS2812_TIM            htim2
#define WS2812_TIM_CHANNEL    TIM_CHANNEL_1

/* Set up ratios assuming clock speed 800kHz */
#define WS2812_HI_VAL         64 // 0.8us of 1.25us
#define WS2812_LOW_VAL        32 // 0.4us of 1.25us
#define WS2812_RESET_PERIODS  40 // >50us/1.25us
#define WS2812_LED_BITS       24 // G7 ... G0 R7 ... R0 B7 ... B0
#define WS2812_DMA_BUFF_LEN   ((WS2812_NUM_LEDS*WS2812_LED_BITS)+WS2812_RESET_PERIODS)

typedef union {
  struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
  };
  uint32_t rgb_data;
} ws2812Colors_t;

// Variables

extern ws2812Colors_t ws2812_color_data[WS2812_NUM_LEDS];
extern uint8_t ws2812_dma_buffer[WS2812_NUM_LEDS];
extern volatile uint8_t ws2812_dma_complete_flag;

// Functions

HAL_StatusTypeDef WS2812_Init(void);
HAL_StatusTypeDef WS2812_Update(void);
void              WS2812_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void              WS2812_Callback(void);


#endif /* WS2812_RGB_LED_H */
