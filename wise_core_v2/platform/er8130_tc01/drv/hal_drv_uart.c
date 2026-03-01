/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_uart.h"
#include "hdl/uart_er8130.h"
#include "hal_intf_uart.h"
#include <stdint.h>

static CALLBACK_ENTRY_T uart_callbacks[UART_MAX_EVENTS];
const uint32_t UART_BASE[CHIP_UART_CHANNEL_NUM] = {UART0_BASEADDR, UART1_BASEADDR, UART2_BASEADDR};
const int32_t UART_IRQ_NO[CHIP_UART_CHANNEL_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};

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

    if (int_type & HAL_UART_EVT_RX_FIN)
        intFlag |= UART_INTEN_ERBI_Msk;
    if (int_type & HAL_UART_EVT_TX_FIN)
        intFlag |= UART_INTEN_ETHEI_Msk; // kevinyang, 20241220, not sure
    
    uart_irq_en_er8130(UART_BASE[uart_idx], intFlag);

    if(int_type != 0)
        NVIC_EnableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
    else
        NVIC_DisableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
}

void hal_drv_uart_irq_disable(uint8_t uart_idx) //kevinyang, 20251112, depreciated.
{
    uart_irq_dis_er8130(UART_BASE[uart_idx]);
    NVIC_DisableIRQ((IRQn_Type)UART_IRQ_NO[uart_idx]);
}

HAL_STATUS hal_drv_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    if (event < UART_MAX_EVENTS) {
        uart_callbacks[event].callback = cb;
        uart_callbacks[event].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_uart_unregister_callback(UART_CB_EVENT_T event)
{
    if (event < UART_MAX_EVENTS) {
        uart_callbacks[event].callback = NULL;
        uart_callbacks[event].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_uart_trigger_callback(UART_CB_EVENT_T event, uint8_t gptmr_idx)
{
    if (uart_callbacks[event].callback) {
        uart_callbacks[event].callback(uart_callbacks[event].context, gptmr_idx);
    }
}

void uart_IRQHandler(uint8_t uart_idx)
{
    uint32_t int_status = hal_drv_uart_get_int_status(uart_idx);

    if ((int_status & UART_RXINT) || (int_status & UART_TOINT)) {
        hal_drv_uart_trigger_callback(HAL_UART_EVT_RX_FIN, uart_idx);
    }

    if (int_status & UART_TXINT) {
        hal_drv_uart_trigger_callback(HAL_UART_EVT_TX_FIN, uart_idx);
    }
}

WEAK_ISR void UART0_IRQHandler()
{
    uart_IRQHandler(0);
}

WEAK_ISR void UART1_IRQHandler()
{
    uart_IRQHandler(1);
}

WEAK_ISR void UART2_IRQHandler()
{
    uart_IRQHandler(2);
}
