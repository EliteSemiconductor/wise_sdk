/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_nfc.h"
#include "hdl/nfc_er8130.h"
#include "hal_intf_nfc.h"
#include "drv/hal_drv_pmu.h"
#include "ana_er8130.h"


static CALLBACK_ENTRY_T nfc_callbacks[NFC_INT_NUM];
static uint8_t nfc_pwr_src = 0;
    
uint8_t hal_drv_nfc_get_interrupt_idx(void)
{
    return NFC_GET_INTERRUPT_IDX();
}

void hal_drv_nfc_set_interrupt(uint8_t int_idx, uint8_t enable)
{
    NFC_SET_INTERRUPT(int_idx, enable);

    NVIC_EnableIRQ((IRQn_Type)NFC_IRQn);
}

uint8_t hal_drv_nfc_get_mem_work_mode(void)
{
    return NFC_GET_MEM_WORK_MODE();
}

void hal_drv_nfc_set_mem_work_mode(uint8_t work_mode)
{
    NFC_SET_MEM_WORK_MODE(work_mode);
}

uint8_t hal_drv_nfc_get_mem_lock_mode(void)
{
    return NFC_GET_MEM_LOCK_MODE();
}

void hal_drv_nfc_set_mem_lock_mode(uint8_t lock_mode)
{
    NFC_SET_MEM_LOCK_MODE(lock_mode);
}

uint8_t hal_drv_nfc_is_mem_locked(void)
{
    return NFC_IS_MEM_LOCKED();
}

void hal_drv_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff)
{
    NFC_BLOCK_READ_DATA(block_idx, block_len, rx_data_buff);
}

void hal_drv_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff)
{
    NFC_BLOCK_WRITE_DATA(block_idx, block_len, tx_data_buff);
}

uint8_t hal_drv_nfc_get_dpe_status_info(void)
{
    return NFC_GET_DPE_STATUS_INFO();
}

uint8_t hal_drv_nfc_get_dpe_ctrl_info(void)
{
    return NFC_GET_DPE_CTRL_INFO();
}


void hal_drv_nfc_register_int_callback(uint8_t int_idx, CALLBACK_T cb,              void *context)
{
    if (int_idx >= NFC_INT_NUM) {
        printf("NFC IRQ register: index %u exceeds limit (%u)\n", int_idx, NFC_INT_NUM);
        return;
    }

    nfc_callbacks[int_idx].callback = cb;
    nfc_callbacks[int_idx].context  = context;
}

void hal_drv_nfc_unregister_int_callback(uint8_t int_idx)
{
    if (int_idx >= NFC_INT_NUM) {
        printf("NFC IRQ unregister: index %u exceeds limit (%u)\n", int_idx, NFC_INT_NUM);
        return;
    }

    nfc_callbacks[int_idx].callback = NULL;
    nfc_callbacks[int_idx].context  = NULL;
}

void hal_drv_nfc_set_host_locked(uint8_t enable)
{
    NFC_SET_HOST_LOCKED(enable);
}

void hal_drv_nfc_switch_pwr_src(uint8_t src)
{
    if (src == NFC_PWR_SRC_MCU) {
        hal_drv_ana_switch_nfc_pwr_src(src);
        hal_drv_pmu_switch_nfc_pwr_src(src);
    }

    if (src == NFC_PWR_SRC_READER) {
        hal_drv_ana_switch_nfc_pwr_src(src);
        hal_drv_pmu_switch_nfc_pwr_src(src);
    }

    nfc_pwr_src = src;
}

uint8_t hal_drv_nfc_get_pwr_src_idx(void)
{
    return nfc_pwr_src;
}

WEAK_ISR void NFC_IRQHandler(void)
{
    uint8_t i;
    uint8_t int_mask = NFC_GET_DPE_STATUS_INT_MASK();
    
    NFC_CLEAR_INT_MASK();

    for (i = 0; i < NFC_INT_NUM; i++) {
        if (int_mask & (1UL << i) && nfc_callbacks[i].callback) {
            nfc_callbacks[i].callback(nfc_callbacks[i].context, i);
        }
    }
}
