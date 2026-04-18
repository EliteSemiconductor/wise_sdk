/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_uart.h"
#include "drv/hal_drv_uart.h"

void hal_intf_uart_reset_fifo(uint8_t uart_idx, HAL_UART_RESET_TYPE type)
{
    hal_drv_uart_reset_fifo(uart_idx, type);
}

int32_t hal_intf_uart_set_config(uint8_t uart_idx, HAL_UART_CFG_T *uart_cfg)
{
    if (!uart_cfg) {
        return HAL_ERR;
    }

#ifdef CHIP_HAS_DMA
    hal_drv_uart_config(uart_idx, uart_cfg->baudrate, uart_cfg->data, uart_cfg->parity, uart_cfg->stop, uart_cfg->dmaConfig);
#else
    hal_drv_uart_config(uart_idx, uart_cfg->baudrate, uart_cfg->data, uart_cfg->parity, uart_cfg->stop, 0);
#endif

    return HAL_NO_ERR;
}

int32_t hal_intf_uart_enable_irq(uint8_t uart_idx, uint8_t intFlag)
{
    hal_drv_uart_irq_enable(uart_idx, intFlag);

    return HAL_NO_ERR;
}

int32_t hal_intf_uart_disable_irq(uint8_t uart_idx)
{
    hal_drv_uart_irq_enable(uart_idx, 0); //kevinyang, 20251112
    //hal_drv_uart_irq_disable(uart_idx);
    
    return HAL_NO_ERR;
}

int32_t hal_intf_uart_write_data(uint8_t uart_idx, uint8_t *buf, uint32_t req_len)
{
    hal_drv_uart_write_data(uart_idx, buf, req_len);
    return HAL_NO_ERR;
}

int32_t hal_intf_uart_poll_byte(uint8_t uart_idx, uint8_t *outByte)
{
    return hal_drv_uart_read_byte(uart_idx, outByte);
}

int32_t hal_intf_uart_write_char(uint8_t uart_idx, uint8_t ch)
{
    return hal_drv_uart_write_char(uart_idx, ch);
}

int32_t hal_intf_uart_read_char(uint8_t uart_idx, uint8_t *ch)
{
    return hal_drv_uart_read_char(uart_idx, ch);
}

int32_t hal_intf_uart_wait_tx_done(uint8_t uart_idx)
{
    return hal_drv_uart_wait_tx_done(uart_idx);
}

uint8_t hal_intf_uart_get_lsr(uint8_t uart_idx)
{
    return hal_drv_uart_get_lsr(uart_idx);
}

uint8_t hal_intf_uart_get_last_lsr(uint8_t uart_idx)
{
    return hal_drv_uart_get_last_lsr(uart_idx);
}

void hal_intf_uart_set_fifo_trigger(uint8_t uart_idx, uint8_t rx_trigger, uint8_t tx_trigger)
{
    hal_drv_uart_set_fifo_trigger(uart_idx, rx_trigger, tx_trigger);
}

void hal_intf_uart_set_break(uint8_t uart_idx, uint8_t enable)
{
    hal_drv_uart_set_break(uart_idx, enable);
}

int32_t hal_intf_uart_set_flow_control(uint8_t uart_idx, uint8_t enable)
{
    hal_drv_uart_set_flow_control(uart_idx, enable);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_drv_uart_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_uart_unregister_callback(UART_CB_EVENT_T event)
{
    return hal_drv_uart_unregister_callback(event);
}
