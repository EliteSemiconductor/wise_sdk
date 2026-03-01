/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/dma_er8130.h"
#include "hdl/pmu_er8130.h"
#include "types.h"
#include <stdint.h>

void dma_module_reset_er8130(void)
{
    pmu_module_reset_er8130(DMA_MODULE);
}
void dma_sw_reset_er8130(uint32_t ch_ofs)
{
    uint32_t addr = DMA_CH0_SWRST_REG_ADDR + ch_ofs;

    REG_W32(addr, DMA_CH0_SWRST_MASK);
    // for (volatile int i = 0; i < 10000; i++)
    //     ;                      // wait for reset to complete
    REG_W32(addr, 0x00000000); // clear reset
}

void dma_channel_init_er8130(uint32_t ch_ofs)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    dma_sw_reset_er8130(ch_ofs);

    reg |= DMA_CH0_EN_MASK;
    REG_W32(addr, reg);
}

void dma_buffer_trigger_er8130(uint32_t ch_ofs, uint32_t buf_op)
{
    uint32_t trig_addr = DMA_CH0_TRIG_REG_ADDR + ch_ofs;
    uint32_t trig_val  = REG_R32(trig_addr) & ~DMA_W1T_CH0_M2M_TRIG_MASK;
    REG_W32(trig_addr, trig_val);

    uint32_t cfg_addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t cfg_val  = REG_R32(cfg_addr) | DMA_CH0_EN_MASK;
    REG_W32(cfg_addr, cfg_val);

    uint32_t buf_addr = DMA_CH0_BUF_EN_REG_ADDR + ch_ofs;
    uint32_t buf_val  = buf_op ? DMA_CH0_BUF_EN_B_MASK : DMA_CH0_BUF_EN_A_MASK;

    REG_W32(buf_addr, buf_val);
}

void dma_set_channel_buf_er8130(uint32_t ch_ofs, uint8_t buf_op, uint32_t addr, uint32_t len)
{
    uint32_t trans_len = (len >> 2) + ((len & (sizeof(uint32_t) - 1)) ? 1 : 0);
    uint32_t buf_addr, buf_en;

    switch (buf_op) {
    case DMA_BUF_A:
        buf_addr = DMA_CH0_BUF_BASE_ADDR_A_REG_ADDR;
        buf_en   = DMA_CH0_BUF_EN_A_MASK;
        break;
    case DMA_BUF_B:
        buf_addr = DMA_CH0_BUF_BASE_ADDR_B_REG_ADDR;
        buf_en   = DMA_CH0_BUF_EN_B_MASK;
        break;
    default:
        buf_addr = DMA_CH0_BUF_BASE_ADDR_A_REG_ADDR;
        buf_en   = DMA_CH0_BUF_EN_A_MASK;
        break;
    }

    REG_W32(buf_addr + ch_ofs, addr);
    REG_W32(DMA_CH0_BUF_WORD_SIZE_REG_ADDR + ch_ofs, trans_len);
    REG_W32(DMA_CH0_BUF_EN_REG_ADDR + ch_ofs, buf_en);
}

void dma_set_periph_req_idx_er8130(uint32_t ch_ofs, uint8_t req_idx)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg &= ~DMA_CH0_PERI_REQ_IDX_MASK;
    reg |= ((req_idx << DMA_CH0_PERI_REQ_IDX_POS) & DMA_CH0_PERI_REQ_IDX_MASK);

    REG_W32(addr, reg);
}

void dma_set_extsrc_req_idx_er8130(uint32_t ch_ofs, uint8_t req_idx)
{
    uint32_t addr = DMA_CH0_TRIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    uint32_t addr1 = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg1  = REG_R32(addr1);

    reg1 &= ~DMA_CH0_PERI_REQ_IDX_MASK;

    REG_W32(addr1, reg1);
    //printf("ch_ofs = %lx\n", ch_ofs);
    reg &= ~DMA_CH0_M2M_EXT_SRC_MASK;
    reg |= ((req_idx << DMA_CH0_M2M_EXT_SRC_POS) & DMA_CH0_M2M_EXT_SRC_MASK);
    //printf("addr = %lx reg = %lx\n", addr, reg);
    REG_W32(addr, reg);
}

