/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_i2c_api.h"
#include "api/wise_tick_api.h"
#include "hal_intf_i2c.h"
#include "types.h"
#include <stdint.h>

#define IS_VALID_I2C_CHANNEL(channel) ((channel) < CHIP_I2C_CHANNEL_NUM)
#define GET_I2C_MODULE(i2c_channel) ((i2c_channel) == 0 ? I2C0_MODULE : I2C1_MODULE)

typedef struct {
    HAL_I2C_CONF_T cfg;
    /* Slave watchdog state */
    I2C_EVT_CALLBACK_T user_cb;
    void *user_ctx;
    uint32_t slave_timeout_ms;
    volatile uint32_t slave_addr_hit_tick;
    volatile uint32_t slave_last_done_tick;   /* tick of last TRANSFER_DONE */
    volatile uint8_t slave_bus_active;
    volatile uint8_t slave_armed;             /* set after arm_rx, cleared on TRANSFER_DONE */
    uint8_t *slave_rx_buf;
    uint32_t slave_rx_len;
} WISE_I2C_CTRL_T;

static WISE_I2C_CTRL_T i2c_ctrl_array[CHIP_I2C_CHANNEL_NUM];

static void wise_i2c_slave_recover(uint8_t i2cIntf, WISE_I2C_CTRL_T *i2c)
{
    /*
     * The entire recovery (HW reconfigure + context setup + arm) must be
     * atomic w.r.t. the I2C ISR.  Without this, the config path enables
     * IICEn + NVIC before arm_rx sets up the context — a master transaction
     * arriving in that window triggers F_AH preload, but then arm_rx's
     * CLEAR_FIFO wipes the data mid-transfer → slave returns 00 00 00 00.
     *
     * __disable_irq() (CPSID) globally masks all interrupts.  arm_rx
     * internally calls __enable_irq() after the context is fully set up,
     * so interrupts are re-enabled only when the slave is ready.
     */
    __disable_irq();

    hal_intf_i2c_config(i2cIntf, &i2c->cfg);

    if (i2c->slave_rx_buf && i2c->slave_rx_len > 0) {
        wise_i2c_slave_arm_rx(i2cIntf, i2c->slave_rx_buf, i2c->slave_rx_len);
    } else {
        __enable_irq();
    }
}

WISE_STATUS wise_i2c_init(void)
{
    for (int i = 0; i < CHIP_I2C_CHANNEL_NUM; i++) {
        i2c_ctrl_array[i] = (WISE_I2C_CTRL_T){0};
    }
    return WISE_SUCCESS;
}

WISE_STATUS wise_i2c_deinit(void)
{
    return WISE_SUCCESS;
}

WISE_STATUS wise_i2c_open(uint8_t i2c_channel)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel)) {
        return WISE_INVALID_INDEX;
    }

    return hal_intf_module_clk_enable(GET_I2C_MODULE(i2c_channel));
}

WISE_STATUS wise_i2c_close(uint8_t i2c_channel)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel)) {
        return WISE_INVALID_INDEX;
    }

    return hal_intf_module_clk_disable(GET_I2C_MODULE(i2c_channel));
}

static void wise_to_hal_config(const WISE_I2C_CONF_T *wise_config, HAL_I2C_CONF_T *hal_config)
{
    hal_config->idx                  = wise_config->i2c_idx;
    hal_config->role                 = wise_config->role;
    hal_config->i2cEn                = wise_config->i2cEn;
    hal_config->dmaEn                = wise_config->dmaEn;
    hal_config->speedMode            = wise_config->speedMode;
    hal_config->bus_hz               = wise_config->bus_hz;
    hal_config->actual_bus_hz        = wise_config->actual_bus_hz;
    hal_config->addressing           = wise_config->addressing;
    hal_config->dir                  = wise_config->dir;
    hal_config->target_address       = wise_config->target_address;
    hal_config->fifo_size            = 0;
    hal_config->dataptr              = 0;
    hal_config->dataCnt              = 0;
    hal_config->data_transferred_ptr = 0;
    hal_config->status               = 0;
}

int32_t wise_i2c_config(uint8_t i2c_channel, WISE_I2C_CONF_T *i2c_conf)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel)) {
        return WISE_INVALID_INDEX;
    }

    if (hal_intf_module_clk_is_enabled(GET_I2C_MODULE(i2c_channel)) == DISABLE) {
        hal_intf_module_clk_enable(GET_I2C_MODULE(i2c_channel));
    }
    // Initialize the selected I2C interface
    WISE_I2C_CTRL_T *i2c = &i2c_ctrl_array[i2c_channel];
    wise_to_hal_config(i2c_conf, &i2c->cfg);

    // Enable clock for the selected I2C interface
    int32_t ret = hal_intf_i2c_config(i2c_channel, &i2c->cfg);
    i2c_conf->speedMode = (I2C_SPEED_SEL_T)i2c->cfg.speedMode;
    i2c_conf->bus_hz = i2c->cfg.bus_hz;
    i2c_conf->actual_bus_hz = i2c->cfg.actual_bus_hz;
    return ret;
}

