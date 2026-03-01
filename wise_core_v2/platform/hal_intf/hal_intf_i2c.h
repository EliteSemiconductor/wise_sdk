/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_I2C_H
#define __HAL_INTF_I2C_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    HAL_I2C_SPEED_STANDARD = 0,
    HAL_I2C_SPEED_FAST,
    HAL_I2C_SPEED_FAST_PLUS
} HAL_I2C_SPEED_MODE_T;

/* APB clock frequency is 40MHz */
#define SETUP_T_SUDAT_STD (0x4)
#define SETUP_T_SP_STD (0x2)
#define SETUP_T_HDDAT_STD (0x6)
#define SETUP_T_SCL_RATIO_STD (0x0)
#define SETUP_T_SCLHI_STD (194)

#define SETUP_T_SUDAT_FAST (0x0)
#define SETUP_T_SP_FAST (0x2)
#define SETUP_T_HDDAT_FAST (0x6)
#define SETUP_T_SCL_RATIO_FAST (0x1)
#define SETUP_T_SCLHI_FAST (29)

#define SETUP_T_SUDAT_FAST_P (0x0)
#define SETUP_T_SP_FAST_P (0x2)
#define SETUP_T_HDDAT_FAST_P (0x0)
#define SETUP_T_SCL_RATIO_FAST_P (0x1)
#define SETUP_T_SCLHI_FAST_P (8)

typedef enum {
    HAL_I2C_MODE_TX = 0,
    HAL_I2C_MODE_RX = 1,
} HAL_I2C_TRANSFER_MODE_T;

typedef struct {
    uint8_t idx;
    uint8_t role;
    uint8_t i2cEn;
    uint8_t dmaEn;
    uint8_t speedMode;
    uint8_t addressing;
    uint8_t dir;            /**< I2C direct */
    uint8_t target_address; /**< I2C target's address. */
    uint8_t fifo_size;
    volatile uint8_t *dataptr;
    uint8_t dataCnt;
    uint32_t data_transferred_ptr;
    uint32_t status;
} HAL_I2C_CONF_T;

typedef struct {
    bool en_start;
    bool en_addr;
    bool en_data;
    bool en_stop;
} HAL_I2C_MSG_T;

typedef enum {
    I2C_EVENT_TRANSFER_DONE,
    I2C_EVENT_ADDRESS_HIT,
    I2C_EVENT_FIFO_EMPTY,
    I2C_EVENT_FIFO_FULL,
    I2C_EVENT_TRANSFER_COMPLETE,
    I2C_MAX_EVENTS,
} I2C_CB_EVENT_T;

HAL_STATUS hal_intf_i2c_register_event_callback(uint8_t i2c_channel, EVT_CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_i2c_unregister_event_callback(uint8_t i2c_channel);

void hal_intf_i2c_set_direction(uint8_t i2c_channel, bool dir);
HAL_STATUS hal_intf_i2c_transmit(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, uint8_t *tx_fifo, uint32_t tx_len);
HAL_STATUS hal_intf_i2c_receive(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, uint8_t *rx_fifo, uint32_t rx_len);
HAL_STATUS hal_intf_i2c_transmit_ex(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, HAL_I2C_MSG_T *i2c_msg, uint8_t *tx_fifo, uint32_t tx_len);
HAL_STATUS hal_intf_i2c_receive_ex(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg, HAL_I2C_MSG_T *i2c_msg, uint8_t *rx_fifo, uint32_t rx_len);
HAL_STATUS hal_intf_i2c_config(uint8_t i2c_idx, HAL_I2C_CONF_T *i2c_cfg);
uint8_t hal_intf_i2c_get_data_count(uint8_t i2c_channel);
uint8_t hal_intf_i2c_get_data(uint8_t i2c_channel);
void hal_intf_i2c_clear_fifo(uint8_t i2c_channel);
#endif /* __HAL_INTF_I2C_H */
