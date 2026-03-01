/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_dma.h"
#include "esmt_chip_specific.h"
#include "hdl/dma_er8130.h"
#include "types.h"
#include <stdint.h>

static EVT_CALLBACK_ENTRY_T dma_callbacks[CHIP_DMA_CHANNEL_NUM];
uint32_t dma_mem_bkp[(DMA_BACKUP_LEN >> 2)];

const DMA_EXTSRC_INFO_T dma_extsrc_info[DMA_FUNC_MAX] = {

    [DMA_SPI0_TX]  = {SPI0_DAT_ADDR, 2, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_SPI0_TX},
    [DMA_SPI0_RX]  = {SPI0_DAT_ADDR, 2, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_SPI0_RX},
    [DMA_SPI1_TX]  = {SPI1_DAT_ADDR, 2, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_SPI1_TX},
    [DMA_SPI1_RX]  = {SPI1_DAT_ADDR, 2, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_SPI1_RX},
    [DMA_UART0_TX] = {UART0_DAT_ADDR, 0, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_UART0_TX},
    [DMA_UART0_RX] = {UART0_DAT_ADDR, 0, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_UART0_RX},
    [DMA_UART1_TX] = {UART1_DAT_ADDR, 0, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_UART1_TX},
    [DMA_UART1_RX] = {UART1_DAT_ADDR, 0, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_UART1_RX},
    [DMA_UART2_TX] = {UART2_DAT_ADDR, 0, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_UART2_TX},
    [DMA_UART2_RX] = {UART2_DAT_ADDR, 0, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_UART2_RX},
    [DMA_I2C0_TX]  = {I2C0_DAT_ADDR, 0, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_I2C0},
    [DMA_I2C0_RX]  = {I2C0_DAT_ADDR, 0, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_I2C0},
    [DMA_I2C1_TX]  = {I2C1_DAT_ADDR, 0, DMA_DIR_MEM_TO_PERIPH, EXT_SRC_DMA_I2C1},
    [DMA_I2C1_RX]  = {I2C1_DAT_ADDR, 0, DMA_DIR_PERIPH_TO_MEM, EXT_SRC_DMA_I2C1},
};

HAL_STATUS hal_drv_dma_extsrc_config(uint8_t channel, DMA_SRC_REQ_SEL_T func)
{
    if (func >= DMA_USER || func <= DMA_SHA) {
        return HAL_ERR;
    }

    uint32_t ch_ofs = DMA_CH_OFS(channel);

    dma_channel_init_er8130(ch_ofs);
    dma_m2m_start_er8130(ch_ofs);
    dma_set_ping_pong_mode_er8130(ch_ofs, DISABLE);

    return HAL_NO_ERR;
}
HAL_STATUS hal_drv_dma_extsrc_update(uint8_t channel, DMA_SRC_REQ_SEL_T func, uint32_t mem_addr, uint32_t len)
{
    if (func >= DMA_USER || func <= DMA_SHA) {
        return HAL_ERR;
    }
    EXT_DIR_TYPE dir = dma_extsrc_info[func].def_dir;

    const DMA_EXTSRC_INFO_T *info = &dma_extsrc_info[func];
    uint32_t ch_ofs               = DMA_CH_OFS(channel);

    uint32_t src = (dir == DMA_DIR_MEM_TO_PERIPH) ? mem_addr : info->periph_addr;
    uint32_t dst = (dir == DMA_DIR_MEM_TO_PERIPH) ? info->periph_addr : mem_addr;

    uint8_t src_type = (dir == DMA_DIR_MEM_TO_PERIPH) ? M2M_ADDR_TYPE_INCRE : M2M_ADDR_TYPE_FIXED;
    uint8_t dst_type = (dir == DMA_DIR_MEM_TO_PERIPH) ? M2M_ADDR_TYPE_FIXED : M2M_ADDR_TYPE_INCRE;

    dma_channel_init_er8130(ch_ofs);
    dma_m2m_start_er8130(ch_ofs);
    dma_m2m_setup_transfer_er8130(ch_ofs, src, dst, len);
    dma_m2m_set_src_type_er8130(ch_ofs, src_type, dst_type);
    dma_m2m_set_dst_type_er8130(ch_ofs, dst_type, src_type);
    dma_m2m_set_data_size_er8130(ch_ofs, info->data_size);

    return HAL_NO_ERR;
}

void hal_drv_dma_extsrc_trigger(uint8_t channel, DMA_SRC_REQ_SEL_T func)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);

    const DMA_EXTSRC_INFO_T *info = &dma_extsrc_info[func];
    dma_set_extsrc_req_idx_er8130(ch_ofs, info->ext_src_sel);
    dma_set_m2m_ext_run_er8130(ch_ofs, ENABLE, info->ext_src_sel);
}

