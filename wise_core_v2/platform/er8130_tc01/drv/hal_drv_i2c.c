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
    uint16_t debug_rx_drained_len;
    uint16_t fifo_size;
    I2C_TRANSFER_MODE_T mode;
    uint8_t role;
    uint8_t dma_en;
    /* Slave TX response buffer (pre-registered, survives arm_rx) */
    uint8_t *slave_tx_buf;
    uint16_t slave_tx_len;
} I2C_TRANSFER_CONTEXT_T;

typedef void (*SPI_IRQ_HANDLER_T)(uint8_t i2c_idx);

I2C_T *i2c_channel[CHIP_I2C_CHANNEL_NUM] = {I2C0, I2C1};
EVT_CALLBACK_ENTRY_T i2c_callbacks[CHIP_I2C_CHANNEL_NUM][I2C_MAX_EVENTS];
static I2C_TRANSFER_CONTEXT_T i2c_context[CHIP_I2C_CHANNEL_NUM];
static SPI_IRQ_HANDLER_T _i2c_irq_handler[CHIP_I2C_CHANNEL_NUM] = {NULL};

#define I2C_DEBUG_TRACE_DEPTH 16
#define I2C_DBG_EVT_ADDR_HIT  1
#define I2C_DBG_EVT_FIFO_EMP  2
#define I2C_DBG_EVT_BYTE_TX   3
#define I2C_DBG_EVT_FIFO_FUL  4
#define I2C_DBG_EVT_CMPL      5

#if HAL_I2C_DEBUG_TRACE_ENABLE
static HAL_I2C_DEBUG_TRACE_T i2c_debug_trace[CHIP_I2C_CHANNEL_NUM][I2C_DEBUG_TRACE_DEPTH];
static uint8_t i2c_debug_trace_count[CHIP_I2C_CHANNEL_NUM];
#endif
static bool i2c_probe_done_latched[CHIP_I2C_CHANNEL_NUM];
static bool i2c_probe_ack_latched[CHIP_I2C_CHANNEL_NUM];
static bool i2c_probe_arb_lost_latched[CHIP_I2C_CHANNEL_NUM];

static void hal_drv_i2c_trigger_event(uint8_t i2c_channel, uint32_t i2c_event);
static void hal_drv_i2c_pull_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
static void hal_drv_i2c_push_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
static void i2c_irq_handler(uint8_t i2c_idx);

static void i2c_debug_trace_log(uint8_t i2c_idx, uint8_t evt, I2C_TRANSFER_CONTEXT_T *ctx)
{
#if HAL_I2C_DEBUG_TRACE_ENABLE
    uint8_t pos = i2c_debug_trace_count[i2c_idx];
    if (pos >= I2C_DEBUG_TRACE_DEPTH) {
        return;
    }

    i2c_debug_trace[i2c_idx][pos].evt        = evt;
    i2c_debug_trace[i2c_idx][pos].tx_len     = (uint8_t)ctx->xfered_tx_len;
    i2c_debug_trace[i2c_idx][pos].rx_len     = (uint8_t)ctx->debug_rx_drained_len;
    i2c_debug_trace[i2c_idx][pos].status_lsb = (uint8_t)(i2c_get_status(i2c_channel[i2c_idx]) & 0xFFu);
    i2c_debug_trace_count[i2c_idx]           = (uint8_t)(pos + 1u);
#else
    (void)i2c_idx;
    (void)evt;
    (void)ctx;
#endif
}

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

HAL_STATUS hal_drv_i2c_get_probe_state(uint8_t i2c_idx, bool *done, bool *ack, bool *arb_lost)
{
    if (done == NULL || ack == NULL || arb_lost == NULL) {
        return HAL_ERR;
    }

    *done     = i2c_probe_done_latched[i2c_idx];
    *ack      = i2c_probe_ack_latched[i2c_idx];
    *arb_lost = i2c_probe_arb_lost_latched[i2c_idx];
    return HAL_NO_ERR;
}

void hal_drv_i2c_debug_trace_reset(uint8_t i2c_idx)
{
#if HAL_I2C_DEBUG_TRACE_ENABLE
    i2c_debug_trace_count[i2c_idx] = 0;
#else
    (void)i2c_idx;
#endif
}

