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

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[64];
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData)
				!= HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}
		if (HAL_FDCAN_ActivateNotification(hfdcan,
		FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
		osMessageQueuePut(CANMessageRecieveQHandle, &RxHeader.Identifier, 0, 0);
		osMessageQueuePut(CANMessageRecieveQHandle, &RxHeader.DataLength, 0, 0);
		for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
			osMessageQueuePut(CANMessageRecieveQHandle, &RxData[i], 0, 0);
		}
	}
}

void StartCANCommunicationTask(void *argument) {
	/* USER CODE BEGIN StartCANCommunicationTask */

	// Take CAN Tranciever out of standby.
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
			case FDCAN_SYNCLED_ID:

				break;
			default:
				break;
			}
		}
		osDelay(10);
	}
	/* USER CODE END StartCANCommunicationTask */
}
