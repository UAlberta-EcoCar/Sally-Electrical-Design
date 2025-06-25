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

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin,
			GPIO_PIN_RESET);

	uint8_t prepped_buffer = (uint8_t) reg_addr | 0x7f;

	if (!HAL_SPI_Transmit(drv->hspi, prepped_buffer, 1, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	if (!HAL_SPI_Receive(drv->hspi, read_result, 4, HAL_MAX_DELAY) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_SET);
}

int driver_spi_write(driver_t *drv, uint8_t reg_addr, uint8_t write_data[]) {

	// 1 the MSB since that is the wnr bit. we are writing so it must be 1.
	uint8_t prep_register_buffer[5] = { ((uint8_t) reg_addr | 0x80u),
			rf_register_value };

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin,
			GPIO_PIN_RESET);

	if (HAL_SPI_Transmit(rf_handle->rf_spi_handle, prep_register_buffer, 2,
			rf_handle->rf_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to set register 0x%x : 0x%x", rf_register_address,
				rf_register_value);
		return 0;
	}

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_SET);
}

int driver_enable_bridges(driver_t *drv, int enable) {

}

int driver_reset(driver_t *drv) {

}
