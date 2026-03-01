/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_spi_api.h"
#include "util_debug_log.h"
#include <stdint.h>

#define IS_VALID_SPI_CHANNEL(channel) ((channel) < CHIP_SPI_CHANNEL_NUM)
#define IS_VALID_SPI_CLOCK(clock) ((clock) < E_SPI_CLOCK_SEL_MAX)
#define GET_SPI_MODULE(spi_channel) ((spi_channel) == 0 ? SPI0_MODULE : SPI1_MODULE)

const uint32_t SPI_CLOCK[] = {100000, 200000, 400000, 500000, 800000, 1000000, 2000000, 4000000, 5000000, 10000000};

typedef struct {
    uint8_t role;
    HAL_SPI_CONF_T cfg;
    uint8_t state;
} WISE_SPI_CTRL_T;

static WISE_SPI_CTRL_T spi_ctrl_array[CHIP_SPI_CHANNEL_NUM];

static WISE_STATUS _spi_hal_mode_setting(uint8_t channel, uint8_t spi_mode, HAL_SPI_CONF_T *spi_cfg)
{
    spi_cfg->mosi_bir_dir    = 0;
    spi_cfg->dual_quard_mode = 0;

    switch (spi_mode) {
    case SPI_MODE_REGULAR:
        spi_cfg->dual_quard_mode = 0;
        spi_cfg->mosi_bir_dir    = 0;
        break;

    case SPI_MODE_3WIRE:
        spi_cfg->dual_quard_mode = 0;
        spi_cfg->mosi_bir_dir    = 1;
        break;

    case SPI_MODE_DUAL:
        spi_cfg->dual_quard_mode = 1;
        spi_cfg->mosi_bir_dir    = 0;
        break;

    case SPI_MODE_QUAD:
        spi_cfg->dual_quard_mode = 2;
        spi_cfg->mosi_bir_dir    = 0;
        break;

    default:
        debug_printf("Unsupported SPI mode: %d\n", spi_mode);
        return WISE_INVALID_INDEX;
    }

    return WISE_SUCCESS;
}

WISE_STATUS wise_spi_register_event_callback(uint8_t spi_channel, SPI_EVT_CALLBACK_T cb, void *context)
{
    return hal_intf_spi_register_event_callback(spi_channel, cb, context);
}

WISE_STATUS wise_spi_unregister_event_callback(uint8_t spi_channel)
{
    return hal_intf_spi_unregister_event_callback(spi_channel);
}

WISE_STATUS wise_spi_init(void)
{
    for (int i = 0; i < CHIP_SPI_CHANNEL_NUM; i++) {
        spi_ctrl_array[i] = (WISE_SPI_CTRL_T){0};
    }
    return WISE_SUCCESS;
}

WISE_STATUS wise_spi_deinit(void)
{
    return WISE_SUCCESS;
}

WISE_STATUS wise_spi_master_open(uint8_t spi_channel, WISE_SPI_CONF_T *spi_conf)
{
    WISE_SPI_CTRL_T *spi = &spi_ctrl_array[spi_channel];

    if (!IS_VALID_SPI_CHANNEL(spi_channel) || (!IS_VALID_SPI_CLOCK(spi_conf->clock_sel))) {
        return WISE_INVALID_INDEX;
    }

    if (hal_intf_module_clk_is_enabled(GET_SPI_MODULE(spi_channel)) == DISABLE) {
        hal_intf_module_clk_enable(GET_SPI_MODULE(spi_channel));
    }

    spi_ctrl_array[spi_channel].state = E_SPI_STATE_ON;

    spi->role = E_SPI_ROLE_MASTER;

    spi->cfg.clock_mode     = spi_conf->clock_mode;
    spi->cfg.role           = E_SPI_ROLE_MASTER;
    spi->cfg.addr_len       = spi_conf->addr_len;
    spi->cfg.bus_clock      = SPI_CLOCK[spi_conf->clock_sel];
    spi->cfg.addr_fmt       = spi_conf->addr_fmt;
    spi->cfg.data_bit_width = spi_conf->data_bit_width;
    spi->cfg.data_merge     = spi_conf->data_merge;
    spi->cfg.bit_order      = spi_conf->bit_order;
    spi->cfg.block_mode     = spi_conf->block_mode;
    spi->cfg.dma_enable     = spi_conf->dma_enable;

    if (_spi_hal_mode_setting(spi_channel, spi_conf->spi_mode, &spi->cfg) != WISE_SUCCESS) {
        return WISE_INVALID_INDEX;
    }
    return hal_intf_spi_set_config(spi_channel, &spi->cfg);
}

