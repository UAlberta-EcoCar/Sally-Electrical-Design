/*
 * driver.c
 *
 *  Created on: Jan 30, 2025
 *      Author: abina
 */
#include "tmc6100_driver.h"
#include "tmc6100.h"
#include "main.h"
#include "debug-log.h"
#include "spi.h"

int driver_initialize(driver_t *drv) {

	uint8_t r[4] = { 0 };

	driver_spi_read(drv, TMC6100_IOIN, &r);

	tmc6100_ioin_t io = { 0 };

	io.reg_rawa_ioin[0] = r[0];
	io.reg_rawa_ioin[1] = r[1];
	io.reg_rawa_ioin[2] = r[2];
	io.reg_rawa_ioin[3] = r[3];

	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;

	driver_spi_read(drv, TMC6100_GCONF, &r);

	tmc6100_gconf_t conf = { 0 };

	conf.reg_rawa_gconf[0] = r[0];
	conf.reg_rawa_gconf[1] = r[1];
	conf.reg_rawa_gconf[2] = r[2];
	conf.reg_rawa_gconf[3] = r[3];

	conf.driver_disable = 0;
	conf.singleline = 0;
	conf.test_mode = 0;
	conf.fault_direct = 1;

	driver_spi_write(drv, TMC6100_GCONF, conf.reg_rawa_gconf);

	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;

	driver_spi_read(drv, TMC6100_GCONF, &r);

	conf.reg_rawa_gconf[0] = r[0];
	conf.reg_rawa_gconf[1] = r[1];
	conf.reg_rawa_gconf[2] = r[2];
	conf.reg_rawa_gconf[3] = r[3];

	driver_spi_read(drv, TMC6100_IOIN, &r);

	io.reg_rawa_ioin[0] = r[0];
	io.reg_rawa_ioin[1] = r[1];
	io.reg_rawa_ioin[2] = r[2];
	io.reg_rawa_ioin[3] = r[3];

	return DRV_OK;
}

int driver_spi_read(driver_t *drv, uint8_t reg_addr, uint8_t read_result[]) {

	HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
			GPIO_PIN_RESET);

	uint8_t prepped_buffer = (uint8_t) reg_addr & 0x7f;

	if (!HAL_SPI_Transmit(&hspi3, &prepped_buffer, 1, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}
//	uint8_t r[4] = {0};
	if (!HAL_SPI_Receive(&hspi3, (uint8_t*) read_result, 4, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
			GPIO_PIN_SET);

	return 0;

}

int driver_spi_write(driver_t *drv, uint8_t reg_addr, uint8_t write_data[]) {

	// 1 the MSB since that is the wnr bit. we are writing so it must be 1.
	uint8_t prep_register_buffer[5] = { ((uint8_t) reg_addr | 0x80u),
			write_data[3], write_data[2], write_data[1], write_data[0] };

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin,
			GPIO_PIN_RESET);

//	if (!HAL_SPI_Transmit(&hspi3, &prepped_buffer, 5, HAL_MAX_DELAY)
//			!= HAL_OK) {
//		LOG_ERROR("Failed to read register 0x%x", reg_addr);
//	}

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_SET);

	return DRV_OK;

}

int driver_enable_bridges(driver_t *drv, int enable) {

	return DRV_OK;
}

int driver_reset(driver_t *drv) {

	return DRV_OK;
}
