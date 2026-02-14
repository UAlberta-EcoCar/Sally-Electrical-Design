/*
 * tmc6100.h
 *
 *  Created on: Jan 20, 2025
 *      Author: abina
 */

#ifndef INC_TMC6100_H_
#define INC_TMC6100_H_

#define TMC6100_GCONF 0x00

typedef struct {
	union {
		struct {
			uint8_t driver_disable :1;
			uint8_t singleline :1;
			uint8_t fault_direct :1;
			uint8_t unused :3;
			uint8_t normal_operation :1;
			uint8_t test_mode :1;
		// rest unused.
		};
		uint32_t reg_raw_gconf;
		uint8_t reg_gconf[4];
	};
} tmc6100_gconf_t;

#define TMC6100_GSTAT 0x01

typedef struct {
	union {
		struct {
			uint8_t rest :1;
			uint8_t drv_otpw :1;
			uint8_t drv_otpw :1;
			uint8_t drv_ot :1;
			uint8_t uv_cp :1;
			uint8_t shortdet_u :1;
			uint8_t s2gu :1;
			uint8_t s2vsu :1;
			uint8_t unused :1;
			uint8_t shortdet_v :1;
			uint8_t s2gv :1;
			uint8_t s2vsv :1;
			uint8_t unused :1;
			uint8_t shortdet_w :1;
			uint8_t s2gw :1;
			uint8_t s2vsw :1;

		};
		uint32_t reg_raw_gstat;
		uint8_t reg_gstat[4];
	};
} tmc6100_gstat_t;

#define TMC6100_IOIN 0x04

typedef struct {
	union {
		struct {
			uint8_t ul :1;
			uint8_t uh :1;
			uint8_t vl :1;
			uint8_t vh :1;
			uint8_t wl :1;
			uint8_t wh :1;
			uint8_t drv_en :1;
			uint8_t zero :1;
			uint8_t otpw :1;
			uint8_t ot136 :1;
			uint8_t ot143 :1;
			uint8_t ot150 :1;
			uint32_t unused :13;
			uint8_t version;
		};
		uint32_t reg_raw_ioin;
		uint8_t reg_raw_ioin[4];
	};
} tmc6100_ioin_t;

#define TMC6100_otp_prog 0x06

typedef struct {
	union {
		struct {
			uint8_t otpbit :3;
			uint8_t otpbyte :2;
			uint8_t otpmagic;
		};
		uint8_t reg_raw_otp_prog[4];
		uint32_t reg_raw_otp_prog;
	};
} tmc6100_otp_prog_t;

#endif /* INC_TMC6100_H_ */