void dma_m2m_set_src_type_er8130(uint32_t ch_ofs, uint8_t type, uint8_t nxt_unit)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg &= ~(DMA_CH0_M2M_SRC_TYP_MASK | DMA_CH0_M2M_SRC_NXT_MASK);
    reg |= ((type << DMA_CH0_M2M_SRC_TYP_POS) & DMA_CH0_M2M_SRC_TYP_MASK);
    reg |= ((nxt_unit << DMA_CH0_M2M_SRC_NXT_POS) & DMA_CH0_M2M_SRC_NXT_MASK);

    REG_W32(addr, reg);
}

void dma_m2m_set_dst_type_er8130(uint32_t ch_ofs, uint8_t type, uint8_t nxt_unit)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg &= ~(DMA_CH0_M2M_DEST_TYP_MASK | DMA_CH0_M2M_DEST_NXT_MASK);
    reg |= ((type << DMA_CH0_M2M_DEST_TYP_POS) & DMA_CH0_M2M_DEST_TYP_MASK);
    reg |= ((nxt_unit << DMA_CH0_M2M_DEST_NXT_POS) & DMA_CH0_M2M_DEST_NXT_MASK);

    REG_W32(addr, reg);
}

void dma_m2m_set_data_size_er8130(uint32_t ch_ofs, uint8_t data_size)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg &= ~DMA_CH0_M2M_DAT_SIZE_MASK;
    reg |= ((data_size << DMA_CH0_M2M_DAT_SIZE_POS) & DMA_CH0_M2M_DAT_SIZE_MASK);

    REG_W32(addr, reg);
}

void dma_set_ping_pong_mode_er8130(uint32_t ch_ofs, bool enable)
{
    uint32_t addr = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg &= ~(DMA_CH0_PING_PONG_EN_MASK);
    reg |= ((enable << DMA_CH0_PING_PONG_EN_POS) & DMA_CH0_PING_PONG_EN_MASK);

    REG_W32(addr, reg);
}

void dma_m2m_setup_transfer_er8130(uint32_t ch_ofs, uint32_t src_addr, uint32_t dst_addr, uint32_t len)
{
    //uint32_t word_len = (len + 3) & ~0x3;
    REG_W32(DMA_CH0_M2M_SRC_ADDR_REG_ADDR + ch_ofs, src_addr);
    REG_W32(DMA_CH0_M2M_DEST_ADDR_REG_ADDR + ch_ofs, dst_addr);
    REG_W32(DMA_CH0_M2M_LEN_REG_ADDR + ch_ofs, len & DMA_CH0_M2M_LEN_MASK);
}

void dma_m2m_start_er8130(uint32_t ch_ofs)
{
    uint32_t addr_cfg = DMA_CH0_CONFIG_REG_ADDR + ch_ofs;
    uint32_t reg_cfg  = REG_R32(addr_cfg);

    reg_cfg |= (DMA_CH0_M2M_MASK | DMA_CH0_EN_MASK);
    REG_W32(addr_cfg, reg_cfg);
}

void dma_set_m2m_run_er8130(uint32_t ch_ofs, bool enable)
{
    uint32_t addr_trig = DMA_CH0_TRIG_REG_ADDR + ch_ofs;
    uint32_t reg_trig  = REG_R32(addr_trig);

    reg_trig |= DMA_W1T_CH0_M2M_TRIG_MASK;
    REG_W32(addr_trig, reg_trig);
}

void dma_set_m2m_ext_run_er8130(uint32_t ch_ofs, bool enable, uint8_t req_idx)
{
    uint32_t addr = DMA_CH0_TRIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    if (enable) {
        reg |= ((req_idx << DMA_CH0_M2M_EXT_SRC_POS) & DMA_CH0_M2M_EXT_SRC_MASK);
        reg |= (DMA_CH0_M2M_EXT_RUN_MASK);
    } else {
        reg &= ~(DMA_CH0_M2M_EXT_RUN_MASK);
    }
    REG_W32(addr, reg);
}

void dma_set_m2m_ext_run_trigger_er8130(uint32_t ch_ofs)
{
    uint32_t addr = DMA_CH0_TRIG_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    reg |= (DMA_CH0_M2M_EXT_RUN_MASK);
    REG_W32(addr, reg);
}

void dma_clear_buf_cmplt_sts_er8130(uint8_t buf_type, uint32_t status)
{
    uint32_t buf_addr;

    buf_addr = (buf_type == DMA_BUF_A) ? DMA_INT_ST_CH_CMPLT_A_REG_ADDR : DMA_INT_ST_CH_CMPLT_B_REG_ADDR;
    REG_W32(buf_addr, status);
}