int32_t wise_i2c_set_timing_profile(uint8_t i2c_channel, uint32_t bus_hz)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel)) {
        return WISE_INVALID_INDEX;
    }

    if (hal_intf_module_clk_is_enabled(GET_I2C_MODULE(i2c_channel)) == DISABLE) {
        hal_intf_module_clk_enable(GET_I2C_MODULE(i2c_channel));
    }

    i2c_ctrl_array[i2c_channel].cfg.bus_hz = bus_hz;
    return (hal_intf_i2c_config_bus_freq(i2c_channel, &i2c_ctrl_array[i2c_channel].cfg, bus_hz) == HAL_NO_ERR) ? WISE_SUCCESS : WISE_FAIL;
}

int32_t wise_i2c_get_effective_bus_hz(uint8_t i2c_channel, uint32_t *bus_hz)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel) || bus_hz == NULL) {
        return WISE_INVALID_INDEX;
    }

    *bus_hz = i2c_ctrl_array[i2c_channel].cfg.actual_bus_hz;
    return WISE_SUCCESS;
}

void wise_i2c_set_direction(uint8_t i2c_channel, bool dir)
{
    hal_intf_i2c_set_direction(i2c_channel, dir);
}
//----------
/*
 * Internal callback wrapper for slave watchdog.
 * Intercepts ADDRESS_HIT / TRANSFER_DONE to track bus-active state,
 * then forwards the event to the user's callback.
 */
static void _i2c_slave_watchdog_cb(uint8_t ch, uint32_t event, void *ctx)
{
    (void)ctx;
    WISE_I2C_CTRL_T *i2c = &i2c_ctrl_array[ch];

    if (event == WISE_I2C_EVENT_ADDRESS_HIT) {
        i2c->slave_bus_active = 1;
        i2c->slave_addr_hit_tick = wise_tick_get_counter();
    }
    if (event == WISE_I2C_EVENT_TRANSFER_DONE) {
        i2c->slave_bus_active = 0;
        i2c->slave_last_done_tick = wise_tick_get_counter();
    }

    if (i2c->user_cb) {
        i2c->user_cb(ch, event, i2c->user_ctx);
    }
}

WISE_STATUS wise_i2c_register_event_callback(uint8_t i2c_channel, I2C_EVT_CALLBACK_T cb, void *context)
{
    if (!IS_VALID_I2C_CHANNEL(i2c_channel)) {
        return WISE_FAIL;
    }
    i2c_ctrl_array[i2c_channel].user_cb  = cb;
    i2c_ctrl_array[i2c_channel].user_ctx = context;
    return hal_intf_i2c_register_event_callback(i2c_channel, _i2c_slave_watchdog_cb, NULL);
}

WISE_STATUS wise_i2c_unregister_event_callback(uint8_t i2c_channel)
{
    if (IS_VALID_I2C_CHANNEL(i2c_channel)) {
        i2c_ctrl_array[i2c_channel].user_cb  = NULL;
        i2c_ctrl_array[i2c_channel].user_ctx = NULL;
    }
    return hal_intf_i2c_unregister_event_callback(i2c_channel);
}

/* ========================================================================== */
/*  Static Helpers                                                            */
/* ========================================================================== */
static inline WISE_STATUS _wise_i2c_xfer_core(uint8_t i2cIntf, bool set_addr, WISE_I2C_MSG_T *i2c_msg)

{
    if (i2c_msg->buf_ptr == NULL) {
        return WISE_INVALID_INDEX;
    }
    if (i2c_msg->data_count > CHIP_I2C_MAX_XFER) {
        return WISE_INVALID_INDEX;
    }

    WISE_I2C_CTRL_T *i2c = &i2c_ctrl_array[i2cIntf];
    if (set_addr) {
        i2c->cfg.target_address = i2c_msg->target_address;
    }

    HAL_I2C_MSG_T hal_i2c_msg = {
        .en_stop = i2c_msg->en_stop, .en_data = i2c_msg->en_data, .en_addr = i2c_msg->en_addr, .en_start = i2c_msg->en_start};

    HAL_STATUS ret;
    if (i2c_msg->dir == WISE_I2C_DIR_TX) {
        ret = hal_intf_i2c_transmit_ex(i2cIntf, &i2c->cfg, &hal_i2c_msg, i2c_msg->buf_ptr, i2c_msg->data_count);
    } else {
        ret = hal_intf_i2c_receive_ex(i2cIntf, &i2c->cfg, &hal_i2c_msg, i2c_msg->buf_ptr, i2c_msg->data_count);
    }

    return (ret == HAL_NO_ERR) ? WISE_SUCCESS : WISE_FAIL;
}

