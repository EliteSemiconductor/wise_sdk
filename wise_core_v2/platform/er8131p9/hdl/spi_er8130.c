/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/spi_er8130.h"
#include "types.h"
#include "util_debug_log.h"
#include <stdint.h>

#define CHK_WO_MODE(tm) (tm == SPI_TM_WRITE_ONLY) || (tm == SPI_TM_DMY_WRITE)

#define SPI_ENABLE_IRQ(spi_base)                                                                                                                     \
    do {                                                                                                                                             \
        if ((spi_base) == SPI_BASE) {                                                                                                                \
            NVIC_EnableIRQ((IRQn_Type)SPI0_IRQn);                                                                                                    \
        } else if ((spi_base) == (SPI_BASE + 0x1000)) {                                                                                              \
            NVIC_EnableIRQ((IRQn_Type)SPI1_IRQn);                                                                                                    \
        } else {                                                                                                                                     \
            debug_printf("error SPI address");                                                                                                       \
        }                                                                                                                                            \
    } while (0)
#if 0
void spi_reset_fifo_er8130(uint32_t spi_base)
{
    uint32_t reg   = 0;
    uint32_t to    = 0;
    uint32_t mask  = (SPI_SPI_RST_MASK + SPI_RX_FIFO_RST_MASK + SPI_TX_FIFO_RST_MASK);
    SPI_T *SPI     = (SPI_T *)spi_base;
    reg            = REG_R32(SPI_CTRL_ADDR);
    reg           |= mask;
    REG_W32(SPI_CTRL_ADDR, reg);
    while ((REG_R32(SPI_CTRL_ADDR) & mask) && (to++ < SPI_TIMEOUT)) {
        ;
    }
}
#endif
static void _spi_wait_reset_done(uint32_t spi_base, uint32_t mask)
{
    uint32_t to = 0;
    SPI_T *SPI  = (SPI_T *)spi_base;
    while ((REG_R32(SPI_CTRL_ADDR) & mask) && (to++ < SPI_TIMEOUT)) {
        __NOP();
    }

    if (to >= SPI_TIMEOUT) {
        printf("Warning: SPI reset (mask=0x%lX) timeout!\n", mask);
    }
}
void spi_reset_er8130(uint32_t spi_base, SPI_RESET_TYPE type)
{
    SPI_T *SPI    = (SPI_T *)spi_base;
    uint32_t reg  = REG_R32(SPI_CTRL_ADDR);
    uint32_t mask = 0;

    if (type & SPI_RESET_CORE) {
        mask |= SPI_SPI_RST_MASK;
    }
    if (type & SPI_RESET_RX) {
        mask |= SPI_RX_FIFO_RST_MASK;
    }
    if (type & SPI_RESET_TX) {
        mask |= SPI_TX_FIFO_RST_MASK;
    }

    if (mask == 0) {
        return; // Nothing to reset
    }

    reg |= mask;
    REG_W32(SPI_CTRL_ADDR, reg);
    _spi_wait_reset_done(spi_base, mask);
}
#if 0
void spi_reset_fifo_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    uint32_t reg;
    uint32_t to         = 0;
    const uint32_t mask = SPI_SPI_RST_MASK | SPI_RX_FIFO_RST_MASK | SPI_TX_FIFO_RST_MASK;

    reg  = REG_R32(SPI_CTRL_ADDR);
    reg |= mask;
    REG_W32(SPI_CTRL_ADDR, reg);

    while ((REG_R32(SPI_CTRL_ADDR) & mask) && (to++ < SPI_TIMEOUT)) {
        __NOP();
    }

    if (to >= SPI_TIMEOUT) {
        printf("Warning: SPI reset timeout!\n");
    }
}
#endif
static int8_t spi_wait_for_completion_er8130(uint32_t spi_base)
{
    uint32_t timeout_count = 0;
    SPI_T *SPI             = (SPI_T *)spi_base;

    while ((REG_R32(SPI_STS_ADDR) & SPI_SPI_ACTIVE_MASK) && (timeout_count++ < SPI_TIMEOUT)) {
        ;
    }

    return (timeout_count >= SPI_TIMEOUT) ? 1 : 0;
}

