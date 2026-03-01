/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_PIO_H
#define __HAL_DRV_PIO_H

#include "hal_intf_pio.h"
#include <stdint.h>

HAL_STATUS hal_drv_pio_spi_io_conifg(uint8_t spi_channel, uint32_t clk_pin,
                                      uint32_t cs_pin, uint32_t miso_pin,
                                      uint32_t mosi_pin, uint32_t hold_pin,
                                      uint32_t wp_pin);
HAL_STATUS hal_drv_pio_spi_io_enable(uint8_t spi_channel, uint32_t clk_en,
                                      uint32_t cs_en, uint32_t miso_en,
                                      uint32_t mosi_en, uint32_t hold_en,
                                      uint32_t wp_en);
HAL_STATUS hal_drv_pio_uart_io_conifg(uint8_t uart_channel, uint32_t tx_pin,
                                       uint32_t rx_pin, uint32_t rts_pin,
                                       uint32_t cts_pin);
HAL_STATUS hal_drv_pio_uart_io_enable(uint8_t uart_channel, uint8_t tx_en,
                                       uint8_t rx_en, uint8_t rts_en,
                                       uint8_t cts_en);
HAL_STATUS hal_drv_pio_i2c_io_conifg(uint8_t i2c_channel, uint32_t scl_pin,
                                      uint32_t sda_pin);
HAL_STATUS hal_drv_pio_i2c_io_enable(uint8_t i2c_channel, uint8_t scl_en,
                                      uint8_t sda_en);

HAL_STATUS hal_drv_pio_spi_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin);
HAL_STATUS hal_drv_pio_uart_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin);
HAL_STATUS hal_drv_pio_i2c_io_cfg(uint8_t ch, uint8_t func, uint8_t en, uint8_t pin);

#endif /* __HAL_DRV_PIO_H  */
