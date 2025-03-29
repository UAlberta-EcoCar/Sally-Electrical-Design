/*
 * WS2812 Driver
 */
#include "cmsis_os.h"
#include "ws2812.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_def.h"
#include "stm32g4xx_hal_tim.h"
#include <stdint.h>
#include <stdbool.h>

ws2812Colors_t ws2812_color_data[WS2812_NUM_LEDS];
uint8_t ws2812_dma_buffer[WS2812_DMA_BUFF_LEN];
volatile uint8_t ws2812_dma_complete_flag;

// Default Brightness for an LED, range 0 -255
#define WS2812_BRIGHTNESS 4

// Default Colors [g, r, b]

const ws2812Colors_t LED_RED = {0, WS2812_BRIGHTNESS, 0};
const ws2812Colors_t LED_GREEN = {WS2812_BRIGHTNESS, 0, 0};
const ws2812Colors_t LED_BLUE = {0, 0, WS2812_BRIGHTNESS};
const ws2812Colors_t LED_PURPLE = {0, WS2812_BRIGHTNESS, WS2812_BRIGHTNESS};
const ws2812Colors_t LED_YELLOW = {WS2812_BRIGHTNESS, WS2812_BRIGHTNESS, 0};

/*Reverse order of bits in a byte */
uint8_t
flipByte(uint8_t c)
{
  c = ((c >> 1) & 0x55) | ((c << 1) & 0xAA);
  c = ((c >> 2) & 0x33) | ((c << 2) & 0xCC);
  c = (c >> 4) | (c << 4);

  return c;
}

HAL_StatusTypeDef WS2812_Init(void)
{

  // Initialize timer just incase
  HAL_StatusTypeDef hal_ok = HAL_TIM_PWM_Init(&WS2812_TIM);

  // Clear buffers
  for (uint32_t bufIndex = 0; bufIndex < WS2812_DMA_BUFF_LEN; bufIndex++)
  {
    ws2812_dma_buffer[bufIndex] = 0;
  }

  // Set DMA transfer ready flag
  ws2812_dma_complete_flag = 1;

  return hal_ok;
}

HAL_StatusTypeDef WS2812_Update(void)
{

  if (!ws2812_dma_complete_flag)
  {
    return HAL_BUSY;
  }

  // Loop through RGB LED Data and check if bits are set
  uint16_t bufIndex = 0;
  //  For each LED
  for (uint8_t ledIndex = 0; ledIndex < WS2812_NUM_LEDS; ledIndex++)
  {
    // Loop through all 24 bits of that LED
    // for (uint8_t bitIndex = WS2812_LED_BITS - 1; bitIndex >= 0; bitIndex--)
    for (uint8_t bitIndex = 0; bitIndex < WS2812_LED_BITS; bitIndex++)
    {
      // If set, select 'HI' code. If not set, select 'LO' code.
      if ((ws2812_color_data[ledIndex].rgb_data >> bitIndex) & 0x01)
      {
        ws2812_dma_buffer[bufIndex] = WS2812_HI_VAL;
      }
      else
      {
        ws2812_dma_buffer[bufIndex] = WS2812_LOW_VAL;
      }
      bufIndex++;
    }
  }

  // Note: Remaining bits are set to zero as they are set in init function

  HAL_StatusTypeDef hal_ok = HAL_TIM_PWM_Start_DMA(
      &WS2812_TIM, WS2812_TIM_CHANNEL, (uint32_t *)&ws2812_dma_buffer,
      WS2812_DMA_BUFF_LEN);

  if (hal_ok == HAL_OK)
  {
    // DMA Transfer began successfully
    ws2812_dma_complete_flag = 0;
  }

  return hal_ok;
}

void WS2812_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
  ws2812_color_data[index].color.r = flipByte(r);
  ws2812_color_data[index].color.g = flipByte(g);
  ws2812_color_data[index].color.b = flipByte(b);
}

void WS2812_SetColor_and_Brightness(uint8_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
  brightness %= WS2812_BRIGHTNESS + 1;
  WS2812_SetColor(index, r * ((float)brightness / WS2812_BRIGHTNESS), g * ((float)brightness / WS2812_BRIGHTNESS), b * ((float)brightness / WS2812_BRIGHTNESS));
}
/* Call in HAL_TIM_PWM_PulseFinishedCallback() */
void WS2812_Callback(void)
{
  HAL_TIM_PWM_Stop_DMA(&WS2812_TIM, WS2812_TIM_CHANNEL);
  ws2812_dma_complete_flag = 1;
}

