/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_pio.h"
#include "drv/hal_drv_pio.h"

#if 0
HAL_STATUS hal_intf_pio_spi_io_config(uint8_t spi_channel, HAL_PIO_SPI_CONFIG_T *spi_pio_cfg)
{
#ifdef CHIP_GPIO_HAS_PIO
    hal_drv_pio_spi_io_conifg(spi_channel, spi_pio_cfg->clk.pin, spi_pio_cfg->cs.pin, spi_pio_cfg->miso.pin, spi_pio_cfg->mosi.pin,
                              spi_pio_cfg->hold.pin, spi_pio_cfg->wp.pin);
    return hal_drv_pio_spi_io_enable(spi_channel, spi_pio_cfg->clk.enable, spi_pio_cfg->cs.enable, spi_pio_cfg->miso.enable, spi_pio_cfg->mosi.enable,
                                     spi_pio_cfg->hold.enable, spi_pio_cfg->wp.enable);
#else
    return HAL_ERR;
#endif
}

HAL_STATUS hal_intf_pio_uart_io_config(uint8_t uart_channel, HAL_PIO_UART_CONFIG_T *uart_pio_cfg)
{
#ifdef CHIP_GPIO_HAS_PIO
    hal_drv_pio_uart_io_conifg(uart_channel, uart_pio_cfg->tx.pin, uart_pio_cfg->rx.pin, uart_pio_cfg->rts.pin, uart_pio_cfg->cts.pin);
    return hal_drv_pio_uart_io_enable(uart_channel, uart_pio_cfg->tx.enable, uart_pio_cfg->rx.enable, uart_pio_cfg->rts.enable,
                                      uart_pio_cfg->cts.enable);
#else
    return HAL_ERR;
#endif
}

HAL_STATUS hal_intf_pio_i2c_io_config(uint8_t i2c_channel, HAL_PIO_I2C_CONFIG_T *i2c_pio_cfg)
{
#ifdef CHIP_GPIO_HAS_PIO
    hal_drv_pio_i2c_io_conifg(i2c_channel, i2c_pio_cfg->scl.pin, i2c_pio_cfg->sda.pin);
    return hal_drv_pio_i2c_io_enable(i2c_channel, i2c_pio_cfg->scl.enable, i2c_pio_cfg->sda.enable);
#else
    return HAL_ERR;
#endif
}
#endif

HAL_STATUS hal_intf_pio_config(uint8_t type, uint8_t ch, uint8_t func, uint8_t en, uint8_t pinIdx)
{
#ifdef CHIP_GPIO_HAS_PIO
    switch(type)
    {
        case PIO_TYPE_UART:
            hal_drv_pio_uart_io_cfg(ch, func, en, pinIdx);
        break;

        case PIO_TYPE_SPI:
            hal_drv_pio_spi_io_cfg(ch, func, en, pinIdx);
        break;

        case PIO_TYPE_I2C:
            hal_drv_pio_i2c_io_cfg(ch, func, en, pinIdx);
        break;
    }
    
    return HAL_NO_ERR;
#else
    return HAL_ERR;
#endif
}