WISE_STATUS wise_i2c_send_nbyte(uint8_t i2cIntf, uint8_t *bufptr, uint32_t data_count)
{
    WISE_I2C_MSG_T i2c_msg = {.buf_ptr    = bufptr,
                              .data_count = data_count,
                              .dir        = WISE_I2C_DIR_TX,
                              .en_start   = ENABLE,
                              .en_addr    = ENABLE,
                              .en_data    = ENABLE,
                              .en_stop    = ENABLE};

    return _wise_i2c_xfer_core(i2cIntf, false, &i2c_msg);
}

WISE_STATUS wise_i2c_send_nbyte_with_addr(uint8_t i2cIntf, uint16_t address, uint8_t *bufptr, uint32_t data_count)
{
    WISE_I2C_MSG_T i2c_msg = {.buf_ptr        = bufptr,
                              .data_count     = data_count,
                              .dir            = WISE_I2C_DIR_TX,
                              .target_address = address,
                              .en_start       = ENABLE,
                              .en_addr        = ENABLE,
                              .en_data        = ENABLE,
                              .en_stop        = ENABLE};

    return _wise_i2c_xfer_core(i2cIntf, true, &i2c_msg);
}

WISE_STATUS wise_i2c_send_nbyte_with_nonstop(uint8_t i2cIntf, uint16_t address, uint8_t *bufptr, uint32_t data_count)
{
    WISE_I2C_MSG_T i2c_msg = {.buf_ptr        = bufptr,
                              .data_count     = data_count,
                              .dir            = WISE_I2C_DIR_TX,
                              .target_address = address,
                              .en_start       = ENABLE,
                              .en_addr        = ENABLE,
                              .en_data        = ENABLE,
                              .en_stop        = DISABLE};

    return _wise_i2c_xfer_core(i2cIntf, true, &i2c_msg);
}

WISE_STATUS wise_i2c_recv_nbyte(uint8_t i2cIntf, uint8_t *bufptr, uint32_t data_count)
{
    WISE_I2C_MSG_T i2c_msg = {.buf_ptr    = bufptr,
                              .data_count = data_count,
                              .dir        = WISE_I2C_DIR_RX,
                              .en_start   = ENABLE,
                              .en_addr    = ENABLE,
                              .en_data    = ENABLE,
                              .en_stop    = ENABLE};

    return _wise_i2c_xfer_core(i2cIntf, false, &i2c_msg);
}

WISE_STATUS wise_i2c_recv_nbyte_with_addr(uint8_t i2cIntf, uint16_t address, uint8_t *bufptr, uint32_t data_count)
{
    WISE_I2C_MSG_T i2c_msg = {.buf_ptr        = bufptr,
                              .data_count     = data_count,
                              .dir            = WISE_I2C_DIR_RX,
                              .target_address = address,
                              .en_start       = ENABLE,
                              .en_addr        = ENABLE,
                              .en_data        = ENABLE,
                              .en_stop        = ENABLE};

    return _wise_i2c_xfer_core(i2cIntf, true, &i2c_msg);
}

uint8_t wise_i2c_get_data_count(uint8_t i2cIntf)
{
    return hal_intf_i2c_get_data_count(i2cIntf);
}

uint8_t wise_i2c_get_data(uint8_t i2cIntf)
{
    return hal_intf_i2c_get_data(i2cIntf);
}

void wise_i2c_clear_fifo(uint8_t i2cIntf)
{
    hal_intf_i2c_clear_fifo(i2cIntf);
}

