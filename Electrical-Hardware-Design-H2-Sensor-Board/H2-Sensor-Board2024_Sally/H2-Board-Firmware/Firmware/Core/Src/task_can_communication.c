/*
 * task_can_communication.c
 *
 *  Created on: Feb 28, 2025
 *      Author: abina
 */

#include "task_can_communication.h"
#include "main.h"
#include "fdcan.h"
#include "log/debug-log.h"
#include <FreeRTOS.h>
#include <task.h>
#include "ecocar_can.h"

extern osMessageQueueId_t CANMessageRecieveQHandle;
extern osSemaphoreId_t H2AlarmSemHandle;
extern osMessageQueueId_t CANMessageTransmitQHandle;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[64]; // kept this so it is compatible with CAN FD aswell so i dont rip my hair out next year.
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData)
				!= HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		switch (RxHeader.Identifier) {
		case FDCAN_SYNCLED_ID:
			HAL_GPIO_WritePin(GPLED5_GPIO_Port, GPLED5_Pin, RxData[0]);
			break;
		default:
			osMessageQueuePut(CANMessageRecieveQHandle, &RxHeader.Identifier, 0,
					0);
			osMessageQueuePut(CANMessageRecieveQHandle, &RxHeader.DataLength, 0,
					0);
			for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
				osMessageQueuePut(CANMessageRecieveQHandle, &RxData[i], 0, 0);
			}
			break;
		}
	}
}

/* USER CODE BEGIN Header_StartCANTransmitTask */
/**
 * @brief Function implementing the CANTransmit thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCANTransmitTask */
void StartCANTransmitTask(void *argument) {
	/* USER CODE BEGIN StartCANTransmitTask */
	HAL_GPIO_WritePin(CAN_STDBY_GPIO_Port, CAN_STDBY_Pin, GPIO_PIN_RESET);
	uint8_t send_data[64];
	FDCAN_TxHeaderTypeDef TxHeader = { 0 };
	/* Infinite loop */
	for (;;) {

		if (0 != osMessageQueueGetCount(CANMessageTransmitQHandle)) {
			if (osOK
					== osMessageQueueGet(CANMessageTransmitQHandle,
							&TxHeader.Identifier, 0, 0)) {
				osMessageQueueGet(CANMessageTransmitQHandle,
						&TxHeader.DataLength, 0, 0);
				for (uint8_t i = 0; i < mapDlcToBytes(TxHeader.DataLength);
						i++) {
					osMessageQueueGet(CANMessageTransmitQHandle, &send_data[i],
							0, 0);
				}
				// Handle Sending
			} else {
				log_err("CAN Transmit Message Queue Error.");
			}
		}

		// Transmit the basic data

//		if (0 == HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
//			if (HAL_OK
//					== HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &h2,
//							&sensor_data)) {
//				log_info("Successfully transmitted H2 Alarm");
//			}
//		} else {
//			log_err("Failed to send H2 Alarm signal, fifo full");
//			// this means there was no room in the tx fifo, so give the semaphore again and retry.
//			if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//				log_info("Retrying");
//			}
//		}
//		if (0 == HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
//			if (HAL_OK
//					== HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &h2,
//							&sensor_data)) {
//				log_info("Successfully transmitted H2 Alarm");
//			}
//		} else {
//			log_err("Failed to send H2 Alarm signal, fifo full");
//			// this means there was no room in the tx fifo, so give the semaphore again and retry.
//			if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//				log_info("Retrying");
//			}
//		}

//		if (H2_ALARM_TRIGGERED == alarm_state) {
//			if (0 != HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
//				if (HAL_OK
//						== HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &h2,
//								&sensor_data)) {
//					log_info("Successfully transmitted H2 Alarm");
//				}
//			} else {
//				log_err("Failed to send H2 Alarm signal, fifo full");
//				// this means there was no room in the tx fifo, so give the semaphore again and retry.
//				if (osOK == osSemaphoreRelease(H2AlarmSemHandle)) {
//					log_info("Retrying");
//				}
//			}
//		}

		osDelay(20);
	}
	/* USER CODE END StartCANTransmitTask */
}

/* USER CODE BEGIN Header_StartCANRecieve */
/**
 * @brief Function implementing the CANRecieve thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCANRecieve */
void StartCANRecieve(void *argument) {
	/* USER CODE BEGIN StartCANRecieve */
	HAL_GPIO_WritePin(CAN_STDBY_GPIO_Port, CAN_STDBY_Pin, GPIO_PIN_RESET);

	FDCAN_RxHeaderTypeDef incomming = { 0 };

	uint8_t dlc = 0;
	uint8_t id = 0;

	uint8_t incomming_data[64] = { 0 };

	/* Infinite loop */
	for (;;) {

		if (0 != osMessageQueueGetCount(CANMessageRecieveQHandle)) { // If theres something in the queue
			if (osOK
					!= osMessageQueueGet(CANMessageRecieveQHandle, &id, 0,
							10)) {
				log_err("Error Reading q");
			}
			if (osOK
					!= osMessageQueueGet(CANMessageRecieveQHandle, &dlc, 0,
							10)) {
				log_err("Error Reading q");
			}

			for (uint8_t i = 0; i < mapDlcToBytes(dlc); i++) {
				if (osOK
						!= osMessageQueueGet(CANMessageRecieveQHandle,
								&incomming_data[i], 0, 10)) {
					log_err("Error Reading q");
				}
			}
			switch (id) {
			case ECOCAN_H2_ARM_ALARM_ID:
//				HAL_GPIO_WritePin(GPLED4_GPIO_Port, GPLED4_Pin, RxD)
				ECOCAN_H2_ARM_ALARM_t inc = {0};
				memcpy(inc.ECOCAN_raw_pack, incomming_data, FDCAN_BYTES_8);
				alarm_state = inc.h2_alarm_armed;
				break;
			default:
				break;
			}
		}
		osDelay(10);
		/* USER CODE END StartCANRecieve */
	}
}
