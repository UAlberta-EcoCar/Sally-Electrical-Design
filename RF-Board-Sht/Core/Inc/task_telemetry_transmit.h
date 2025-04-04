/*
 * task_telemetry_transmit.h
 *
 *  Created on: Apr 2, 2025
 *      Author: abina
 */

#ifndef INC_TASK_TELEMETRY_TRANSMIT_H_
#define INC_TASK_TELEMETRY_TRANSMIT_H_

#include "ecocar_can.h"
#include "exported_typedef.h"

typedef struct {
	//FDCAN_FetPack_t fet_data;
	FDCAN_RelPackMtr_t mtr_data;
	FDCAN_RelPackCap_t cap_data;
	FDCAN_FccPack1_t fc_data1;
	FDCAN_FccPack2_t fc_data2;
} telemetry_data1_t;

typedef struct {
	FDCAN_FccPack3_t fc_data3;
	//FDCAN_H2Pack_t h2_data;
	FDCAN_BOOSTPack_t boost_data1;
	FDCAN_BOOSTPack2_t boost_data2;
	FDCAN_RelPackFc_t RelPackFc;
} telemetry_data2_t;


typedef struct {
	ECOCAN_H2Pack1_t h2_data;
	rbState_t rb_state;
} telemetry_data3_t;


#endif /* INC_TASK_TELEMETRY_TRANSMIT_H_ */
