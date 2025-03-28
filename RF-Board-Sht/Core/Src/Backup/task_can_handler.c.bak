#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fdcan.h"

#include "ecocar_can.h"
#include "exported_typedef.h"
#include "debug-log.h"

extern osMessageQueueId_t canQueRxHeaderHandle;
extern osMessageQueueId_t canQueRxDataHandle;

uint8_t gps_can[8] = {0};

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

telemetry_data1_t data1;
telemetry_data2_t data2;
//FDCAN_FccPack1_t fc_data1 = { 0 };

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[64];
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retrieve Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData)
				!= HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		if (FDCAN_SYNCLED_ID == RxHeader.Identifier) {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,
					(RxData[0]) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		} else {

			if (osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0,
					0) != osOK
					|| osMessageQueuePut(canQueRxHeaderHandle,
							&RxHeader.DataLength, 0, 0) != osOK) {
				Error_Handler();
			}
			for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
				if (osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0)
						!= osOK) {
					Error_Handler();
				}
			}
		}
	}
}

/* USER CODE BEGIN Header_StartCANHandler */
/**
 * @brief Function implementing the CANHandler thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCANHandler */
void StartCanSend(void *argument) {
	/* USER CODE BEGIN StartCANHandler */

	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8] = "H";

	TxHeader.Identifier = 0x321;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	/* Infinite loop */
	for (;;) {
//		printf("HEllo");
//		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, &TxData);

		osDelay(100);
	}
	/* USER CODE END StartCANHandler */
}

/* USER CODE BEGIN Header_StartCanReceive */
/**
 * @brief Function implementing the CANReceive thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCanReceive */
void StartCanReceive(void *argument) {
	/* USER CODE BEGIN StartCanReceive */
	FDCAN_RxHeaderTypeDef localRxHeader = { 0 };
	uint8_t ret[64] = { 0 };
	uint32_t *pointer_to_data_store = 0;
	/* Infinite loop */
	for (;;) {
		if (osMessageQueueGet(canQueRxHeaderHandle, &localRxHeader.Identifier,
				0, osWaitForever) == osOK) {

			if (osMessageQueueGet(canQueRxHeaderHandle,
					&localRxHeader.DataLength, 0, osWaitForever) != osOK) {
				Error_Handler();
			}
			for (uint8_t i = 0; i < mapDlcToBytes(localRxHeader.DataLength);
					i++) {
				if (osMessageQueueGet(canQueRxDataHandle, &ret[i], 0,
				osWaitForever) != osOK) {
					Error_Handler();
				}
			}
			if (FDCAN_REMOTE_FRAME != localRxHeader.RxFrameType) {
				switch (localRxHeader.Identifier) {
				case FDCAN_SYNCLED_ID:
//				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,
//						(ret[0]) ? GPIO_PIN_SET : GPIO_PIN_RESET);
					break;
				case FDCAN_FCCPACK1_ID:
					// Copy data fc pres and temp
//				memcpy(fc_data1.FDCAN_RawFccPack, ret,
//						mapDlcToBytes(localRxHeader.DataLength));
					pointer_to_data_store = &data1.fc_data1.FDCAN_RawFccPack;
					break;
				case FDCAN_RELPACKFC_ID:
					// Copy data fc pres and temp
//				memcpy(RelPackFc.FDCAN_RawRelPackFc, ret,
//						mapDlcToBytes(localRxHeader.DataLength));
					pointer_to_data_store = &data2.RelPackFc.FDCAN_RawRelPackFc;
					break;
				case FDCAN_FETPACK_ID:
//					pointer_to_data_store = &data2.;
					break;
				case FDCAN_RELPACKMTR_ID:
					pointer_to_data_store = &data1.mtr_data.FDCAN_RawRelPackMtr;
					break;
				case FDCAN_RELPACKCAP_ID:
					pointer_to_data_store = &data1.cap_data.FDCAN_RawRelPackCap;
					break;
				case FDCAN_RELSTATE_ID:
//					pointer_to_data_store =;
					break;
				case FDCAN_FCCPACK2_ID:
					pointer_to_data_store = &data1.fc_data2.FDCAN_RawFccPack;
					break;
				case FDCAN_FCCPACK3_ID:
					pointer_to_data_store = &data2.fc_data3.FDCAN_RawFccPack;
					break;
//				case FDCAN_H2PACK_ID:
//					pointer_to_data_store = &data1.fc_data1.FDCAN_RawFccPack;
//					break;
				case FDCAN_BOOSTPACK_ID:
					pointer_to_data_store = &data2.boost_data1.FDCAN_RawBOOSTPack;
					break;
				case FDCAN_BOOSTPACK2_ID:
					pointer_to_data_store = &data2.boost_data2.FDCAN_RawBOOSTPack2;
					break;
				case 0x123:
//					memcpy(gps_can, ret, 8);
					break;
				default:
					log_err("CANID 0x%x not handled!",
							localRxHeader.Identifier);
					break;

				}
				memcpy(pointer_to_data_store, ret,
						mapDlcToBytes(localRxHeader.DataLength));
			} else {
				log_err("Some Thing is sending remote frames. %x", localRxHeader.Identifier);
			}
		}
//		osDelay(1);
	}
	/* USER CODE END StartCanReceive */
}
