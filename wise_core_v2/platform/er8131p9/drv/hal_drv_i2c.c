/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_i2c.h"
#include "hdl/i2c_er8130.h"
#include "hal_intf_dma.h"
#include <stdint.h>

/*
 * Dir polling iterations for slave AddrHit.
 * At 40 MHz core, each iteration ≈ 175 ns.  80000 iterations ≈ 14 ms.
 * At ≥2 kHz Dir updates well within this window and the loop exits
 * early.  The fast-path poll also has an adaptive STS check that
 * exits on Cmpl/ByteRecv for write transactions, preventing
 * excessive ISR blocking.
 *
 * WARNING: values >80000 cause cascading state corruption at 1 kHz
 * that poisons subsequent higher-frequency transactions.
 */
#define I2C_DIR_POLL_ITERS 80000

/*
 * Place ISR hot-path functions in SRAM for faster execution.
 * Set to 0 to run from flash (for comparison / code-size testing).
 */
#define I2C_ISR_RUN_IN_RAM 1

#if I2C_ISR_RUN_IN_RAM
#define I2C_ISR_FUNC  RAM_TEXT
#else
#define I2C_ISR_FUNC
#endif

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
    /* Set when switching RX→TX on Sr; cleared when write-phase CMPL is consumed */
    bool sr_write_cmpl_pending;
    /* Set when write-phase CMPL arrives before the Sr read AddrHit */
    bool sr_rx_cmpl_deferred;
    /* AddrHit preloaded byte0 and returned; drain+callback deferred to ByteTrans */
    bool slave_addr_hit_deferred;
} I2C_TRANSFER_CONTEXT_T;

typedef void (*I2C_IRQ_HANDLER_T)(uint8_t i2c_idx);

I2C_T *i2c_channel[CHIP_I2C_CHANNEL_NUM] = {I2C0, I2C1};
EVT_CALLBACK_ENTRY_T i2c_callbacks[CHIP_I2C_CHANNEL_NUM][I2C_MAX_EVENTS];
static I2C_TRANSFER_CONTEXT_T i2c_context[CHIP_I2C_CHANNEL_NUM];
static I2C_IRQ_HANDLER_T _i2c_irq_handler[CHIP_I2C_CHANNEL_NUM] = {NULL};

static void i2c_irq_handler(uint8_t i2c_idx);

static void hal_drv_i2c_bind_dispatch(uint8_t i2c_index)
{
    _i2c_irq_handler[i2c_index] = i2c_irq_handler;
}

#define I2C_DEBUG_TRACE_DEPTH 32
#define I2C_DBG_EVT_ADDR_HIT  1
#define I2C_DBG_EVT_FIFO_EMP  2
#define I2C_DBG_EVT_BYTE_TX   3
#define I2C_DBG_EVT_FIFO_FUL  4
#define I2C_DBG_EVT_CMPL      5
#define I2C_DBG_EVT_CMPL_SR   6   /* write-phase CMPL consumed (sr_pending) */
#define I2C_DBG_EVT_CMPL_INL  7   /* CMPL consumed inline (combined CMPL+AddrHit) */
#define I2C_DBG_EVT_REARM     8   /* main loop re-arm */
#define I2C_DBG_EVT_CMPL_DEF  9   /* RX write-phase CMPL deferred awaiting Sr */
#define I2C_DBG_EVT_PRELOAD  10   /* slave TX byte0 preload in ADDRESS_HIT */
#define I2C_DBG_EVT_FAST_AH 11   /* ultra-fast AddrHit path (preload+return) */
#define I2C_DBG_EVT_FAST_SR 12   /* ultra-fast CMPL+AddrHit path (drain+preload+return) */

#if HAL_I2C_DEBUG_TRACE_ENABLE
static HAL_I2C_DEBUG_TRACE_T i2c_debug_trace[CHIP_I2C_CHANNEL_NUM][I2C_DEBUG_TRACE_DEPTH];
static uint8_t i2c_debug_trace_count[CHIP_I2C_CHANNEL_NUM];
#endif
static bool i2c_probe_done_latched[CHIP_I2C_CHANNEL_NUM];
static bool i2c_probe_ack_latched[CHIP_I2C_CHANNEL_NUM];
static bool i2c_probe_arb_lost_latched[CHIP_I2C_CHANNEL_NUM];

