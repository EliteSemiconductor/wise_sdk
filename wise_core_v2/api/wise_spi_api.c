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

const uint32_t SPI_CLOCK[] = {100000, 200000, 400000, 500000, 800000, 1000000, 2000000, 4000000, 5000000, 10000000, 20000000};

typedef struct {
    uint8_t role;
    HAL_SPI_CONF_T cfg;
    uint8_t state;
} WISE_SPI_CTRL_T;

static WISE_SPI_CTRL_T spi_ctrl_array[CHIP_SPI_CHANNEL_NUM];

/* Common gate for every transfer entry point: channel valid, opened, and
 * opened in the role this function requires. Mismatched role returns
 * WISE_FAIL instead of poking the hardware. */
static WISE_STATUS _spi_check_opened(uint8_t spi_channel)
{
    if (!IS_VALID_SPI_CHANNEL(spi_channel)) {
        return WISE_INVALID_INDEX;
    }
    if (spi_ctrl_array[spi_channel].state != E_SPI_STATE_ON) {
        WISE_LOG_ERR("SPI ch%d is not enabled\n", spi_channel);
        return WISE_FAIL;
    }
    return WISE_SUCCESS;
}

static WISE_STATUS _spi_check_role(uint8_t spi_channel, uint8_t expect_role)
{
    WISE_STATUS status = _spi_check_opened(spi_channel);

    if (status != WISE_SUCCESS) {
        return status;
    }
    if (spi_ctrl_array[spi_channel].role != expect_role) {
        WISE_LOG_ERR("SPI ch%d role mismatch (expect %d)\n", spi_channel, expect_role);
        return WISE_FAIL;
    }
    return WISE_SUCCESS;
}