int32_t wise_i2c_transfer(uint8_t intf, const WISE_I2C_XFER_MSG_T *msgs, size_t nmsgs, uint32_t timeout_ms)
{
    if (!msgs || nmsgs == 0) {
        return WISE_FAIL;
    }
    if (!IS_VALID_I2C_CHANNEL(intf)) {
        return WISE_INVALID_INDEX;
    }

    for (size_t i = 0; i < nmsgs; ++i) {
        if (!msgs[i].buf || msgs[i].len == 0) {
            return WISE_INVALID_INDEX;
        }
        if (msgs[i].len > CHIP_I2C_MAX_XFER) {
            return WISE_INVALID_INDEX;
        }
    }

    WISE_I2C_CTRL_T *i2c     = &i2c_ctrl_array[intf];
    uint8_t saved_addressing = i2c->cfg.addressing;

    WISE_STATUS st = WISE_SUCCESS;

    for (size_t i = 0; i < nmsgs; ++i) {
        const WISE_I2C_XFER_MSG_T *m = &msgs[i];

        i2c->cfg.addressing = (m->flags & WISE_I2C_M_TEN) ? I2C_ADDR_10_BITS : I2C_ADDR_7_BITS;

        bool is_read = (m->flags & WISE_I2C_M_RD) != 0;
        bool is_last = (i == (nmsgs - 1));
        bool no_stop = (!is_last) || (m->flags & WISE_I2C_M_NO_STOP);

        WISE_I2C_MSG_T low = {.buf_ptr        = m->buf,
                              .data_count     = m->len,
                              .dir            = is_read ? WISE_I2C_DIR_RX : WISE_I2C_DIR_TX,
                              .target_address = m->addr,
                              .en_start       = ENABLE,
                              .en_addr        = ENABLE,
                              .en_data        = ENABLE,
                              .en_stop        = no_stop ? DISABLE : ENABLE};

        st = _wise_i2c_xfer_core(intf, /*set_addr=*/true, &low);
        if (st != WISE_SUCCESS) {
            break;
        }

        /*
         * If timeout_ms > 0, wait for segment completion before proceeding
         * to the next message.  This ensures proper repeated-start semantics
         * for combined transactions (e.g. mem_read: write reg + read data).
         *
         * If timeout_ms == 0, fire-and-forget (caller handles completion,
         * e.g. wise_i2c_probe does its own polling loop).
         */
        if (timeout_ms > 0) {
            bool done = false, ack = false, arb_lost = false;
            uint32_t elapsed;
            for (elapsed = 0; elapsed <= timeout_ms; ++elapsed) {
                if (hal_intf_i2c_get_probe_state(intf, &done, &ack, &arb_lost) != HAL_NO_ERR) {
                    st = WISE_FAIL;
                    break;
                }
                if (arb_lost) {
                    st = WISE_FAIL;
                    break;
                }
                if (done) {
                    break;
                }
                wise_tick_delay_ms(1);
            }
            if (st != WISE_SUCCESS) {
                break;
            }
            if (!done) {
                st = WISE_FAIL; /* timeout */
                break;
            }
        }
    }

    i2c->cfg.addressing = saved_addressing;

    return (st == WISE_SUCCESS) ? WISE_SUCCESS : WISE_FAIL;
}

int32_t wise_i2c_probe(uint8_t intf, uint16_t addr, uint32_t timeout_ms)
{
    bool done = false;
    bool ack = false;
    bool arb_lost = false;

    if (!IS_VALID_I2C_CHANNEL(intf)) {
        return WISE_INVALID_INDEX;
    }

    uint8_t dummy = 0;
    WISE_I2C_XFER_MSG_T msg = {
        .addr = addr,
        .flags = 0,
        .buf = &dummy,
        .len = 1,
    };

    if (wise_i2c_transfer(intf, &msg, 1, 0) != WISE_SUCCESS) {
        return WISE_FAIL;
    }

    for (uint32_t elapsed = 0; elapsed <= timeout_ms; ++elapsed) {
        if (hal_intf_i2c_get_probe_state(intf, &done, &ack, &arb_lost) != HAL_NO_ERR) {
            return WISE_FAIL;
        }
        if (arb_lost) {
            return WISE_FAIL;
        }
        if (done) {
            return ack ? WISE_SUCCESS : WISE_FAIL;
        }
        wise_tick_delay_ms(1);
    }

    return WISE_FAIL;
}

int32_t wise_i2c_mem_read(uint8_t intf, uint16_t addr, uint32_t reg, uint8_t reg_len, uint8_t *rx, uint16_t count, uint32_t timeout_ms)
{
    if (!rx || !count || reg_len == 0 || reg_len > 4) {
        return WISE_INVALID_INDEX;
    }

    uint8_t hdr[4];
    /* MSB-first */
    for (uint8_t i = 0; i < reg_len; ++i) {
        hdr[i] = (uint8_t)((reg >> (8 * (reg_len - 1 - i))) & 0xFF);
    }

    WISE_I2C_XFER_MSG_T seq[2] = {
        {.addr = addr, .flags = 0, .buf = hdr, .len = reg_len},         /* SLA+W + reg bytes */
        {.addr = addr, .flags = WISE_I2C_M_RD, .buf = rx, .len = count} /* RESTART + SLA+R + read */
    };
    return wise_i2c_transfer(intf, seq, 2, timeout_ms);
}

