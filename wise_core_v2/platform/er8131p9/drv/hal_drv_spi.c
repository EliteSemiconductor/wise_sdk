/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_spi.h"
#include "hdl/spi_er8130.h"
#include "hal_intf_spi.h"
#include "hal_intf_dma.h"
#include "types.h"
#include <stdint.h>

typedef struct {
    volatile void *tx_fifo;
    volatile void *tx_fifo_ptr;
    volatile uint16_t tx_unit_count;
    volatile uint16_t xfered_unit_count;
    volatile void *rx_fifo;
    volatile void *rx_fifo_ptr;
    volatile uint16_t rx_unit_count;
    volatile uint16_t recved_unit_count;
    volatile uint8_t transfer_end_flag;
    volatile uint8_t data_bit_width;
    volatile uint8_t data_merge;
    volatile uint8_t block_mode;
    volatile uint8_t dma_enable;
    volatile uint8_t role;
    volatile uint8_t io_mode; /* HAL_SPI_IO_* : channel wire + lane width */
} SPI_TRANSFER_CONTEXT_T;

typedef void (*SPI_IRQ_HANDLER_T)(uint8_t spi_index);

EVT_CALLBACK_ENTRY_T spi_callbacks[CHIP_SPI_CHANNEL_NUM][SPI_MAX_EVENTS];
static SPI_TRANSFER_CONTEXT_T spi_context[CHIP_SPI_CHANNEL_NUM];
uint32_t spi_base[CHIP_SPI_CHANNEL_NUM] = {SPI_BASE, (SPI_BASE + 0x1000)};
static SPI_IRQ_HANDLER_T _spi_irq_handler[CHIP_SPI_CHANNEL_NUM] = {NULL};

/* Lazy-init cache: 1 = TRANS_CTRL / INT_EN already match the write-byte
 * configuration, so hal_drv_spi_master_write_byte() can skip the
 * set_xfer_fmt + disable_interrupt steps. Any function that reprograms
 * TRANS_CTRL or INT_EN must clear this. */
static uint8_t spi_wbyte_fmt_cached[CHIP_SPI_CHANNEL_NUM] = {0};

static void spi_int_handler(uint8_t spi_index);
static uint8_t spi_get_unit_size(SPI_TRANSFER_CONTEXT_T *ctx);

static void hal_drv_spi_bind_dispatch(uint8_t spi_index)
{
    _spi_irq_handler[spi_index] = spi_int_handler;
}

void hal_drv_spi_reset(uint32_t spi_index, DRV_SPI_RESET_TYPE type)
{
    spi_reset_er8130(spi_base[spi_index], type);
    spi_wbyte_fmt_cached[spi_index] = 0;
}
void hal_drv_spi_config(uint8_t spi_index, uint16_t clock_mode, uint8_t role, uint8_t data_bit_width, uint32_t bus_clock,
                        uint8_t bit_order, uint8_t data_merge, uint8_t io_mode, uint8_t block_mode, uint8_t dma_enable)
{
    /* io_mode (HAL_SPI_IO_*) bundles wire topology + lane width. MOSIBiDir
     * (3-wire) lives in TransFmt and is set here at open; DualQuad (dual/quad)
     * lives in TransCtrl and is applied per transfer (see hal_drv_spi_transfer). */
    uint8_t mosi_bir_dir = (io_mode == HAL_SPI_IO_3WIRE) ? 1 : 0;

    spi_context[spi_index].data_bit_width = data_bit_width - 1;
    spi_context[spi_index].data_merge     = data_merge;
    spi_context[spi_index].block_mode     = block_mode;
    spi_context[spi_index].dma_enable     = dma_enable;
    spi_context[spi_index].role           = role;
    spi_context[spi_index].io_mode        = io_mode;
    spi_config_er8130(spi_base[spi_index], clock_mode, role, data_bit_width, bus_clock, bit_order, data_merge, mosi_bir_dir, dma_enable);
    spi_wbyte_fmt_cached[spi_index] = 0;

    hal_drv_spi_bind_dispatch(spi_index);

    if (dma_enable == ENABLE) {
        hal_intf_spi_dma_init();
    }
}