HAL_STATUS hal_drv_dma_p2m2p_update_transfer(uint8_t ch, uint32_t addr, uint32_t len)
{
    uint32_t ch_ofs = DMA_CH_OFS(ch);
    if (ch >= CHIP_DMA_CHANNEL_NUM) {
        return HAL_ERR;
    }

    dma_set_channel_buf_er8130(ch_ofs, DMA_BUF_A, addr, len);
    return HAL_NO_ERR;
}
void hal_drv_dma_buffer_trigger(uint8_t ch, uint32_t buf_op)
{
    uint32_t ch_ofs = DMA_CH_OFS(ch);
    dma_buffer_trigger_er8130(ch_ofs, buf_op);
}

void hal_drv_dma_p2m2p_config(uint8_t ch, uint8_t req_idx)
{
    uint32_t ch_ofs = DMA_CH_OFS(ch);
    dma_channel_init_er8130(ch_ofs);
    dma_set_periph_req_idx_er8130(ch_ofs, req_idx);

    if (req_idx != DMA_PERI_REQ_MAC_TX && req_idx != DMA_PERI_REQ_MAC_RX) {
        dma_set_buf_end_by_peri_er8130(ch_ofs, 1);
    }
}

void hal_drv_dma_p2m2p_trigger(uint8_t ch, uint32_t addr, uint32_t len)
{
    uint32_t ch_ofs = DMA_CH_OFS(ch);
    dma_set_channel_buf_er8130(ch_ofs, DMA_BUF_A, addr, len);
}

void hal_drv_dma_p2m2p_config_and_trigger(uint8_t ch, uint8_t req_idx, uint32_t addr, uint32_t len)
{
    hal_drv_dma_p2m2p_config(ch, req_idx);
    hal_drv_dma_p2m2p_trigger(ch, addr, len);
}

void hal_drv_dma_modem_backup(void)
{
    uint32_t ch_ofs = DMA_M2M_BKUP_CHANL * CH_ADDR_ALIGN;
    // backup setting
    dma_channel_init_er8130(ch_ofs);
    dma_m2m_set_src_type_er8130(ch_ofs, M2M_ADDR_INCRE_MODE, M2M_ADDR_INCRE_TYPE_DEFT);
    dma_m2m_set_dst_type_er8130(ch_ofs, M2M_ADDR_INCRE_MODE, M2M_ADDR_INCRE_TYPE_DEFT);
    dma_m2m_set_data_size_er8130(ch_ofs, DATA_SIZE_32BITS);
    dma_m2m_setup_transfer_er8130(ch_ofs, DMA_CSR_BKUP_ADDR, (uint32_t)dma_mem_bkp, DMA_BACKUP_LEN);

    // recover setting
    ch_ofs = DMA_M2M_RCOV_CHANL * CH_ADDR_ALIGN;
    dma_channel_init_er8130(ch_ofs);
    dma_m2m_set_src_type_er8130(ch_ofs, M2M_ADDR_INCRE_MODE, M2M_ADDR_INCRE_TYPE_DEFT);
    dma_m2m_set_dst_type_er8130(ch_ofs, M2M_ADDR_INCRE_MODE, M2M_ADDR_INCRE_TYPE_DEFT);
    dma_m2m_set_data_size_er8130(ch_ofs, DATA_SIZE_32BITS);
    dma_m2m_setup_transfer_er8130(ch_ofs, (uint32_t)dma_mem_bkp, DMA_CSR_BKUP_ADDR, DMA_BACKUP_LEN);
}