uint32_t dma_get_buf_a_cmplt_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_CMPLT_A_REG_ADDR);
}

void dma_clear_buf_a_cmplt_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_CMPLT_A_REG_ADDR, status);
}

uint32_t dma_get_buf_b_cmplt_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_CMPLT_B_REG_ADDR);
}

void dma_clear_buf_b_cmplt_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_CMPLT_B_REG_ADDR, status);
}

uint32_t dma_get_ch_m2m_cmplt_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_M2M_CMPLT_REG_ADDR);
}

void dma_clear_ch_m2m_cmplt_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_M2M_CMPLT_REG_ADDR, status);
}

uint32_t dma_get_ch_err_ovrn_a_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_ERR_OVRN_A_REG_ADDR);
}

void dma_clear_ch_err_ovrn_a_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_ERR_OVRN_A_REG_ADDR, status);
}

uint32_t dma_get_ch_err_ovrn_b_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_ERR_OVRN_B_REG_ADDR);
}

void dma_clear_ch_err_ovrn_b_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_ERR_OVRN_B_REG_ADDR, status);
}

uint32_t dma_get_ch_err_ex_size_a_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_ERR_EX_SIZE_A_REG_ADDR);
}

void dma_clear_ch_err_ex_size_a_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_ERR_EX_SIZE_A_REG_ADDR, status);
}

uint32_t dma_get_ch_err_ex_size_b_sts_er8130(void)
{
    return REG_R32(DMA_INT_ST_CH_ERR_EX_SIZE_B_REG_ADDR);
}

void dma_clear_ch_err_ex_size_b_sts_er8130(uint32_t status)
{
    REG_W32(DMA_INT_ST_CH_ERR_EX_SIZE_B_REG_ADDR, status);
}

void dma_enable_ch_cmplt_a_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_CMPLT_A_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_CMPLT_A_REG_ADDR, reg);
    }
}

void dma_enable_ch_cmplt_b_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_CMPLT_B_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_CMPLT_B_REG_ADDR, reg);
    }
}

void dma_enable_ch_err_ex_size_a_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_ERR_EX_SIZE_A_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_ERR_EX_SIZE_A_REG_ADDR, reg);
    }
}

void dma_enable_ch_err_ex_size_b_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_ERR_EX_SIZE_B_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_ERR_EX_SIZE_B_REG_ADDR, reg);
    }
}

void dma_enable_ch_err_ovrn_a_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_ERR_OVRN_A_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_ERR_OVRN_A_REG_ADDR, reg);
    }
}

void dma_enable_ch_err_ovrn_b_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_ERR_OVRN_B_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_ERR_OVRN_B_REG_ADDR, reg);
    }
}

void dma_enable_ch_m2m_cmplt_er8130(uint8_t ch, bool enable)
{
    uint32_t reg = REG_R32(DMA_INT_EN_CH_M2M_CMPLT_REG_ADDR);

    if (ch < 8) {
        if (enable) {
            reg |= (1 << ch);
        } else {
            reg &= ~(1 << ch);
        }
        REG_W32(DMA_INT_EN_CH_M2M_CMPLT_REG_ADDR, reg);
    }
}

bool dma_is_all_channel_disable_er8130(void)
{
    uint32_t i;
    uint32_t ch_ofs;

    for (i = 0; i < CHIP_DMA_CHANNEL_NUM; i++) {
        ch_ofs = i * CH_ADDR_ALIGN;
        if (REG_R32(DMA_CH0_CONFIG_REG_ADDR + ch_ofs) & DMA_CH0_EN_MASK) {
            return false; // found a channel that is not disabled
        }
    }
    return true; // all channels are disabled
}

void dma_set_buf_end_by_peri_er8130(uint32_t ch_ofs, uint8_t enable)
{
    uint32_t addr = DMA_CH0_BUF_WORD_SIZE_REG_ADDR + ch_ofs;
    uint32_t reg  = REG_R32(addr);

    if (enable) {
        reg |= DMA_CH0_BUF_END_BY_PERI_MASK;
    } else {
        reg &= ~DMA_CH0_BUF_END_BY_PERI_MASK;
    }

    REG_W32(addr, reg);
}

void dma_init_er8130(void)
{
}

void dma_deinit_er8130(void)
{
}