I2C_ISR_FUNC static void hal_drv_i2c_trigger_event(uint8_t i2c_channel, uint32_t i2c_event);
I2C_ISR_FUNC static void hal_drv_i2c_pull_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
I2C_ISR_FUNC static void hal_drv_i2c_push_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx);
static void i2c_irq_handler(uint8_t i2c_idx);

I2C_ISR_FUNC static void i2c_debug_trace_log(uint8_t i2c_idx, uint8_t evt, I2C_TRANSFER_CONTEXT_T *ctx)
{
#if HAL_I2C_DEBUG_TRACE_ENABLE
    uint8_t pos = i2c_debug_trace_count[i2c_idx];
    if (pos >= I2C_DEBUG_TRACE_DEPTH) {
        return;
    }

    i2c_debug_trace[i2c_idx][pos].evt        = evt;
    i2c_debug_trace[i2c_idx][pos].tx_len     = (uint8_t)ctx->xfered_tx_len;
    i2c_debug_trace[i2c_idx][pos].rx_len     = (uint8_t)ctx->debug_rx_drained_len;
    i2c_debug_trace[i2c_idx][pos].hw_dir     = (uint8_t)i2c_channel[i2c_idx]->CTRL.bitfield.Dir;
    i2c_debug_trace[i2c_idx][pos].mode       = (uint8_t)ctx->mode;
    i2c_debug_trace[i2c_idx][pos].sr_pending = (uint8_t)ctx->sr_write_cmpl_pending;
    i2c_debug_trace[i2c_idx][pos].data_cnt   = i2c_get_data_byte_count_er8130(i2c_channel[i2c_idx]);
    i2c_debug_trace[i2c_idx][pos].fifo_size  = (uint8_t)ctx->fifo_size;
    i2c_debug_trace[i2c_idx][pos].status     = (uint16_t)(i2c_get_status(i2c_channel[i2c_idx]) & 0xFFFFu);
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

I2C_ISR_FUNC static void hal_drv_i2c_pull_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx)
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

I2C_ISR_FUNC static void hal_drv_i2c_push_data(uint8_t i2c_index, I2C_TRANSFER_CONTEXT_T *ctx)
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

