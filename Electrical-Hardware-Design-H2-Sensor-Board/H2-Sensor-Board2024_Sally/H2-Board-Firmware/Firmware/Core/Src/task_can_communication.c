/*
 * task_can_communication.c
 *
 *  Created on: Feb 28, 2025
 *      Author: abina
 */

#include "task_can_communication.h"
#include "main.h"
#include "fdcan.h"

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
		osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.Identifier, 0, 0);
		osMessageQueuePut(canQueRxHeaderHandle, &RxHeader.DataLength, 0, 0);
		for (uint32_t i = 0; i < mapDlcToBytes(RxHeader.DataLength); i++) {
			osMessageQueuePut(canQueRxDataHandle, &RxData[i], 0, 0);
		}
	}
}

void StartCANCommunicationTask(void *argument) {
	/* USER CODE BEGIN StartCANCommunicationTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartCANCommunicationTask */
}
