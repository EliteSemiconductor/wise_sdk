/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_UART_H
#define __HAL_DRV_UART_H

#include "hal_intf_uart.h"
#include <stdint.h>

typedef enum {
    DRV_UART_RESET_RX  = (1 << 1),
    DRV_UART_RESET_TX  = (1 << 2),
    DRV_UART_RESET_ALL = DRV_UART_RESET_RX | DRV_UART_RESET_TX,
} DRV_UART_RESET_TYPE;

void hal_drv_uart_reset_fifo(uint8_t uart_idx, DRV_UART_RESET_TYPE type);
void hal_drv_uart_config(uint8_t uart_idx, uint32_t baudrate, uint8_t data, uint8_t parity, uint8_t stop, uint8_t dmaConfig);
uint8_t hal_drv_uart_get_int_status(uint8_t uart_idx);
void hal_drv_uart_write_byte(uint8_t uart_idx, uint8_t ch);
int8_t hal_drv_uart_read_byte(uint8_t uart_idx, uint8_t *ch);
uint32_t hal_drv_uart_write_data(uint8_t uart_idx, uint8_t *data, uint32_t req_len);
void hal_drv_uart_irq_enable(uint8_t uart_idx, uint8_t int_type);
void hal_drv_uart_irq_disable(uint8_t uart_idx);

HAL_STATUS hal_drv_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context);
HAL_STATUS hal_drv_uart_unregister_callback(UART_CB_EVENT_T event);
#endif /* __HAL_DRV_UART_H */
