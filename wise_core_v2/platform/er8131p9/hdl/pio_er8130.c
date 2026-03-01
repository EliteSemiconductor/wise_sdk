/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/pio_er8130.h"

const uint32_t PIO_SPI_BASE[] = {PIO_SPI0_REG_ADDR, PIO_SPI1_REG_ADDR};
const uint32_t PIO_UART_BASE[] = {PIO_UART0_REG_ADDR, PIO_UART1_REG_ADDR, PIO_UART2_REG_ADDR};
const uint32_t PIO_I2C_BASE[] = {PIO_I2C0_REG_ADDR, PIO_I2C1_REG_ADDR};

void pio_cfg_spi_clk_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->CLK_TAR = pinIdx;
}

void pio_cfg_spi_cs_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->CS_TAR = pinIdx;
}

void pio_cfg_spi_miso_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->MISO_TAR = pinIdx;
}

void pio_cfg_spi_mosi_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->MOSI_TAR = pinIdx;
}

void pio_cfg_spi_hold_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->HOLD_TAR = pinIdx;
}

void pio_cfg_spi_wp_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->WP_TAR = pinIdx;
}

void pio_cfg_uart_tx_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->TX_TAR = pinIdx;
}

void pio_cfg_uart_rx_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->RX_TAR = pinIdx;
}

void pio_cfg_uart_cts_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->CTS_TAR = pinIdx;
}

void pio_cfg_uart_rts_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->RTS_TAR = pinIdx;
}

void pio_cfg_i2c_scl_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_i2c_reg*)PIO_I2C_BASE[ch])->SCL_TAR = pinIdx;
}

void pio_cfg_i2c_sda_er8130(uint8_t ch, uint8_t pinIdx)
{
    ((struct _pio_i2c_reg*)PIO_I2C_BASE[ch])->SDA_TAR = pinIdx;
}

void pio_enable_spi_clk_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->CLK_EN = en;
}

void pio_enable_spi_cs_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->CS_EN = en;
}

void pio_enable_spi_miso_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->MISO_EN = en;
}

void pio_enable_spi_mosi_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->MOSI_EN = en;
}

void pio_enable_spi_hold_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->HOLD_EN = en;
}

void pio_enable_spi_wp_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_spi_reg*)PIO_SPI_BASE[ch])->WP_EN = en;
}

void pio_enable_uart_tx_er8130(uint8_t ch, uint8_t en)
{      
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->TX_EN = en;
}

void pio_enable_uart_rx_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->RX_EN = en;
}

void pio_enable_uart_cts_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->CTS_EN = en;
}

void pio_enable_uart_rts_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_uart_reg*)PIO_UART_BASE[ch])->RTS_EN = en;
}

void pio_enable_i2c_scl_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_i2c_reg*)PIO_I2C_BASE[ch])->SCL_EN = en;    
}

void pio_enable_i2c_sda_er8130(uint8_t ch, uint8_t en)
{
    ((struct _pio_i2c_reg*)PIO_I2C_BASE[ch])->SDA_EN = en;
}