uint8_t hal_drv_i2c_debug_trace_copy(uint8_t i2c_idx, HAL_I2C_DEBUG_TRACE_T *out, uint8_t max_count)
{
#if HAL_I2C_DEBUG_TRACE_ENABLE
    uint8_t count = i2c_debug_trace_count[i2c_idx];
    if (count > max_count) {
        count = max_count;
    }
    for (uint8_t i = 0; i < count; ++i) {
        out[i] = i2c_debug_trace[i2c_idx][i];
    }
    return count;
#else
    (void)i2c_idx;
    (void)out;
    (void)max_count;
    return 0;
#endif
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
        ctx->debug_rx_drained_len++;

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

    if (ctx->role == I2C_SLAVE) {
        if (ctx->xfered_tx_len < ctx->tx_len) {
            i2c_set_data_er8130((i2c_channel[i2c_index]), ctx->tx_fifo[ctx->xfered_tx_len]);
            __asm__ volatile("" ::: "memory");
            ctx->xfered_tx_len++;
        }

        if (ctx->xfered_tx_len >= ctx->tx_len) {
            i2c_disable_byte_trans_interrupt_er8130(i2c_channel[i2c_index]);
        }
        return;
    }

    while (ctx->xfered_tx_len < ctx->tx_len) {
        uint32_t status = i2c_get_status(i2c_channel[i2c_index]);

        if (status & I2C_STS_FIFOFull_MASK) {
            break;
        }

        data_count = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);
        i2c_set_data_er8130((i2c_channel[i2c_index]), ctx->tx_fifo[ctx->xfered_tx_len]);
        __asm__ volatile("" ::: "memory");
        ctx->xfered_tx_len++;

        /*
         * Confirm each FIFO write is accepted before pushing next byte.
         * Master uses DataCnt (remaining count), slave uses FIFO byte count.
         */
        timeout = 10;
        if (ctx->role == I2C_MASTER) {
            do {
                uint8_t new_data_count = i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]);
                if (new_data_count == (data_count - 1)) {
                    break;
                }
                timeout--;
            } while (timeout > 0);
        }

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

    /*
     * Per ATCIIC100 Datasheet Section 5.3.1 Step 4:
     * Read the Dir field of the Control Register to identify the
     * transaction direction.
     *   Slave Dir=0 → Receiver  (master writes to us)
     *   Slave Dir=1 → Transmitter (master reads from us)
     */
    uint8_t hw_dir = i2c_channel[i2c_index]->CTRL.bitfield.Dir;

    if (ctx->role == I2C_SLAVE) {
        if (hw_dir == 1) {
            /* Slave Transmitter: master wants to read (Repeated Start) */

            /*
             * Drain any remaining RX data from FIFO before switching
             * to TX.  FIFOFull only fires when FIFO is *full*; if
             * the master sent fewer bytes than the FIFO size (e.g. a
             * single cmd byte), those bytes are still sitting in the
             * FIFO unread.  We must pull them now so the callback can
             * inspect the received command.
             */
            while (ctx->rx_fifo != NULL
                   && ctx->recved_rx_len < ctx->rx_len
                   && i2c_get_data_byte_count_er8130(i2c_channel[i2c_index]) > 0) {
                ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
            }

            /* Clear FIFO so TX data starts fresh */
            i2c_clear_fifo_er8130(i2c_channel[i2c_index]);

            ctx->mode          = I2C_MODE_TX;
            ctx->xfered_tx_len = 0;

            /*
             * Fire callback FIRST so user can decide TX response
             * based on the cmd bytes already received in rx_fifo.
             * User calls wise_i2c_slave_set_tx_buf() inside callback.
             */
            hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_ADDRESS_HIT);

            /* Now load whatever the user set (or pre-registered) */
            if (ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
                ctx->tx_fifo = ctx->slave_tx_buf;
                ctx->tx_len  = ctx->slave_tx_len;
            }

            i2c_disable_fifo_full_interrupt_er8130(i2c_channel[i2c_index]);
            i2c_disable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
            i2c_enable_byte_trans_interrupt_er8130(i2c_channel[i2c_index]);

            /* Preload the first byte; remaining bytes are refilled on ByteTrans. */
            hal_drv_i2c_push_data(i2c_index, ctx);
        } else {
            /* Slave Receiver: master wants to write */
            ctx->mode          = I2C_MODE_RX;
            ctx->recved_rx_len = 0;
            ctx->debug_rx_drained_len = 0;
            i2c_disable_byte_trans_interrupt_er8130(i2c_channel[i2c_index]);
            i2c_disable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
            i2c_enable_fifo_full_interrupt_er8130(i2c_channel[i2c_index]);
            hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_ADDRESS_HIT);
        }
    } else {
        /* Master mode: keep original behavior */
        if (ctx->mode == I2C_MODE_RX) {
            i2c_enable_fifo_full_interrupt_er8130(i2c_channel[i2c_index]);
        } else {
            i2c_enable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
        }
        hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_ADDRESS_HIT);
    }
}

