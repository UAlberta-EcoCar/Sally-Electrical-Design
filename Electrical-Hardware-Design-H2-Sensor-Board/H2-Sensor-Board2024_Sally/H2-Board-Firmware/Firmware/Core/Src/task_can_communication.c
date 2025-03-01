/*
 * task_can_communication.c
 *
 *  Created on: Feb 28, 2025
 *      Author: abina
 */

#include "task_can_communication.h"
#include "main.h"
#include "fdcan.h"

void StartCANCommunicationTask(void *argument)
{
  /* USER CODE BEGIN StartCANCommunicationTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCANCommunicationTask */
}