WISE_STATUS wise_spi_slave_open(uint8_t spi_channel, WISE_SPI_CONF_T *spi_conf)
{
    WISE_SPI_CTRL_T *spi = &spi_ctrl_array[spi_channel];

    if (!IS_VALID_SPI_CHANNEL(spi_channel) || (!IS_VALID_SPI_CLOCK(spi_conf->clock_sel))) {
        return WISE_INVALID_INDEX;
    }

    if (hal_intf_module_clk_is_enabled(GET_SPI_MODULE(spi_channel)) == DISABLE) {
        hal_intf_module_clk_enable(GET_SPI_MODULE(spi_channel));
    }

    spi_ctrl_array[spi_channel].state = E_SPI_STATE_ON;

    spi->role = E_SPI_ROLE_SLAVE;

    spi->cfg.clock_mode     = spi_conf->clock_mode;
    spi->cfg.role           = E_SPI_ROLE_SLAVE;
    spi->cfg.addr_len       = spi_conf->addr_len;
    spi->cfg.bus_clock      = SPI_CLOCK[spi_conf->clock_sel];
    spi->cfg.addr_fmt       = spi_conf->addr_fmt;
    spi->cfg.data_bit_width = spi_conf->data_bit_width;
    spi->cfg.data_merge     = spi_conf->data_merge;
    spi->cfg.bit_order      = spi_conf->bit_order;
    spi->cfg.block_mode     = spi_conf->block_mode;
    spi->cfg.dma_enable     = spi_conf->dma_enable;

    if (_spi_hal_mode_setting(spi_channel, spi_conf->spi_mode, &spi->cfg) != WISE_SUCCESS) {
        return WISE_INVALID_INDEX;
    }
    return hal_intf_spi_set_config(spi_channel, &spi->cfg);
}

WISE_STATUS wise_spi_slave_set_ready(uint8_t spi_channel)
{
    return hal_intf_spi_slave_set_ready(spi_channel);
}

uint16_t wise_spi_slave_get_recv_count(uint8_t spi_channel)
{
    return hal_intf_spi_slave_get_recv_count(spi_channel);
}
static WISE_STATUS _wise_spi_transfer(uint8_t spi_channel, uint8_t cmd, uint8_t cmd_en, uint8_t transMode, uint16_t address, uint8_t address_en,
                                      uint8_t dummy_len, void *tx_data_buff, uint16_t tx_unit_count, void *rx_data_buff, uint16_t rx_unit_count)

{
    WISE_SPI_CTRL_T *spi    = &spi_ctrl_array[spi_channel];
    HAL_SPI_TRANS_FMT_T fmt = {0};

    if (spi_ctrl_array[spi_channel].state != E_SPI_STATE_ON) {
        debug_printf("SPI is not enabled\n");
        return WISE_FAIL;
    }

    fmt.trans_mode    = transMode;
    fmt.tx_unit_count = tx_unit_count;
    fmt.rx_unit_count = rx_unit_count;

    if (spi->role == E_SPI_ROLE_MASTER) {
        if (cmd_en == ENABLE) {
            fmt.flag_en   |= (SPI_MSG_FMT_CMD_EN);
            fmt.cmd_value  = cmd;
        }
        if (address_en == ENABLE) {
            fmt.flag_en    |= SPI_MSG_FMT_ADDR_EN;
            fmt.addr_value  = address;
        }
    } else {
        fmt.addr_value = address;
    }
    fmt.dummy_len = dummy_len;
    return hal_intf_spi_xfer_exec(spi_channel, &fmt, tx_data_buff, rx_data_buff);
}

WISE_STATUS wise_spi_close(uint8_t spi_channel)
{
    spi_ctrl_array[spi_channel].state = E_SPI_STATE_OFF;
    hal_intf_module_clk_disable(GET_SPI_MODULE(spi_channel));

    return WISE_SUCCESS;
}

uint8_t wise_spi_get_cmd(uint8_t spi_channel)
{
    return hal_intf_spi_get_cmd(spi_channel);
}

WISE_STATUS wise_spi_msg_xfer(uint8_t spi_channel, WISE_SPI_XFER_MSG_T *msg)
{
    uint8_t cmd_en  = !!(msg->msg_fmt & SPI_MSG_FMT_CMD_EN) ? 1 : 0;
    uint8_t addr_en = !!(msg->msg_fmt & SPI_MSG_FMT_ADDR_EN) ? 1 : 0;

    return _wise_spi_transfer(spi_channel, msg->cmd, cmd_en, msg->trans_mode, msg->address, addr_en, msg->dummy_len, msg->tx_data_buff,
                              msg->tx_unit_count, msg->rx_data_buff, msg->rx_unit_count);
}

WISE_STATUS wise_spi_master_write(uint8_t spi_channel, void *tx_data_buff, uint16_t tx_unit_count)
{
    WISE_SPI_XFER_MSG_T trans_msg = {0};
    trans_msg.tx_unit_count       = tx_unit_count;
    trans_msg.tx_data_buff        = tx_data_buff;
    trans_msg.trans_mode          = SPI_TM_WRITE_ONLY;

    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_spi_master_read(uint8_t spi_channel, void *rx_data_buff, uint16_t rx_unit_count)
{
    WISE_SPI_XFER_MSG_T trans_msg = {0};
    trans_msg.rx_unit_count       = rx_unit_count;
    trans_msg.rx_data_buff        = rx_data_buff;
    trans_msg.trans_mode          = SPI_TM_READ_ONLY;

    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_spi_master_write_byte(uint8_t spi_channel, uint8_t in_byte)
{
    uint8_t buff = in_byte;
    return wise_spi_master_write(spi_channel, &buff, 1);
}

WISE_STATUS wise_spi_master_read_byte(uint8_t spi_channel, uint8_t *out_byte)
{
    return wise_spi_master_read(spi_channel, out_byte, 1);
}

void wise_spi_data_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count)
{
    hal_intf_spi_data_prepare(spi_channel, tx_fifo_ptr, tx_unit_count);
}

void wise_spi_reset(uint8_t spi_channel, WISE_SPI_RESET_TYPE type)
{
    hal_intf_spi_reset(spi_channel, type);
}