/*
void pio_spi_pin_config_er8130(uint8_t ch, uint32_t clk, uint32_t cs, uint32_t miso, uint32_t mosi, uint32_t hold, uint32_t wp)
{
    struct _pio_spi_reg* pioSpi = (struct _pio_spi_reg*)PIO_SPI_BASE[ch];

    pioSpi->CLK_TAR  |= clk;
    pioSpi->CS_TAR   |= cs;
    pioSpi->MISO_TAR |= miso;
    pioSpi->MOSI_TAR |= mosi;
    pioSpi->HOLD_TAR |= hold;
    pioSpi->WP_TAR   |= wp;
}

void pio_spi_pin_enable_er8130(uint8_t ch, uint32_t clk, uint32_t cs, uint32_t miso, uint32_t mosi, uint32_t hold, uint32_t wp)
{
    struct _pio_spi_reg* pioSpi = (struct _pio_spi_reg*)PIO_SPI_BASE[ch];

    pioSpi->CLK_EN  |= clk;
    pioSpi->CS_EN   |= cs;
    pioSpi->MISO_EN |= miso;
    pioSpi->MOSI_EN |= mosi;
    pioSpi->HOLD_EN |= hold;
    pioSpi->WP_EN   |= wp;
}

void pio_uart_pin_config_er8130(uint8_t ch, uint32_t tx, uint32_t rx, uint32_t rts, uint32_t cts)
{
    struct _pio_uart_reg* pioUart = (struct _pio_uart_reg*)PIO_UART_BASE[ch];

    pioUart->TX_TAR  |= tx;
    pioUart->RX_TAR  |= rx;
    pioUart->RTS_TAR |= rts;
    pioUart->CTS_TAR |= cts;
}

void pio_uart_pin_enable_er8130(uint8_t ch, uint8_t tx, uint8_t rx, uint8_t rts, uint8_t cts)
{
    struct _pio_uart_reg* pioUart = (struct _pio_uart_reg*)PIO_UART_BASE[ch];
    
    pioUart->TX_EN  |= tx;
    pioUart->RX_EN  |= rx;
    pioUart->RTS_EN |= rts;
    pioUart->CTS_EN |= cts;
}

void pio_i2c_pin_config_er8130(uint8_t ch, uint32_t scl, uint32_t sda)
{
    struct _pio_i2c_reg* pioI2C = (struct _pio_i2c_reg*)PIO_I2C_BASE[ch];

    pioI2C->SCL_TAR |= scl;
    pioI2C->SDA_TAR |= sda;
}

void pio_i2c_pin_enable_er8130(uint8_t ch, uint8_t scl, uint8_t sda)
{
    struct _pio_i2c_reg* pioI2C = (struct _pio_i2c_reg*)PIO_I2C_BASE[ch];

    pioI2C->SCL_EN |= scl;
    pioI2C->SDA_EN |= sda;
}
*/


/*
void pio_spi0_gpio_config_er8130(uint32_t clk_pin, uint32_t cs_pin, uint32_t miso_pin, uint32_t mosi_pin, uint32_t hold_pin, uint32_t wp_pin)
{
    PIO->SPI0_REG_b.CLK_TAR  |= clk_pin;
    PIO->SPI0_REG_b.CS_TAR   |= cs_pin;
    PIO->SPI0_REG_b.MISO_TAR |= miso_pin;
    PIO->SPI0_REG_b.MOSI_TAR |= mosi_pin;
    PIO->SPI0_REG_b.HOLD_TAR |= hold_pin;
    PIO->SPI0_REG_b.WP_TAR   |= wp_pin;
}

void pio_spi0_gpio_enable_er8130(uint32_t clk_en, uint32_t cs_en, uint32_t miso_en, uint32_t mosi_en, uint32_t hold_en, uint32_t wp_en)
{
    PIO->SPI0_REG_b.CLK_EN  |= clk_en;
    PIO->SPI0_REG_b.CS_EN   |= cs_en;
    PIO->SPI0_REG_b.MISO_EN |= miso_en;
    PIO->SPI0_REG_b.MOSI_EN |= mosi_en;
    PIO->SPI0_REG_b.HOLD_EN |= hold_en;
    PIO->SPI0_REG_b.WP_EN   |= wp_en;
}

void pio_spi1_gpio_config_er8130(uint32_t clk_pin, uint32_t cs_pin, uint32_t miso_pin, uint32_t mosi_pin, uint32_t hold_pin, uint32_t wp_pin)
{
    PIO->SPI1_REG_b.CLK_TAR  |= clk_pin;
    PIO->SPI1_REG_b.CS_TAR   |= cs_pin;
    PIO->SPI1_REG_b.MISO_TAR |= miso_pin;
    PIO->SPI1_REG_b.MOSI_TAR |= mosi_pin;
    PIO->SPI1_REG_b.HOLD_TAR |= hold_pin;
    PIO->SPI1_REG_b.WP_TAR   |= wp_pin;
}

void pio_spi1_gpio_enable_er8130(uint32_t clk_en, uint32_t cs_en, uint32_t miso_en, uint32_t mosi_en, uint32_t hold_en, uint32_t wp_en)
{
    PIO->SPI1_REG_b.CLK_EN  |= clk_en;
    PIO->SPI1_REG_b.CS_EN   |= cs_en;
    PIO->SPI1_REG_b.MISO_EN |= miso_en;
    PIO->SPI1_REG_b.MOSI_EN |= mosi_en;
    PIO->SPI1_REG_b.HOLD_EN |= hold_en;
    PIO->SPI1_REG_b.WP_EN   |= wp_en;
}
*/

