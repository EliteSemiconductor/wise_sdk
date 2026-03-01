/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/* Version : csr_table_periph_io_xs_d1_20241011.xlsx */
/* ================================================================================
 */
/* ================                      PIO_T ================ */
/* ================================================================================
 */
#ifndef __PIO_REG_ER8130_H__
#define __PIO_REG_ER8130_H__

#include "cmsis/include/er8xxx.h"

struct _pio_spi_reg
{
    __IO uint32_t CLK_EN   : 1; /*!< PIO_SPI0_CLK_EN */
    __IO uint32_t CS_EN    : 1; /*!< PIO_SPI0_CS_EN */
    __IO uint32_t MISO_EN  : 1; /*!< PIO_SPI0_MISO_EN */
    __IO uint32_t MOSI_EN  : 1; /*!< PIO_SPI0_MOSI_EN */
    __IO uint32_t HOLD_EN  : 1; /*!< PIO_SPI0_HOLD_EN */
    __IO uint32_t WP_EN    : 1; /*!< PIO_SPI0_WP_EN */
    uint32_t RESERVED      : 2; /*!< RESERVED */
    __IO uint32_t CLK_TAR  : 4; /*!< PIO_SPI0_CLK_TAR */
    __IO uint32_t CS_TAR   : 4; /*!< PIO_SPI0_CS_TAR */
    __IO uint32_t MISO_TAR : 4; /*!< PIO_SPI0_MISO_TAR */
    __IO uint32_t MOSI_TAR : 4; /*!< PIO_SPI0_MOSI_TAR */
    __IO uint32_t HOLD_TAR : 4; /*!< PIO_SPI0_HOLD_TAR */
    __IO uint32_t WP_TAR   : 4; /*!< PIO_SPI0_WP_TAR */
};

struct _pio_uart_reg
{
    __IO uint32_t TX_EN   : 1; /*!< PIO_UART0_TX_EN */
    __IO uint32_t RX_EN   : 1; /*!< PIO_UART0_RX_EN */
    __IO uint32_t RTS_EN  : 1; /*!< PIO_UART0_RTS_EN */
    __IO uint32_t CTS_EN  : 1; /*!< PIO_UART0_CTS_EN */
    uint32_t RESERVED     : 4; /*!< RESERVED */
    __IO uint32_t TX_TAR  : 4; /*!< PIO_UART0_TX_TAR */
    __IO uint32_t RX_TAR  : 4; /*!< PIO_UART0_RX_TAR */
    __IO uint32_t RTS_TAR : 4; /*!< PIO_UART0_RTS_TAR */
    __IO uint32_t CTS_TAR : 4; /*!< PIO_UART0_CTS_TAR */
    uint32_t RESERVED1    : 8; /*!< RESERVED */
};

struct _pio_i2c_reg
{
    __IO uint32_t SCL_EN  : 1;  /*!< PIO_I2C0_SCL_EN */
    __IO uint32_t SDA_EN  : 1;  /*!< PIO_I2C0_SDA_EN */
    uint32_t RESERVED     : 6;  /*!< RESERVED */
    __IO uint32_t SCL_TAR : 4;  /*!< PIO_I2C0_SCL_TAR */
    __IO uint32_t SDA_TAR : 4;  /*!< PIO_I2C0_SDA_TAR */
    uint32_t RESERVED1    : 16; /*!< RESERVED */
};

/**
 * @brief PIO (PIO_T)
 */
