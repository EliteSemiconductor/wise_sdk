/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/nfc_er8130.h"

uint8_t nfc_get_mem_work_mode_er8130(void)
{
    uint8_t rst;
    uint32_t cfg;

    cfg = REG_R32(NFC_NFC_SRAM_FULL_EN_ADDR);
    rst = (uint8_t)((cfg & NFC_NFC_SRAM_FULL_EN_MASK) >> NFC_NFC_SRAM_FULL_EN_POS);

    return rst;
}


void nfc_set_mem_work_mode_er8130(uint8_t work_mode)
{
    uint32_t cfg;

    //0x510[0]
    //default mode = 0, full mode = 1
    cfg = REG_R32(NFC_NFC_SRAM_FULL_EN_ADDR);
    cfg = (cfg & ~NFC_NFC_SRAM_FULL_EN_MASK) |
          ((work_mode << NFC_NFC_SRAM_FULL_EN_POS) & NFC_NFC_SRAM_FULL_EN_MASK);
    REG_W32(NFC_NFC_SRAM_FULL_EN_ADDR, cfg);
}

uint8_t nfc_get_mem_lock_mode_er8130(void)
{
    uint8_t rst;
    uint32_t cfg;

    cfg = REG_R32(NFC_NFC_MEM_LCK_MOD_ADDR);
    rst = (uint8_t)((cfg & NFC_NFC_MEM_LCK_MOD_MASK) >> NFC_NFC_MEM_LCK_MOD_POS);

    return rst;

}

void nfc_set_mem_lock_mode_er8130(uint8_t lock_mode)
{
    uint32_t cfg;

    //0x510[2]
    //0 = mode 1 / 1 = mode 2
    cfg = REG_R32(NFC_NFC_MEM_LCK_MOD_ADDR);
    cfg = (cfg & ~NFC_NFC_MEM_LCK_MOD_MASK) |
          ((lock_mode << NFC_NFC_MEM_LCK_MOD_POS) & NFC_NFC_MEM_LCK_MOD_MASK);
    REG_W32(NFC_NFC_MEM_LCK_MOD_ADDR, cfg);
}

uint8_t nfc_is_mem_locked_er8130(void)
{
    uint8_t is_mem_lock;

    is_mem_lock = (REG_R32(NFC_NFC_IS_LOCKED_ADDR) & NFC_NFC_IS_LOCKED_MASK);

    return is_mem_lock;
}

void nfc_block_read_data_er8130(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff)
{
    uint32_t data_content[MAX_NFC_BLOCK_IDX] = {0};
    uint32_t i;
    uint32_t addr = NFC_BASE + ((uint32_t)block_idx<<2);

    for (i = 0; i < block_len; i++) {
        data_content[i] = REG_R32(addr + (i * 4));
    }

    memcpy((void *)rx_data_buff, (void *)data_content, (block_len*4));
}

void nfc_block_write_data_er8130(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff)
{
    uint32_t addr = NFC_BASE + ((uint32_t)block_idx<<2);
    uint32_t i;

    for (i = 0; i < block_len; i++) {
        REG_W32(addr+((uint32_t)i<<2), tx_data_buff[i]);
        //printf("block[%ld] (0x%08lx) : ", (block_idx+i), addr+((uint32_t)i<<2));
        //printf("val = 0x%08lx\n", tx_data_buff[i]);
    }
}

uint8_t nfc_get_dpe_status_info_er8130(void)
{
    uint32_t cfg;
    uint8_t rst;

    //0x3FC
    cfg = REG_R32(NFC_DPE_ADDR);

    rst = (cfg & 0xFF0000) >> 16;

    return rst;
}

uint8_t nfc_get_dpe_ctrl_info_er8130(void)
{
    uint32_t cfg;
    uint8_t rst;

    //0x3FF
    cfg = REG_R32(NFC_DPE_ADDR);

    rst = (cfg & 0xFF000000) >> 24;

    return rst;
}

#define NFC_INT_ID_0 0x4
#define NFC_INT_ID_1 0x5
uint8_t nfc_get_interrupt_idx_er8130(void)
{
    uint8_t int_idx = 0x0;
    uint32_t cfg;
    
    //0x3FC[26:24]
    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = ((cfg & 0x07000000) >> NFC_DPE_IRQ_SRC_POS);

    //printf("cfg = 0x%08x\n", cfg);

    if (cfg == NFC_INT_ID_0)
        int_idx = 0;
    if (cfg == NFC_INT_ID_1)
        int_idx = 1;

    return int_idx;
}

