/*
 * driver.h
 *
 *  Created on: Jan 30, 2025
 *      Author: abina
 */

#ifndef INC_DRIVER_H_
#define INC_DRIVER_H_

#include "main.h"

typedef struct {
	SPI_HandleTypeDef *hspi;

	GPIO_TypeDef *driver_nss_port;

	uint16_t driver_nss_pin;

} driver_t;

int driver_initialize(driver_t *drv);

int driver_spi_read(driver_t *drv, uint8_t reg_addr, uint32_t *read_result);

int driver_spi_write(driver_t *drv, uint8_t reg_addr, uint32_t write_data);

int driver_enable_bridges(driver_t *drv, int enable);

int driver_reset(driver_t *drv);

#endif /* INC_DRIVER_H_ */