typedef struct {
    union {
        __IO uint32_t SPI0_REG; /*!< SPI0_REG Register (0x000) */
        struct _pio_spi_reg SPI0_REG_b;
    };
    
    union {
        __IO uint32_t SPI1_REG; /*!< SPI1_REG Register (0x004)*/
        struct _pio_spi_reg SPI1_REG_b;
    };
    
    union {
        __IO uint32_t UART0_REG; /*!< UART0_REG Register (0x008) */
        struct _pio_uart_reg UART0_REG_b;
    };
    
    union {
        __IO uint32_t UART1_REG; /*!< UART1_REG Register (0x00C) */
        struct _pio_uart_reg UART1_REG_b;
    };
    
    union {
        __IO uint32_t UART2_REG; /*!< UART2_REG Register (0x010) */
        struct _pio_uart_reg UART2_REG_b;
    };
    
    union {
        __IO uint32_t I2C0_REG; /*!< I2C0_REG Register (0x014) */
        struct _pio_i2c_reg I2C0_REG_b;
    };
    union {
        __IO uint32_t I2C1_REG; /*!< I2C1_REG Register (0x018) */
        struct _pio_i2c_reg I2C1_REG_b;
    };
} PIO_T;

#define PIO_SPI0_REG_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_CLK_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_CLK_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_SPI0_CLK_EN_MASK (0x1ul << PIO_PIO_SPI0_CLK_EN_POS)
#define PIO_PIO_SPI0_CS_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_CS_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_SPI0_CS_EN_MASK (0x1ul << PIO_PIO_SPI0_CS_EN_POS)
#define PIO_PIO_SPI0_MISO_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_MISO_EN_POS (2) /*< bit[2]      */
#define PIO_PIO_SPI0_MISO_EN_MASK (0x1ul << PIO_PIO_SPI0_MISO_EN_POS)
#define PIO_PIO_SPI0_MOSI_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_MOSI_EN_POS (3) /*< bit[3]      */
#define PIO_PIO_SPI0_MOSI_EN_MASK (0x1ul << PIO_PIO_SPI0_MOSI_EN_POS)
#define PIO_PIO_SPI0_HOLD_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_HOLD_EN_POS (4) /*< bit[4]      */
#define PIO_PIO_SPI0_HOLD_EN_MASK (0x1ul << PIO_PIO_SPI0_HOLD_EN_POS)
#define PIO_PIO_SPI0_WP_EN_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_WP_EN_POS (5) /*< bit[5]      */
#define PIO_PIO_SPI0_WP_EN_MASK (0x1ul << PIO_PIO_SPI0_WP_EN_POS)
#define PIO_PIO_SPI0_CLK_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_CLK_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_SPI0_CLK_TAR_MASK (0xFul << PIO_PIO_SPI0_CLK_TAR_POS)
#define PIO_PIO_SPI0_CS_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_CS_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_SPI0_CS_TAR_MASK (0xFul << PIO_PIO_SPI0_CS_TAR_POS)
#define PIO_PIO_SPI0_MISO_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_MISO_TAR_POS (16) /*< bit[19:16]  */
#define PIO_PIO_SPI0_MISO_TAR_MASK (0xFul << PIO_PIO_SPI0_MISO_TAR_POS)
#define PIO_PIO_SPI0_MOSI_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_MOSI_TAR_POS (20) /*< bit[23:20]  */
#define PIO_PIO_SPI0_MOSI_TAR_MASK (0xFul << PIO_PIO_SPI0_MOSI_TAR_POS)
#define PIO_PIO_SPI0_HOLD_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_HOLD_TAR_POS (24) /*< bit[27:24]  */
#define PIO_PIO_SPI0_HOLD_TAR_MASK (0xFul << PIO_PIO_SPI0_HOLD_TAR_POS)
#define PIO_PIO_SPI0_WP_TAR_ADDR (uint32_t)&(PIO->SPI0_REG)
#define PIO_PIO_SPI0_WP_TAR_POS (28) /*< bit[31:28]  */
#define PIO_PIO_SPI0_WP_TAR_MASK (0xFul << PIO_PIO_SPI0_WP_TAR_POS)

