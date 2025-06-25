#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "exported_typedef.h"
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>

/* Users must provide these char queues */
extern osMessageQueueId_t usbQueReceiveHandle;
extern osMessageQueueId_t usbQueSendHandle;

/* Add variables as necessary for your application
 * by toggling the commented defines
 * Just note that whatever variables are relevant
 * to your application should be defined here with
 * the extern key word */

#define NEED_FET_STATE_COMMAND
#ifdef NEED_FET_STATE_COMMAND
extern fetState_t fet_state;
#endif

#define NEED_REL_STATE_COMMAND
#ifdef NEED_REL_STATE_COMMAND
extern rbState_t relay_state;
#endif

// Syscall implementation that printf will use
int _write(int file, char *ptr, int len) {
  UNUSED(file);
  //  CDC_Transmit_FS((uint8_t *)ptr, len);
  for (uint32_t i = 0; i < (uint32_t)len; i++) {
    if (osMessageQueuePut(usbQueSendHandle, ptr + i, 0, 0) != osOK) {
      // Error_Handler();
    }
  }
  return len;
}

// Remember to put data into usbQueReceiveHandle from
// CDC_Receive_FS function in usbd_cdc_if.c
void doReceiveUsbTask(void);
void doSendUsbTask(void);

/* Header_StartUsbReceive */
/**
 * @brief Function implementing the usbReceiveTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUsbReceive */
void StartUsbTask(void *argument) {
  /* USER CODE BEGIN StartAdcConv */
  UNUSED(argument);

  /* Infinite loop */
  for (;;) {
    doReceiveUsbTask();
    doSendUsbTask();
    osDelay(10);
  }
  /* USER CODE END StartAdcConv */
}

void doReceiveUsbTask(void) {
  char ret[2048] = {0};
  uint8_t iter = 0;
  if (osMessageQueueGetCount(usbQueReceiveHandle) > 0) {
    do {
      osMessageQueueGet(usbQueReceiveHandle, &ret[iter], 0, 0);
    } while (ret[iter++] != '\n');
    ret[iter] = '\0';
    iter = 0;

    // If a prefix for a command is found
    if (ret[0] == ':') {
      switch (ret[1]) {
#ifdef NEED_FET_STATE_COMMAND
      case 'L':
        switch (ret[3]) {
        case '0':
          fet_state = FET_STBY;
          printf("VALID: Vehicle in STANDBY\r\n");
          break;
        case '1':
          fet_state = FET_CHRGE;
          printf("VALID: Vehicle in CHARGE\r\n");
          break;
        case '2':
          fet_state = FET_RUN;
          printf("VALID: Vehicle in RUN\r\n");
          break;
        default:
          printf("ERROR: Invalid command input: %s\r\n", ret);
          break;
        }
        break;
#endif
#ifdef NEED_REL_STATE_COMMAND
      case 'R':
        switch (ret[3]) {
        case '0':
          relay_state = RELAY_STBY;
          printf("VALID: Vehicle in STANDBY\r\n");
          break;
        case '1':
          relay_state = RELAY_STRTP;
          printf("VALID: Vehicle in STARTUP\r\n");
          break;
        case '2':
          relay_state = RELAY_RUN;
          printf("VALID: Vehicle in RUN\r\n");
          break;
        default:
          printf("ERROR: Invalid command input: %s\r\n", ret);
          break;
        }
        break;
#endif
      default:
        printf("ERROR: Invalid command input: %s\r\n", ret);
      }
    } else {
      printf("ECHO: %s", ret);
    }
  }
}

__weak void doSendUsbTask(void) {
  char ret[2048] = {0};
  uint16_t iter = 0;
  if (osMessageQueueGetCount(usbQueSendHandle) > 0) {
    osDelay(10);
    do {
      osMessageQueueGet(usbQueSendHandle, &ret[iter], 0, 0);
      if (iter == 2048 - 1) {
        break;
      }
    } while (ret[iter++] != '\n');
    // TODO: Verify the iter length is proper for CDC
    CDC_Transmit_FS((uint8_t *)ret, iter);
  }
}