/*
void pio_uart0_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin, uint32_t rts_pin, uint32_t cts_pin)
{
    PIO->UART0_REG_b.TX_TAR  |= tx_pin;
    PIO->UART0_REG_b.RX_TAR  |= rx_pin;
    PIO->UART0_REG_b.RTS_TAR |= rts_pin;
    PIO->UART0_REG_b.CTS_TAR |= cts_pin;
}

void pio_uart0_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en, uint8_t cts_en)
{
    PIO->UART0_REG_b.TX_EN  |= tx_en;
    PIO->UART0_REG_b.RX_EN  |= rx_en;
    PIO->UART0_REG_b.RTS_EN |= rts_en;
    PIO->UART0_REG_b.CTS_EN |= cts_en;
}

void pio_uart1_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin, uint32_t rts_pin, uint32_t cts_pin)
{
    PIO->UART1_REG_b.TX_TAR  |= tx_pin;
    PIO->UART1_REG_b.RX_TAR  |= rx_pin;
    PIO->UART1_REG_b.RTS_TAR |= rts_pin;
    PIO->UART1_REG_b.CTS_TAR |= cts_pin;
}

void pio_uart1_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en, uint8_t cts_en)
{
    PIO->UART1_REG_b.TX_EN  |= tx_en;
    PIO->UART1_REG_b.RX_EN  |= rx_en;
    PIO->UART1_REG_b.RTS_EN |= rts_en;
    PIO->UART1_REG_b.CTS_EN |= cts_en;
}

void pio_uart2_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin, uint32_t rts_pin, uint32_t cts_pin)
{
    PIO->UART2_REG_b.TX_TAR  |= tx_pin;
    PIO->UART2_REG_b.RX_TAR  |= rx_pin;
    PIO->UART2_REG_b.RTS_TAR |= rts_pin;
    PIO->UART2_REG_b.CTS_TAR |= cts_pin;
}

void pio_uart2_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en, uint8_t cts_en)
{
    PIO->UART2_REG_b.TX_EN  |= tx_en;
    PIO->UART2_REG_b.RX_EN  |= rx_en;
    PIO->UART2_REG_b.RTS_EN |= rts_en;
    PIO->UART2_REG_b.CTS_EN |= cts_en;
}
*/

/*
void pio_i2c0_gpio_config_er8130(uint32_t scl_pin, uint32_t sda_pin)
{
    PIO->I2C0_REG_b.SCL_TAR |= scl_pin;
    PIO->I2C0_REG_b.SDA_TAR |= sda_pin;
}

void pio_i2c0_enable_er8130(uint8_t scl_en, uint8_t sda_en)
{
    PIO->I2C0_REG_b.SCL_EN |= scl_en;
    PIO->I2C0_REG_b.SDA_EN |= sda_en;
}

void pio_i2c1_gpio_config_er8130(uint32_t scl_pin, uint32_t sda_pin)
{
    PIO->I2C1_REG_b.SCL_TAR |= scl_pin;
    PIO->I2C1_REG_b.SDA_TAR |= sda_pin;
}

void pio_i2c1_enable_er8130(uint8_t scl_en, uint8_t sda_en)
{
    PIO->I2C1_REG_b.SCL_EN |= scl_en;
    PIO->I2C1_REG_b.SDA_EN |= sda_en;
}
*/