void nfc_set_interrupt_er8130(uint8_t int_idx, uint8_t enable)
{
    uint32_t cfg;
   
    uint8_t int_id;
    uint8_t int_en = 0x1;

    //int_idx = 1; //POR int
    if (int_idx == 0)
        int_id = NFC_INT_ID_0;

    if (int_idx == 1)
        int_id = NFC_INT_ID_1;

    //0x3FC[26:24] clean old int idx
    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = (cfg & ~(0x07000000));
    REG_W32(NFC_DPE_ADDR, cfg);

    //0x3FC[26:24]
    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = (cfg & ~(0xFF000000)) |
          ((int_id << NFC_DPE_IRQ_SRC_POS) & 0x07000000);
    REG_W32(NFC_DPE_ADDR, cfg);
    
    //0x504[0]
    cfg = REG_R32(NFC_INT_EN_ADDR);
    cfg = (cfg & ~(NFC_INT_EN_NFC_EVT_MASK)) |
          ((int_en << NFC_INT_EN_NFC_EVT_POS) & NFC_INT_EN_NFC_EVT_MASK);
    REG_W32(NFC_INT_EN_ADDR, cfg);

    //printf("enable interrupt idx = %d\n", int_id);
}


uint8_t nfc_get_dpe_status_int_mask_er8130(void)
{
    uint8_t dpe_status;

    dpe_status = nfc_get_dpe_status_info_er8130();

    dpe_status = ((dpe_status>>4)&0x00000003);

    return dpe_status;
}

void nfc_clear_int_mask_er8130(void)
{
    uint32_t cfg;

    //0x3FC[22:21] clear INT by user
    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = (cfg & ~(NFC_DPE_STATUS_4_MASK+NFC_DPE_STATUS_5_MASK)) |
          ((0x0 << NFC_DPE_STATUS_4_POS) & NFC_DPE_STATUS_4_MASK)|
          ((0x0 << NFC_DPE_STATUS_5_POS) & NFC_DPE_STATUS_5_MASK);
    REG_W32(NFC_DPE_ADDR, cfg);

    //0x3FC[27] (set 1 then clear to 0)
    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = (cfg & ~NFC_DPE_IRQ_CLR_MASK) |
          ((0x1 << NFC_DPE_IRQ_CLR_POS) & NFC_DPE_IRQ_CLR_MASK);
    REG_W32(NFC_DPE_ADDR, cfg);

    cfg = REG_R32(NFC_DPE_ADDR);
    cfg = (cfg & ~NFC_DPE_IRQ_CLR_MASK) |
          ((0x0 << NFC_DPE_IRQ_CLR_POS) & NFC_DPE_IRQ_CLR_MASK);
    REG_W32(NFC_DPE_ADDR, cfg);

    // 0x500[0] clear NFC evt (write 1 clear)
    cfg = REG_R32(NFC_INT_ST_NFC_EVT_ADDR);
    cfg = (cfg & ~NFC_INT_ST_NFC_EVT_MASK) |
          ((0x1 << NFC_INT_ST_NFC_EVT_POS) & NFC_INT_ST_NFC_EVT_MASK);
    REG_W32(NFC_INT_ST_NFC_EVT_ADDR, cfg);

}

void nfc_set_host_locked_er8130(uint8_t enable)
{
    uint32_t cfg;

    if (enable) {
        // 0x514[4] host lock
        cfg = REG_R32(NFC_SET_MEM_LOCK_ADDR);
        cfg = (cfg & ~NFC_SET_MEM_LOCK_MASK) |
              ((0x1 << NFC_SET_MEM_LOCK_POS) & NFC_SET_MEM_LOCK_MASK);
        REG_W32(NFC_SET_MEM_LOCK_ADDR, cfg);
    
    } else {
        // 0x514[0] host lock clear (write 1 clear)
        cfg = REG_R32(NFC_CLR_NFC_LOCK_ADDR);
        cfg = (cfg & ~NFC_CLR_NFC_LOCK_MASK) |
              ((0x1 << NFC_CLR_NFC_LOCK_POS) & NFC_CLR_NFC_LOCK_MASK);
        REG_W32(NFC_CLR_NFC_LOCK_ADDR, cfg);

    }

}

