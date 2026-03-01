/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_i2c.h"
#include "hdl/i2c_er8130.h"
#include "hal_intf_dma.h"
#include <stdint.h>

typedef struct {
    uint8_t *tx_fifo;
    uint16_t tx_len;
    uint16_t xfered_tx_len;
    uint8_t *rx_fifo;
    uint16_t rx_len;
    uint16_t recved_rx_len;
    uint16_t fifo_size;
    I2C_TRANSFER_MODE_T mode;
    uint8_t role;
    uint8_t dma_en;
} I2C_TRANSFER_CONTEXT_T;

typedef void (*SPI_IRQ_HANDLER_T)(uint8_t i2c_idx);


I2C_T *i2c_channel[CHIP_I2C_CHANNEL_NUM] = {I2C0, I2C1};
EVT_CALLBACK_ENTRY_T i2c_callbacks[CHIP_I2C_CHANNEL_NUM][I2C_MAX_EVENTS];
static I2C_TRANSFER_CONTEXT_T i2c_context[CHIP_I2C_CHANNEL_NUM];
static SPI_IRQ_HANDLER_T _i2c_irq_handler[CHIP_I2C_CHANNEL_NUM] = {NULL};

static void hal_drv_i2c_trigger_event(uint8_t i2c_channel, uint32_t i2c_event);
static void hal_drv_i2c_pull_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
static void hal_drv_i2c_push_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
static void i2c_irq_handler(uint8_t i2c_idx);

void hal_drv_i2c_set_direction(uint8_t i2c_idx, bool dir)
{
    i2c_set_direction_er8130(i2c_channel[i2c_idx], dir);
}

void hal_drv_i2c_clear_fifo(uint8_t i2c_idx)
{
    i2c_clear_fifo_er8130(i2c_channel[i2c_idx]);
}

uint8_t hal_drv_i2c_get_data_count(uint8_t i2c_idx)
{
    return i2c_get_data_byte_count_er8130(i2c_channel[i2c_idx]);
}

uint8_t hal_drv_i2c_get_data(uint8_t i2c_idx)
{
    return i2c_get_data_er8130(i2c_channel[i2c_idx]);
}

static void hal_drv_i2c_pull_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx)
{
    uint32_t timeout   = 0;
    uint8_t data_count = 0;
    while (ctx->recved_rx_len < ctx->rx_len) {
        uint32_t status = i2c_get_status(i2c_channel[i2c_index]);

        if (status & (I2C_STS_FIFOEmpty_MASK | I2C_STS_Cmpl_MASK)) {
            break;
        }

        data_count                       = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);
        ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
        ctx->recved_rx_len++;

        timeout = 10;
        do {
            uint8_t new_data_count = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);

            if (ctx->role == I2C_MASTER) {
                if (new_data_count == 0) {
                    break;
                }
                if (new_data_count == (data_count - 1)) {
                    break;
                }
            } else {
                if (new_data_count == ctx->rx_len) {
                    break;
                }
                if (new_data_count == (data_count + 1)) {
                    break;
                }
            }

            timeout--;
        } while (timeout > 0);

        if (timeout == 0) {
            return;
        }
    }

    if (ctx->recved_rx_len >= ctx->rx_len) {
        i2c_disable_fifo_full_interrupt_er8130(i2c_channel[i2c_index]);
    }
}

static void hal_drv_i2c_push_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx)
{
    uint8_t data_count = 0;
    uint32_t timeout   = 0;

    while (ctx->xfered_tx_len < ctx->tx_len) {
        uint32_t status = i2c_get_status(i2c_channel[i2c_index]);

        if (status & I2C_STS_FIFOFull_MASK) {
            break;
        }
        data_count = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);

        i2c_set_data_er8130((i2c_channel[i2c_index]), ctx->tx_fifo[ctx->xfered_tx_len]);
        __asm__ volatile("" ::: "memory");
        ctx->xfered_tx_len++;

        timeout = 10;
        do {
            uint8_t new_data_count = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);
            if (ctx->role == I2C_MASTER) {
                if (new_data_count == (data_count - 1)) {
                    break;
                }
            } else {
                if (new_data_count == (data_count + 1)) {
                    break;
                }
            }
            timeout--;
        } while (timeout > 0);

        if (timeout == 0) {
            return;
        }
    }

    if (ctx->xfered_tx_len >= ctx->tx_len) {
        i2c_disable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
    }
}

