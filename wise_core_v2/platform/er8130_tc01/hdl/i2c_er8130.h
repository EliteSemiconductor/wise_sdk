/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __I2C_ER8130_H
#define __I2C_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    I2C_MODE_TX = 0,
    I2C_MODE_RX = 1,
} I2C_TRANSFER_MODE_T;

typedef struct {
    bool en_start;
    bool en_addr;
    bool en_data;
    bool en_stop;
} I2C_MSG_T;

#define I2C_DISABLE (0x0U)
#define I2C_ENABLE (0x1U)

#define I2C_SLAVE (0x0U)
#define I2C_MASTER (0x1U)

#define I2C_ADDR_7_BITS (0x0U)
#define I2C_ADDR_10_BITS (0x1U)

#define I2C_DMA_DISNABLE (0x0U)
#define I2C_DMA_ENABLE (0x1U)

typedef enum {
    I2C_ACTION_NO_ACTION        = 0x0, // no action
    I2C_ACTION_DATA_TRANSACTION = 0x1, // issue a data transaction (Master only)
    I2C_ACTION_RESPOND_ACK      = 0x2, // respond with an ACK to the received byte
    I2C_ACTION_RESPOND_NACK     = 0x3, // respond with a NACK to the received byte
    I2C_ACTION_CLEAR_FIFO       = 0x4, // clear the FIFO
    I2C_ACTION_RESET_CONTROLLER = 0x5  // reset the I2C controller
} I2C_ACTION_T;

uint8_t i2c_get_data_er8130(I2C_T *i2c);
void i2c_set_data_er8130(I2C_T *i2c, uint8_t value);
uint8_t i2c_get_fifo_size_er8130(I2C_T *i2c);
void i2c_clear_cmpl_status_er8130(I2C_T *i2c);
void i2c_enable_fifo_empty_interrupt_er8130(I2C_T *i2c);
void i2c_disable_fifo_empty_interrupt_er8130(I2C_T *i2c);
void i2c_enable_fifo_full_interrupt_er8130(I2C_T *i2c);
void i2c_disable_fifo_full_interrupt_er8130(I2C_T *i2c);
void i2c_disable_all_interrupts_er8130(I2C_T *i2c);
uint32_t i2c_get_status(I2C_T *i2c);
void i2c_set_status(I2C_T *i2c, uint32_t value);
void i2c_set_data_byte_count_er8130(I2C_T *i2c, uint8_t len);
uint8_t i2c_get_data_byte_count_er8130(I2C_T *i2c);
void i2c_config_er8130(I2C_T *i2c, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp, uint8_t hddat,
                       uint8_t scl_ratio, uint8_t scl_hi, bool dir, uint16_t target_address);

void i2c_ctrl_set_phase_atomic(__IO uint32_t *ctrl_reg, bool stop, bool data, bool addr, bool start, bool dir);
void i2c_master_xfer_config_er8130(I2C_T *i2c, bool en_stop, bool en_data, bool en_addr, bool en_start, bool dir);
HAL_STATUS i2c_transfer_er8130(I2C_T *i2c, bool role, bool dma_enable, I2C_TRANSFER_MODE_T mode);

void i2c_clear_fifo_er8130(I2C_T *i2c);
void i2c_set_target_address_er8130(I2C_T *i2c, uint16_t target_address);
void i2c_set_direction_er8130(I2C_T *i2c, bool dir);
#endif /* __I2C_ER8130_H */
