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

void hal_drv_i2c_set_direction(uint8_t i2c_idx, bool dir);
void hal_drv_i2c_clear_fifo(uint8_t i2c_idx);
uint8_t hal_drv_i2c_get_data_count(uint8_t i2c_idx);
uint8_t hal_drv_i2c_get_data(uint8_t i2c_idx);
HAL_STATUS hal_drv_i2c_config(uint8_t i2c_index, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp,
                              uint8_t hddat, uint8_t scl_ratio, uint8_t scl_hi, bool dir, uint16_t target_address);
void hal_drv_i2c_set_tartget_addres(uint8_t i2c_index, uint16_t target_address);
HAL_STATUS hal_drv_i2c_transmit(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len);
HAL_STATUS hal_drv_i2c_receive(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len);
HAL_STATUS hal_drv_i2c_transmit_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len, bool en_addr, bool en_data,
                                   bool en_start, bool en_stop);
HAL_STATUS hal_drv_i2c_receive_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len, bool en_addr, bool en_data,
                                  bool en_start, bool en_stop);
HAL_STATUS hal_drv_i2c_register_event_callback(uint8_t i2c_channel, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_i2c_unregister_event_callback(uint8_t i2c_channel);

#endif /* __HAL_DRV_I2C_H */
