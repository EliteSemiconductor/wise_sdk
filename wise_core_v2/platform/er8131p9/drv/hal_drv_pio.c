/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_pio.h"
#include "hdl/pio_er8130.h"
#include "hal_intf_pio.h"

typedef void (*HAL_PIO_CFG_PROC_T)(uint8_t ch, uint8_t pinIdx);
typedef void (*HAL_PIO_EN_PROC_T)(uint8_t ch, uint8_t en);

#if 0
HAL_STATUS hal_drv_pio_spi_io_conifg(uint8_t spi_channel, uint32_t clk_pin,
                                      uint32_t cs_pin, uint32_t miso_pin,
                                      uint32_t mosi_pin, uint32_t hold_pin,
                                      uint32_t wp_pin)
{
    switch (spi_channel) {
    case 0:
    case 1:
        pio_spi_pin_config_er8130(spi_channel, clk_pin, cs_pin, miso_pin, mosi_pin, hold_pin, wp_pin);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_spi_io_enable(uint8_t spi_channel, uint32_t clk_en,
                                      uint32_t cs_en, uint32_t miso_en,
                                      uint32_t mosi_en, uint32_t hold_en,
                                      uint32_t wp_en)
{
    switch (spi_channel) {
    case 0:
    case 1:
        pio_spi_pin_enable_er8130(spi_channel, clk_en, cs_en, miso_en, mosi_en, hold_en, wp_en);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_uart_io_conifg(uint8_t uart_channel, uint32_t tx_pin,
                                       uint32_t rx_pin, uint32_t rts_pin,
                                       uint32_t cts_pin)
{
    switch (uart_channel) {
    case 0:
    case 1:
    case 2:
        pio_uart_pin_config_er8130(uart_channel, tx_pin, rx_pin, rts_pin, cts_pin);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_uart_io_enable(uint8_t uart_channel, uint8_t tx_en,
                                       uint8_t rx_en, uint8_t rts_en,
                                       uint8_t cts_en)
{
    switch (uart_channel) {
    case 0:
    case 1:
    case 2:
        pio_uart_pin_enable_er8130(uart_channel, tx_en, rx_en, rts_en, cts_en);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_i2c_io_conifg(uint8_t i2c_channel, uint32_t scl_pin,
                                      uint32_t sda_pin)
{
    switch (i2c_channel) {
    case 0:
    case 1:
        pio_i2c_pin_config_er8130(i2c_channel, scl_pin, sda_pin);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_i2c_io_enable(uint8_t i2c_channel, uint8_t scl_en,
                                      uint8_t sda_en)
{
    switch (i2c_channel) {
    case 0:
    case 1:
        pio_i2c_pin_enable_er8130(i2c_channel, scl_en, sda_en);
        break;
        
    default:
        return HAL_ERR;
        break;
    }
    return HAL_NO_ERR;
}
#endif

HAL_STATUS hal_drv_pio_spi_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin)
{
    const HAL_PIO_CFG_PROC_T spiPinCfgProc[] =
    {
        pio_cfg_spi_clk_er8130,
        pio_cfg_spi_cs_er8130,
        pio_cfg_spi_mosi_er8130,
        pio_cfg_spi_miso_er8130,
        pio_cfg_spi_hold_er8130,
        pio_cfg_spi_wp_er8130,
    };

    const HAL_PIO_EN_PROC_T spiPinEnProc[] =
    {
        pio_enable_spi_clk_er8130,
        pio_enable_spi_cs_er8130,
        pio_enable_spi_mosi_er8130,
        pio_enable_spi_miso_er8130,
        pio_enable_spi_hold_er8130,
        pio_enable_spi_wp_er8130,
    };

    if(func > PIO_FUNC_SPI_WP)
        return HAL_ERR;
    
    if(en)
    {
        (spiPinCfgProc[func])(ch, pin);
    }

    (spiPinEnProc[func])(ch, en);

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_uart_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin)
{
    const HAL_PIO_CFG_PROC_T uartPinCfgProc[] =
    {
        pio_cfg_uart_tx_er8130,
        pio_cfg_uart_rx_er8130,
        pio_cfg_uart_rts_er8130,
        pio_cfg_uart_cts_er8130,
    };

    const HAL_PIO_EN_PROC_T uartPinEnProc[] =
    {
        pio_enable_uart_tx_er8130,
        pio_enable_uart_rx_er8130,
        pio_enable_uart_rts_er8130,
        pio_enable_uart_cts_er8130,
    };
    
    if(func > PIO_FUNC_UART_CTS)
        return HAL_ERR;
        
    if(en)
    {
        (uartPinCfgProc[func])(ch, pin);
    }

    (uartPinEnProc[func])(ch, en);

    return HAL_NO_ERR;
}

HAL_STATUS hal_drv_pio_i2c_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin)
{
    const HAL_PIO_CFG_PROC_T i2cPinCfgProc[] =
    {
        pio_cfg_i2c_scl_er8130,
        pio_cfg_i2c_sda_er8130,
    };

    const HAL_PIO_EN_PROC_T i2cPinEnProc[] =
    {
        pio_enable_i2c_scl_er8130,
        pio_enable_i2c_sda_er8130,
    };

    if(func > PIO_FUNC_I2C_SDA)
        return HAL_ERR;
        
    if(en)
    {
        (i2cPinCfgProc[func])(ch, pin);
    }

    (i2cPinEnProc[func])(ch, en);

    return HAL_NO_ERR;
}