static void i2c_address_hit_interrupt_handler(void *context, uint8_t i2c_index)
{
    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)context;

    if (ctx->mode == I2C_MODE_RX) {
        i2c_enable_fifo_full_interrupt_er8130(i2c_channel[i2c_index]);
    } else {
        i2c_enable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
    }

    hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_ADDRESS_HIT);
}

static void i2c_transfer_end_interrupt_handler(void *context, uint8_t i2c_index)
{
    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)context;
    i2c_clear_cmpl_status_er8130(i2c_channel[i2c_index]);
    if (ctx->role == I2C_MASTER) {
        i2c_disable_all_interrupts_er8130(i2c_channel[i2c_index]);
    }
    if (ctx->dma_en == DISABLE) {
        if (ctx->recved_rx_len < ctx->rx_len) {
            while (ctx->recved_rx_len < ctx->rx_len) {
                ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
                ctx->recved_rx_len++;
            }
        }
        if (ctx->xfered_tx_len < ctx->tx_len) {
            return;
        }
    }
    hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_TRANSFER_DONE);
}
static HAL_STATUS hal_drv_i2c_transfer(uint8_t i2c_idx, bool role, bool dma_enable, I2C_TRANSFER_MODE_T xfer_mode, uint8_t *ptr_fifo, uint32_t len,
                                       I2C_MSG_T *xfer_msg)
{
    memset(&i2c_context[i2c_idx], 0, sizeof(I2C_TRANSFER_CONTEXT_T));

    if (xfer_mode == I2C_MODE_TX) {
        i2c_context[i2c_idx].tx_fifo       = ptr_fifo;
        i2c_context[i2c_idx].tx_len        = len;
        i2c_context[i2c_idx].xfered_tx_len = 0;
        i2c_context[i2c_idx].mode          = I2C_MODE_TX;
    } else {
        i2c_context[i2c_idx].rx_fifo       = ptr_fifo;
        i2c_context[i2c_idx].rx_len        = len;
        i2c_context[i2c_idx].recved_rx_len = 0;
        i2c_context[i2c_idx].mode          = I2C_MODE_RX;
    }

    i2c_context[i2c_idx].role      = role;
    i2c_context[i2c_idx].dma_en    = dma_enable;
    i2c_context[i2c_idx].fifo_size = i2c_get_fifo_size_er8130(i2c_channel[i2c_idx]);

    if (role == I2C_MASTER) {
        i2c_master_xfer_config_er8130(i2c_channel[i2c_idx], xfer_msg->en_stop, xfer_msg->en_data, xfer_msg->en_addr, xfer_msg->en_start, xfer_mode);
        i2c_set_data_byte_count_er8130(i2c_channel[i2c_idx], len);
    }

    return i2c_transfer_er8130(i2c_channel[i2c_idx], role, dma_enable, xfer_mode);
}

HAL_STATUS hal_drv_i2c_config(uint8_t i2c_index, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp,
                              uint8_t hddat, uint8_t scl_ratio, uint8_t scl_hi, bool dir, uint16_t target_address)
{
    i2c_config_er8130(i2c_channel[i2c_index], i2c_enable, role, addressing, dma_enable, sudat, sp, hddat, scl_ratio, scl_hi, dir, target_address);
    _i2c_irq_handler[i2c_index] = i2c_irq_handler;
    
    return HAL_NO_ERR;
}
void hal_drv_i2c_set_tartget_addres(uint8_t i2c_index, uint16_t target_address)
{
    i2c_set_target_address_er8130(i2c_channel[i2c_index], target_address);
}

HAL_STATUS hal_drv_i2c_transmit(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len)
{
    I2C_MSG_T xfer_msg = {.en_addr = 1, .en_data = 1, .en_start = 1, .en_stop = 1};
    return hal_drv_i2c_transfer(i2c_idx, role, dma_enable, I2C_MODE_TX, tx_fifo, tx_len, &xfer_msg);
}

HAL_STATUS hal_drv_i2c_receive(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len)
{
    I2C_MSG_T xfer_msg = {.en_addr = 1, .en_data = 1, .en_start = 1, .en_stop = 1};
    return hal_drv_i2c_transfer(i2c_idx, role, dma_enable, I2C_MODE_RX, rx_fifo, rx_len, &xfer_msg);
}

