/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_uart.h"
#include "hdl/uart_er8130.h"
#include "hal_intf_uart.h"
#include <stdint.h>

typedef void (*uart_dispatch_fn_t)(uint8_t uart_idx);

static uart_dispatch_fn_t s_uart_dispatch[CHIP_UART_CHANNEL_NUM];
static CALLBACK_ENTRY_T s_uart_callbacks[UART_MAX_EVENTS];
static uint8_t s_uart_last_lsr[CHIP_UART_CHANNEL_NUM];

const uint32_t UART_BASE[CHIP_UART_CHANNEL_NUM] = {UART0_BASEADDR, UART1_BASEADDR, UART2_BASEADDR};
const int32_t UART_IRQ_NO[CHIP_UART_CHANNEL_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};

static void uart_isr_body(uint8_t uart_idx)
{
    uint32_t int_status = hal_drv_uart_get_int_status(uart_idx);

    if (int_status & UART_ERRINT) {
        s_uart_last_lsr[uart_idx] = uart_get_lsr_er8130(UART_BASE[uart_idx]);
    }

    if ((int_status & UART_RXINT) || (int_status & UART_TOINT)) {
        if (s_uart_callbacks[HAL_UART_EVT_RX_FIN].callback) {
            s_uart_callbacks[HAL_UART_EVT_RX_FIN].callback(
                s_uart_callbacks[HAL_UART_EVT_RX_FIN].context, uart_idx);
        }
    }

    if (int_status & UART_TXINT) {
        if (s_uart_callbacks[HAL_UART_EVT_TX_FIN].callback) {
            s_uart_callbacks[HAL_UART_EVT_TX_FIN].callback(
                s_uart_callbacks[HAL_UART_EVT_TX_FIN].context, uart_idx);
        }
    }
}

static void hal_drv_uart_init_all_channels(void)
{
    uint8_t ch;

    for (ch = 0; ch < CHIP_UART_CHANNEL_NUM; ch++) {
        s_uart_dispatch[ch] = uart_isr_body;
    }
}

void hal_drv_uart_reset_fifo(uint8_t uart_idx, DRV_UART_RESET_TYPE type)
{
    uart_reset_fifo_er8130(UART_BASE[uart_idx], type);
}

void hal_drv_uart_config(uint8_t uart_idx, uint32_t baudrate, uint8_t data, uint8_t parity, uint8_t stop, uint8_t dmaConfig)
{
    uart_config_er8130(UART_BASE[uart_idx], baudrate, data, parity, stop, dmaConfig);
}

uint8_t hal_drv_uart_get_int_status(uint8_t uart_idx)
{
    return uart_get_int_status_er8130(UART_BASE[uart_idx]);
}

void hal_drv_uart_write_byte(uint8_t uart_idx, uint8_t ch)
{
    uart_write_byte_er8130(UART_BASE[uart_idx], ch);
}

int8_t hal_drv_uart_read_byte(uint8_t uart_idx, uint8_t *ch)
{
    return uart_read_byte_er8130(UART_BASE[uart_idx], ch);
}

uint32_t hal_drv_uart_write_data(uint8_t uart_idx, uint8_t *data, uint32_t req_len)
{
    uint32_t tx_len = 0;
    uint32_t uart   = UART_BASE[uart_idx];

    if (req_len == 0) {
        return 0;
    }

    for (tx_len = 0; tx_len < req_len; tx_len++) {
        uart_write_byte_er8130(uart, data[tx_len]);
    }

    return req_len;
}

void hal_drv_uart_irq_enable(uint8_t uart_idx, uint8_t int_type)
{
    uint32_t intFlag = 0;

    if (int_type & HAL_UART_EVT_RX_FIN) {
        intFlag |= UART_INTEN_ERBI_Msk;
        intFlag |= UART_INTEN_ELSI_Msk;    /* enable receiver line status interrupt */
    }
    if (int_type & HAL_UART_EVT_TX_FIN) {
        intFlag |= UART_INTEN_ETHEI_Msk;
    }

    uart_irq_en_er8130(UART_BASE[uart_idx], intFlag);

    if(int_type != 0) {
        NVIC_EnableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
    }
    else {
        NVIC_DisableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
    }
}

void hal_drv_uart_irq_disable(uint8_t uart_idx) //kevinyang, 20251112, depreciated.
{
    uart_irq_dis_er8130(UART_BASE[uart_idx]);
    NVIC_DisableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
}

int32_t hal_drv_uart_write_char(uint8_t uart_idx, uint8_t ch)
{
    uart_write_byte_er8130(UART_BASE[uart_idx], ch);
    return HAL_NO_ERR;
}

int32_t hal_drv_uart_read_char(uint8_t uart_idx, uint8_t *ch)
{
    uint32_t timeout = UART_TIMEOUT;

    while (uart_read_byte_er8130(UART_BASE[uart_idx], ch) != HAL_NO_ERR) {
        if (--timeout == 0) {
            return HAL_ERR;
        }
    }
    return HAL_NO_ERR;
}

int32_t hal_drv_uart_wait_tx_done(uint8_t uart_idx)
{
    return uart_wait_tx_done_er8130(UART_BASE[uart_idx]);
}

uint8_t hal_drv_uart_get_lsr(uint8_t uart_idx)
{
    return uart_get_lsr_er8130(UART_BASE[uart_idx]);
}

uint8_t hal_drv_uart_get_last_lsr(uint8_t uart_idx)
{
    uint8_t lsr = s_uart_last_lsr[uart_idx];
    s_uart_last_lsr[uart_idx] = 0;
    return lsr;
}

void hal_drv_uart_set_fifo_trigger(uint8_t uart_idx, uint8_t rx_trigger, uint8_t tx_trigger)
{
    uart_set_fifo_trigger_er8130(UART_BASE[uart_idx], rx_trigger, tx_trigger);
}

void hal_drv_uart_set_break(uint8_t uart_idx, uint8_t enable)
{
    uart_set_break_er8130(UART_BASE[uart_idx], enable);
}

void hal_drv_uart_set_flow_control(uint8_t uart_idx, uint8_t enable)
{
    uart_set_flow_control_er8130(UART_BASE[uart_idx], enable);
}

HAL_STATUS hal_drv_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    uint32_t primask;

    if (event >= UART_MAX_EVENTS) {
        return HAL_ERR;
    }

    hal_drv_uart_init_all_channels();

    primask = __get_PRIMASK();
    __disable_irq();
    s_uart_callbacks[event].callback = cb;
    s_uart_callbacks[event].context  = context;
    if (!primask) {
        __enable_irq();
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_uart_unregister_callback(UART_CB_EVENT_T event)
{
    uint32_t primask;

    if (event >= UART_MAX_EVENTS) {
        return HAL_ERR;
    }

    primask = __get_PRIMASK();
    __disable_irq();
    s_uart_callbacks[event].callback = NULL;
    s_uart_callbacks[event].context  = NULL;
    if (!primask) {
        __enable_irq();
    }
    return HAL_NO_ERR;
}

WEAK_ISR void UART0_IRQHandler(void)
{
    uart_dispatch_fn_t fn = s_uart_dispatch[0];
    if (fn) {
        fn(0);
    }
}

WEAK_ISR void UART1_IRQHandler(void)
{
    uart_dispatch_fn_t fn = s_uart_dispatch[1];
    if (fn) {
        fn(1);
    }
}

WEAK_ISR void UART2_IRQHandler(void)
{
    uart_dispatch_fn_t fn = s_uart_dispatch[2];
    if (fn) {
        fn(2);
    }
}
