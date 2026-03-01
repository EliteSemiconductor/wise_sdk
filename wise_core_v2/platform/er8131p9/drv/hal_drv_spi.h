/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_SPI_H
#define __HAL_DRV_SPI_H

#include "hal_intf_spi.h"
#include <stdint.h>

typedef enum {
    SPI_BLOCK_MODE = 0, /**< Blocking mode for SPI operations. */
    SPI_NONBLOCK_MODE,  /**< Non-blocking mode for SPI operations. */
} SPI_BLOCK_MODE_T;

typedef enum {
    SPI_ROLE_MASTER = 0, /**< SPI master role. */
    SPI_ROLE_SLAVE,      /**< SPI slave role. */
} HAL_SPI_ROLE;

typedef enum {
    DRV_SPI_RESET_CORE = (1 << 0),
    DRV_SPI_RESET_RX   = (1 << 1),
    DRV_SPI_RESET_TX   = (1 << 2),
    DRV_SPI_RESET_ALL  = DRV_SPI_RESET_CORE | DRV_SPI_RESET_RX | DRV_SPI_RESET_TX
} DRV_SPI_RESET_TYPE;

void hal_drv_spi_reset(uint32_t spi_index, DRV_SPI_RESET_TYPE type);
uint8_t hal_drv_spi_get_cmd(uint8_t spi_index);
void hal_drv_spi_config(uint8_t spi_idx, uint16_t clock_mode, uint8_t role, uint8_t data_bit_width, uint8_t addr_len, uint32_t bus_clock,
                        uint8_t bit_order, uint8_t data_merge, uint8_t mosi_bir_dir, uint8_t dual_quard_mode, uint8_t addr_fmt, uint8_t block_mode,
                        uint8_t dma_enable);

int32_t hal_drv_spi_transfer(uint8_t spi_idx, uint8_t role, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len, uint8_t trans_mode,
                             uint8_t flag_en, uint32_t addr_value, uint8_t cmd_value, void *tx_fifo, void *rx_fifo);

HAL_STATUS hal_drv_spi_register_event_callback(uint8_t spi_channel, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_spi_unregister_event_callback(uint8_t spi_channel);

uint8_t hal_intf_spi_get_cmd(uint8_t spi_channel);
int32_t hal_drv_spi_slave_set_ready(uint8_t spi_index);
uint16_t hal_drv_spi_slave_get_recv_count(uint8_t spi_index);
void hal_drv_spi_data_prepare(uint8_t spi_index, void *tx_fifo, uint16_t tx_unit_count);
#endif /* __HAL_DRV_SPI_H */