#define PIO_SPI1_REG_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_CLK_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_CLK_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_SPI1_CLK_EN_MASK (0x1ul << PIO_PIO_SPI1_CLK_EN_POS)
#define PIO_PIO_SPI1_CS_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_CS_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_SPI1_CS_EN_MASK (0x1ul << PIO_PIO_SPI1_CS_EN_POS)
#define PIO_PIO_SPI1_MISO_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_MISO_EN_POS (2) /*< bit[2]      */
#define PIO_PIO_SPI1_MISO_EN_MASK (0x1ul << PIO_PIO_SPI1_MISO_EN_POS)
#define PIO_PIO_SPI1_MOSI_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_MOSI_EN_POS (3) /*< bit[3]      */
#define PIO_PIO_SPI1_MOSI_EN_MASK (0x1ul << PIO_PIO_SPI1_MOSI_EN_POS)
#define PIO_PIO_SPI1_HOLD_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_HOLD_EN_POS (4) /*< bit[4]      */
#define PIO_PIO_SPI1_HOLD_EN_MASK (0x1ul << PIO_PIO_SPI1_HOLD_EN_POS)
#define PIO_PIO_SPI1_WP_EN_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_WP_EN_POS (5) /*< bit[5]      */
#define PIO_PIO_SPI1_WP_EN_MASK (0x1ul << PIO_PIO_SPI1_WP_EN_POS)
#define PIO_PIO_SPI1_CLK_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_CLK_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_SPI1_CLK_TAR_MASK (0xFul << PIO_PIO_SPI1_CLK_TAR_POS)
#define PIO_PIO_SPI1_CS_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_CS_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_SPI1_CS_TAR_MASK (0xFul << PIO_PIO_SPI1_CS_TAR_POS)
#define PIO_PIO_SPI1_MISO_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_MISO_TAR_POS (16) /*< bit[19:16]  */
#define PIO_PIO_SPI1_MISO_TAR_MASK (0xFul << PIO_PIO_SPI1_MISO_TAR_POS)
#define PIO_PIO_SPI1_MOSI_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_MOSI_TAR_POS (20) /*< bit[23:20]  */
#define PIO_PIO_SPI1_MOSI_TAR_MASK (0xFul << PIO_PIO_SPI1_MOSI_TAR_POS)
#define PIO_PIO_SPI1_HOLD_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_HOLD_TAR_POS (24) /*< bit[27:24]  */
#define PIO_PIO_SPI1_HOLD_TAR_MASK (0xFul << PIO_PIO_SPI1_HOLD_TAR_POS)
#define PIO_PIO_SPI1_WP_TAR_ADDR (uint32_t)&(PIO->SPI1_REG)
#define PIO_PIO_SPI1_WP_TAR_POS (28) /*< bit[31:28]  */
#define PIO_PIO_SPI1_WP_TAR_MASK (0xFul << PIO_PIO_SPI1_WP_TAR_POS)

#define PIO_UART0_REG_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_TX_EN_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_TX_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_UART0_TX_EN_MASK (0x1ul << PIO_PIO_UART0_TX_EN_POS)
#define PIO_PIO_UART0_RX_EN_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_RX_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_UART0_RX_EN_MASK (0x1ul << PIO_PIO_UART0_RX_EN_POS)
#define PIO_PIO_UART0_RTS_EN_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_RTS_EN_POS (2) /*< bit[2]      */
#define PIO_PIO_UART0_RTS_EN_MASK (0x1ul << PIO_PIO_UART0_RTS_EN_POS)
#define PIO_PIO_UART0_CTS_EN_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_CTS_EN_POS (3) /*< bit[3]      */
#define PIO_PIO_UART0_CTS_EN_MASK (0x1ul << PIO_PIO_UART0_CTS_EN_POS)
#define PIO_PIO_UART0_TX_TAR_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_TX_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_UART0_TX_TAR_MASK (0xFul << PIO_PIO_UART0_TX_TAR_POS)
#define PIO_PIO_UART0_RX_TAR_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_RX_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_UART0_RX_TAR_MASK (0xFul << PIO_PIO_UART0_RX_TAR_POS)
#define PIO_PIO_UART0_RTS_TAR_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_RTS_TAR_POS (16) /*< bit[19:16]  */
#define PIO_PIO_UART0_RTS_TAR_MASK (0xFul << PIO_PIO_UART0_RTS_TAR_POS)
#define PIO_PIO_UART0_CTS_TAR_ADDR (uint32_t)&(PIO->UART0_REG)
#define PIO_PIO_UART0_CTS_TAR_POS (20) /*< bit[23:20]  */
#define PIO_PIO_UART0_CTS_TAR_MASK (0xFul << PIO_PIO_UART0_CTS_TAR_POS)

