#ifndef __LIS3DH_CONF_H
#define __LIS3DH_CONF_H

#if defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#else
#error "No defined STM32 series!"
#endif

/* Uncomment or add the appropriate i2c handle */
// extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

#endif
