/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_spi.h"
#include "hal_intf_dma.h"
#include "drv/hal_drv_spi.h"
#include "types.h"
#include <stdint.h>

int32_t hal_intf_spi_set_config(uint8_t spi_idx, HAL_SPI_CONF_T *spi_cfg)
{
    hal_intf_spi_reset(spi_idx, HAL_SPI_RESET_ALL);

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    hal_drv_spi_config(spi_idx, spi_cfg->clock_mode, spi_cfg->role, spi_cfg->data_bit_width, spi_cfg->addr_len, spi_cfg->bus_clock,
                       spi_cfg->bit_order, spi_cfg->data_merge, spi_cfg->mosi_bir_dir, spi_cfg->dual_quard_mode, spi_cfg->addr_fmt,
                       spi_cfg->block_mode, spi_cfg->dma_enable);
#else
    hal_drv_spi_config(spi_idx, spi_cfg->clock_mode, spi_cfg->role, spi_cfg->data_bit_width, spi_cfg->addr_len, spi_cfg->bus_clock,
                       spi_cfg->bit_order, spi_cfg->data_merge, spi_cfg->mosi_bir_dir, spi_cfg->dual_quard_mode, spi_cfg->addr_fmt,
                       spi_cfg->block_mode,DISABLE);
#endif
    return HAL_NO_ERR;
}

int32_t hal_intf_spi_xfer_exec(uint8_t spi_idx, HAL_SPI_TRANS_FMT_T *fmt, void *tx_buf, void *rx_buf)
{
    if (fmt->tx_unit_count != 0 && tx_buf != NULL) {
        hal_intf_spi_reset(spi_idx, HAL_SPI_RESET_TX);
    }
    if (fmt->rx_unit_count != 0 && rx_buf != NULL) {
        hal_intf_spi_reset(spi_idx, HAL_SPI_RESET_RX);
    }
    return hal_drv_spi_transfer(spi_idx, fmt->role, fmt->rx_unit_count, fmt->tx_unit_count, fmt->dummy_len, fmt->trans_mode, fmt->flag_en,
                                fmt->addr_value, fmt->cmd_value, tx_buf, rx_buf);
}

HAL_STATUS hal_intf_spi_register_event_callback(uint8_t spi_channel, EVT_CALLBACK_T cb, void *context)
{
    return hal_drv_spi_register_event_callback(spi_channel, cb, context);
}

HAL_STATUS hal_intf_spi_unregister_event_callback(uint8_t spi_channel)
{
    return hal_drv_spi_unregister_event_callback(spi_channel);
}

uint8_t hal_intf_spi_get_cmd(uint8_t spi_channel)
{
    return hal_drv_spi_get_cmd(spi_channel);
}

HAL_STATUS hal_intf_spi_slave_set_ready(uint8_t spi_channel)
{
    return hal_drv_spi_slave_set_ready(spi_channel);
}

uint16_t hal_intf_spi_slave_get_recv_count(uint8_t spi_channel)
{
    return hal_drv_spi_slave_get_recv_count(spi_channel);
}

void hal_intf_spi_data_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count)
{
    hal_intf_spi_reset(spi_channel, HAL_SPI_RESET_TX);
    hal_drv_spi_data_prepare(spi_channel, tx_fifo_ptr, tx_unit_count);
}

void hal_intf_spi_reset(uint8_t spi_channel, HAL_SPI_RESET_TYPE type)
{
    hal_drv_spi_reset(spi_channel, type);
}
