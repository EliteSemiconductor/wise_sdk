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

HAL_STATUS hal_intf_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_drv_uart_register_callback(event, cb, context);
}

HAL_STATUS hal_intf_uart_unregister_callback(UART_CB_EVENT_T event)
{
    return hal_drv_uart_unregister_callback(event);
}
