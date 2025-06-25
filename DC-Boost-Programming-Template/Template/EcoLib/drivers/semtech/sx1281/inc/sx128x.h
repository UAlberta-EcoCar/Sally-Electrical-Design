#ifndef __SX128X_H__
#define __SX128X_H__

#include "stm32g4xx_hal.h"

typedef void (*rf24_delay_func_t)(uint32_t);

typedef void (*rf24_post_init_func_t)();

typedef struct {
	SPI_HandleTypeDef *rf24_spi_handle;

	uint32_t rf24_spi_timeout;

	GPIO_TypeDef *rf24_txen_port;

	uint16_t rf24_txen_pin;

	GPIO_TypeDef *rf24_rxen_port;

	uint16_t rf24_rxen_pin;

	GPIO_TypeDef *rf24_nss_port;

	uint16_t rf24_nss_pin;

	GPIO_TypeDef *rf24_reset_port;

	uint16_t rf24_reset_pin;

	rf24_post_init_func_t rf24_post_init_clbk;

	rf24_delay_func_t rf24_delay_func;

	uint8_t rf24_module_identifier;

	uint32_t rf24_carrier_frequency;

	uint32_t rf24_carrier_frequency_offset;

	uint8_t rf24_rxtx_enable;

} rf24_handle_t;

typedef enum {
	RF24_OK = 0, RF24_ERROR = 1
} rf24_return_status_t;

int rf24_initialize_radio(rf24_handle_t *_rf24_handle);

int rf24_reset(rf24_handle_t *_rf24_handle);

int rf24_check_device(rf24_handle_t *_rf24_handle);

int rf24_check_busy(rf24_handle_t *rf24_handle);

int rf24_write_command(rf24_handle_t *rf24_handle, uint8_t rf24_command_address,
		uint8_t *rf24_command_buffer, uint16_t rf_command_size);

int rf24_spi_write_registers(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_to_write_buffer,
		uint16_t rf24_write_size);

int rf24_spi_write_register(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t rf24_register_to_write_value);

int rf24_spi_read_registers(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_result,
		uint16_t rf24_read_size);

int rf24_spi_read_register(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_result);

int rf24_set_mode(rf24_handle_t *rf24_handle, SX128X_Circuit_Mode_t rf24_mode);

int rf24_set_regulator_mode(rf24_handle_t *rf24_handle,
		SX128X_Circuit_Mode_t rf24_mode);

int rf24_set_packet_type(rf24_handle_t *rf24_handle,
		SX128X_Packet_Type_t rf24_packet_type);

int rf24_set_frequency(rf24_handle_t *rf24_handle, uint32_t rf24_frequency,
		uint32_t rf24_offset);

int rf24_set_buffer_base_address(rf24_handle_t *rf24_handle,
		SX128X_Circuit_Mode_t rf24_mode);

int rf24_set_modulation_parameters(rf24_handle_t *rf24_handle,
		SX128X_Modulation_Parameters_LORA_t rf24_mode);

int rf24_set_packet_parameters(rf24_handle_t *rf24_handle,
		uint8_t rf24_preamble_length_mantissa,
		uint8_t rf24_preamble_length_exponent, uint8_t rf24_header_mode,
		uint8_t rf24_payload_length, uint8_t rf24_lora_crc,
		uint8_t rf24_lora_iq);

int rf24_set_dio_parameters(rf24_handle_t *rf24_handle,
		SX128X_IRQ_Register_t rf24_irq_register);

int rf24_set_high_sensitivity(rf24_handle_t *rf24_handle,
		uint8_t rf24_set_high_sensitivity);

#endif //__SX128X_H__
