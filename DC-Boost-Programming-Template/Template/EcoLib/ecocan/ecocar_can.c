#include "ecocar_can.h"
#include <stdint.h>

fdcanBytes_t mapDlcToBytes(uint32_t fdcanDlc) {
  fdcanBytes_t bytes;
  if (fdcanDlc <= 8) {
    return fdcanDlc;
  } else {
    switch (fdcanDlc) {
    case 9:
      bytes = FDCAN_BYTES_12;
      break;
    case 10:
      bytes = FDCAN_BYTES_16;
      break;
    case 11:
      bytes = FDCAN_BYTES_20;
      break;
    case 12:
      bytes = FDCAN_BYTES_24;
      break;
    case 13:
      bytes = FDCAN_BYTES_32;
      break;
    case 14:
      bytes = FDCAN_BYTES_48;
      break;
    case 15:
      bytes = FDCAN_BYTES_64;
      break;
    }
  }
  return bytes;
}