static WISE_STATUS _spi_open(uint8_t spi_channel, const WISE_SPI_CONF_T *spi_conf, uint8_t role)
{
    WISE_SPI_CTRL_T *spi = &spi_ctrl_array[spi_channel];

    if (!IS_VALID_SPI_CHANNEL(spi_channel) || (!IS_VALID_SPI_CLOCK(spi_conf->clock_sel))) {
        return WISE_INVALID_INDEX;
    }

    /* Slave mode supports SINGLE (and 3-wire) IO only. Dual/quad slave RECEIVE
     * is unreliable on this controller (multi-lane slave RX premature-ends /
     * drops leading bytes -- HW errata), so reject it at open. */
    if (role == E_SPI_ROLE_SLAVE &&
        (spi_conf->io_mode == WISE_SPI_IO_DUAL || spi_conf->io_mode == WISE_SPI_IO_QUAD)) {
        return WISE_FAIL;
    }

    if (hal_intf_module_clk_is_enabled(GET_SPI_MODULE(spi_channel)) == DISABLE) {
        hal_intf_module_clk_enable(GET_SPI_MODULE(spi_channel));
    }

    spi->state = E_SPI_STATE_ON;
    spi->role  = role;

    spi->cfg.clock_mode     = spi_conf->clock_mode;
    spi->cfg.role           = role;
    spi->cfg.bus_clock      = SPI_CLOCK[spi_conf->clock_sel];
    spi->cfg.data_bit_width = spi_conf->data_bit_width;
    spi->cfg.data_merge     = spi_conf->data_merge;
    spi->cfg.bit_order      = spi_conf->bit_order;
    spi->cfg.io_mode        = (uint8_t)spi_conf->io_mode;
    spi->cfg.block_mode     = spi_conf->block_mode;
    spi->cfg.dma_enable     = spi_conf->dma_enable;

    return hal_intf_spi_set_config(spi_channel, &spi->cfg);
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

WISE_STATUS wise_spi_master_open(uint8_t spi_channel, const WISE_SPI_CONF_T *spi_conf)
{
    return _spi_open(spi_channel, spi_conf, E_SPI_ROLE_MASTER);
}

WISE_STATUS wise_spi_slave_open(uint8_t spi_channel, const WISE_SPI_CONF_T *spi_conf)
{
    return _spi_open(spi_channel, spi_conf, E_SPI_ROLE_SLAVE);
}

WISE_STATUS wise_spi_close(uint8_t spi_channel)
{
    if (!IS_VALID_SPI_CHANNEL(spi_channel)) {
        return WISE_INVALID_INDEX;
    }
    spi_ctrl_array[spi_channel].state = E_SPI_STATE_OFF;
    hal_intf_module_clk_disable(GET_SPI_MODULE(spi_channel));

    return WISE_SUCCESS;
}

WISE_STATUS wise_spi_msg_xfer(uint8_t spi_channel, WISE_SPI_XFER_MSG_T *msg)
{
    HAL_SPI_TRANS_FMT_T fmt = {0};
    /* Role-agnostic: usable by master transfers and by slave RX arming
     * (slave listen path). Only requires the channel to be open. */
    WISE_STATUS status = _spi_check_opened(spi_channel);

    if (status != WISE_SUCCESS) {
        return status;
    }

    fmt.role          = spi_ctrl_array[spi_channel].role;
    fmt.trans_mode    = (uint8_t)msg->trans_mode;
    fmt.tx_unit_count = msg->tx_unit_count;
    fmt.rx_unit_count = msg->rx_unit_count;
    fmt.cmd_en        = (msg->cmd_en == WISE_SPI_ENABLE) ? 1 : 0;
    fmt.cmd_value     = msg->cmd;
    fmt.addr_len      = (uint8_t)msg->addr_len; /* 0 = no address phase */
    fmt.addr_fmt      = (uint8_t)msg->addr_fmt;
    fmt.addr_value    = msg->address;
    fmt.dummy_len     = msg->dummy_len;

    return hal_intf_spi_xfer_exec(spi_channel, &fmt, msg->tx_data_buff, msg->rx_data_buff);
}

WISE_STATUS wise_spi_write(uint8_t spi_channel, void *data_ptr, uint16_t len)
{
    WISE_SPI_XFER_MSG_T trans_msg = {0};
    WISE_STATUS status            = _spi_check_role(spi_channel, E_SPI_ROLE_MASTER);

    if (status != WISE_SUCCESS) {
        return status;
    }
    trans_msg.tx_unit_count = len;
    trans_msg.tx_data_buff  = data_ptr;
    trans_msg.trans_mode    = WISE_SPI_TM_WRITE_ONLY;

    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_spi_read(uint8_t spi_channel, void *data_ptr, uint16_t len)
{
    WISE_SPI_XFER_MSG_T trans_msg = {0};
    WISE_STATUS status            = _spi_check_role(spi_channel, E_SPI_ROLE_MASTER);

    if (status != WISE_SUCCESS) {
        return status;
    }
    trans_msg.rx_unit_count = len;
    trans_msg.rx_data_buff  = data_ptr;
    trans_msg.trans_mode    = WISE_SPI_TM_READ_ONLY;

    return wise_spi_msg_xfer(spi_channel, &trans_msg);
}

WISE_STATUS wise_spi_write_byte(uint8_t spi_channel, uint8_t in_byte)
{
    WISE_STATUS status = _spi_check_role(spi_channel, E_SPI_ROLE_MASTER);

    if (status != WISE_SUCCESS) {
        return status;
    }
    return hal_intf_spi_master_write_byte(spi_channel, in_byte);
}

WISE_STATUS wise_spi_read_byte(uint8_t spi_channel, uint8_t *out_byte)
{
    WISE_STATUS status = _spi_check_role(spi_channel, E_SPI_ROLE_MASTER);

    if (status != WISE_SUCCESS) {
        return status;
    }
    return hal_intf_spi_master_read_byte(spi_channel, out_byte);
}

WISE_STATUS wise_spi_slave_set_ready(uint8_t spi_channel)
{
    WISE_STATUS status = _spi_check_role(spi_channel, E_SPI_ROLE_SLAVE);

    if (status != WISE_SUCCESS) {
        return status;
    }
    return hal_intf_spi_slave_set_ready(spi_channel);
}

uint16_t wise_spi_slave_get_recv_count(uint8_t spi_channel)
{
    if (_spi_check_role(spi_channel, E_SPI_ROLE_SLAVE) != WISE_SUCCESS) {
        return 0;
    }
    return hal_intf_spi_slave_get_recv_count(spi_channel);
}

uint8_t wise_spi_slave_get_cmd(uint8_t spi_channel)
{
    if (_spi_check_role(spi_channel, E_SPI_ROLE_SLAVE) != WISE_SUCCESS) {
        return 0;
    }
    return hal_intf_spi_get_cmd(spi_channel);
}

void wise_spi_slave_prepare(uint8_t spi_channel, void *tx_fifo_ptr, uint16_t tx_unit_count)
{
    if (_spi_check_role(spi_channel, E_SPI_ROLE_SLAVE) != WISE_SUCCESS) {
        return;
    }
    hal_intf_spi_data_prepare(spi_channel, tx_fifo_ptr, tx_unit_count);
}

void wise_spi_reset(uint8_t spi_channel, WISE_SPI_RESET_TYPE type)
{
    hal_intf_spi_reset(spi_channel, type);
}