#define PIO_UART1_REG_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_TX_EN_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_TX_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_UART1_TX_EN_MASK (0x1ul << PIO_PIO_UART1_TX_EN_POS)
#define PIO_PIO_UART1_RX_EN_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_RX_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_UART1_RX_EN_MASK (0x1ul << PIO_PIO_UART1_RX_EN_POS)
#define PIO_PIO_UART1_RTS_EN_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_RTS_EN_POS (2) /*< bit[2]      */
#define PIO_PIO_UART1_RTS_EN_MASK (0x1ul << PIO_PIO_UART1_RTS_EN_POS)
#define PIO_PIO_UART1_CTS_EN_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_CTS_EN_POS (3) /*< bit[3]      */
#define PIO_PIO_UART1_CTS_EN_MASK (0x1ul << PIO_PIO_UART1_CTS_EN_POS)
#define PIO_PIO_UART1_TX_TAR_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_TX_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_UART1_TX_TAR_MASK (0xFul << PIO_PIO_UART1_TX_TAR_POS)
#define PIO_PIO_UART1_RX_TAR_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_RX_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_UART1_RX_TAR_MASK (0xFul << PIO_PIO_UART1_RX_TAR_POS)
#define PIO_PIO_UART1_RTS_TAR_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_RTS_TAR_POS (16) /*< bit[19:16]  */
#define PIO_PIO_UART1_RTS_TAR_MASK (0xFul << PIO_PIO_UART1_RTS_TAR_POS)
#define PIO_PIO_UART1_CTS_TAR_ADDR (uint32_t)&(PIO->UART1_REG)
#define PIO_PIO_UART1_CTS_TAR_POS (20) /*< bit[23:20]  */
#define PIO_PIO_UART1_CTS_TAR_MASK (0xFul << PIO_PIO_UART1_CTS_TAR_POS)

#define PIO_UART2_REG_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_TX_EN_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_TX_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_UART2_TX_EN_MASK (0x1ul << PIO_PIO_UART2_TX_EN_POS)
#define PIO_PIO_UART2_RX_EN_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_RX_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_UART2_RX_EN_MASK (0x1ul << PIO_PIO_UART2_RX_EN_POS)
#define PIO_PIO_UART2_RTS_EN_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_RTS_EN_POS (2) /*< bit[2]      */
#define PIO_PIO_UART2_RTS_EN_MASK (0x1ul << PIO_PIO_UART2_RTS_EN_POS)
#define PIO_PIO_UART2_CTS_EN_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_CTS_EN_POS (3) /*< bit[3]      */
#define PIO_PIO_UART2_CTS_EN_MASK (0x1ul << PIO_PIO_UART2_CTS_EN_POS)
#define PIO_PIO_UART2_TX_TAR_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_TX_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_UART2_TX_TAR_MASK (0xFul << PIO_PIO_UART2_TX_TAR_POS)
#define PIO_PIO_UART2_RX_TAR_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_RX_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_UART2_RX_TAR_MASK (0xFul << PIO_PIO_UART2_RX_TAR_POS)
#define PIO_PIO_UART2_RTS_TAR_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_RTS_TAR_POS (16) /*< bit[19:16]  */
#define PIO_PIO_UART2_RTS_TAR_MASK (0xFul << PIO_PIO_UART2_RTS_TAR_POS)
#define PIO_PIO_UART2_CTS_TAR_ADDR (uint32_t)&(PIO->UART2_REG)
#define PIO_PIO_UART2_CTS_TAR_POS (20) /*< bit[23:20]  */
#define PIO_PIO_UART2_CTS_TAR_MASK (0xFul << PIO_PIO_UART2_CTS_TAR_POS)

