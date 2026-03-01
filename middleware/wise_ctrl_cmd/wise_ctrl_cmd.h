/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_ctrl_cmd.h
 * @brief wise_ctrl_cmd.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_CTRL_CMD_H
#define __WISE_CTRL_CMD_H

#ifndef TARGET_ARDUINO
#include "wise.h"
#endif

#include "wise_ctrl_packet.h"
#include <stdint.h>

#if 0
typedef union
{ /* General Packet */
  uint8_t cmd_pkt[UART_COMM_MAX_PAYLOAD_LEN];
  struct
  {
    uint8_t command[2];
    uint8_t data[UART_COMM_MAX_PAYLOAD_LEN-0x2];
  };
} WISE_CTRL_CMD_FORMAT_T;
#endif

#define WISE_FLAG_CMD 0x00
#define WISE_FLAG_RESP 0x80
#define WISE_MASK_CMD_TYPE 0x7F

int32_t wise_ctrl_cmd_init();
int32_t wise_ctrl_cmd_process(uint8_t *cmdBuf, uint16_t len);

int32_t wise_ctrl_evt_interrupt_status(uint32_t intStatus);

#endif
