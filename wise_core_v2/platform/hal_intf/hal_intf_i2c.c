/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_i2c.h"
#include "hal_intf_dma.h"
#include "drv/hal_drv_i2c.h"
#include <stdint.h>

typedef struct {
    uint8_t sudat;
    uint8_t sp;
    uint8_t hddat;
    uint8_t scl_ratio;
    uint16_t scl_hi;
} I2C_TIMING_T;

typedef struct {
    uint32_t min_high_ns;
    uint32_t min_low_ns;
    uint8_t sudat;
    uint8_t sp;
    uint8_t hddat;
    uint8_t speed_mode;
} I2C_TIMING_SPEC_T;

static uint32_t get_i2c_actual_bus_hz(const I2C_TIMING_T *timing)
{
    uint32_t ratio_mul;
    uint32_t period_cycles;

    if (timing == NULL) {
        return 0u;
    }

    ratio_mul = (timing->scl_ratio == 0u) ? 1u : 2u;
    period_cycles = 8u + (2u * timing->sp) + timing->scl_hi + (timing->scl_hi * ratio_mul);
    if (period_cycles == 0u) {
        return 0u;
    }

    return HAL_I2C_APB_CLK_HZ / period_cycles;
}

static uint32_t get_i2c_preset_target_hz(HAL_I2C_SPEED_MODE_T speed_mode)
{
    switch (speed_mode) {
    case HAL_I2C_SPEED_STANDARD:
        return 100000u;
    case HAL_I2C_SPEED_FAST:
        return 400000u;
    case HAL_I2C_SPEED_FAST_PLUS:
        return 1000000u;
    default:
        return 0u;
    }
}

static uint32_t ceil_div_u32(uint32_t n, uint32_t d)
{
    return (n + d - 1u) / d;
}

static void get_i2c_timing_spec(uint32_t bus_hz, I2C_TIMING_SPEC_T *spec)
{
    if (bus_hz <= 100000u) {
        spec->min_high_ns = 4000u;
        spec->min_low_ns  = 4700u;
        spec->sudat       = SETUP_T_SUDAT_STD;
        spec->sp          = SETUP_T_SP_STD;
        spec->hddat       = SETUP_T_HDDAT_STD;
        spec->speed_mode  = HAL_I2C_SPEED_STANDARD;
        return;
    }

    if (bus_hz <= 400000u) {
        spec->min_high_ns = 600u;
        spec->min_low_ns  = 1300u;
        spec->sudat       = SETUP_T_SUDAT_FAST;
        spec->sp          = SETUP_T_SP_FAST;
        spec->hddat       = SETUP_T_HDDAT_FAST;
        spec->speed_mode  = HAL_I2C_SPEED_FAST;
        return;
    }

    spec->min_high_ns = 260u;
    spec->min_low_ns  = 500u;
    spec->sudat       = SETUP_T_SUDAT_FAST_P;
    spec->sp          = SETUP_T_SP_FAST_P;
    spec->hddat       = SETUP_T_HDDAT_FAST_P;
    spec->speed_mode  = HAL_I2C_SPEED_FAST_PLUS;
}

static HAL_STATUS get_i2c_custom_timing(uint32_t bus_hz, I2C_TIMING_T *timing, uint8_t *speed_mode, uint32_t *actual_bus_hz)
{
    I2C_TIMING_SPEC_T spec = {0};
    uint32_t tpclk_ns;
    uint32_t target_cycles;
    uint32_t best_actual_hz = 0;
    bool found = false;

    if (timing == NULL || speed_mode == NULL || actual_bus_hz == NULL || bus_hz == 0u || bus_hz > HAL_I2C_MAX_BUS_FREQ_HZ) {
        return HAL_ERR;
    }

    get_i2c_timing_spec(bus_hz, &spec);

    tpclk_ns      = 1000000000u / HAL_I2C_APB_CLK_HZ;
    target_cycles = ceil_div_u32(HAL_I2C_APB_CLK_HZ, bus_hz);

    for (uint8_t ratio_sel = 0; ratio_sel <= 1; ++ratio_sel) {
        uint32_t ratio_mul = (ratio_sel == 0u) ? 1u : 2u;
        uint32_t overhead  = 8u + (2u * spec.sp);
        uint32_t min_hi    = 0u;
        uint32_t min_lo    = 0u;
        uint32_t min_scl_hi;
        uint32_t scl_hi;
        uint32_t period_cycles;
        uint32_t actual_hz;

        if (target_cycles <= overhead) {
            scl_hi = 0u;
        } else {
            scl_hi = ceil_div_u32(target_cycles - overhead, 1u + ratio_mul);
        }

        min_hi = ceil_div_u32(spec.min_high_ns, tpclk_ns);
        min_lo = ceil_div_u32(spec.min_low_ns, tpclk_ns);

        if (min_hi > (4u + spec.sp)) {
            min_hi -= (4u + spec.sp);
        } else {
            min_hi = 0u;
        }

        if (min_lo > (4u + spec.sp)) {
            min_lo = ceil_div_u32(min_lo - (4u + spec.sp), ratio_mul);
        } else {
            min_lo = 0u;
        }

        min_scl_hi = min_hi;
        if (min_lo > min_scl_hi) {
            min_scl_hi = min_lo;
        }
        if (min_scl_hi <= spec.sp) {
            min_scl_hi = (uint32_t)spec.sp + 1u;
        }
        if (min_scl_hi <= spec.hddat) {
            min_scl_hi = (uint32_t)spec.hddat + 1u;
        }

        if (scl_hi < min_scl_hi) {
            scl_hi = min_scl_hi;
        }
        if (scl_hi > 0x1FFu) {
            continue;
        }

        period_cycles = overhead + scl_hi + (scl_hi * ratio_mul);
        actual_hz     = HAL_I2C_APB_CLK_HZ / period_cycles;

        if (!found || actual_hz > best_actual_hz) {
            timing->sudat     = spec.sudat;
            timing->sp        = spec.sp;
            timing->hddat     = spec.hddat;
            timing->scl_ratio = ratio_sel;
            timing->scl_hi    = (uint16_t)scl_hi;
            *speed_mode       = spec.speed_mode;
            best_actual_hz    = actual_hz;
            *actual_bus_hz    = actual_hz;
            found             = true;
        }
    }

    return found ? HAL_NO_ERR : HAL_ERR;
}