void spi_enable_interrupt_er8130(uint32_t spi_base, uint32_t int_src)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    REG_W32(SPI_INT_EN_ADDR, int_src);
}

void spi_disable_interrupt_er8130(uint32_t spi_base, uint32_t int_src)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    REG_W32(SPI_INT_EN_ADDR, REG_R32(SPI_INT_EN_ADDR) & ~int_src);
}

uint8_t spi_get_tx_fifo_depth_er8130(uint32_t spi_base)
{
    SPI_T *SPI           = (SPI_T *)spi_base;
    uint8_t tx_fifo_size = ((REG_R32(SPI_CFG_ADDR) & SPI_TX_FIFO_SIZE_MASK) >> SPI_TX_FIFO_SIZE_POS);
    return ((tx_fifo_size == 0) ? 2 : (tx_fifo_size == 1) ? 4 : (tx_fifo_size == 2) ? 8 : 16);
}

uint8_t spi_get_rx_fifo_depth_er8130(uint32_t spi_base)
{
    SPI_T *SPI           = (SPI_T *)spi_base;
    uint8_t rx_fifo_size = ((REG_R32(SPI_CFG_ADDR) & SPI_RX_FIFO_SIZE_MASK) >> SPI_RX_FIFO_SIZE_POS);
    return ((rx_fifo_size == 0) ? 2 : (rx_fifo_size == 1) ? 4 : (rx_fifo_size == 2) ? 8 : 16);
}

void spi_set_tx_fifo_threshold_er8130(uint32_t spi_base, uint8_t threshold)
{
    SPI_T *SPI   = (SPI_T *)spi_base;
    uint32_t reg = REG_R32(SPI_CTRL_ADDR);

    reg &= ~SPI_TX_THRES_MASK;
    reg |= (threshold << SPI_TX_THRES_POS) & SPI_TX_THRES_MASK;
    REG_W32(SPI_CTRL_ADDR, reg);
}

void spi_set_rx_fifo_threshold_er8130(uint32_t spi_base, uint8_t threshold)
{
    SPI_T *SPI   = (SPI_T *)spi_base;
    uint32_t reg = REG_R32(SPI_CTRL_ADDR);

    reg &= ~SPI_RX_THRES_MASK;
    reg |= (threshold << SPI_RX_THRES_POS) & SPI_RX_THRES_MASK;
    REG_W32(SPI_CTRL_ADDR, reg);
}

uint8_t spi_get_tx_fifo_threshold_er8130(uint32_t spi_base)
{
    SPI_T *SPI         = (SPI_T *)spi_base;
    uint32_t reg       = REG_R32(SPI_CTRL_ADDR);
    uint8_t fifo_units = (reg & SPI_TX_THRES_MASK) >> SPI_TX_THRES_POS;
    return fifo_units;
}

uint8_t spi_get_rx_fifo_threshold_er8130(uint32_t spi_base)
{
    SPI_T *SPI         = (SPI_T *)spi_base;
    uint32_t reg       = REG_R32(SPI_CTRL_ADDR);
    uint8_t fifo_units = (reg & SPI_RX_THRES_MASK) >> SPI_RX_THRES_POS;
    return fifo_units;
}

void spi_set_slv_rdy_er8130(uint32_t spi_base, uint8_t rdy)
{
    uint32_t reg = 0;
    SPI_T *SPI   = (SPI_T *)spi_base;

    reg = REG_R32(SPI_SLV_STS_ADDR);
    reg = (reg & ~SPI_READY_MASK) | ((rdy << SPI_READY_POS) & SPI_READY_MASK);

    REG_W32(SPI_SLV_STS_ADDR, reg);
}

void spi_cmd_trigger_er8130(uint32_t spi_base, uint8_t flag_en, uint8_t cmd)
{
    uint8_t spi_cmd = (flag_en & SPI_MSG_CMD_EN) ? cmd : 0;
    SPI_T *SPI      = (SPI_T *)spi_base;
    REG_W32(SPI_CMD_ADDR, spi_cmd);
}

