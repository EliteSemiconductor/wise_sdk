/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/i2c_er8130.h"
#include <stdint.h>

static inline void i2c_cmd_action_er8130(I2C_T *i2c, uint32_t action)
{
    i2c->CMD.bitfield.CMD = action;
}

static void i2c_enable_master_transmit_interrupts_er8130(I2C_T *i2c, bool dma_enable)
{
    i2c->INT_EN.bitfield.Cmpl      = 1;
    i2c->INT_EN.bitfield.FIFOEmpty = 0;

    if (!dma_enable) {
        i2c->INT_EN.bitfield.FIFOEmpty = 1;
    }
}

static void i2c_enable_master_receiver_interrupts_er8130(I2C_T *i2c, bool dma_enable)
{
    i2c->INT_EN.bitfield.Cmpl     = 1;
    i2c->INT_EN.bitfield.FIFOFull = 0;

    if (!dma_enable) {
        i2c->INT_EN.bitfield.FIFOFull = 1;
    }
}

static void i2c_enable_slave_transfer_interrupts_er8130(I2C_T *i2c, bool dma_enable)
{
    i2c->INT_EN.bitfield.Cmpl    = 1;
    i2c->INT_EN.bitfield.AddrHit = 1;
}

uint8_t i2c_get_data_er8130(I2C_T *i2c)
{
    return i2c->DATA.bitfield.DATAFIELD;
}

void i2c_set_data_er8130(I2C_T *i2c, uint8_t value)
{
    i2c->DATA.bitfield.DATAFIELD = value;
}

void i2c_set_data_byte_count_er8130(I2C_T *i2c, uint8_t value)
{
    i2c->CTRL.bitfield.DataCnt = value;
}

uint8_t i2c_get_data_byte_count_er8130(I2C_T *i2c)
{
    return i2c->CTRL.bitfield.DataCnt;
}

uint8_t i2c_get_fifo_size_er8130(I2C_T *i2c)
{
    uint32_t fifo_size = i2c->CFG.bitfield.FIFOSize;
    return ((fifo_size == 0) ? 2 : (fifo_size == 1) ? 4 : (fifo_size == 2) ? 8 : 16);
}

void i2c_clear_cmpl_status_er8130(I2C_T *i2c)
{
    i2c->STS.bitfield.Cmpl = 0;
}

void i2c_enable_fifo_empty_interrupt_er8130(I2C_T *i2c)
{
    i2c->INT_EN.bitfield.FIFOEmpty = 1;
}

void i2c_disable_fifo_empty_interrupt_er8130(I2C_T *i2c)
{
    i2c->INT_EN.bitfield.FIFOEmpty = 0;
}

void i2c_enable_fifo_full_interrupt_er8130(I2C_T *i2c)
{
    i2c->INT_EN.bitfield.FIFOFull = 1;
}

void i2c_disable_fifo_full_interrupt_er8130(I2C_T *i2c)
{
    i2c->INT_EN.bitfield.FIFOFull = 0;
}

void i2c_disable_all_interrupts_er8130(I2C_T *i2c)
{
    i2c->INT_EN.reg &= ~0x3FF;
}

uint32_t i2c_get_status(I2C_T *i2c)
{
    return i2c->STS.reg;
}

void i2c_set_status(I2C_T *i2c, uint32_t value)
{
    i2c->STS.reg = value;
}

void i2c_clear_fifo_er8130(I2C_T *i2c)
{
    i2c_cmd_action_er8130(i2c, I2C_ACTION_CLEAR_FIFO);
}

static inline uint32_t i2c_ctrl_pack_phase(bool stop, bool data, bool addr, bool start, bool dir)
{
    uint32_t v = 0;
    if (dir) {
        v |= I2C_Dir_MASK;
    }
    if (stop) {
        v |= I2C_Phase_stop_MASK;
    }
    if (data) {
        v |= I2C_Phase_data_MASK;
    }
    if (addr) {
        v |= I2C_Phase_addr_MASK;
    }
    if (start) {
        v |= I2C_Phase_start_MASK;
    }
    return v;
}

