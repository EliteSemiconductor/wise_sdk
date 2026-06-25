/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_SPI_H
#define __HAL_INTF_SPI_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef struct hal_info_spi_t {
    uint8_t spi_idx;
    void *spi;
    uint8_t block_mode;
} HAL_INFO_SPI_T;

/* Merged IO-mode values carried in HAL_SPI_CONF_T.io_mode (mirror of
 * WISE_SPI_IO_MODE_T). The driver derives TransFmt.MOSIBiDir (3-wire) and
 * TransCtrl.DualQuad (dual/quad) from this single value. */
#define HAL_SPI_IO_SINGLE 0
#define HAL_SPI_IO_3WIRE  1
#define HAL_SPI_IO_DUAL   2
#define HAL_SPI_IO_QUAD   3

/* Channel-wide config, applied once at open. addr_len / addr_fmt are per-
 * transfer (HAL_SPI_TRANS_FMT_T); io_mode (wire + lane width) is channel-wide. */
typedef struct {
    uint16_t clock_mode;
    uint8_t role;
    uint8_t data_bit_width;
    uint32_t bus_clock;
    uint8_t bit_order;
    uint8_t data_merge;
    uint8_t io_mode;        /* HAL_SPI_IO_* : wire topology + data-lane width */
    uint8_t block_mode;
    uint8_t dma_enable;
} HAL_SPI_CONF_T;

/* Per-transfer descriptor. addr_len == 0 means no address phase. */
typedef struct {
    uint8_t role;
    uint16_t rx_unit_count;
    uint16_t tx_unit_count;
    uint8_t dummy_len;
    uint8_t trans_mode;
    uint8_t cmd_en;
    uint8_t cmd_value;
    uint32_t addr_value;
    uint8_t addr_len;
    uint8_t addr_fmt;
} HAL_SPI_TRANS_FMT_T;

typedef enum {
    HAL_SPI_RESET_CORE = (1 << 0),
    HAL_SPI_RESET_RX   = (1 << 1),
    HAL_SPI_RESET_TX   = (1 << 2),
    HAL_SPI_RESET_ALL  = HAL_SPI_RESET_CORE | HAL_SPI_RESET_RX | HAL_SPI_RESET_TX
} HAL_SPI_RESET_TYPE;

typedef enum {
    SPI_EVENT_SLAVE_COMMAND_INTERRUPT,        // Slave Command Interrupt
    SPI_EVENT_TRANSFER_DONE,                  // Done of SPI Transfer
    SPI_EVENT_ON_SPI_TRANSFER_END_INTERRUPT,  // End of SPI Transfer Interrupt
    SPI_EVENT_ON_TX_FIFO_THRESHOLD_INTERRUPT, // TX FIFO Threshold Interrupt
    SPI_EVENT_ON_RX_FIFO_THRESHOLD_INTERRUPT, // RX FIFO Threshold Interrupt
    SPI_EVENT_ON_TX_FIFO_UNDERRUN_INTERRUPT,  // TX FIFO Underrun Interrupt
                                              // (Slave mode only)
    SPI_EVENT_ON_RX_FIFO_OVERRUN_INTERRUPT,   // RX FIFO Overrun Interrupt
    SPI_MAX_EVENTS,                           // Total number of SPI events
} SPI_CB_EVENT_T;

void hal_intf_spi_reset(uint8_t spi_channel, HAL_SPI_RESET_TYPE type);
int32_t hal_intf_spi_set_config(uint8_t spi_idx, HAL_SPI_CONF_T *spi_cfg);
int32_t hal_intf_spi_xfer_exec(uint8_t spi_idx, HAL_SPI_TRANS_FMT_T *fmt, void *tx_buf, void *rx_buf);
HAL_STATUS hal_intf_spi_register_event_callback(uint8_t spi_channel, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_spi_unregister_event_callback(uint8_t spi_channel);

uint8_t hal_intf_spi_get_cmd(uint8_t spi_channel);
HAL_STATUS hal_intf_spi_slave_set_ready(uint8_t spi_channel);
uint16_t hal_intf_spi_slave_get_recv_count(uint8_t spi_channel);
void hal_intf_spi_data_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count);
int32_t hal_intf_spi_master_write_byte(uint8_t spi_channel, uint8_t in_byte);
int32_t hal_intf_spi_master_read_byte(uint8_t spi_channel, uint8_t *out_byte);
#endif /* __HAL_INTF_SPI_H */