uint8_t spi_get_cmd_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    return REG_R32(SPI_CMD_ADDR);
}

uint8_t spi_is_tx_fifo_full_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    return (REG_R32(SPI_STS_ADDR) & SPI_TX_FULL_MASK) ? (1) : (0);
}

uint8_t spi_is_rx_fifo_empty_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    return (REG_R32(SPI_STS_ADDR) & SPI_RX_EMPTY_MASK) ? (1) : (0);
}

uint8_t spi_get_interrupt_status_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    return (uint8_t)REG_R32(SPI_INT_STS_ADDR);
}

void spi_clear_interrupt_status_er8130(uint32_t spi_base, uint8_t status)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    REG_W32(SPI_INT_STS_ADDR, status);
}

void spi_write_data_er8130(uint32_t spi_base, uint32_t value)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    REG_W32(SPI_DATA_ADDR, value);
}

uint32_t spi_read_data_er8130(uint32_t spi_base)
{
    SPI_T *SPI = (SPI_T *)spi_base;
    return REG_R32(SPI_DATA_ADDR);
}

uint16_t spi_get_slv_write_cnt_er8130(uint32_t spi_base)
{
    SPI_T *SPI    = (SPI_T *)spi_base;
    uint16_t wcnt = ((REG_R32(SPI_SLV_DATA_CNT_ADDR) & SPI_WCNT_MASK) >> SPI_WCNT_POS);
    return wcnt;
}

uint16_t spi_get_slv_read_cnt_er8130(uint32_t spi_base)
{
    SPI_T *SPI    = (SPI_T *)spi_base;
    uint16_t rcnt = ((REG_R32(SPI_SLV_DATA_CNT_ADDR) & SPI_RCNT_MASK) >> SPI_RCNT_POS);
    return rcnt;
}