static void i2c_transfer_end_interrupt_handler(void *context, uint8_t i2c_index)
{
    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)context;
    i2c_clear_cmpl_status_er8130(i2c_channel[i2c_index]);

    /*
     * Some masters can assert Repeated Start quickly enough that the
     * preceding write-phase completion arrives after AddrHit has already
     * switched the slave into TX mode and preloaded byte0.
     *
     * If CMPL fires before all TX bytes are actually consumed, treat it as
     * an aborted/previous phase completion: drop the preloaded FIFO content
     * and restart from byte0 on the next AddrHit.  Otherwise the next read
     * phase starts with a stale first byte already queued, which shows up as
     * a right-shifted response like 00 00 00 90.
     */
    if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_TX
        && ctx->xfered_tx_len < ctx->tx_len) {
        i2c_disable_byte_trans_interrupt_er8130(i2c_channel[i2c_index]);
        i2c_disable_fifo_empty_interrupt_er8130(i2c_channel[i2c_index]);
        i2c_clear_fifo_er8130(i2c_channel[i2c_index]);
        ctx->xfered_tx_len = 0;
        hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_TRANSFER_DONE);
        return;
    }

    if (ctx->role == I2C_MASTER) {
        i2c_disable_all_interrupts_er8130(i2c_channel[i2c_index]);
    }
    if (ctx->dma_en == DISABLE) {
        if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_RX
            && ctx->recved_rx_len < ctx->rx_len) {
            while (ctx->recved_rx_len < ctx->rx_len
                   && (i2c_get_status(i2c_channel[i2c_index]) & I2C_STS_FIFOEmpty_MASK) == 0) {
                ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
            }
        }
        if (ctx->role == I2C_MASTER && ctx->mode == I2C_MODE_RX && ctx->recved_rx_len < ctx->rx_len) {
            while (ctx->recved_rx_len < ctx->rx_len
                   && (i2c_get_status(i2c_channel[i2c_index]) & I2C_STS_FIFOEmpty_MASK) == 0) {
                ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
            }
        }
        if (ctx->mode == I2C_MODE_TX && ctx->xfered_tx_len < ctx->tx_len) {
            return;
        }
    }

    /*
     * Per ATCIIC100 Datasheet Section 5.3.1 Step 7:
     * Slave mode — clear FIFO after transfer completes to prepare
     * for the next transaction (avoids stale data in FIFO).
     */
    if (ctx->role == I2C_SLAVE) {
        i2c_clear_fifo_er8130(i2c_channel[i2c_index]);
    }

    hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_TRANSFER_DONE);
}
static HAL_STATUS hal_drv_i2c_transfer(uint8_t i2c_idx, bool role, bool dma_enable, I2C_TRANSFER_MODE_T xfer_mode, uint8_t *ptr_fifo, uint32_t len,
                                       I2C_MSG_T *xfer_msg)
{
    /* Preserve slave TX response buffer across re-arm */
    uint8_t *saved_slave_tx_buf = i2c_context[i2c_idx].slave_tx_buf;
    uint16_t saved_slave_tx_len = i2c_context[i2c_idx].slave_tx_len;
    memset(&i2c_context[i2c_idx], 0, sizeof(I2C_TRANSFER_CONTEXT_T));
    i2c_context[i2c_idx].slave_tx_buf = saved_slave_tx_buf;
    i2c_context[i2c_idx].slave_tx_len = saved_slave_tx_len;

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
    i2c_probe_done_latched[i2c_idx] = false;
    i2c_probe_ack_latched[i2c_idx] = false;
    i2c_probe_arb_lost_latched[i2c_idx] = false;

    if (role == I2C_MASTER) {
        i2c_master_xfer_config_er8130(i2c_channel[i2c_idx], xfer_msg->en_stop, xfer_msg->en_data, xfer_msg->en_addr, xfer_msg->en_start, xfer_mode);
        i2c_set_data_byte_count_er8130(i2c_channel[i2c_idx], len);
    }

    return i2c_transfer_er8130(i2c_channel[i2c_idx], role, dma_enable, xfer_mode);
}