uint8_t next_brightness()
{
  // LED Brightness Iterator
  static bool increasing = true;
  static uint8_t brightness = 0;

  if (brightness >= WS2812_BRIGHTNESS)
  {
    increasing = false;
  }
  else if (brightness < 1)
  {
    increasing = true;
  }
  return (increasing ? brightness++ : brightness--);
}

HAL_StatusTypeDef WS2812_Standby_Animation(const uint32_t delay)
{
  static uint8_t animation_index = 0;
  const ws2812Colors_t led_colors[] = {LED_RED,
                                       LED_YELLOW,
                                       LED_GREEN,
                                       LED_BLUE,
                                       LED_PURPLE};
  const uint8_t led_brightness = next_brightness();
  for (uint8_t led_index = 0; led_index < WS2812_NUM_LEDS; led_index++)
  {
    const ws2812Colors_t led_color = led_colors[(animation_index + led_index) % WS2812_NUM_LEDS];
    WS2812_SetColor_and_Brightness(led_index, led_brightness, led_color.color.r, led_color.color.g, led_color.color.b);
  }
  ++animation_index;

  // Set LED colors
  const HAL_StatusTypeDef hal_status = WS2812_Update();

  // Animation Delay
  osDelay(delay);

  return hal_status;
}

HAL_StatusTypeDef WS2812_Running_Animation(const uint32_t delay)
{
  static uint8_t animation_index = 0;
  const ws2812Colors_t led_colors[] = {LED_RED,
                                       LED_YELLOW,
                                       LED_GREEN,
                                       LED_BLUE,
                                       LED_PURPLE};
  for (uint8_t led_index = 0; led_index < WS2812_NUM_LEDS; led_index++)
  {
    const ws2812Colors_t led_color = led_colors[(animation_index + led_index) % WS2812_NUM_LEDS];
    WS2812_SetColor(led_index, led_color.color.r, led_color.color.g, led_color.color.b);
  }
  ++animation_index;

  // Set LED colors
  const HAL_StatusTypeDef hal_status = WS2812_Update();

  // Animation Delay
  osDelay(delay);

  return hal_status;
}

HAL_StatusTypeDef WS2812_Charging_Animation(const uint32_t delay)
{
  static uint8_t animation_index = 0;
  static uint8_t led_brightness = 1;
  const ws2812Colors_t led_color = LED_GREEN;
  if (animation_index % (WS2812_NUM_LEDS - 1) == 0)
  {
    led_brightness = next_brightness() + 1;
  }

  // Clear LEDs
  for (uint8_t led_index = 0; led_index < WS2812_NUM_LEDS; ++led_index)
  {
    WS2812_SetColor(led_index, 0, 0, 0);
  }
  // Set loading LEDs
  for (uint8_t led_index = 0; led_index < (animation_index % (WS2812_NUM_LEDS + 1)); ++led_index)
  {
    WS2812_SetColor_and_Brightness(led_index, led_brightness, led_color.color.r, led_color.color.g, led_color.color.b);
  }
  ++animation_index;

  // Set LED colors
  const HAL_StatusTypeDef hal_status = WS2812_Update();

  // Animation Delay
  osDelay(delay);

  return hal_status;
}
HAL_StatusTypeDef WS2812_Startup_Animation(const uint32_t delay)
{
  static uint8_t animation_index = 0;
  const ws2812Colors_t led_colors[] = {
      LED_GREEN,
      LED_YELLOW,
      LED_RED,
  };
  const ws2812Colors_t led_color = led_colors[animation_index % WS2812_NUM_LEDS];

  for (uint8_t led_index = 0; led_index < WS2812_NUM_LEDS; ++led_index)
  {
    // WS2812_SetColor_and_Brightness(led_index, 2, led_color.color.r, led_color.color.g, led_color.color.b);
    WS2812_SetColor(led_index, led_color.color.r, led_color.color.g, led_color.color.b);
  }

  ++animation_index;

  // Set LED colors
  const HAL_StatusTypeDef hal_status = WS2812_Update();

  // Animation Delay
  osDelay(delay);

  return hal_status;
}
