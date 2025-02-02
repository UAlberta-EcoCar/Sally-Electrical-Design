#ifndef CAN_PACKS_H
#define CAN_PACKS_H

#include <stdint.h>

// Structs must be a certain size for FDCAN to transfer
// The following package sizes (in bytes) are 0, 1, 2, 3, 4, 5, 6,
// 7, 8, 12, 16, 20, 24, 32, 48, 64.
//
#define CAN_BYTES_24 24

typedef struct {
  union {
    struct {
      uint32_t fet_config;
      uint32_t input_volt;
      uint32_t cap_volt;
      uint32_t cap_curr;
      uint32_t res_curr;
      uint32_t out_curr;
    };
    uint8_t FDCAN_RawFetPack[CAN_BYTES_24];
  };
} FDCAN_FetPack_t;

#endif 