HAL_STATUS hal_drv_i2c_config(uint8_t i2c_index, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp,
                              uint8_t hddat, uint8_t scl_ratio, uint16_t scl_hi, bool dir, uint16_t target_address)
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

void hal_drv_i2c_slave_set_tx_buf(uint8_t i2c_idx, uint8_t *buf, uint16_t len)
{
    i2c_context[i2c_idx].slave_tx_buf = buf;
    i2c_context[i2c_idx].slave_tx_len = len;
}

uint8_t hal_drv_i2c_slave_get_rx_count(uint8_t i2c_idx)
{
    return i2c_context[i2c_idx].recved_rx_len;
}

void i2c_irq_handler(uint8_t i2c_idx)
{
    uint32_t int_status = i2c_get_status(i2c_channel[i2c_idx]);

    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)&i2c_context[i2c_idx];

    if (ctx->role == I2C_MASTER && (int_status & I2C_STS_ArbLose_MASK)) {
        i2c_probe_arb_lost_latched[i2c_idx] = true;
    }

    /*
     * Fast repeated-start masters can present write-phase CMPL and the
     * following read-phase AddrHit in the same IRQ snapshot.  In that case,
     * finishing the RX phase first lets the callback prepare the slave TX
     * buffer before AddrHit switches into TX/preload, avoiding a dummy first
     * read byte.
     */
    if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_RX
        && (int_status & I2C_STS_Cmpl_MASK)
        && (int_status & I2C_STS_AddrHit_MASK)) {
        int_status &= ~I2C_STS_Cmpl_MASK;
        i2c_transfer_end_interrupt_handler(&i2c_context[i2c_idx], i2c_idx);
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_CMPL, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_Cmpl_MASK);
    }

    /*
     * ISR priority order (per ATCIIC100 Datasheet):
     *   1. AddrHit  — must be first so Repeated Start sets direction
     *                 before FIFO/Cmpl handlers run with stale mode
     *   2. FIFOEmpty — TX data push
     *   3. FIFOFull  — RX data pull
     *   4. Cmpl      — transfer complete (clear status for next xfer)
     */
    if (int_status & I2C_STS_AddrHit_MASK) {
        int_status &= ~I2C_STS_AddrHit_MASK;
        i2c_address_hit_interrupt_handler(&i2c_context[i2c_idx], i2c_idx);
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_ADDR_HIT, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_AddrHit_MASK);
    }

    if ((int_status & I2C_STS_FIFOEmpty_MASK)) {
        int_status &= ~I2C_STS_FIFOEmpty_MASK;
        if (!(ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_TX)) {
            hal_drv_i2c_push_data(i2c_idx, ctx);
        }
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_FIFO_EMP, ctx);
    }

    if (int_status & I2C_STS_ByteTrans_MASK) {
        int_status &= ~I2C_STS_ByteTrans_MASK;
        if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_TX) {
            hal_drv_i2c_push_data(i2c_idx, ctx);
        }
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_BYTE_TX, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_ByteTrans_MASK);
    }

    if ((int_status & I2C_STS_FIFOFull_MASK)) {
        int_status &= ~I2C_STS_FIFOFull_MASK;
        hal_drv_i2c_pull_data(i2c_idx, ctx);
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_FIFO_FUL, ctx);
    }

    if (int_status & I2C_STS_Cmpl_MASK) {
        if (ctx->role == I2C_MASTER) {
            i2c_probe_done_latched[i2c_idx] = true;
            i2c_probe_ack_latched[i2c_idx] = !i2c_is_ack_status_er8130(int_status);
            i2c_probe_arb_lost_latched[i2c_idx] = i2c_is_arb_lost_status_er8130(int_status);
        }
        int_status &= ~I2C_STS_Cmpl_MASK;
        i2c_transfer_end_interrupt_handler(&i2c_context[i2c_idx], i2c_idx);
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_CMPL, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_Cmpl_MASK);
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
