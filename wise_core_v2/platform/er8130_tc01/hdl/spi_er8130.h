/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology IncG
 * All rights reserved.
 *
 */

#ifndef __SPI_ER8130_H
#define __SPI_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"
#include <stdint.h>

#define SPI_TIMEOUT 100000
#define MASTER_MODE 0
#define SLAVE_MODE 1

typedef enum {
    ROLE_MASTER = 0, /**< SPI master role. */
    ROLE_SLAVE,      /**< SPI slave role. */
} SPI_ROLE;

typedef enum {
    SPI_RESET_CORE = (1 << 0),
    SPI_RESET_RX   = (1 << 1),
    SPI_RESET_TX   = (1 << 2),
    SPI_RESET_ALL  = SPI_RESET_CORE | SPI_RESET_RX | SPI_RESET_TX
} SPI_RESET_TYPE;

// SPI message flags
#define SPI_MSG_CMD_EN BIT0
#define SPI_MSG_ADDR_EN BIT1
#define SPI_MSG_BLOCKING BIT8
// ATCSPI200 transmission mode
#define SPI_TM_WRITE_ONLY 0x01
#define SPI_TM_READ_ONLY 0x02
#define SPI_TM_WRITE_READ 0x03
#define SPI_TM_READ_WRITE 0x04
#define SPI_TM_WRITE_DMY_READ 0x05
#define SPI_TM_READ_DMY_WRITE 0x06
#define SPI_TM_NONE_DATA 0x07
#define SPI_TM_DMY_WRITE 0x08
#define SPI_TM_DMY_READ 0x09

#define SPI_Int_RXOR (0x1ul << 0)   //!<	RX FIFO Over run			0x01
#define SPI_Int_TXUR (0x1ul << 1)   //!<	TX FIFO Under run			0x02
#define SPI_Int_RXTH (0x1ul << 2)   //!<	RX Threshold					0x04
#define SPI_Int_TXTH (0x1ul << 3)   //!<	TX Threshold					0x08
#define SPI_Int_END (0x1ul << 4)    //!<	End of Transfer				0x10
#define SPI_Int_SlvCmd (0x1ul << 5) //!<	Slave Command					0x20

void spi_reset_fifo_er8130(uint32_t spi_base);
void spi_reset_er8130(uint32_t spi_base, SPI_RESET_TYPE type);
void spi_enable_interrupt_er8130(uint32_t spi_base, uint32_t int_src);
void spi_disable_interrupt_er8130(uint32_t spi_base, uint32_t int_src);
uint8_t spi_get_tx_fifo_depth_er8130(uint32_t spi_base);
uint8_t spi_get_rx_fifo_depth_er8130(uint32_t spi_base);
void spi_set_tx_fifo_threshold_er8130(uint32_t spi_base, uint8_t threshold);
uint8_t spi_get_tx_fifo_threshold_er8130(uint32_t spi_base);
void spi_set_rx_fifo_threshold_er8130(uint32_t spi_base, uint8_t threshold);
uint8_t spi_get_rx_fifo_threshold_er8130(uint32_t spi_base);
void spi_set_slv_rdy_er8130(uint32_t spi_base, uint8_t rdy);
uint16_t spi_get_slv_write_cnt_er8130(uint32_t spi_base);
uint16_t spi_get_slv_read_cnt_er8130(uint32_t spi_base);
void spi_cmd_trigger_er8130(uint32_t spi_base, uint8_t flag_en, uint8_t cmd);
uint8_t spi_get_cmd_er8130(uint32_t spi_base);
uint8_t spi_is_tx_fifo_full_er8130(uint32_t spi_base);
uint8_t spi_is_rx_fifo_empty_er8130(uint32_t spi_base);
uint8_t spi_get_interrupt_status_er8130(uint32_t spi_base);
void spi_clear_interrupt_status_er8130(uint32_t spi_base, uint8_t status);
void spi_write_data_er8130(uint32_t spi_base, uint32_t value);
uint32_t spi_read_data_er8130(uint32_t spi_base);
void spi_config_er8130(uint32_t spi_base, uint16_t clock_mode, uint8_t role, uint8_t data_bit_width, uint8_t addr_len, uint32_t bus_clock,
                       uint8_t bit_order, uint8_t data_merge, uint8_t mosi_bir_dir, uint8_t dual_quard_mode, uint8_t addr_fmt, uint8_t dma_enable);
void spi_set_xfer_fmt_er8130(uint32_t spi_base, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len, uint8_t trans_mode,
                             uint8_t flag_en, uint32_t addr_value);
HAL_STATUS spi_xfer_exec_er8130(uint32_t spi_base, uint8_t role, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len,
                                uint8_t trans_mode, uint8_t flag_en, uint32_t addr_value, uint8_t cmd_value, void *tx_fifo, void *rx_fifo,
                                uint8_t dma_enable);

HAL_STATUS spi_tx_dma_enable_er8130(uint32_t spi_base, uint8_t dma_enable);
HAL_STATUS spi_rx_dma_enable_er8130(uint32_t spi_base, uint8_t dma_enable);
HAL_STATUS spi_set_sclk_div_er8130(uint32_t spi_base, uint8_t val);
#endif /* __SPI_ER8130_H */
