/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_I2C_H
#define __HAL_DRV_I2C_H

#include "hal_intf_i2c.h"
#include <stdint.h>

#define STATUS_W1C_ALL (BIT_MASK(7) << 3)
#define HAL_I2C_DEBUG_TRACE_ENABLE 0

typedef struct {
    uint8_t evt;        /* I2C_DBG_EVT_xxx */
    uint8_t tx_len;     /* xfered_tx_len snapshot */
    uint8_t rx_len;     /* debug_rx_drained_len snapshot */
    uint8_t hw_dir;     /* CTRL.Dir at event time (0=RX, 1=TX) */
    uint8_t mode;       /* ctx->mode (I2C_MODE_RX / I2C_MODE_TX) */
    uint8_t sr_pending; /* ctx->sr_write_cmpl_pending */
    uint8_t data_cnt;   /* HW FIFO byte count snapshot */
    uint8_t fifo_size;  /* ctx->fifo_size */
    uint16_t status;    /* full STS register snapshot */
} HAL_I2C_DEBUG_TRACE_T;

void hal_drv_i2c_set_direction(uint8_t i2c_idx, bool dir);
void hal_drv_i2c_clear_fifo(uint8_t i2c_idx);
uint8_t hal_drv_i2c_get_data_count(uint8_t i2c_idx);
uint8_t hal_drv_i2c_get_data(uint8_t i2c_idx);
HAL_STATUS hal_drv_i2c_get_probe_state(uint8_t i2c_idx, bool *done, bool *ack, bool *arb_lost);
HAL_STATUS hal_drv_i2c_config(uint8_t i2c_index, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp,
                              uint8_t hddat, uint8_t scl_ratio, uint16_t scl_hi, bool dir, uint16_t target_address);
void hal_drv_i2c_set_tartget_addres(uint8_t i2c_index, uint16_t target_address);
HAL_STATUS hal_drv_i2c_transmit(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len);
HAL_STATUS hal_drv_i2c_receive(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len);
HAL_STATUS hal_drv_i2c_transmit_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len, bool en_addr, bool en_data,
                                   bool en_start, bool en_stop);
HAL_STATUS hal_drv_i2c_receive_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len, bool en_addr, bool en_data,
                                  bool en_start, bool en_stop);
HAL_STATUS hal_drv_i2c_register_event_callback(uint8_t i2c_channel, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_i2c_unregister_event_callback(uint8_t i2c_channel);
void hal_drv_i2c_slave_set_tx_buf(uint8_t i2c_idx, uint8_t *buf, uint16_t len);
uint8_t hal_drv_i2c_slave_get_rx_count(uint8_t i2c_idx);
void hal_drv_i2c_debug_trace_reset(uint8_t i2c_idx);
uint8_t hal_drv_i2c_debug_trace_copy(uint8_t i2c_idx, HAL_I2C_DEBUG_TRACE_T *out, uint8_t max_count);

#endif /* __HAL_DRV_I2C_H */
