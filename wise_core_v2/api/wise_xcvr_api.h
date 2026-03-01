/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef _WISE_XVCR_API_H_
#define _WISE_XVCR_API_H_

#include "api/wise_spi_api.h"

WISE_STATUS wise_xcvr_reset_fifo(uint8_t spi_channel);

WISE_STATUS wise_xcvr_read_rx_fifo2(uint8_t spi_channel, uint16_t nbytes_len, uint32_t *rx_ptr);

WISE_STATUS wise_xcvr_register_write(uint8_t spi_channel, uint16_t address, uint32_t *value);

WISE_STATUS wise_xcvr_register_read(uint8_t spi_channel, uint16_t address, uint32_t *value);

#endif /* _WISE_XVCR_API_H_  */
