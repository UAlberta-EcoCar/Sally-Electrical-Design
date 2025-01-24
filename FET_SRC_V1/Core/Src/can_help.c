#include "main.h"

uint32_t mapDlcToBytes(uint32_t DLC) {
  uint32_t bytes;
  if (DLC <= 0x08) {
    return DLC;
  } else {
    switch (DLC) {
    case 0x09:
      bytes = 12;
      break;
    case 0x0A:
      bytes = 16;
      break;
    case 0x0B:
      bytes = 20;
      break;
    case 0x0C:
      bytes = 24;
      break;
    case 0x0D:
      bytes = 32;
      break;
    case 0x0E:
      bytes = 48;
      break;
    case 0x0F:
      bytes = 64;
      break;
    }
  }
  return bytes;
}
