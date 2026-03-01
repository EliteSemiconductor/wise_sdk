/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __DMA_ER813X_H
#define __DMA_ER813X_H

#include "esmt_chip_specific.h"
#include "types.h"

#define CH_ADDR_ALIGN 0x40
#define DMA_CH_OFS(ch) ((ch) * CH_ADDR_ALIGN)

#define DMA_CFG_INCRE_ADDR 0x00
#define DMA_CFG_FIXED_ADDR 0x10

#define UART0_DAT_ADDR (0x40030000UL + 0x20)
#define UART1_DAT_ADDR (0x40031000UL + 0x20)
#define UART2_DAT_ADDR (0x40032000UL + 0x20)
#define SPI0_DAT_ADDR (0x40033000UL + 0x2C)
#define SPI1_DAT_ADDR (0x40034000UL + 0x2C)
#define I2C0_DAT_ADDR (0x40035000UL + 0x20)
#define I2C1_DAT_ADDR (0x40036000UL + 0x20)

#define M2M_ADDR_TYPE_INCRE 0x00
#define M2M_ADDR_TYPE_FIXED 0x02

#define M2M_MAX_LEN 0x3FFF

enum dma_buf_type {
    DMA_BUF_A,
    DMA_BUF_B
};

void dma_module_reset_er8130(void);
void dma_sw_reset_er8130(uint32_t ch_ofs);
void dma_channel_init_er8130(uint32_t ch_ofs);
void dma_set_channel_buf_er8130(uint32_t ch_ofs, uint8_t buf_op, uint32_t addr, uint32_t len);
void dma_set_periph_req_idx_er8130(uint32_t ch_ofs, uint8_t req_idx);
void dma_set_extsrc_req_idx_er8130(uint32_t ch_ofs, uint8_t req_idx);
void dma_set_m2m_ext_run_trigger_er8130(uint32_t ch_ofs);
void dma_p2m2p_config_er8130(uint8_t ch, uint8_t req_idx, uint32_t addr, uint32_t len);
void dma_p2m2p_start_er8130(uint8_t ch);
void dma_m2m_set_src_type_er8130(uint32_t ch_ofs, uint8_t type, uint8_t nxt_unit);
void dma_m2m_set_dst_type_er8130(uint32_t ch_ofs, uint8_t type, uint8_t nxt_unit);
void dma_m2m_set_data_size_er8130(uint32_t ch_ofs, uint8_t data_size);
void dma_m2m_setup_transfer_er8130(uint32_t ch_ofs, uint32_t src_addr, uint32_t dst_addr, uint32_t len);
void dma_m2m_start_er8130(uint32_t ch_ofs);
void dma_clear_buf_cmplt_sts_er8130(uint8_t buf_type, uint32_t status);
uint32_t dma_get_buf_a_cmplt_sts_er8130(void);
void dma_clear_buf_a_cmplt_sts_er8130(uint32_t status);
uint32_t dma_get_buf_b_cmplt_sts_er8130(void);
void dma_clear_buf_b_cmplt_sts_er8130(uint32_t status);
uint32_t dma_get_ch_m2m_cmplt_sts_er8130(void);
void dma_clear_ch_m2m_cmplt_sts_er8130(uint32_t status);
uint32_t dma_get_ch_err_ovrn_a_sts_er8130(void);
void dma_clear_ch_err_ovrn_a_sts_er8130(uint32_t status);
uint32_t dma_get_ch_err_ovrn_b_sts_er8130(void);
void dma_clear_ch_err_ovrn_b_sts_er8130(uint32_t status);
uint32_t dma_get_ch_err_ex_size_a_sts_er8130(void);
void dma_clear_ch_err_ex_size_a_sts_er8130(uint32_t status);
uint32_t dma_get_ch_err_ex_size_b_sts_er8130(void);
void dma_clear_ch_err_ex_size_b_sts_er8130(uint32_t status);
bool dma_is_all_channel_disable_er8130(void);
void dma_set_ping_pong_mode_er8130(uint32_t ch_ofs, bool enable);
void dma_set_m2m_run_er8130(uint32_t ch_ofs, bool enable);
void dma_set_m2m_ext_run_er8130(uint32_t ch_ofs, bool enable, uint8_t req_idx);
void dma_enable_ch_cmplt_a_er8130(uint8_t ch, bool enable);
void dma_enable_ch_cmplt_b_er8130(uint8_t ch, bool enable);
void dma_enable_ch_err_ex_size_a_er8130(uint8_t ch, bool enable);
void dma_enable_ch_err_ex_size_b_er8130(uint8_t ch, bool enable);
void dma_enable_ch_err_ovrn_a_er8130(uint8_t ch, bool enable);
void dma_enable_ch_err_ovrn_b_er8130(uint8_t ch, bool enable);
void dma_enable_ch_m2m_cmplt_er8130(uint8_t ch, bool enable);
void dma_buffer_trigger_er8130(uint32_t ch_ofs, uint32_t buf_op);
void dma_set_buf_end_by_peri_er8130(uint32_t ch_ofs, uint8_t enable);
#endif
