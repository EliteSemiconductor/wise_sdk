/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_UART_H
#define __HAL_INTF_UART_H

#include "esmt_chip_specific.h"
#include "types.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Define UART Interrupt Identification Register */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_ERRINT (0x06) //!<		UART Error interrupt	  0x06
#define UART_RXINT (0x04)  //!<		UART RX interrupt			  0x04
#define UART_TOINT (0x0C)  //!<		UART time out interrupt	0x0C
#define UART_TXINT (0x02)  //!<		UART TX interrupt			  0x02

typedef struct {
    uint32_t baudrate;
    uint8_t data;
    uint8_t parity;
    uint8_t stop;
#ifdef CHIP_HAS_DMA
    uint8_t dmaConfig;
    int8_t rxDMAChannel;
    int8_t txDMAChannel;
#endif
} HAL_UART_CFG_T;

typedef enum {
    UART_EVENT_RX_FINISH,
    UART_EVENT_TX_FINISH,
    UART_EVENT_RX_ERROR,
    UART_EVENT_TX_ERROR,
    UART_MAX_EVENTS,
} UART_CB_EVENT_T;

typedef enum {
    HAL_UART_RESET_RX  = (1 << 1),
    HAL_UART_RESET_TX  = (1 << 2),
    HAL_UART_RESET_ALL = HAL_UART_RESET_RX | HAL_UART_RESET_TX,
} HAL_UART_RESET_TYPE;

#define HAL_UART_EVT_RX_FIN 0x01
#define HAL_UART_EVT_TX_FIN 0x02
#define HAL_UART_EVT_RX_ERROR 0x04
#define HAL_UART_EVT_TX_ERROR 0x08

typedef void (*HAL_UART_ISR_CALLBACK)(uint8_t evt, void *userData);

void hal_intf_uart_reset_fifo(uint8_t uart_idx, HAL_UART_RESET_TYPE type);
void hal_intf_uart_probe();

int32_t hal_intf_uart_init();
int32_t hal_intf_uart_set_config(uint8_t uart_idx, HAL_UART_CFG_T *uart_cfg);

int32_t hal_intf_uart_enable_irq(uint8_t uart_idx, uint8_t intFlag);
int32_t hal_intf_uart_disable_irq(uint8_t uart_idx);
int32_t hal_intf_uart_write_data(uint8_t uart_idx, uint8_t *buf, uint32_t req_len);
int32_t hal_intf_uart_poll_byte(uint8_t uart_idx, uint8_t *outByte);

HAL_STATUS hal_intf_uart_register_callback(UART_CB_EVENT_T event, CALLBACK_T cb, void *context);
HAL_STATUS hal_intf_uart_unregister_callback(UART_CB_EVENT_T event);

#endif
