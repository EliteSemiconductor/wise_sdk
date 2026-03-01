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
    uint8_t scl_hi;
} I2C_TIMING_T;

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

    if (get_i2c_timing(i2c_cfg->speedMode, &timing) != HAL_NO_ERR) {
        return HAL_ERR;
    }

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