/* ===== Slave convenience APIs ===== */

void wise_i2c_slave_set_tx_buf(uint8_t i2cIntf, uint8_t *buf, uint16_t len)
{
    hal_intf_i2c_slave_set_tx_buf(i2cIntf, buf, len);
}

WISE_STATUS wise_i2c_slave_arm_rx(uint8_t i2cIntf, uint8_t *buf, uint32_t len)
{
    if (IS_VALID_I2C_CHANNEL(i2cIntf)) {
        i2c_ctrl_array[i2cIntf].slave_rx_buf = buf;
        i2c_ctrl_array[i2cIntf].slave_rx_len = len;
        i2c_ctrl_array[i2cIntf].slave_armed  = 1;
        /* Do NOT set slave_last_done_tick here — idle watchdog should only
         * trigger after a transaction has started (ADDRESS_HIT) but never
         * completed.  Setting it on arm causes false timeouts during idle. */
    }
    /*
     * Do NOT touch hardware (Dir, FIFO) here — the IRQ from the previous
     * transfer may still be enabled.  All hardware setup is done atomically
     * inside hal_drv_i2c_transfer → i2c_transfer_er8130, which disables
     * the NVIC IRQ first, clears status/FIFO, then re-enables.
     */
    return wise_i2c_recv_nbyte(i2cIntf, buf, len);
}

uint8_t wise_i2c_slave_get_rx_count(uint8_t i2cIntf)
{
    return hal_intf_i2c_slave_get_rx_count(i2cIntf);
}

int32_t wise_i2c_mem_write(uint8_t intf, uint16_t addr, uint32_t reg, uint8_t reg_len, const uint8_t *tx, uint16_t count, uint32_t timeout_ms)
{
    if (!tx || !count || reg_len == 0 || reg_len > 4) {
        return WISE_INVALID_INDEX;
    }

    uint8_t tmp[4 + 255];
    if ((uint32_t)reg_len + count > sizeof(tmp)) {
        return WISE_INVALID_INDEX;
    }

    for (uint8_t i = 0; i < reg_len; ++i) {
        tmp[i] = (uint8_t)((reg >> (8 * (reg_len - 1 - i))) & 0xFF);
    }
    memcpy(&tmp[reg_len], tx, count);

    WISE_I2C_XFER_MSG_T m = {.addr = addr, .flags = 0, .buf = tmp, .len = (uint16_t)(reg_len + count)};
    return wise_i2c_transfer(intf, &m, 1, timeout_ms);
}

void wise_i2c_slave_set_timeout(uint8_t i2cIntf, uint32_t timeout_ms)
{
    if (IS_VALID_I2C_CHANNEL(i2cIntf)) {
        i2c_ctrl_array[i2cIntf].slave_timeout_ms = timeout_ms;
    }
}

int32_t wise_i2c_slave_proc(uint8_t i2cIntf)
{
    if (!IS_VALID_I2C_CHANNEL(i2cIntf)) {
        return 0;
    }

    WISE_I2C_CTRL_T *i2c = &i2c_ctrl_array[i2cIntf];

    if (i2c->slave_timeout_ms == 0) {
        return 0;
    }

    uint32_t now = wise_tick_get_counter();
    bool need_recovery = false;

    if (i2c->slave_bus_active) {
        /* ADDRESS_HIT fired but TRANSFER_DONE never came */
        if ((now - i2c->slave_addr_hit_tick) >= MS_TO_CLK(i2c->slave_timeout_ms)) {
            need_recovery = true;
        }
    }

    /*
     * Idle watchdog: if slave completed a transaction (last_done_tick != 0)
     * but hasn't seen another TRANSFER_DONE for 2×timeout, the I2C
     * peripheral may be in a bad state.  Force full re-initialization.
     * Note: last_done_tick is only set on TRANSFER_DONE, NOT on arm,
     * so this won't trigger during normal idle waiting.
     */
    if (!need_recovery && i2c->slave_armed && !i2c->slave_bus_active
        && i2c->slave_last_done_tick != 0) {
        if ((now - i2c->slave_last_done_tick) >= MS_TO_CLK(i2c->slave_timeout_ms * 2)) {
            need_recovery = true;
        }
    }

    if (!need_recovery) {
        return 0;
    }

    /* Timeout — reconfigure hardware and re-arm slave RX */
    i2c->slave_bus_active = 0;
    i2c->slave_armed = 0;
    i2c->slave_last_done_tick = 0;  /* prevent repeated idle-timeout recovery */
    wise_i2c_slave_recover(i2cIntf, i2c);

    return 1;
}