HAL_STATUS hal_drv_i2c_transmit_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *tx_fifo, uint32_t tx_len, bool en_addr, bool en_data,
                                   bool en_start, bool en_stop)
{
    I2C_MSG_T xfer_msg = {.en_addr = en_addr, .en_data = en_data, .en_start = en_start, .en_stop = en_stop};
    return hal_drv_i2c_transfer(i2c_idx, role, dma_enable, I2C_MODE_TX, tx_fifo, tx_len, &xfer_msg);
}

HAL_STATUS hal_drv_i2c_receive_ex(uint8_t i2c_idx, bool role, bool dma_enable, uint8_t *rx_fifo, uint32_t rx_len, bool en_addr, bool en_data,
                                  bool en_start, bool en_stop)
{
    I2C_MSG_T xfer_msg = {.en_addr = en_addr, .en_data = en_data, .en_start = en_start, .en_stop = en_stop};
    return hal_drv_i2c_transfer(i2c_idx, role, dma_enable, I2C_MODE_RX, rx_fifo, rx_len, &xfer_msg);
}

HAL_STATUS hal_drv_i2c_register_event_callback(uint8_t i2c_channel, EVT_CALLBACK_T cb, void *context)
{
    if (i2c_channel < CHIP_I2C_CHANNEL_NUM) {
        for (int i = 0; i < I2C_MAX_EVENTS; i++) {
            if (i2c_callbacks[i2c_channel][i].callback == NULL) {
                i2c_callbacks[i2c_channel][i].callback = cb;
                i2c_callbacks[i2c_channel][i].context  = context;
            }
        }
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_i2c_unregister_event_callback(uint8_t i2c_channel)
{
    if (i2c_channel < CHIP_I2C_CHANNEL_NUM) {
        for (int i = 0; i < I2C_MAX_EVENTS; i++) {
            i2c_callbacks[i2c_channel][i].callback = NULL;
            i2c_callbacks[i2c_channel][i].context  = NULL;
        }
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}
static void hal_drv_i2c_trigger_event(uint8_t i2c_channel, uint32_t i2c_event)
{
    if (i2c_channel < CHIP_I2C_CHANNEL_NUM && i2c_event < I2C_MAX_EVENTS) {
        EVT_CALLBACK_T callback = i2c_callbacks[i2c_channel][i2c_event].callback;
        void *context           = i2c_callbacks[i2c_channel][i2c_event].context;

        if (callback != NULL) {
            callback(i2c_channel, i2c_event, context);
        }
    }
}

void i2c_irq_handler(uint8_t i2c_idx)
{
    uint32_t int_status = i2c_get_status(i2c_channel[i2c_idx]);

    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)&i2c_context[i2c_idx];
    if ((int_status & I2C_STS_FIFOEmpty_MASK)) {
        int_status &= ~I2C_STS_FIFOEmpty_MASK;
        hal_drv_i2c_push_data(i2c_idx, ctx);
    }

    if ((int_status & I2C_STS_FIFOFull_MASK)) {
        int_status &= ~I2C_STS_FIFOFull_MASK;
        hal_drv_i2c_pull_data(i2c_idx, ctx);
    }

    if (int_status & I2C_STS_Cmpl_MASK) {
        int_status &= ~I2C_STS_Cmpl_MASK;
        i2c_transfer_end_interrupt_handler(&i2c_context[i2c_idx], i2c_idx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_Cmpl_MASK);
    }

    if (int_status & I2C_STS_AddrHit_MASK) {
        int_status &= ~I2C_STS_AddrHit_MASK;
        i2c_address_hit_interrupt_handler(&i2c_context[i2c_idx], i2c_idx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_AddrHit_MASK);
    }
}

WEAK_ISR RAM_TEXT void I2C0_IRQHandler(void)
{
    //i2c_irq_handler(0);
    if(_i2c_irq_handler[0])
    {
        (_i2c_irq_handler[0])(0);
    }
}

WEAK_ISR RAM_TEXT void I2C1_IRQHandler(void)
{
    //i2c_irq_handler(1);
    if(_i2c_irq_handler[1])
    {
        (_i2c_irq_handler[1])(1);
    }
}