int32_t hal_drv_spi_slave_set_ready(uint8_t spi_index)
{
    if (spi_context[spi_index].dma_enable == ENABLE) {
        spi_enable_interrupt_er8130(spi_base[spi_index],
                                    (SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
    } else {
        spi_enable_interrupt_er8130(spi_base[spi_index],
                                    (SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
    }
    spi_set_slv_rdy_er8130(spi_base[spi_index], 1);
    spi_wbyte_fmt_cached[spi_index] = 0;
    return HAL_NO_ERR;
}

uint16_t hal_drv_spi_slave_get_recv_count(uint8_t spi_index)
{
    return spi_get_slv_read_cnt_er8130(spi_base[spi_index]);
}

int32_t hal_drv_spi_transfer(uint8_t spi_index, uint8_t role, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len, uint8_t trans_mode,
                             uint8_t cmd_en, uint32_t addr_value, uint8_t cmd_value, uint8_t addr_len, uint8_t addr_fmt,
                             void *tx_fifo, void *rx_fifo)
{
    uint8_t dma_en = spi_context[spi_index].dma_enable;
    uint32_t base  = spi_base[spi_index];

    /* Derive the HW flag bits: cmd phase from cmd_en, address phase from
     * addr_len (0 => no address phase). */
    uint8_t flag_en = (cmd_en ? SPI_MSG_CMD_EN : 0) | (addr_len > 0 ? SPI_MSG_ADDR_EN : 0);

    /* DualQuad value (TransCtrl) derived from the channel's io_mode (set at
     * open): single/3wire -> 0, dual -> 1, quad -> 2. */
    uint8_t io_mode = (spi_context[spi_index].io_mode == HAL_SPI_IO_DUAL)   ? 1 :
                      (spi_context[spi_index].io_mode == HAL_SPI_IO_QUAD)   ? 2 : 0;

    SPI_TRANSFER_CONTEXT_T *ctx = &spi_context[spi_index];
    ctx->role                   = role;
    ctx->tx_fifo                = tx_fifo;
    ctx->tx_fifo_ptr            = tx_fifo;
    ctx->tx_unit_count          = tx_unit_count;
    ctx->rx_fifo                = rx_fifo;
    ctx->rx_fifo_ptr            = rx_fifo;
    ctx->rx_unit_count          = rx_unit_count;
    ctx->xfered_unit_count      = 0;
    ctx->recved_unit_count      = 0;
    ctx->transfer_end_flag      = false;

    // DMA Setup
    if (dma_en) {
        /* Match the DMA beat width to the SPI Data Register access width: with
         * 8-bit data-merge each access moves 4 bytes (32-bit), otherwise it is
         * 1/2/4 bytes per the data length. The static dma_extsrc_info table
         * always assumes 32-bit, so the width is passed in explicitly here. */
        uint8_t unit_sz   = spi_get_unit_size(ctx);
        uint8_t data_size = (unit_sz >= 4) ? DATA_SIZE_32BITS : (unit_sz == 2) ? DATA_SIZE_16BITS : DATA_SIZE_8BITS;
        hal_intf_spi_dma_update(spi_index, tx_fifo, tx_unit_count, rx_fifo, rx_unit_count, data_size);
    }

    // SPI Core Transfer
    spi_xfer_exec_er8130(base, role, rx_unit_count, tx_unit_count, dummy_len, trans_mode, flag_en, addr_value, cmd_value, tx_fifo, rx_fifo, dma_en,
                         io_mode, addr_len, addr_fmt);
    spi_wbyte_fmt_cached[spi_index] = 0;

    // DMA Trigger
    if (dma_en) {
        hal_intf_spi_dma_trigger(spi_index, tx_fifo, tx_unit_count, rx_fifo, rx_unit_count);
    }

    // Blocking wait
    if (ctx->block_mode == SPI_BLOCK_MODE) {
        while (!ctx->transfer_end_flag) {
            __asm__ volatile("nop");
        }
    }

    return HAL_NO_ERR;
}

uint8_t hal_drv_spi_get_cmd(uint8_t spi_index)
{
    return spi_get_cmd_er8130(spi_base[spi_index]);
}

static uint8_t spi_get_unit_size(SPI_TRANSFER_CONTEXT_T *ctx)
{
    switch (ctx->data_bit_width) {
    case 7:
        return (ctx->data_merge != 1) ? 1 : 4;
    case 15:
        return 2;
    case 31:
        return 4;
    default:
        return 1;
    }
}

static void spi_write_push_units(uint8_t spi_index, SPI_TRANSFER_CONTEXT_T *ctx, uint8_t unit_size, uint16_t push_units)
{
    if (unit_size == 1 && ctx->data_merge != 1) {
        uint8_t *p = (uint8_t *)ctx->tx_fifo_ptr;
        for (uint16_t i = 0; i < push_units; i++) {
            spi_write_data_er8130(spi_base[spi_index], p[i]);
        }
        ctx->tx_fifo_ptr = p + push_units;

    } else if (unit_size == 2) {
        uint16_t *p = (uint16_t *)ctx->tx_fifo_ptr;
        for (uint16_t i = 0; i < push_units; i++) {
            spi_write_data_er8130(spi_base[spi_index], p[i]);
        }
        ctx->tx_fifo_ptr = p + push_units;

    } else {
        uint32_t *p = (uint32_t *)ctx->tx_fifo_ptr;
        for (uint16_t i = 0; i < push_units; i++) {
            spi_write_data_er8130(spi_base[spi_index], p[i]);
        }
        ctx->tx_fifo_ptr = p + push_units;
    }
}

static void spi_read_pull_units(uint8_t spi_index, SPI_TRANSFER_CONTEXT_T *ctx, uint8_t unit_size, uint16_t pull_units)
{
    if (unit_size == 1 && ctx->data_merge != 1) {
        uint8_t *p = (uint8_t *)ctx->rx_fifo_ptr;
        for (uint16_t i = 0; i < pull_units; i++) {
            p[i] = (uint8_t)spi_read_data_er8130(spi_base[spi_index]);
        }
        ctx->rx_fifo_ptr = p + pull_units;

    } else if (unit_size == 2) {
        uint16_t *p = (uint16_t *)ctx->rx_fifo_ptr;
        for (uint16_t i = 0; i < pull_units; i++) {
            p[i] = (uint16_t)spi_read_data_er8130(spi_base[spi_index]);
        }
        ctx->rx_fifo_ptr = p + pull_units;

    } else {
        uint32_t *p = (uint32_t *)ctx->rx_fifo_ptr;
        for (uint16_t i = 0; i < pull_units; i++) {
            p[i] = (uint32_t)spi_read_data_er8130(spi_base[spi_index]);
        }
        ctx->rx_fifo_ptr = p + pull_units;
    }
}

static void hal_drv_spi_data_push(uint8_t spi_index, void *tx_fifo, uint16_t tx_unit_count)
{
    SPI_TRANSFER_CONTEXT_T *ctx = &spi_context[spi_index];
    uint8_t unit_size           = spi_get_unit_size(ctx);

    uint32_t real_unit_count = tx_unit_count;
    if (ctx->data_bit_width == 7 && ctx->data_merge == 1) {
        real_unit_count = (tx_unit_count + unit_size - 1) / unit_size;
    }

    uint16_t remain_units = real_unit_count - ctx->xfered_unit_count;
    if (remain_units == 0) {
        spi_disable_interrupt_er8130(spi_base[spi_index], SPI_TX_FIFO_INT_EN_MASK);
        return;
    }

    uint16_t fifo_depth_units = 8;
    uint16_t fifo_space_units = 4;
    uint16_t push_units       = (ctx->xfered_unit_count == 0) ? MIN(remain_units, fifo_depth_units) : MIN(remain_units, fifo_space_units);

    spi_write_push_units(spi_index, ctx, unit_size, push_units);
    ctx->xfered_unit_count += push_units;

    if (ctx->xfered_unit_count >= real_unit_count) {
        spi_disable_interrupt_er8130(spi_base[spi_index], SPI_TX_FIFO_INT_EN_MASK);
    }
}

void hal_drv_spi_data_prepare(uint8_t spi_index, void *tx_fifo, uint16_t tx_unit_count)
{
    SPI_TRANSFER_CONTEXT_T *ctx = &spi_context[spi_index];

    ctx->tx_fifo_ptr       = tx_fifo;
    ctx->xfered_unit_count = 0;

    hal_drv_spi_data_push(spi_index, tx_fifo, tx_unit_count);
}

static void hal_drv_spi_data_pull(uint8_t spi_index, void *rx_fifo, uint16_t rx_unit_count)
{
    SPI_TRANSFER_CONTEXT_T *ctx = &spi_context[spi_index];
    uint8_t unit_size           = spi_get_unit_size(ctx);

    uint32_t real_unit_count = rx_unit_count;
    if (ctx->data_bit_width == 7 && ctx->data_merge == 1) {
        real_unit_count = (rx_unit_count + unit_size - 1) / unit_size;
    }

    uint16_t remain_units = real_unit_count - ctx->recved_unit_count;
    if (remain_units == 0) {
        if (ctx->transfer_end_flag == true) {
            while (!spi_is_rx_fifo_empty_er8130(spi_base[spi_index])) {
                (void)spi_read_data_er8130(spi_base[spi_index]);
            }
        }
        spi_disable_interrupt_er8130(spi_base[spi_index], SPI_RX_FIFO_INT_EN_MASK);
        return;
    }

    /* Drain the whole RX FIFO this pass instead of a fixed chunk. With a fixed
     * pull, transfers larger than the FIFO depth overrun and silently drop
     * bytes whenever the ISR is momentarily delayed (slave RX). Emptying the
     * FIFO each time gives a full FIFO-depth of headroom before the next pull. */
    uint16_t pulled = 0;
    while (pulled < remain_units && !spi_is_rx_fifo_empty_er8130(spi_base[spi_index])) {
        spi_read_pull_units(spi_index, ctx, unit_size, 1);
        pulled++;
    }
    ctx->recved_unit_count += pulled;

    if (ctx->recved_unit_count >= real_unit_count) {
        spi_disable_interrupt_er8130(spi_base[spi_index], SPI_RX_FIFO_INT_EN_MASK);
    }
}

int32_t hal_drv_spi_master_write_byte(uint8_t spi_index, uint8_t in_byte)
{
    uint32_t base = spi_base[spi_index];

    if (spi_wait_for_completion_er8130(base)) {
        return HAL_ERR;
    }

    /* Fast path: TRANS_CTRL/INT_EN already match write-byte config from a
     * previous call, so skip the RMW setup. */
    if (!spi_wbyte_fmt_cached[spi_index]) {
        spi_disable_interrupt_er8130(base, SPI_TX_FIFO_INT_EN_MASK | SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK);
        spi_set_xfer_fmt_er8130(base, 0, 1, 0, SPI_TM_WRITE_ONLY, 0, 0, 0, 0, 0);
        spi_wbyte_fmt_cached[spi_index] = 1;
    }
    spi_write_data_er8130(base, in_byte);
    spi_cmd_trigger_er8130(base, 0, 0);

    if (spi_wait_for_completion_er8130(base)) {
        return HAL_ERR;
    }

    return HAL_NO_ERR;
}

int32_t hal_drv_spi_master_read_byte(uint8_t spi_index, uint8_t *out_byte)
{
    uint32_t base = spi_base[spi_index];

    if (spi_wait_for_completion_er8130(base)) {
        return HAL_ERR;
    }

    spi_disable_interrupt_er8130(base, SPI_TX_FIFO_INT_EN_MASK | SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK);
    spi_set_xfer_fmt_er8130(base, 1, 0, 0, SPI_TM_READ_ONLY, 0, 0, 0, 0, 0);
    spi_wbyte_fmt_cached[spi_index] = 0;
    spi_cmd_trigger_er8130(base, 0, 0);

    if (spi_wait_for_completion_er8130(base)) {
        return HAL_ERR;
    }

    *out_byte = (uint8_t)(spi_read_data_er8130(base) & 0xFF);

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_spi_register_event_callback(uint8_t spi_channel, EVT_CALLBACK_T cb, void *context)
{
    if (spi_channel < CHIP_SPI_CHANNEL_NUM) {
        for (int i = 0; i < SPI_MAX_EVENTS; i++) {
            if (spi_callbacks[spi_channel][i].callback == NULL) {
                spi_callbacks[spi_channel][i].callback = cb;
                spi_callbacks[spi_channel][i].context  = context;
            }
        }
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_spi_unregister_event_callback(uint8_t spi_channel)
{
    if (spi_channel < CHIP_SPI_CHANNEL_NUM) {
        for (int i = 0; i < SPI_MAX_EVENTS; i++) {
            spi_callbacks[spi_channel][i].callback = NULL;
            spi_callbacks[spi_channel][i].context  = NULL;
        }
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}
static void hal_drv_spi_trigger_event(uint8_t spi_channel, uint32_t spi_event)
{
    if (spi_channel < CHIP_SPI_CHANNEL_NUM && spi_event < SPI_MAX_EVENTS) {
        EVT_CALLBACK_T callback = spi_callbacks[spi_channel][spi_event].callback;
        void *context           = spi_callbacks[spi_channel][spi_event].context;

        if (callback != NULL) {
            callback(spi_channel, spi_event, context);
        }
    }
}

void spi_int_handler(uint8_t spi_index)
{
    SPI_TRANSFER_CONTEXT_T *ctx = (SPI_TRANSFER_CONTEXT_T *)&spi_context[spi_index];
    uint32_t int_status         = spi_get_interrupt_status_er8130(spi_base[spi_index]);

    if (int_status & SPI_Int_SlvCmd) {
        spi_disable_interrupt_er8130(spi_base[spi_index], SPI_SLV_CMD_EN_MASK);
        hal_drv_spi_trigger_event(spi_index, SPI_EVENT_SLAVE_COMMAND_INTERRUPT);
    }

    if ((int_status & SPI_Int_TXTH)) {
        hal_drv_spi_data_push(spi_index, (void *)ctx->tx_fifo, ctx->tx_unit_count);
    }

    if (int_status & SPI_Int_RXTH) {
        hal_drv_spi_data_pull(spi_index, (void *)ctx->rx_fifo, ctx->rx_unit_count);
    }

    if (int_status & SPI_Int_END) {
        /* A full transfer wraps the 9-bit slave RCNT to 0. Read the programmed
         * unit count so a complete transfer (RCNT==0) is not mistaken for
         * "nothing received" -- needed for the DMA path, where recved_unit_count
         * is otherwise left at 0 (PIO already counts what it drained). */
        uint16_t rcnt_at_end = spi_get_slv_read_cnt_er8130(spi_base[spi_index]);
        if (rcnt_at_end == 0 && ctx->rx_unit_count > 0 && ctx->recved_unit_count == 0) {
            ctx->recved_unit_count = (ctx->data_bit_width == 7 && ctx->data_merge == 1)
                                         ? (uint16_t)((ctx->rx_unit_count + 3) / 4)
                                         : ctx->rx_unit_count;
        }
        spi_context[spi_index].transfer_end_flag = true;
        spi_disable_interrupt_er8130(spi_base[spi_index], (SPI_Int_END | SPI_Int_RXTH));
        if (spi_context[spi_index].dma_enable == false) {
            hal_drv_spi_data_pull(spi_index, (void *)ctx->rx_fifo, ctx->rx_unit_count);
        }
        hal_drv_spi_trigger_event(spi_index, SPI_EVENT_TRANSFER_DONE);
    }

    spi_clear_interrupt_status_er8130(spi_base[spi_index], int_status);
}

WEAK_ISR RAM_TEXT void SPI0_IRQHandler()
{
    //spi_int_handler(0);
    if(_spi_irq_handler[0])
    {
        (_spi_irq_handler[0])(0);
    }
}

WEAK_ISR RAM_TEXT void SPI1_IRQHandler()
{
    //spi_int_handler(1);
    if(_spi_irq_handler[1])
    {
        (_spi_irq_handler[1])(1);
    }
}
