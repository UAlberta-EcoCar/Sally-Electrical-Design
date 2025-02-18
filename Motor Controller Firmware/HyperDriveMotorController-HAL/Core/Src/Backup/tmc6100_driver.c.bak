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

	uint32_t r = { 0 };

	driver_spi_read(drv, TMC6100_IOIN, &r);

	tmc6100_ioin_t io = { 0 };

	io.reg_raw_ioin = r;

	r = 0;

	driver_spi_read(drv, TMC6100_GCONF, &r);

	tmc6100_gconf_t conf = { 0 };

	conf.reg_raw_gconf = r;

	conf.driver_disable = 0;
	conf.singleline = 1;
	conf.test_mode = 0;
	conf.fault_direct = 1;
	conf.normal_operation = 1;

	driver_spi_write(drv, TMC6100_GCONF, conf.reg_raw_gconf);

	r = 0;

	driver_spi_read(drv, TMC6100_GCONF, &r);

	conf.reg_raw_gconf = r;

	r = 0;

	driver_spi_read(drv, TMC6100_IOIN, &r);

	io.reg_raw_ioin = r;

	tmc6100_drv_conf_t drv_conf = { 0 };

	drv_conf.bbm_clks = 10;

	drv_conf.drvstrength = 3;

	driver_spi_write(drv, TMC6100_DRV_CONF, drv_conf.reg_raw_drv_conf);

	drv_conf.reg_raw_drv_conf = 0;

	driver_spi_read(drv, TMC6100_DRV_CONF, &drv_conf.reg_raw_drv_conf);

	return DRV_OK;
}

int driver_spi_read(driver_t *drv, uint8_t reg_addr, uint32_t *read_result) {

	HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
			GPIO_PIN_RESET);

	uint8_t prepped_buffer = (uint8_t) reg_addr & 0x7f;

	if (!HAL_SPI_Transmit(&hspi3, &prepped_buffer, 1, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}
	uint8_t read_value[4] = { 0 };
	if (!HAL_SPI_Receive(&hspi3, (uint8_t*) &read_value, 4, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	(*read_result) = (read_value[0] << 24) | (read_value[1] << 16)
			| (read_value[2] << 8) | (read_value[3]);

	HAL_GPIO_WritePin(SPI_NSCS_DRIVER_GPIO_Port, SPI_NSCS_DRIVER_Pin,
			GPIO_PIN_SET);

	return DRV_OK;

}

int driver_spi_write(driver_t *drv, uint8_t reg_addr, uint32_t write_data) {

	// 1 the MSB since that is the wnr bit. we are writing so it must be 1.
	uint8_t prepped_buffer = ((uint8_t) reg_addr | 0x80);

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin,
			GPIO_PIN_RESET);

	if (!HAL_SPI_Transmit(&hspi3, (uint8_t*) &prepped_buffer, 1, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	uint8_t write_buf[4] = { 0 };
	write_buf[0] = (write_data >> 24) & 0xFF;
	write_buf[1] = (write_data >> 16) & 0xFF;
	write_buf[2] = (write_data >> 8) & 0xFF;
	write_buf[3] = (write_data >> 0) & 0xFF;

	if (!HAL_SPI_Transmit(&hspi3, (uint8_t*) &write_buf, 4, HAL_MAX_DELAY)
			!= HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", reg_addr);
	}

	HAL_GPIO_WritePin(drv->driver_nss_port, drv->driver_nss_pin, GPIO_PIN_SET);

	return DRV_OK;

}

int driver_read_error(driver_t *drv) {

	tmc6100_gstat_t stat = { 0 };

	driver_spi_read(drv, TMC6100_GSTAT, &stat.reg_raw_gstat);

	driver_spi_write(drv, TMC6100_GSTAT, 0xFFFF);

	tmc6100_ioin_t io = { 0 };

	driver_spi_read(drv, TMC6100_IOIN, &io.reg_raw_ioin);

	tmc6100_gconf_t conf = { 0 };

	driver_spi_read(drv, TMC6100_GCONF, &conf.reg_raw_gconf);

	tmc6100_drv_conf_t drv_conf = { 0 };

	driver_spi_read(drv, TMC6100_DRV_CONF, &drv_conf.reg_raw_drv_conf);

}

int driver_enable_bridges(driver_t *drv, int enable) {

	return DRV_OK;
}

int driver_reset(driver_t *drv) {

	return DRV_OK;
}
