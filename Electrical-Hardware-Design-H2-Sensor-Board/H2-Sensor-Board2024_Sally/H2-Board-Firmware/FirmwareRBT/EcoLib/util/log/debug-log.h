/*
 * debug-log.h
 *
 *  Created on: Dec 29, 2024
 *      Author: abina
 */

#ifndef INC_DEBUG_LOG_H_
#define INC_DEBUG_LOG_H_

#include "ansi-codes.h"

#define log_err(M, ...) printf(SETBG_RED SETFG_WHITE "[ERROR] (%s:%d) " M FG_DEFAULT BG_DEFAULT "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define log_warn(M, ...) printf(SETFG_YELLOW "[WARN] (%s:%d) " M FG_DEFAULT "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define log_info(M, ...) printf(SETFG_BLUE "[INFO] " M FG_DEFAULT "\r\n", ##__VA_ARGS__)

#define log_critical(M, ...) printf(SETBG_RED FG_RED "[INFO] " M FG_DEFAULT "\r\n", ##__VA_ARGS__)

#endif /* INC_DEBUG_LOG_H_ */
