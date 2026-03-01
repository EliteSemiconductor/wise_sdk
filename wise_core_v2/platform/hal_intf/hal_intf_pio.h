/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_PIO_H
#define __HAL_INTF_PIO_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef struct {
    uint32_t pin;
    uint8_t enable;
} HAL_PIO_SIGNAL_CONFIG_T;

typedef struct {
    HAL_PIO_SIGNAL_CONFIG_T clk;
    HAL_PIO_SIGNAL_CONFIG_T cs;
    HAL_PIO_SIGNAL_CONFIG_T miso;
    HAL_PIO_SIGNAL_CONFIG_T mosi;
    HAL_PIO_SIGNAL_CONFIG_T hold;
    HAL_PIO_SIGNAL_CONFIG_T wp;
} HAL_PIO_SPI_CONFIG_T;

typedef struct {
    HAL_PIO_SIGNAL_CONFIG_T tx;
    HAL_PIO_SIGNAL_CONFIG_T rx;
    HAL_PIO_SIGNAL_CONFIG_T rts;
    HAL_PIO_SIGNAL_CONFIG_T cts;
} HAL_PIO_UART_CONFIG_T;

typedef struct {
    HAL_PIO_SIGNAL_CONFIG_T scl;
    HAL_PIO_SIGNAL_CONFIG_T sda;
} HAL_PIO_I2C_CONFIG_T;

enum
{
    PIO_TYPE_UART = 0,
    PIO_TYPE_SPI,
    PIO_TYPE_I2C,
};

enum
{
    PIO_FUNC_DISABLE = 0,
    PIO_FUNC_ENABLE,
};

enum
{
    PIO_FUNC_UART_TX= 0,
    PIO_FUNC_UART_RX,
    PIO_FUNC_UART_RTS,
    PIO_FUNC_UART_CTS,
};

enum
{
    PIO_FUNC_SPI_CLK = 0,
    PIO_FUNC_SPI_CS,
    PIO_FUNC_SPI_MOSI,
    PIO_FUNC_SPI_MISO,
    PIO_FUNC_SPI_HOLD,
    PIO_FUNC_SPI_WP,
};

enum
{
    PIO_FUNC_I2C_SCL = 0,
    PIO_FUNC_I2C_SDA,
};

HAL_STATUS hal_intf_pio_config(uint8_t type, uint8_t channel, uint8_t func, uint8_t en, uint8_t pinIdx);

#if 0
HAL_STATUS hal_intf_pio_spi_io_config(uint8_t spi_channel,
                                       HAL_PIO_SPI_CONFIG_T *spi_pio_cfg);
HAL_STATUS hal_intf_pio_uart_io_config(uint8_t uart_channel,
                                        HAL_PIO_UART_CONFIG_T *uart_pio_cfg);
HAL_STATUS hal_intf_pio_i2c_io_config(uint8_t i2c_channel,
                                       HAL_PIO_I2C_CONFIG_T *i2c_pio_cfg);
#endif

#endif /* __HAL_INTF_PIO_H  */