I2C_ISR_FUNC static void i2c_address_hit_interrupt_handler(void *context, uint8_t i2c_index)
{
    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)context;

    /*
     * Per ATCIIC100 Datasheet Section 5.3.1 Step 4:
     * Read the Dir field of the Control Register to identify the
     * transaction direction.
     *   Slave Dir=0 → Receiver  (master writes to us)
     *   Slave Dir=1 → Transmitter (master reads from us)
     */
    /*
     * Poll Dir — HW may lag AddrHit by a few µs.
     * 16 iterations (~2 µs) is sufficient for ≥100 kHz.
     * At lower frequencies Dir may not update in time; caller
     * handles Dir=0 as RX (safe default).
     */
    uint8_t hw_dir = 0;
    for (int _d = 0; _d < I2C_DIR_POLL_ITERS; _d++) {
        hw_dir = i2c_channel[i2c_index]->CTRL.bitfield.Dir;
        if (hw_dir) {
            break;
        }
        __NOP(); __NOP();
    }

    if (ctx->role == I2C_SLAVE) {
        if (hw_dir == 1) {
            /* Slave Transmitter: master wants to read */
            I2C_T *hw = i2c_channel[i2c_index];

            /*
             * === LATENCY-CRITICAL: preload byte0 FIRST ===
             *
             * Direct register write — bypasses all Flash function
             * calls to minimize cycles before data is on the wire.
             * 1st DATA write after Dir→TX goes to shift register
             * (not FIFO), so RX FIFO data is still intact for the
             * drain below.
             *
             * Uses pre-registered slave_tx_buf (set by prior
             * TRANSFER_DONE callback or initial setup).
             */
            if (ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
                hw->DATA.reg = (uint32_t)ctx->slave_tx_buf[0];
            }

            /*
             * Drain remaining RX FIFO — direct register access.
             * byte0 preload only touched shift register, RX data
             * is preserved.  Callback needs this data to decide
             * what to send next time.
             *
             * IMPORTANT: use FIFOEmpty status bit, NOT DataCnt.
             * After preload (Dir→TX), DataCnt reflects the TX byte
             * and does NOT decrement on read, causing an infinite
             * drain loop that reads garbage and wastes critical time.
             */
            while (ctx->rx_fifo != NULL
                   && ctx->recved_rx_len < ctx->rx_len
                   && !(hw->STS.reg & I2C_STS_FIFOEmpty_MASK)) {
                ctx->rx_fifo[ctx->recved_rx_len] =
                    (uint8_t)hw->DATA.bitfield.DATAFIELD;
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
            }

            ctx->mode          = I2C_MODE_TX;
            ctx->xfered_tx_len = 1;
            ctx->sr_write_cmpl_pending = (ctx->recved_rx_len > 0)
                                         && !ctx->sr_rx_cmpl_deferred;
            ctx->sr_rx_cmpl_deferred = false;

            if (ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
                ctx->tx_fifo = ctx->slave_tx_buf;
                ctx->tx_len  = ctx->slave_tx_len;
            }

            /* Interrupt config — direct register writes */
            hw->INT_EN.bitfield.FIFOFull  = 0;
            hw->INT_EN.bitfield.FIFOEmpty = 0;
            if (ctx->xfered_tx_len < ctx->tx_len) {
                hw->INT_EN.bitfield.ByteTrans = 1;
            } else {
                hw->INT_EN.bitfield.ByteTrans = 0;
            }

            i2c_debug_trace_log(i2c_index, I2C_DBG_EVT_PRELOAD, ctx);

            /*
             * Callback AFTER preload — byte0 is already committed
             * to shift register.  User can inspect RX data and
             * optionally change tx_buf; ByteTrans ISR will use the
             * updated buffer for byte1+.
             */
            hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_ADDRESS_HIT);

            /* If callback changed tx_buf, update for ByteTrans */
            if (ctx->slave_tx_buf != ctx->tx_fifo
                && ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
                ctx->tx_fifo = ctx->slave_tx_buf;
                ctx->tx_len  = ctx->slave_tx_len;
            }
        } else {
            /* Slave Receiver: master wants to write */
            ctx->mode          = I2C_MODE_RX;
            ctx->recved_rx_len = 0;
            ctx->debug_rx_drained_len = 0;
            ctx->sr_write_cmpl_pending = false;
            ctx->sr_rx_cmpl_deferred = false;
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

I2C_ISR_FUNC static void i2c_transfer_end_interrupt_handler(void *context, uint8_t i2c_index)
{
    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)context;
    uint32_t status = i2c_get_status(i2c_channel[i2c_index]);

    i2c_clear_cmpl_status_er8130(i2c_channel[i2c_index]);

    /*
     * Drain remaining RX data from FIFO BEFORE the Sr-deferred check.
     * For a single command byte (e.g. 0x05), the data sits in the FIFO
     * because FIFOFull never fires.  We must pull it now so recved_rx_len
     * reflects the actual received count for the deferred decision.
     */
    if (ctx->dma_en == DISABLE
        && ctx->mode == I2C_MODE_RX
        && ctx->recved_rx_len < ctx->rx_len) {
        while (ctx->recved_rx_len < ctx->rx_len
               && (i2c_get_status(i2c_channel[i2c_index]) & I2C_STS_FIFOEmpty_MASK) == 0) {
            ctx->rx_fifo[ctx->recved_rx_len] = i2c_get_data_er8130(i2c_channel[i2c_index]);
            ctx->recved_rx_len++;
            ctx->debug_rx_drained_len++;
        }
    }

    /*
     * Slave RX CMPL without STOP is a write phase that is still open on bus.
     * Defer completion so a following repeated-start read stays in the same
     * logical transaction instead of forcing the app to re-arm too early.
     *
     * Now that FIFO has been drained above, recved_rx_len correctly reflects
     * whether the master actually sent data (command byte) before the Sr.
     */
    if (ctx->role == I2C_SLAVE
        && ctx->mode == I2C_MODE_RX
        && ctx->recved_rx_len > 0
        && (status & I2C_STS_Stop_MASK) == 0) {
        ctx->sr_rx_cmpl_deferred = true;
        i2c_debug_trace_log(i2c_index, I2C_DBG_EVT_CMPL_DEF, ctx);
        return;
    }

    /*
     * Write-phase residual CMPL during an active slave TX (Sr transaction):
     * The master asserted Repeated Start so quickly that the write-phase CMPL
     * arrives after AddrHit already switched us into TX mode.
     *
     * We must NOT disable TX interrupts or clear the FIFO here — the read
     * phase is still in progress and the master is actively clocking data.
     * Just consume this stale CMPL silently; the read-phase CMPL will fire
     * TRANSFER_DONE when the master finally STOPs.
     */
    if (ctx->role == I2C_SLAVE && ctx->sr_write_cmpl_pending) {
        ctx->sr_write_cmpl_pending = false;
        ctx->sr_rx_cmpl_deferred = false;
        i2c_debug_trace_log(i2c_index, I2C_DBG_EVT_CMPL_SR, ctx);
        return;
    }

    if (ctx->role == I2C_MASTER) {
        i2c_disable_all_interrupts_er8130(i2c_channel[i2c_index]);
    }
    if (ctx->dma_en == DISABLE) {
        if (ctx->mode == I2C_MODE_TX && ctx->xfered_tx_len < ctx->tx_len) {
            /*
             * Master mode: TX still in progress, wait for more interrupts.
             * Slave mode: master NACK'd early — transaction is done, fall
             * through to fire TRANSFER_DONE so the app can re-arm.
             */
            if (ctx->role == I2C_MASTER) {
                return;
            }
        }
    }

    /*
     * Per ATCIIC100 Datasheet Section 5.3.1 Step 7:
     * Slave mode — clear FIFO after transfer completes to prepare
     * for the next transaction (avoids stale data in FIFO).
     */
    if (ctx->role == I2C_SLAVE) {
        ctx->sr_write_cmpl_pending = false;
        ctx->sr_rx_cmpl_deferred = false;
        ctx->slave_addr_hit_deferred = false;
        i2c_clear_fifo_er8130(i2c_channel[i2c_index]);
    }

    hal_drv_i2c_trigger_event(i2c_index, I2C_EVENT_TRANSFER_DONE);
}
static HAL_STATUS hal_drv_i2c_transfer(uint8_t i2c_idx, bool role, bool dma_enable, I2C_TRANSFER_MODE_T xfer_mode, uint8_t *ptr_fifo, uint32_t len,
                                       I2C_MSG_T *xfer_msg)
{
    /*
     * Slave re-arm: the ENTIRE sequence (memset + context setup +
     * i2c_transfer_er8130 + Dir=1 override) must be atomic w.r.t. the
     * I2C ISR.  Use __disable_irq() (CPSID) which globally masks all
     * interrupts and CANNOT be overridden by NVIC_EnableIRQ inside
     * i2c_transfer_er8130().
     *
     * Previous bug: NVIC_DisableIRQ before memset was defeated because
     * i2c_transfer_er8130 internally calls NVIC_EnableIRQ, opening a
     * window where the ISR could fire with Dir=0 before our Dir=1
     * override.  This caused intermittent Direct Read failures.
     */
    if (role == I2C_SLAVE) {
        __disable_irq();
    }

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

    HAL_STATUS ret = i2c_transfer_er8130(i2c_channel[i2c_idx], role, dma_enable, xfer_mode);

    if (role == I2C_SLAVE) {
        i2c_channel[i2c_idx]->CTRL.bitfield.Dir = 1;
        __enable_irq();
    }

    return ret;
}