void spi_config_er8130(uint32_t spi_base, uint16_t clock_mode, uint8_t role, uint8_t data_bit_width, uint8_t addr_len, uint32_t bus_clock,
                       uint8_t bit_order, uint8_t data_merge, uint8_t mosi_bir_dir, uint8_t dual_quard_mode, uint8_t addr_fmt, uint8_t dma_enable)
{
    uint32_t reg     = 0;
    uint8_t sclk_div = SPI_SCLK_DIV_MASK;
    SPI_T *SPI       = (SPI_T *)spi_base;
    /* Set SPI frame format */
    /*
        clock_mode 0: CPOL=0, CPHA=0
        clock_mode 1: CPOL=0, CPHA=1
        clock_mode 2: CPOL=1, CPHA=0
        clock_mode 3: CPOL=1, CPHA=1
    */
    reg = (reg & ~(SPI_CPHA_MASK | SPI_CPOL_MASK | SPI_SLV_MODE_MASK | SPI_LSB_MASK | SPI_MOSI_BI_DIR_MASK | SPI_DATA_MERGE_MASK | SPI_DATA_LEN_MASK |
                   SPI_ADDR_LEN_MASK)) |
          (((clock_mode & 0x01) << SPI_CPHA_POS) & SPI_CPHA_MASK) | (((clock_mode >> 1) << SPI_CPOL_POS) & SPI_CPOL_MASK) |
          ((role << SPI_SLV_MODE_POS) & SPI_SLV_MODE_MASK) | ((bit_order << SPI_LSB_POS) & SPI_LSB_MASK) |
          ((mosi_bir_dir << SPI_MOSI_BI_DIR_POS) & SPI_MOSI_BI_DIR_MASK) | ((data_merge << SPI_DATA_MERGE_POS) & SPI_DATA_MERGE_MASK) |
          (((data_bit_width - 1) << SPI_DATA_LEN_POS) & SPI_DATA_LEN_MASK) | (((addr_len - 1) << SPI_ADDR_LEN_POS) & SPI_ADDR_LEN_MASK);
    REG_W32(SPI_TRANS_FMT_ADDR, reg);

    reg = REG_R32(SPI_TRANS_CTRL_ADDR);
    /* Set spi data phase format */
    reg = (reg & ~SPI_DUAL_QUAD_MASK) | ((dual_quard_mode << SPI_DUAL_QUAD_POS) & SPI_DUAL_QUAD_MASK);
    /* Set spi address phase format */
    reg = (reg & ~SPI_ADDR_FMT_MASK) | ((addr_fmt << SPI_ADDR_FMT_POS) & SPI_ADDR_FMT_MASK);
    REG_W32(SPI_TRANS_CTRL_ADDR, reg);
    /* Set the divisor for SPI interface sclk */
    if (role == MASTER_MODE) {
        if (bus_clock < SystemCoreClock) {
            sclk_div = ((SystemCoreClock / (bus_clock) / 2) - 1);
        }
        reg = REG_R32(SPI_SCLK_DIV_ADDR);
        reg = (reg & ~SPI_SCLK_DIV_MASK) | ((sclk_div << SPI_SCLK_DIV_POS) & SPI_SCLK_DIV_MASK);
        REG_W32(SPI_SCLK_DIV_ADDR, reg);
    }

    spi_set_tx_fifo_threshold_er8130(spi_base, (spi_get_tx_fifo_depth_er8130(spi_base) >> 1));
    spi_set_rx_fifo_threshold_er8130(spi_base, (spi_get_rx_fifo_depth_er8130(spi_base) >> 1));

    //spi_reset_fifo_er8130(spi_base);

    if (role == ROLE_SLAVE) {
        spi_reset_er8130(spi_base, SPI_RESET_RX);
        SPI_ENABLE_IRQ(spi_base);
        if (dma_enable) {
            spi_enable_interrupt_er8130(spi_base, (SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
        } else {
            spi_enable_interrupt_er8130(spi_base, (SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
        }
    }
}

void spi_set_xfer_fmt_er8130(uint32_t spi_base, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len, uint8_t trans_mode,
                             uint8_t flag_en, uint32_t addr_value)
{
    uint32_t reg = 0;
    SPI_T *SPI   = (SPI_T *)spi_base;

    uint16_t rx_data_len    = rx_unit_count > 0 ? rx_unit_count - 1 : 0;
    uint16_t tx_data_len    = tx_unit_count > 0 ? tx_unit_count - 1 : 0;
    uint16_t dummy_data_len = dummy_len > 0 ? dummy_len - 1 : 0;
    uint8_t cmd_en          = (flag_en & SPI_MSG_CMD_EN) ? 1 : 0;
    uint8_t addr_en         = (flag_en & SPI_MSG_ADDR_EN) ? 1 : 0;

    reg = REG_R32(SPI_TRANS_CTRL_ADDR);
    /* Set data length */
    reg = (reg & ~(SPI_RD_TRAN_CNT_MASK | SPI_WR_TRAN_CNT_MASK | SPI_DMY_CNT_MASK)) | ((rx_data_len << SPI_RD_TRAN_CNT_POS) & SPI_RD_TRAN_CNT_MASK) |
          ((tx_data_len << SPI_WR_TRAN_CNT_POS) & SPI_WR_TRAN_CNT_MASK) | ((dummy_data_len << SPI_DMY_CNT_POS) & SPI_DMY_CNT_MASK);
    /* Set transfer mode */
    reg = (reg & ~SPI_TRANS_MODE_MASK) | ((trans_mode << SPI_TRANS_MODE_POS) & SPI_TRANS_MODE_MASK);
    /* Set cmd and address phase enable */
    flag_en &= (SPI_MSG_CMD_EN | SPI_MSG_ADDR_EN);
    reg      = (reg & ~(SPI_CMD_EN_MASK | SPI_ADDR_EN_MASK)) | ((cmd_en << SPI_CMD_EN_POS) & SPI_CMD_EN_MASK) |
          ((addr_en << SPI_ADDR_EN_POS) & SPI_ADDR_EN_MASK);
    REG_W32(SPI_TRANS_CTRL_ADDR, reg);

    if (flag_en & SPI_MSG_ADDR_EN) {
        REG_W32(SPI_ADDR_ADDR, addr_value);
    }

    if (rx_unit_count > 0) {
        spi_reset_er8130(spi_base, SPI_RESET_RX);
    }
    if (tx_unit_count > 0) {
        spi_reset_er8130(spi_base, SPI_RESET_TX);
    }
}

HAL_STATUS spi_xfer_exec_er8130(uint32_t spi_base, uint8_t role, uint16_t rx_unit_count, uint16_t tx_unit_count, uint8_t dummy_len,
                                uint8_t trans_mode, uint8_t flag_en, uint32_t addr_value, uint8_t cmd_value, void *tx_fifo, void *rx_fifo,
                                uint8_t dma_enable)
{
    // SPI_T *SPI = (SPI_T *)spi_base;
    // uint16_t tx_data_conunt = ((tx_len + 3) >> 2);

    if (spi_wait_for_completion_er8130(spi_base)) {
        debug_printf("spi wait prog done timeout\n");
        return HAL_ERR;
    }

    spi_disable_interrupt_er8130(spi_base, SPI_TX_FIFO_INT_EN_MASK | SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK);
    //    spi_reset_fifo_er8130(spi_base);
    spi_set_xfer_fmt_er8130(spi_base, rx_unit_count, tx_unit_count, dummy_len, trans_mode, flag_en, addr_value);
    if (dma_enable) {
        if (tx_unit_count != 0 && tx_fifo != NULL) {
            spi_tx_dma_enable_er8130(spi_base, ENABLE);
        }
        if (rx_unit_count != 0 && rx_fifo != NULL) {
            spi_rx_dma_enable_er8130(spi_base, ENABLE);
        }
        spi_enable_interrupt_er8130(spi_base, (SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
    } else {
        spi_enable_interrupt_er8130(spi_base, (SPI_TX_FIFO_INT_EN_MASK | SPI_RX_FIFO_INT_EN_MASK | SPI_END_INT_EN_MASK | SPI_SLV_CMD_EN_MASK));
    }
    SPI_ENABLE_IRQ(spi_base);

    if (role == ROLE_MASTER) {
        spi_cmd_trigger_er8130(spi_base, flag_en, cmd_value);
    } else {
        spi_set_slv_rdy_er8130(spi_base, 1);
    }

    return HAL_NO_ERR;
}

HAL_STATUS spi_tx_dma_enable_er8130(uint32_t spi_base, uint8_t dma_enable)
{
    SPI_T *SPI   = (SPI_T *)spi_base;
    uint32_t reg = REG_R32(SPI_CTRL_ADDR);

    reg = ((reg & ~SPI_TX_DMA_EN_MASK) | ((dma_enable << SPI_TX_DMA_EN_POS) & SPI_TX_DMA_EN_MASK));
    REG_W32(SPI_CTRL_ADDR, reg);
    return HAL_NO_ERR;
}

HAL_STATUS spi_rx_dma_enable_er8130(uint32_t spi_base, uint8_t dma_enable)
{
    SPI_T *SPI   = (SPI_T *)spi_base;
    uint32_t reg = REG_R32(SPI_CTRL_ADDR);

    reg = ((reg & ~SPI_RX_DMA_EN_MASK) | ((dma_enable << SPI_RX_DMA_EN_POS) & SPI_RX_DMA_EN_MASK));
    REG_W32(SPI_CTRL_ADDR, reg);
    return HAL_NO_ERR;
}

HAL_STATUS spi_set_sclk_div_er8130(uint32_t spi_base, uint8_t val)
{
    SPI_T *SPI   = (SPI_T *)spi_base;
    uint32_t reg = REG_R32(SPI_INTF_TIMING_ADDR);

    reg = ((reg & ~SPI_SCLK_DIV_MASK) | ((val << SPI_SCLK_DIV_POS) & SPI_SCLK_DIV_MASK));
    REG_W32(SPI_INTF_TIMING_ADDR, reg);
    return HAL_NO_ERR;
}
