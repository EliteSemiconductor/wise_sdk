/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_SPI_H
#define __HAL_INTF_SPI_H

#include "esmt_chip_specific.h"
#include "types.h"

#define SPI_TM_WRITE_ONLY 0x01
#define SPI_TM_READ_ONLY 0x02
#define SPI_TM_WRITE_READ 0x03
#define SPI_TM_READ_WRITE 0x04
#define SPI_TM_WRITE_DMY_READ 0x05
#define SPI_TM_READ_DMY_WRITE 0x06
#define SPI_TM_NONE_DATA 0x07
#define SPI_TM_DMY_WRITE 0x08
#define SPI_TM_DMY_READ 0x09

typedef struct hal_info_spi_t {
    uint8_t spi_idx;
    void *spi;
    uint8_t block_mode;
} HAL_INFO_SPI_T;

typedef struct {
    uint16_t clock_mode;
    uint8_t role;
    uint8_t data_bit_width;
    uint8_t addr_len;
    uint32_t bus_clock;
    uint8_t bit_order;
    uint8_t data_merge;
    uint8_t mosi_bir_dir;
    uint8_t dual_quard_mode;
    uint8_t addr_fmt;
    uint8_t block_mode;
    uint8_t dma_enable;
} HAL_SPI_CONF_T;

typedef struct {
    uint8_t role;
    uint16_t rx_unit_count;
    uint16_t tx_unit_count;
    uint8_t dummy_len;
    uint8_t trans_mode;
    uint8_t flag_en;
    uint32_t addr_value;
    uint8_t cmd_value;
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
#endif /* __HAL_INTF_SPI_H */