void hal_intf_i2c_clear_fifo(uint8_t i2c_channel)
{
    hal_drv_i2c_clear_fifo(i2c_channel);
}
uint8_t hal_intf_i2c_get_data_count(uint8_t i2c_channel)
{
    return hal_drv_i2c_get_data_count(i2c_channel);
}

void hal_intf_i2c_set_direction(uint8_t i2c_channel, bool dir)
{
    hal_drv_i2c_set_direction(i2c_channel, dir);
}

uint8_t hal_intf_i2c_get_data(uint8_t i2c_channel)
{
    return hal_drv_i2c_get_data(i2c_channel);
}

HAL_STATUS hal_intf_i2c_get_probe_state(uint8_t i2c_channel, bool *done, bool *ack, bool *arb_lost)
{
    return hal_drv_i2c_get_probe_state(i2c_channel, done, ack, arb_lost);
}

HAL_STATUS hal_intf_i2c_register_event_callback(uint8_t i2c_channel, EVT_CALLBACK_T cb, void *context)
{
    return hal_drv_i2c_register_event_callback(i2c_channel, cb, context);
}

HAL_STATUS hal_intf_i2c_unregister_event_callback(uint8_t i2c_channel)
{
    return hal_drv_i2c_unregister_event_callback(i2c_channel);
}
static HAL_STATUS get_i2c_timing(HAL_I2C_SPEED_MODE_T speed_mode, I2C_TIMING_T *timing)
{
    if (timing == NULL) {
        return HAL_ERR;
    }

    switch (speed_mode) {
    case HAL_I2C_SPEED_STANDARD:
        timing->sudat     = SETUP_T_SUDAT_STD;
        timing->sp        = SETUP_T_SP_STD;
        timing->hddat     = SETUP_T_HDDAT_STD;
        timing->scl_ratio = SETUP_T_SCL_RATIO_STD;
        timing->scl_hi    = SETUP_T_SCLHI_STD;
        break;
    case HAL_I2C_SPEED_FAST:
        timing->sudat     = SETUP_T_SUDAT_FAST;
        timing->sp        = SETUP_T_SP_FAST;
        timing->hddat     = SETUP_T_HDDAT_FAST;
        timing->scl_ratio = SETUP_T_SCL_RATIO_FAST;
        timing->scl_hi    = SETUP_T_SCLHI_FAST;
        break;
    case HAL_I2C_SPEED_FAST_PLUS:
        timing->sudat     = SETUP_T_SUDAT_FAST_P;
        timing->sp        = SETUP_T_SP_FAST_P;
        timing->hddat     = SETUP_T_HDDAT_FAST_P;
        timing->scl_ratio = SETUP_T_SCL_RATIO_FAST_P;
        timing->scl_hi    = SETUP_T_SCLHI_FAST_P;
        break;
    default:
        return HAL_ERR;
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_i2c_config(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg)
{
    I2C_TIMING_T timing = {0};

    if (i2c_cfg->bus_hz != 0u) {
        uint8_t speed_mode = 0;
        if (get_i2c_custom_timing(i2c_cfg->bus_hz, &timing, &speed_mode, &i2c_cfg->actual_bus_hz) != HAL_NO_ERR) {
            return HAL_ERR;
        }
        i2c_cfg->speedMode = speed_mode;
    } else {
        if (get_i2c_timing(i2c_cfg->speedMode, &timing) != HAL_NO_ERR) {
            return HAL_ERR;
        }
        i2c_cfg->bus_hz = get_i2c_preset_target_hz(i2c_cfg->speedMode);
        i2c_cfg->actual_bus_hz = get_i2c_actual_bus_hz(&timing);
    }

    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_init();
    }
    return hal_drv_i2c_config(i2c_idx, i2c_cfg->i2cEn, i2c_cfg->role, i2c_cfg->addressing, i2c_cfg->dmaEn, timing.sudat, timing.sp, timing.hddat,
                              timing.scl_ratio, timing.scl_hi, i2c_cfg->dir, i2c_cfg->target_address);
}

HAL_STATUS hal_intf_i2c_config_bus_freq(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, uint32_t bus_hz)
{
    I2C_TIMING_T timing = {0};
    uint8_t speed_mode = 0;

    if (get_i2c_custom_timing(bus_hz, &timing, &speed_mode, &i2c_cfg->actual_bus_hz) != HAL_NO_ERR) {
        return HAL_ERR;
    }

    i2c_cfg->speedMode = speed_mode;
    i2c_cfg->bus_hz = bus_hz;

    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_init();
    }

    return hal_drv_i2c_config(i2c_idx, i2c_cfg->i2cEn, i2c_cfg->role, i2c_cfg->addressing, i2c_cfg->dmaEn, timing.sudat, timing.sp, timing.hddat,
                              timing.scl_ratio, timing.scl_hi, i2c_cfg->dir, i2c_cfg->target_address);
}