void hal_drv_dma_extsrc_m2m_config(uint8_t channel, uint8_t ext_src_type, DMA_EXT_DIR_TYPE trans_type, uint32_t data_addr, uint32_t data_size,
                                   uint32_t data_len)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);
    uint32_t src_addr, dst_addr;
    uint8_t dat_size = 0, sour_type, dst_type;

    dma_channel_init_er8130(ch_ofs);

    switch (ext_src_type) {
    case EXT_SRC_DMA_SPI0_TX:
    case EXT_SRC_DMA_SPI0_RX:
        src_addr = (trans_type == 0x00) ? data_addr : SPI0_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? SPI0_DAT_ADDR : data_addr;
        dat_size = 2;
        break;
    case EXT_SRC_DMA_SPI1_TX:
    case EXT_SRC_DMA_SPI1_RX:
        src_addr = (trans_type == 0x00) ? data_addr : SPI1_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? SPI1_DAT_ADDR : data_addr;
        dat_size = 2;
        break;
    case EXT_SRC_DMA_UART0_TX:
    case EXT_SRC_DMA_UART0_RX:
        src_addr = (trans_type == 0x00) ? data_addr : UART0_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? UART0_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    case EXT_SRC_DMA_UART1_TX:
    case EXT_SRC_DMA_UART1_RX:
        src_addr = (trans_type == 0x00) ? data_addr : UART1_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? UART1_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    case EXT_SRC_DMA_UART2_TX:
    case EXT_SRC_DMA_UART2_RX:
        src_addr = (trans_type == 0x00) ? data_addr : UART2_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? UART2_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    case EXT_SRC_DMA_I2C0:
        src_addr = (trans_type == 0x00) ? data_addr : I2C0_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? I2C0_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    case EXT_SRC_DMA_I2C1:
        src_addr = (trans_type == 0x00) ? data_addr : I2C1_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? I2C1_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    default:
        src_addr = (trans_type == 0x00) ? data_addr : UART0_DAT_ADDR;
        dst_addr = (trans_type == 0x00) ? UART0_DAT_ADDR : data_addr;
        dat_size = 0;
        break;
    }

    sour_type = (trans_type == 0x00) ? M2M_ADDR_TYPE_INCRE : M2M_ADDR_TYPE_FIXED;
    dst_type  = (trans_type == 0x00) ? M2M_ADDR_TYPE_FIXED : M2M_ADDR_TYPE_INCRE;
    dma_m2m_start_er8130(ch_ofs);
    dma_m2m_setup_transfer_er8130(ch_ofs, src_addr, dst_addr, data_len);
    dma_m2m_set_src_type_er8130(ch_ofs, sour_type, dst_type);
    dma_m2m_set_dst_type_er8130(ch_ofs, dst_type, sour_type);
    dma_m2m_set_data_size_er8130(ch_ofs, dat_size);
    dma_set_ping_pong_mode_er8130(ch_ofs, DISABLE);
    dma_set_extsrc_req_idx_er8130(ch_ofs, ext_src_type);
}

void hal_drv_dma_extsrc_m2m_trigger(uint8_t channel, uint8_t req_idx)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);
    dma_set_m2m_ext_run_er8130(ch_ofs, ENABLE, req_idx);
}
void hal_drv_dma_src_m2m_trigger(uint8_t channel)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);
    dma_set_m2m_run_er8130(ch_ofs, ENABLE);
}
void hal_drv_dma_m2m_dst_src_config(uint8_t channel, uint8_t src_type, uint8_t dst_type, uint8_t src_nxt_unit, uint8_t dst_nxt_unit)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);
    dma_m2m_set_src_type_er8130(ch_ofs, src_type, src_nxt_unit);
    dma_m2m_set_dst_type_er8130(ch_ofs, dst_type, dst_nxt_unit);
}

void hal_drv_dma_m2m_config(uint8_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t data_size, uint32_t data_len)
{
    uint32_t ch_ofs = DMA_CH_OFS(channel);

    dma_channel_init_er8130(ch_ofs);
    dma_m2m_setup_transfer_er8130(ch_ofs, src_addr, dst_addr, data_len);
    dma_m2m_set_data_size_er8130(ch_ofs, data_size);
    dma_set_ping_pong_mode_er8130(ch_ofs, DISABLE);
    dma_m2m_start_er8130(ch_ofs);
}
uint32_t hal_drv_dma_interrupt_enable(uint8_t channel, bool enable)
{
    dma_enable_ch_cmplt_a_er8130(channel, enable);
    dma_enable_ch_cmplt_b_er8130(channel, enable);
    dma_enable_ch_err_ex_size_a_er8130(channel, enable);
    dma_enable_ch_err_ex_size_b_er8130(channel, enable);
    dma_enable_ch_err_ovrn_a_er8130(channel, enable);
    dma_enable_ch_err_ovrn_b_er8130(channel, enable);
    dma_enable_ch_m2m_cmplt_er8130(channel, enable);
    if (enable) {
        NVIC_EnableIRQ(DMA_IRQn);
    }
    return HAL_NO_ERR;
}
uint32_t hal_drv_dma_buf_a_cmplt_status(void)
{
    return dma_get_buf_a_cmplt_sts_er8130();
}

void hal_drv_dma_buf_a_cmplt_clear(uint32_t status)
{
    dma_clear_buf_a_cmplt_sts_er8130(status);
}

uint32_t hal_drv_dma_buf_b_cmplt_status(void)
{
    return dma_get_buf_b_cmplt_sts_er8130();
}

void hal_drv_dma_buf_b_cmplt_clear(uint32_t status)
{
    dma_clear_buf_b_cmplt_sts_er8130(status);
}

uint32_t hal_drv_dma_m2m_cmplt_status(void)
{
    return dma_get_ch_m2m_cmplt_sts_er8130();
}

void hal_drv_dma_m2m_cmplt_clear(uint32_t status)
{
    dma_clear_ch_m2m_cmplt_sts_er8130(status);
}

