/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __UART_ER8130_H
#define __UART_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    UART_RESET_RX  = (1 << 1),
    UART_RESET_TX  = (1 << 2),
    UART_RESET_ALL = UART_RESET_RX | UART_RESET_TX,
} UART_RESET_TYPE;

#define UART_OSCR 8
#define ENABLE_DLAB() REG_W32(UART_LCR_ADDR, UART_DLAB_MASK)
#define DISABLE_DLAB() REG_W32(UART_LCR_ADDR, REG_R32(UART_LCR_ADDR) & ~UART_DLAB_MASK)
#define GET_DIVISOR_VAL(br) ((SystemCoreClock + ((br << 2))) / (br << 3))

/*---------------------------------------------------------------------------------------------------------*/
/* Define UART Interrupt Mask */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_INTEN_ELSI_Msk     (uint32_t)(0x1ul << 2)  //!<		Enable receiver line status interrupt
#define UART_INTEN_ETHEI_Msk    (uint32_t)(0x1ul << 1) //!<		Enable transmitter holding register interrupt
#define UART_INTEN_ERBI_Msk     (uint32_t)(0x1ul << 0)  //!<		Enable received data available interrupt

#define UART_INTEN_MASK         (UART_INTEN_ELSI_Msk | UART_INTEN_ETHEI_Msk | UART_INTEN_ERBI_Msk)

void uart_reset_fifo_er8130(uint32_t uart_base, UART_RESET_TYPE type);
void uart_config_er8130(uint32_t uart_base, uint32_t baudrate, uint8_t data, uint8_t parity, uint8_t stop, uint8_t dmaConfig);
uint8_t uart_get_int_status_er8130(uint32_t uart_base);
void uart_write_byte_er8130(uint32_t uart_base, uint8_t ch);
int8_t uart_read_byte_er8130(uint32_t uart_base, uint8_t *ch);
void uart_irq_en_er8130(uint32_t uart_base, uint8_t int_type);
void uart_irq_dis_er8130(uint32_t uart_base);

#endif
