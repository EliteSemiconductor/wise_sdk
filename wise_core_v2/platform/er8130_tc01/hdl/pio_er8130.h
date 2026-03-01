/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PIO_ER8130_H
#define __PIO_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"

/*
void pio_spi0_gpio_config_er8130(uint32_t clk_pin, uint32_t cs_pin,
                                 uint32_t miso_pin, uint32_t mosi_pin,
                                 uint32_t hold_pin, uint32_t wp_pin);
void pio_spi0_gpio_enable_er8130(uint32_t clk_en, uint32_t cs_en,
                                 uint32_t miso_en, uint32_t mosi_en,
                                 uint32_t hold_en, uint32_t wp_en);
void pio_spi1_gpio_config_er8130(uint32_t clk_pin, uint32_t cs_pin,
                                 uint32_t miso_pin, uint32_t mosi_pin,
                                 uint32_t hold_pin, uint32_t wp_pin);
void pio_spi1_gpio_enable_er8130(uint32_t clk_en, uint32_t cs_en,
                                 uint32_t miso_en, uint32_t mosi_en,
                                 uint32_t hold_en, uint32_t wp_en);
*/

/*
void pio_uart0_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin,
                                  uint32_t rts_pin, uint32_t cts_pin);
void pio_uart0_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en,
                             uint8_t cts_en);
void pio_uart1_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin,
                                  uint32_t rts_pin, uint32_t cts_pin);
void pio_uart1_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en,
                             uint8_t cts_en);
void pio_uart2_gpio_config_er8130(uint32_t tx_pin, uint32_t rx_pin,
                                  uint32_t rts_pin, uint32_t cts_pin);
void pio_uart2_enable_er8130(uint8_t tx_en, uint8_t rx_en, uint8_t rts_en,
                             uint8_t cts_en);
*/

/*
void pio_i2c0_gpio_config_er8130(uint32_t scl_pin, uint32_t sda_pin);
void pio_i2c0_enable_er8130(uint8_t scl_en, uint8_t sda_en);
void pio_i2c1_gpio_config_er8130(uint32_t scl_pin, uint32_t sda_pin);
void pio_i2c1_enable_er8130(uint8_t scl_en, uint8_t sda_en);
*/

/*
void pio_spi_pin_config_er8130(uint8_t ch, uint32_t clk, uint32_t cs, uint32_t miso, uint32_t mosi, uint32_t hold, uint32_t wp);
void pio_spi_pin_enable_er8130(uint8_t ch, uint32_t clk, uint32_t cs, uint32_t miso, uint32_t mosi, uint32_t hold, uint32_t wp);

void pio_uart_pin_config_er8130(uint8_t ch, uint32_t tx, uint32_t rx, uint32_t rts, uint32_t cts);
void pio_uart_pin_enable_er8130(uint8_t ch, uint8_t tx, uint8_t rx, uint8_t rts, uint8_t cts);

void pio_i2c_pin_config_er8130(uint8_t ch, uint32_t scl, uint32_t sda);
void pio_i2c_pin_enable_er8130(uint8_t ch, uint8_t scl, uint8_t sda);
*/

void pio_cfg_spi_clk_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_spi_cs_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_spi_miso_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_spi_mosi_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_spi_hold_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_spi_wp_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_uart_tx_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_uart_rx_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_uart_cts_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_uart_rts_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_i2c_scl_er8130(uint8_t ch, uint8_t pinIdx);
void pio_cfg_i2c_sda_er8130(uint8_t ch, uint8_t pinIdx);


void pio_enable_spi_clk_er8130(uint8_t ch, uint8_t en);
void pio_enable_spi_cs_er8130(uint8_t ch, uint8_t en);
void pio_enable_spi_miso_er8130(uint8_t ch, uint8_t en);
void pio_enable_spi_mosi_er8130(uint8_t ch, uint8_t en);
void pio_enable_spi_hold_er8130(uint8_t ch, uint8_t en);
void pio_enable_spi_wp_er8130(uint8_t ch, uint8_t en);
void pio_enable_uart_tx_er8130(uint8_t ch, uint8_t en);
void pio_enable_uart_rx_er8130(uint8_t ch, uint8_t en);
void pio_enable_uart_cts_er8130(uint8_t ch, uint8_t en);
void pio_enable_uart_rts_er8130(uint8_t ch, uint8_t en);
void pio_enable_i2c_scl_er8130(uint8_t ch, uint8_t en);
void pio_enable_i2c_sda_er8130(uint8_t ch, uint8_t en);


#endif /* __PIO_ER8130_H  */
