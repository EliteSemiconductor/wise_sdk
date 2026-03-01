/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "esmt_chip_specific.h"
#include "hdl/uart_er8130.h"
#include "types.h"

void uart_config_er8130(uint32_t uart_base, uint32_t baudrate, uint8_t data, uint8_t parity, uint8_t stop, uint8_t dmaConfig)
{
    UART_T *UART      = (UART_T *)uart_base;
    uint32_t reg      = 0;
    uint16_t dvsr_val = 0;

    ENABLE_DLAB();

    /* Set baud rate */
    dvsr_val = GET_DIVISOR_VAL(baudrate);
    REG_W32(UART_RBR_THR_DLL_ADDR, dvsr_val & 0xFF);
    REG_W32(UART_IER_DLM_ADDR, ((dvsr_val & 0xFF00) >> 8));

    /* Set line control register */
    reg = REG_R32(UART_LCR_ADDR) | ((data << UART_WLS_POS) & UART_WLS_MASK) | ((stop << UART_STB_POS) & UART_STB_MASK) |
          ((parity << UART_PEN_POS) & UART_PEN_MASK);
    REG_W32(UART_LCR_ADDR, reg);

    REG_W32(UART_OSCR_ADDR, UART_OSCR);

    DISABLE_DLAB();

    /* Set FIFO control register */
    reg = REG_R32(UART_IIR_FCR_ADDR) | UART_FIFOE_MASK | UART_RFIFORST_MASK | UART_TFIFORST_MASK;
    if (dmaConfig == ENABLE) {
        reg |= UART_DMAE_MASK;
    }
    REG_W32(UART_IIR_FCR_ADDR, reg);
}

void uart_reset_fifo_er8130(uint32_t uart_base, UART_RESET_TYPE type)
{
    UART_T *UART = (UART_T *)uart_base;

    if (type == 0) {
        return; // Nothing to reset
    }

    REG_W32(UART_IIR_FCR_ADDR, type);
}

uint8_t uart_get_int_status_er8130(uint32_t uart_base)
{
    UART_T *UART = (UART_T *)uart_base;
    return (REG_R32(UART_IIR_FCR_ADDR) & UART_INTRID_MASK);
}

void uart_write_byte_er8130(uint32_t uart_base, uint8_t ch)
{
    UART_T *UART = (UART_T *)uart_base;

    while (!(REG_R32(UART_LSR_ADDR) & UART_THRE_MASK))
        ;

    REG_W32(UART_RBR_THR_DLL_ADDR, ch);
}

int8_t uart_read_byte_er8130(uint32_t uart_base, uint8_t *ch)
{
    UART_T *UART = (UART_T *)uart_base;

    if (!(REG_R32(UART_LSR_ADDR) & UART_DR_MASK)) {
        return HAL_ERR;
    }

    *ch = REG_R32(UART_RBR_THR_DLL_ADDR);

    return HAL_NO_ERR;
}

void uart_irq_en_er8130(uint32_t uart_base, uint8_t int_type)
{
    UART_T *UART = (UART_T *)uart_base;
    volatile uint32_t regVal = REG_R32(UART_IER_DLM_ADDR);

    regVal &= ~(UART_INTEN_MASK);
    regVal |= int_type;
    REG_W32(UART_IER_DLM_ADDR, regVal);
}

void uart_irq_dis_er8130(uint32_t uart_base)
{
}