HAL_STATUS hal_drv_i2c_config(uint8_t i2c_index, bool i2c_enable, bool role, bool addressing, bool dma_enable, uint8_t sudat, uint8_t sp,
                              uint8_t hddat, uint8_t scl_ratio, uint16_t scl_hi, bool dir, uint16_t target_address)
{
    i2c_config_er8130(i2c_channel[i2c_index], i2c_enable, role, addressing, dma_enable, sudat, sp, hddat, scl_ratio, scl_hi, dir, target_address);

    hal_drv_i2c_bind_dispatch(i2c_index);
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
        /*
         * Register the same callback for ALL event slots.
         * trigger_event() uses the event enum as the array index,
         * so the callback must be present in every slot to receive
         * both TRANSFER_DONE (slot 0) and ADDRESS_HIT (slot 1).
         * The API layer uses a single unified dispatcher
         * (_i2c_slave_watchdog_cb) that demuxes events internally.
         */
        for (int i = 0; i < I2C_MAX_EVENTS; i++) {
            i2c_callbacks[i2c_channel][i].callback = cb;
            i2c_callbacks[i2c_channel][i].context  = context;
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
I2C_ISR_FUNC static void hal_drv_i2c_trigger_event(uint8_t i2c_channel, uint32_t i2c_event)
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

I2C_ISR_FUNC void i2c_irq_handler(uint8_t i2c_idx)
{
    I2C_T *hw = i2c_channel[i2c_idx];
    uint32_t int_status = hw->STS.reg;
    /*
     * Snapshot Dir IMMEDIATELY — before any processing.
     * Hardware sets Dir from the address byte's R/W bit BEFORE
     * AddrHit fires, so Dir is correct at ISR entry.
     *
     * The old Dir poll was racing against push-pull masters that
     * complete the entire read transaction (360 µs at 100 kHz)
     * during the poll, causing Dir to revert and Cmpl to fire,
     * misclassifying reads as writes (Dir=0).
     */
    uint8_t dir_snapshot = hw->CTRL.bitfield.Dir;

    I2C_TRANSFER_CONTEXT_T *ctx = (I2C_TRANSFER_CONTEXT_T *)&i2c_context[i2c_idx];

    /*
     * Ultra-fast path: slave TX AddrHit (no simultaneous CMPL).
     * Preload byte0 into shift register and RETURN IMMEDIATELY so
     * that ByteTrans ISR can fire for byte1 within the byte0 clock
     * window (~80 µs at 100 kHz).
     *
     * Everything else (RX drain, callback, trace) is deferred to the
     * next ByteTrans ISR entry, AFTER byte1 has been pushed.
     */
    if (ctx->role == I2C_SLAVE
        && (int_status & I2C_STS_AddrHit_MASK)
        && !(int_status & I2C_STS_Cmpl_MASK)
        && ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
        /*
         * Repeated-START: if prior RX data exists, fire the ADDRESS_HIT
         * callback FIRST so the app can switch the TX buffer.
         */
        bool cb_done_inline = false;
        if (ctx->recved_rx_len > 0) {
            hal_drv_i2c_trigger_event(i2c_idx, I2C_EVENT_ADDRESS_HIT);
            cb_done_inline = true;
        }

        /*
         * Use the Dir snapshot captured at ISR entry — no polling.
         * Hardware sets Dir from the R/W bit BEFORE asserting AddrHit,
         * so dir_snapshot is the definitive direction.
         *
         * The old poll (80000 iterations) raced against push-pull
         * masters that completed the entire read in <400 µs.  Cmpl
         * appeared mid-poll and forced a Dir=0 early-exit, misclassifying
         * reads as writes.  Removing the poll eliminates this race.
         */
        if (dir_snapshot == 1) {
            /*
             * Confirmed TX — preload byte[0] + burst-fill.
             * CRITICAL: use reg write (pure STR), NOT bitfield
             * write (LDR+BFI+STR pops FIFO on read side-effect).
             */
            hw->DATA.reg = (uint32_t)ctx->slave_tx_buf[0];
            uint8_t prefill = 1;
            {
                uint8_t room = ctx->fifo_size;
                while (prefill < ctx->slave_tx_len && room > 0) {
                    hw->DATA.reg = (uint32_t)ctx->slave_tx_buf[prefill];
                    __DSB();
                    prefill++;
                    room--;
                }
            }
            ctx->tx_fifo       = ctx->slave_tx_buf;
            ctx->tx_len        = ctx->slave_tx_len;
            ctx->mode          = I2C_MODE_TX;
            ctx->xfered_tx_len = prefill;
            ctx->sr_write_cmpl_pending = (ctx->recved_rx_len > 0)
                                         && !ctx->sr_rx_cmpl_deferred;
            ctx->sr_rx_cmpl_deferred   = false;
            hw->INT_EN.bitfield.FIFOFull  = 0;
            hw->INT_EN.bitfield.FIFOEmpty = 0;
            hw->INT_EN.bitfield.ByteTrans =
                (prefill < ctx->slave_tx_len) ? 1 : 0;
            ctx->slave_addr_hit_deferred = !cb_done_inline;
            i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_FAST_AH, ctx);
            hw->STS.reg = I2C_STS_AddrHit_MASK;
            return;
        }
        /*
         * Dir=0 — real write (master sending data to us).
         * No TX data was written, so FIFO is clean for RX.
         */
        ctx->mode                  = I2C_MODE_RX;
        ctx->recved_rx_len         = 0;
        ctx->debug_rx_drained_len  = 0;
        ctx->sr_write_cmpl_pending = false;
        ctx->sr_rx_cmpl_deferred   = false;
        hw->INT_EN.bitfield.ByteTrans = 0;
        hw->INT_EN.bitfield.FIFOEmpty = 0;
        hw->INT_EN.bitfield.FIFOFull  = 1;
        hal_drv_i2c_trigger_event(i2c_idx, I2C_EVENT_ADDRESS_HIT);
        hw->STS.reg = I2C_STS_AddrHit_MASK;
        return;
    }

    /*
     * Fast path: slave TX ByteTrans — push data with absolute minimum
     * latency.  Direct register access, no function calls through Flash.
     *
     * After pushing the byte, run any deferred ADDRESS_HIT work
     * (RX drain + callback).  We have ~80 µs until byte2 is needed.
     */
    if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_TX
        && (int_status & I2C_STS_ByteTrans_MASK)
        && !(int_status & (I2C_STS_AddrHit_MASK | I2C_STS_Cmpl_MASK))) {
        /* Push byte[1] — goes to shift register (FIFO was empty).
         * Use reg write to avoid read-modify-write side effects on DATA. */
        if (ctx->xfered_tx_len < ctx->tx_len) {
            hw->DATA.reg = (uint32_t)ctx->tx_fifo[ctx->xfered_tx_len];
            __DSB();
            ctx->xfered_tx_len++;
        }

        /*
         * Push ALL remaining bytes to FIFO before the slow deferred callback.
         * byte[1] is now in shift register.  Push byte[2..N] into FIFO.
         * At 100kHz each byte takes ~90µs, so the callback has plenty of
         * time.  But if we only push byte[2] and defer byte[3] to the
         * next ByteTrans, the master may finish before byte[3] is ready
         * (observed: CMPL fires at tx=3, byte[3] reads as 0xFF).
         *
         * FIFO room: shift register holds byte[1], FIFO has fifo_size
         * slots.  For 4-byte payload + fifo_size=4: push byte[2,3] →
         * 2 FIFO entries, well within capacity.
         */
        if (ctx->slave_addr_hit_deferred) {
            uint8_t room = ctx->fifo_size;
            while (ctx->xfered_tx_len < ctx->tx_len && room > 0) {
                hw->DATA.reg = (uint32_t)ctx->tx_fifo[ctx->xfered_tx_len];
                __DSB();
                ctx->xfered_tx_len++;
                room--;
            }
        }

        if (ctx->xfered_tx_len >= ctx->tx_len) {
            hw->INT_EN.bitfield.ByteTrans = 0;
        }
        hw->STS.reg = I2C_STS_ByteTrans_MASK | I2C_STS_FIFOEmpty_MASK;

        /*
         * Deferred ADDRESS_HIT: all TX bytes are now safely in FIFO,
         * so we can afford the Flash-traversing callback latency.
         * This also signals the watchdog (slave_bus_active) so it can
         * detect stuck transactions and trigger recovery.
         */
        if (ctx->slave_addr_hit_deferred) {
            ctx->slave_addr_hit_deferred = false;
            i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_PRELOAD, ctx);
            hal_drv_i2c_trigger_event(i2c_idx, I2C_EVENT_ADDRESS_HIT);
            /* If callback changed tx_buf, update for any remaining bytes */
            if (ctx->slave_tx_buf != ctx->tx_fifo
                && ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
                ctx->tx_fifo = ctx->slave_tx_buf;
                ctx->tx_len  = ctx->slave_tx_len;
            }
        }
        return;
    }

    if (ctx->role == I2C_MASTER && (int_status & I2C_STS_ArbLose_MASK)) {
        i2c_probe_arb_lost_latched[i2c_idx] = true;
    }

    /*
     * Fast repeated-start masters can present write-phase CMPL and the
     * following read-phase AddrHit in the same IRQ snapshot.  In that case,
     * drain the write-phase RX data inline but do NOT fire TRANSFER_DONE —
     * the transaction continues with the read phase.
     */
    /*
     * Fast combined CMPL+AddrHit for Repeated Start (slave RX→TX).
     * Drain RX first (need cmd byte), call ADDRESS_HIT callback inline
     * so the app can switch TX buffer, then preload byte0.
     */
    if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_RX
        && (int_status & I2C_STS_Cmpl_MASK)
        && (int_status & I2C_STS_AddrHit_MASK)
        && ctx->slave_tx_buf != NULL && ctx->slave_tx_len > 0) {
        /*
         * Drain RX FIFO first — need cmd byte for callback.
         * Check FIFOEmpty to avoid reading garbage from an empty FIFO
         * (at high speed the HW may have already popped the byte to
         * the shift register for transmission).
         */
        {
            uint8_t max_drain = ctx->fifo_size;
            while (ctx->recved_rx_len < ctx->rx_len
                   && max_drain > 0
                   && !(hw->STS.reg & I2C_STS_FIFOEmpty_MASK)) {
                ctx->rx_fifo[ctx->recved_rx_len] =
                    (uint8_t)hw->DATA.bitfield.DATAFIELD;
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
                max_drain--;
            }
        }
        /* Fire callback INLINE so app can switch TX buf based on cmd */
        hal_drv_i2c_trigger_event(i2c_idx, I2C_EVENT_ADDRESS_HIT);
        /*
         * Clear FIFO before TX preload — ensures no stale RX data
         * remains that could leak into the TX output.  At high speed,
         * the drain above may miss bytes that the HW already moved
         * from FIFO to shift register; ClearFIFO guarantees a clean
         * starting point for the TX data.
         */
        i2c_clear_fifo_er8130(hw);
        /*
         * Preload byte[0] + burst-fill remaining into FIFO.
         * Same preload-first strategy as F_AH: data must be ready
         * before push-pull master starts clocking.
         */
        hw->DATA.reg = (uint32_t)ctx->slave_tx_buf[0];
        uint8_t fsr_prefill = 1;
        {
            uint8_t room = ctx->fifo_size;
            while (fsr_prefill < ctx->slave_tx_len && room > 0) {
                hw->DATA.reg = (uint32_t)ctx->slave_tx_buf[fsr_prefill];
                __DSB();
                fsr_prefill++;
                room--;
            }
        }
        /*
         * Dir poll — essential even for Sr path.  At low frequencies
         * (≤100 kHz) Dir hasn't updated from 0→1 yet when ISR runs.
         * Without this poll, preloaded data goes to RX FIFO (Dir=0)
         * instead of TX shift register, corrupting all subsequent I/O.
         */
        uint8_t fsr_dir = 0;
        for (int _d = 0; _d < I2C_DIR_POLL_ITERS; _d++) {
            fsr_dir = hw->CTRL.bitfield.Dir;
            if (fsr_dir) {
                break;
            }
            __NOP(); __NOP();
        }
        if (fsr_dir == 1) {
            ctx->tx_fifo       = ctx->slave_tx_buf;
            ctx->tx_len        = ctx->slave_tx_len;
            ctx->mode          = I2C_MODE_TX;
            ctx->xfered_tx_len = fsr_prefill;
            ctx->sr_write_cmpl_pending = false;
            ctx->sr_rx_cmpl_deferred   = false;
            hw->INT_EN.bitfield.FIFOFull  = 0;
            hw->INT_EN.bitfield.FIFOEmpty = 0;
            hw->INT_EN.bitfield.ByteTrans =
                (fsr_prefill < ctx->slave_tx_len) ? 1 : 0;
            ctx->slave_addr_hit_deferred = false;
            i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_FAST_SR, ctx);
            hw->STS.reg = I2C_STS_Cmpl_MASK | I2C_STS_AddrHit_MASK;
            return;
        }
        /*
         * Dir=0 at low frequency — data already in FIFO.
         * Set up as TX: for a read, master clocks out the preloaded
         * bytes.  For a true second write (unusual for Sr), the
         * preloaded bytes sit unused and CMPL handles cleanup.
         */
        ctx->tx_fifo       = ctx->slave_tx_buf;
        ctx->tx_len        = ctx->slave_tx_len;
        ctx->mode          = I2C_MODE_TX;
        ctx->xfered_tx_len = fsr_prefill;
        ctx->sr_write_cmpl_pending = false;
        ctx->sr_rx_cmpl_deferred   = false;
        hw->INT_EN.bitfield.FIFOFull  = 0;
        hw->INT_EN.bitfield.FIFOEmpty = 0;
        hw->INT_EN.bitfield.ByteTrans =
            (fsr_prefill < ctx->slave_tx_len) ? 1 : 0;
        ctx->slave_addr_hit_deferred = false;
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_FAST_SR, ctx);
        hw->STS.reg = I2C_STS_Cmpl_MASK | I2C_STS_AddrHit_MASK;
        return;
    }

    bool sr_cmpl_consumed_inline = false;
    if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_RX
        && (int_status & I2C_STS_Cmpl_MASK)
        && (int_status & I2C_STS_AddrHit_MASK)) {
        int_status &= ~I2C_STS_Cmpl_MASK;
        i2c_clear_cmpl_status_er8130(i2c_channel[i2c_idx]);
        {
            uint8_t max_drain = ctx->fifo_size;
            while (ctx->recved_rx_len < ctx->rx_len && max_drain > 0) {
                ctx->rx_fifo[ctx->recved_rx_len] =
                    (uint8_t)hw->DATA.bitfield.DATAFIELD;
                ctx->recved_rx_len++;
                ctx->debug_rx_drained_len++;
                max_drain--;
            }
        }
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_CMPL_INL, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_Cmpl_MASK);
        sr_cmpl_consumed_inline = true;
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
        /*
         * address_hit_handler sets sr_write_cmpl_pending=true when switching
         * RX→TX.  If the write-phase CMPL was already consumed inline above,
         * clear the flag so the read-phase CMPL is not mistakenly ignored.
         */
        if (sr_cmpl_consumed_inline && ctx->sr_write_cmpl_pending) {
            ctx->sr_write_cmpl_pending = false;
        }
        i2c_debug_trace_log(i2c_idx, I2C_DBG_EVT_ADDR_HIT, ctx);
        i2c_set_status(i2c_channel[i2c_idx], I2C_STS_AddrHit_MASK);

        /*
         * Slave TX fast exit: return immediately after preload so
         * ByteTrans ISR can fire ASAP.  At 100 kHz the master starts
         * clocking byte1 ~90 µs after byte0 — every µs counts.
         * Remaining status bits (FIFOEmpty, Cmpl) will be handled
         * in subsequent ISR entries if still pending.
         */
        if (ctx->role == I2C_SLAVE && ctx->mode == I2C_MODE_TX) {
            return;
        }
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