uint32_t hal_drv_dma_get_err_ovrn_a_status(void)
{
    return dma_get_ch_err_ovrn_a_sts_er8130();
}

void hal_drv_dma_clear_err_ovrn_a_status(uint32_t status)
{
    dma_clear_ch_err_ovrn_a_sts_er8130(status);
}

uint32_t hal_drv_dma_get_err_ovrn_b_status(void)
{
    return dma_get_ch_err_ovrn_b_sts_er8130();
}

void hal_drv_dma_clear_err_ovrn_b_status(uint32_t status)
{
    dma_clear_ch_err_ovrn_b_sts_er8130(status);
}

uint32_t hal_drv_dma_get_err_ex_size_a_status(void)
{
    return dma_get_ch_err_ex_size_a_sts_er8130();
}

void hal_drv_dma_clear_err_ex_size_a_status(uint32_t status)
{
    dma_clear_ch_err_ex_size_a_sts_er8130(status);
}

uint32_t hal_drv_dma_get_err_ex_size_b_status(void)
{
    return dma_get_ch_err_ex_size_b_sts_er8130();
}

void hal_drv_dma_clear_err_ex_size_b_status(uint32_t status)
{
    dma_clear_ch_err_ex_size_b_sts_er8130(status);
}

void hal_drv_dma_module_reset(void)
{
    dma_module_reset_er8130();
}

HAL_STATUS hal_drv_dma_register_callback(uint8_t dma_ch_idx, EVT_CALLBACK_T cb, void *context)
{
    if (dma_ch_idx < CHIP_DMA_CHANNEL_NUM) {
        dma_callbacks[dma_ch_idx].callback = cb;
        dma_callbacks[dma_ch_idx].context  = context;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

HAL_STATUS hal_drv_dma_unregister_callback(uint8_t dma_ch_idx)
{
    if (dma_ch_idx < CHIP_DMA_CHANNEL_NUM) {
        dma_callbacks[dma_ch_idx].callback = NULL;
        dma_callbacks[dma_ch_idx].context  = NULL;
        return HAL_NO_ERR;
    }
    return HAL_ERR;
}

static void hal_drv_dma_trigger_callback(uint8_t dma_ch_idx, uint32_t events)
{
    if (events && dma_callbacks[dma_ch_idx].callback) {
        dma_callbacks[dma_ch_idx].callback(dma_ch_idx, events, dma_callbacks[dma_ch_idx].context);
    }
}

WEAK_ISR void DMA_IRQHandler(void)
{
    uint32_t cmplt_a = hal_drv_dma_buf_a_cmplt_status();
    uint32_t cmplt_b = hal_drv_dma_buf_b_cmplt_status();
    uint32_t m2m     = hal_drv_dma_m2m_cmplt_status();
    uint32_t ovrn_a  = hal_drv_dma_get_err_ovrn_a_status();
    uint32_t ovrn_b  = hal_drv_dma_get_err_ovrn_b_status();
    uint32_t ex_a    = hal_drv_dma_get_err_ex_size_a_status();
    uint32_t ex_b    = hal_drv_dma_get_err_ex_size_b_status();
    // Clear all DMA interrupt status
    hal_drv_dma_buf_a_cmplt_clear(cmplt_a);
    hal_drv_dma_buf_b_cmplt_clear(cmplt_b);
    hal_drv_dma_m2m_cmplt_clear(m2m);
    hal_drv_dma_clear_err_ovrn_a_status(ovrn_a);
    hal_drv_dma_clear_err_ovrn_b_status(ovrn_b);
    hal_drv_dma_clear_err_ex_size_a_status(ex_a);
    hal_drv_dma_clear_err_ex_size_b_status(ex_b);

    for (uint8_t ch = 0; ch < CHIP_DMA_CHANNEL_NUM; ch++) {
        uint32_t events = 0;

        if ((cmplt_a >> ch) & 1) {
            events |= DMA_EVT_CMPLT_A;
        }
        if ((cmplt_b >> ch) & 1) {
            events |= DMA_EVT_CMPLT_B;
        }
        if ((m2m >> ch) & 1) {
            events |= DMA_EVT_M2M_CMPLT;
        }
        if ((ovrn_a >> ch) & 1) {
            events |= DMA_EVT_ERR_OVRN_A;
        }
        if ((ovrn_b >> ch) & 1) {
            events |= DMA_EVT_ERR_OVRN_B;
        }
        if ((ex_a >> ch) & 1) {
            events |= DMA_EVT_ERR_SIZE_A;
        }
        if ((ex_b >> ch) & 1) {
            events |= DMA_EVT_ERR_SIZE_B;
        }

        hal_drv_dma_trigger_callback(ch, events);
    }
}
