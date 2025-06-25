#ifndef EXPORTED_TYPEDEF_H
#define EXPORTED_TYPEDEF_H

// Typedefines for FET board state
typedef enum {
  ALL_FET_OFF = 0x00,
  FUELCELL_FET = 0x01,
  CAP_FET = 0x02,
  RES_FET = 0x04,
  OUT_FET = 0x08,
} fetBit_t;

typedef enum {
  FET_STBY = ALL_FET_OFF,
  FET_CHRGE = FUELCELL_FET | CAP_FET | RES_FET,
  FET_RUN = FUELCELL_FET | CAP_FET | RES_FET | OUT_FET,
} fetState_t;

// Typedefines for REL board state
typedef enum {
  ALL_RELAY_OFF = 0x00,
  CAP_RELAY = 0x01,
  RES_RELAY = 0x02,
  DSCHRGE_RELAY = 0x04,
  MTR_RELAY = 0x08,
} relayBit_t;

typedef enum {
  RELAY_STBY = ALL_RELAY_OFF,
  RELAY_STRTP = RES_RELAY | DSCHRGE_RELAY,
  RELAY_CHRGE = RES_RELAY,
  RELAY_RUN = CAP_RELAY | DSCHRGE_RELAY | MTR_RELAY,
} rbState_t;

#endif
