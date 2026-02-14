#ifndef SX1281_REGISTERS_H
#define SX1281_REGISTERS_H

#include <stdint.h>

typedef enum {

	SX128X_COMMAND_RESERVED = 0x0,
	SX128X_TRANCIVER_COMMAND_PROCCESSED_SUCCESSFULLY = 0x1,
	SX128X_DATA_AVAILABLE_TO_HOST = 0x2,
	SX128X_COMMAND_TIMEOUT = 0x3,
	SX128X_COMMAND_PROCESSING_ERROR = 0x4,
	SX128X_COMMAND_EXECUTE_FAIL = 0x5,
	SX128X_COMMAND_TX_DONE = 0x6

} SX128X_Command_Status_t;

typedef enum {

	SX128X_CIRCUIT_MODE_RESERVED = 0x0,
	SX128X_CIRCUIT_MODE_RESERVED1 = 0x1,
	SX128X_CIRCUIT_MODE_STDBY_RC = 0x2,
	SX128X_CIRCUIT_MODE_STDBY_XOSC = 0x3,
	SX128X_CIRCUIT_MODE_FS = 0x4,
	SX128X_CIRCUIT_MODE_RX = 0x5,
	SX128X_CIRCUIT_MODE_TX = 0x6

} SX128X_Circuit_Mode_t;

/* OpCode Commands */

#define SX128X_GET_STATUS_COMMAND_OPCODE 0xC0 // returns status defined below
// OpCode Return Parameters
typedef struct {
	union {
		struct {
			uint8_t isBusy :1;
			uint8_t reserved :1;
			uint8_t command_status :3; // defined in command status enum
			uint8_t circuit_mode :3; // defined in circuit mode enum
		};
		uint8_t sx128x_status_raw;
	};
} SX128X_Get_Status_Command_t;

#define SX128X_WRITE_REGISTER_COMMAND_OPCODE 0x18 // returns status as defined in enum

#define SX128X_READ_REGISTER_COMMAND_OPCODE 0x19 // returns status as defined in enum

#define SX128X_WRITE_BUFFER_COMMAND_OPCODE 0x1A // returns status as defined in enum

#define SX128X_READ_BUFFER_COMMAND_OPCODE 0x1B // returns status as defined in enum

typedef struct {
	union {
		struct {
			uint8_t isRAMretained :1;
			uint8_t isDataBufferretained :1;
		};
		uint8_t sx128x_sleep_config_raw;
	};
} SX128X_Sleep_Mode_Config_t;

/**
 * SX128X Pg. 28 4.2.1  Low Power Mode and High Sensitivity Mode
 * In receive mode, the SX1280 can operate in one of two distinct regimes of operation. Low power mode allows maximum
 * efficiency of the SX1280 to be attained, optimizing the performance of the device for receiver current consumption. This is
 * enabled by default and prevents the receiver LNA from accessing the highest three steps of LNA gain.
 * Conversely, high sensitivity mode enables highest sensitivity gain steps for a slight increase in receiver current
 * consumption. High sensitivity mode is enabled by setting bits 7:6 at address 0x891 to 0x3. Once enabled the noise figure of
 * the receiver is improved by up to 3 dB for 500 μA of additional current consumption.
 */
#define SX128X_LNA_REGIME_REGISTER 0x0891 // consistancy with the 16 bit registers.

#define SX128X_LNA_REGIME_LNA_ENABLE_OR 0xC0
#define SX128X_LNA_REGIME_LNA_DISABLE_AND 0x3F

// pg 90 11.8.1 SetDIOTRQ PARAMS

#define SX128X_SET_DIO_IRQ_PARAMS_COMMAND_OPCODE 0x1B

// 11.8 11-71

typedef struct {
	union {
		struct {
			uint8_t txDone :1;
			uint8_t rxDone :1;
			uint8_t syncWordValid :1;
			uint8_t syncWordError :1;
			uint8_t headerValid :1;
			uint8_t headerError :1;
			uint8_t crcError :1;
			uint8_t rangingSlaveResponseDone :1; // unavaialble on Sx1281
			uint8_t rangingSlaveRequestDiscard :1;
			uint8_t rangingMasterResultValid :1;
			uint8_t rangingMasterTimeout :1;
			uint8_t rangingMasterRequestValid :1;
			uint8_t cadDone :1;
			uint8_t cadDetected :1;
			uint8_t rxtxTimeout :1;
			uint8_t preambleDetected_advancedRangingDone :1;
		};

		uint16_t irq_register;
	};

} SX128X_IRQ_Register_t;

// Packet parameter
// param 1 pg 121 preamble legth

// param2
#define SX128X_PACKET_PARAMETER_EXPLICIT_HEADER 0x00
#define SX128X_PACKET_PARAMETER_IMPLICIT_HEADER 0x80

// param4
#define SX128X_PACKET_PARAMETER_LORA_CRC_ENABLE 0x20
#define SX128X_PACKET_PARAMETER_LORA_CRC_DISABLE 0x00

// param5
#define SX128X_PACKET_PARAMETER_LORA_IQ_STD 0x40
#define SX128X_PACKET_PARAMETER_LORA_IQ_INVERTEED 0x00

#define SX128X_SET_PACKET_PARAMETERS_COMMAND_OPCODE 0x8C

#define SX128X_LORA_SPREADING_FACTOR_5 0x50
#define SX128X_LORA_SPREADING_FACTOR_6 0x60
#define SX128X_LORA_SPREADING_FACTOR_7 0x70
#define SX128X_LORA_SPREADING_FACTOR_8 0x80
#define SX128X_LORA_SPREADING_FACTOR_9 0x90
#define SX128X_LORA_SPREADING_FACTOR_10 0xA0
#define SX128X_LORA_SPREADING_FACTOR_11 0xB0
#define SX128X_LORA_SPREADING_FACTOR_12 0xC0

#define SX128X_SET_MODULATION_PARAMETERS_COMMAND_OPCODE 0x8B

#define SX128X_FREQUENCY_ERROR_COMPENSATION_ERROR_MODE_REGISTER 0x093C

typedef struct {
	union {
		struct {
			uint8_t spreadingFactor;
			uint8_t bandwidth;
			uint8_t codingRate;
		};
		uint8_t modParams[3];
	};

} SX128X_Modulation_Parameters_LORA_t;

#define SX128X_LORA_SET_BASE_ADDRESS_COMMAND_OPCODE 0x8F

#define SX128X_LORA_SET_FREQUENCY_COMMAND_OPCODE 0x86

#define SX128X_LORA_FREQ_STEP                         198.364
#define SX128X_LORA_FREQ_ERROR_CORRECTION             1.55

#define SX128X_SET_PACKET_TYPE_COMMAND_OPCODE 0x8A

typedef enum {
	PACKET_TYPE_GFSK = 0x00,
	PACKET_TYPE_LORA = 0x01,
	PACKET_TYPE_RANGING = 0x02,
	PACKET_TYPE_FLRC = 0x03,
	PACKET_TYPE_BLE = 0x04
} SX128X_Packet_Type_t;

#endif