HAL_STATUS hal_intf_i2c_transmit(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, uint8_t *tx_fifo, uint32_t tx_len)
{
    hal_drv_i2c_set_tartget_addres(i2c_idx, i2c_cfg->target_address);

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_update(i2c_idx, tx_fifo, tx_len, NULL, 0);
        hal_intf_i2c_dma_trigger(i2c_idx, tx_fifo, tx_len, NULL, 0);
    }
#endif //CHIP_DMA_SUPPORT_PERIPHERAL

    return hal_drv_i2c_transmit(i2c_idx, i2c_cfg->role, i2c_cfg->dmaEn, tx_fifo, tx_len);
}

HAL_STATUS hal_intf_i2c_receive(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, uint8_t *rx_fifo, uint32_t rx_len)
{
    hal_drv_i2c_set_tartget_addres(i2c_idx, i2c_cfg->target_address);

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_update(i2c_idx, NULL, 0, rx_fifo, rx_len);
        hal_intf_i2c_dma_trigger(i2c_idx, NULL, 0, rx_fifo, rx_len);
    }
#endif //CHIP_DMA_SUPPORT_PERIPHERAL

    return hal_drv_i2c_receive(i2c_idx, i2c_cfg->role, i2c_cfg->dmaEn, rx_fifo, rx_len);
}

HAL_STATUS hal_intf_i2c_transmit_ex(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, HAL_I2C_MSG_T *i2c_msg, uint8_t *tx_fifo, uint32_t tx_len)
{
    hal_drv_i2c_set_tartget_addres(i2c_idx, i2c_cfg->target_address);

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_update(i2c_idx, tx_fifo, tx_len, NULL, 0);
        hal_intf_i2c_dma_trigger(i2c_idx, tx_fifo, tx_len, NULL, 0);
    }
#endif //CHIP_DMA_SUPPORT_PERIPHERAL

    return hal_drv_i2c_transmit_ex(i2c_idx, i2c_cfg->role, i2c_cfg->dmaEn, tx_fifo, tx_len, i2c_msg->en_addr, i2c_msg->en_data, i2c_msg->en_start,
                                   i2c_msg->en_stop);
}

HAL_STATUS hal_intf_i2c_receive_ex(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, HAL_I2C_MSG_T *i2c_msg, uint8_t *rx_fifo, uint32_t rx_len)
{
    hal_drv_i2c_set_tartget_addres(i2c_idx, i2c_cfg->target_address);

#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    if (i2c_cfg->dmaEn) {
        hal_intf_i2c_dma_update(i2c_idx, NULL, 0, rx_fifo, rx_len);
        hal_intf_i2c_dma_trigger(i2c_idx, NULL, 0, rx_fifo, rx_len);
    }
#endif //CHIP_DMA_SUPPORT_PERIPHERAL

    return hal_drv_i2c_receive_ex(i2c_idx, i2c_cfg->role, i2c_cfg->dmaEn, rx_fifo, rx_len, i2c_msg->en_addr, i2c_msg->en_data, i2c_msg->en_start,
                                  i2c_msg->en_stop);
}

void hal_intf_i2c_slave_set_tx_buf(uint8_t i2c_channel, uint8_t *buf, uint16_t len)
{
    hal_drv_i2c_slave_set_tx_buf(i2c_channel, buf, len);
}

uint8_t hal_intf_i2c_slave_get_rx_count(uint8_t i2c_channel)
{
    return hal_drv_i2c_slave_get_rx_count(i2c_channel);
}
