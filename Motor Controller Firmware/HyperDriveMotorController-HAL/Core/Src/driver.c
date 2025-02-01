/*
 * driver.c
 *
 *  Created on: Jan 30, 2025
 *      Author: abina
 */
#include "driver.h"
#include "main.h"
int driver_initialize(driver_t *drv) {

}

int driver_spi_read(driver_t *drv, uint8_t reg_addr, uint32_t *read_result) {

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_RESET);

	uint8_t prepped_buffer[1] = { reg_addr | 0x80 };

	HAL_SPI_Transmit(drv->hspi, prepped_buffer, 1, HAL_MAX_DELAY);

	HAL_SPI_Receive(drv->hspi, read_result, 4, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_SET);
}

int driver_spi_write(driver_t *drv, uint8_t reg_addr, uint32_t write_data) {

}

int driver_enable_bridges(driver_t *drv, int enable) {

}

int driver_reset(driver_t *drv) {

}