void i2c_ctrl_set_phase_atomic(__IO uint32_t *ctrl_reg, bool stop, bool data, bool addr, bool start, bool dir)
{
    uint32_t reg  = *ctrl_reg;
    reg          &= ~(I2C_Dir_MASK | I2C_Phase_stop_MASK | I2C_Phase_data_MASK | I2C_Phase_addr_MASK | I2C_Phase_start_MASK);
    reg          |= i2c_ctrl_pack_phase(stop, data, addr, start, dir);
    *ctrl_reg     = reg;
}

void i2c_master_xfer_config_er8130(I2C_T *i2c, bool en_stop, bool en_data, bool en_addr, bool en_start, bool dir)
{
    i2c_ctrl_set_phase_atomic(&i2c->CTRL.reg, en_stop, en_data, en_addr, en_start, dir);
}

void i2c_config_er8130(I2C_T *i2c, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp, uint8_t hddat,
                       uint8_t scl_ratio, uint8_t scl_hi, bool dir, uint16_t target_address)
{
    /* reset the I2C controller (abort current transaction, set the
    SDA and SCL line to the open-drain mode, clear the Status
    Register, Interrupt Enable Register and empty the FIFO) */
    i2c->CMD.bitfield.CMD = I2C_ACTION_RESET_CONTROLLER;

    /* set setup configure */
    i2c->SETUP.bitfield.SUDAT      = sudat;
    i2c->SETUP.bitfield.SP         = sp;
    i2c->SETUP.bitfield.HDDAT      = hddat;
    i2c->SETUP.bitfield.SCLRatio   = scl_ratio;
    i2c->SETUP.bitfield.SCLHi      = scl_hi;
    i2c->SETUP.bitfield.IICEn      = i2c_enable;
    i2c->SETUP.bitfield.Master     = role;
    i2c->SETUP.bitfield.Addressing = addressing;
    i2c->SETUP.bitfield.DMAEn      = dma_enable;

    // Master mode => Dir = dir; Slave mode => Dir = !dir
    bool adj_dir = (role ? (dir ? 1 : 0) : (dir ? 0 : 1));
    /* set Control configure */
    // phase ctrl (Master only)
    i2c_ctrl_set_phase_atomic(&i2c->CTRL.reg, role, role, role, role, adj_dir);
    /* set Address configure */
    i2c->ADDR.bitfield.ADDRESS = target_address;
    if (role == I2C_SLAVE) {
        i2c_disable_all_interrupts_er8130(i2c);
        i2c->STS.reg = 0x3f8; // clear status
        i2c_cmd_action_er8130(i2c, I2C_ACTION_CLEAR_FIFO);
        NVIC_EnableIRQ((IRQn_Type)((i2c == I2C0) ? I2C0_IRQn : I2C1_IRQn));
        i2c_enable_slave_transfer_interrupts_er8130(i2c, dma_enable);
    }
}
void i2c_set_direction_er8130(I2C_T *i2c, bool dir)
{
    i2c->CTRL.bitfield.Dir = dir;
}

void i2c_set_target_address_er8130(I2C_T *i2c, uint16_t target_address)
{
    i2c->ADDR.bitfield.ADDRESS = target_address;
}

HAL_STATUS i2c_transfer_er8130(I2C_T *i2c, bool role, bool dma_enable, I2C_TRANSFER_MODE_T mode)
{
    i2c_disable_all_interrupts_er8130(i2c);
    i2c->STS.reg = 0x3f8; // clear status
    i2c_cmd_action_er8130(i2c, I2C_ACTION_CLEAR_FIFO);
    NVIC_EnableIRQ((IRQn_Type)((i2c == I2C0) ? I2C0_IRQn : I2C1_IRQn));

    i2c->CTRL.bitfield.Dir = (role ? (mode ? 1 : 0) : (mode ? 0 : 1));

    if (role == I2C_MASTER) {
        if (mode == I2C_MODE_TX) {
            i2c_enable_master_transmit_interrupts_er8130(i2c, dma_enable);
        } else {
            i2c_enable_master_receiver_interrupts_er8130(i2c, dma_enable);
        }

        i2c_cmd_action_er8130(i2c, I2C_ACTION_DATA_TRANSACTION);
    } else {
        i2c_enable_slave_transfer_interrupts_er8130(i2c, dma_enable);
    }

    return HAL_NO_ERR;
}