#define PIO_I2C0_REG_ADDR (uint32_t)&(PIO->I2C0_REG)
#define PIO_PIO_I2C0_SCL_EN_ADDR (uint32_t)&(PIO->I2C0_REG)
#define PIO_PIO_I2C0_SCL_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_I2C0_SCL_EN_MASK (0x1ul << PIO_PIO_I2C0_SCL_EN_POS)
#define PIO_PIO_I2C0_SDA_EN_ADDR (uint32_t)&(PIO->I2C0_REG)
#define PIO_PIO_I2C0_SDA_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_I2C0_SDA_EN_MASK (0x1ul << PIO_PIO_I2C0_SDA_EN_POS)
#define PIO_PIO_I2C0_SCL_TAR_ADDR (uint32_t)&(PIO->I2C0_REG)
#define PIO_PIO_I2C0_SCL_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_I2C0_SCL_TAR_MASK (0xFul << PIO_PIO_I2C0_SCL_TAR_POS)
#define PIO_PIO_I2C0_SDA_TAR_ADDR (uint32_t)&(PIO->I2C0_REG)
#define PIO_PIO_I2C0_SDA_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_I2C0_SDA_TAR_MASK (0xFul << PIO_PIO_I2C0_SDA_TAR_POS)

#define PIO_I2C1_REG_ADDR (uint32_t)&(PIO->I2C1_REG)
#define PIO_PIO_I2C1_SCL_EN_ADDR (uint32_t)&(PIO->I2C1_REG)
#define PIO_PIO_I2C1_SCL_EN_POS (0) /*< bit[0]      */
#define PIO_PIO_I2C1_SCL_EN_MASK (0x1ul << PIO_PIO_I2C1_SCL_EN_POS)
#define PIO_PIO_I2C1_SDA_EN_ADDR (uint32_t)&(PIO->I2C1_REG)
#define PIO_PIO_I2C1_SDA_EN_POS (1) /*< bit[1]      */
#define PIO_PIO_I2C1_SDA_EN_MASK (0x1ul << PIO_PIO_I2C1_SDA_EN_POS)
#define PIO_PIO_I2C1_SCL_TAR_ADDR (uint32_t)&(PIO->I2C1_REG)
#define PIO_PIO_I2C1_SCL_TAR_POS (8) /*< bit[11:8]   */
#define PIO_PIO_I2C1_SCL_TAR_MASK (0xFul << PIO_PIO_I2C1_SCL_TAR_POS)
#define PIO_PIO_I2C1_SDA_TAR_ADDR (uint32_t)&(PIO->I2C1_REG)
#define PIO_PIO_I2C1_SDA_TAR_POS (12) /*< bit[15:12]  */
#define PIO_PIO_I2C1_SDA_TAR_MASK (0xFul << PIO_PIO_I2C1_SDA_TAR_POS)

#define PIO_PERIPH_ID_LEN_REG_ADDR (uint32_t)&(PIO->PERIPH_ID_LEN_REG)
#define PIO_PERIPH_ID_LEN_ADDR (uint32_t)&(PIO->PERIPH_ID_LEN_REG)
#define PIO_PERIPH_ID_LEN_POS (0) /*< bit[3:0]    */
#define PIO_PERIPH_ID_LEN_MASK (0xFul << PIO_PERIPH_ID_LEN_POS)

#define PIO_PERIPH_ID_0_REG_ADDR (uint32_t)&(PIO->PERIPH_ID_0_REG)
#define PIO_PERIPH_ID_1_REG_ADDR (uint32_t)&(PIO->PERIPH_ID_1_REG)
#define PIO_PERIPH_ID_2_REG_ADDR (uint32_t)&(PIO->PERIPH_ID_2_REG)
#define PIO_BASE 0x4003D000UL
#define PIO ((PIO_T *)PIO_BASE)

#endif /* __PIO_REG_ER8130_H__ */
