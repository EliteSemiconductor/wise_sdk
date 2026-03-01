/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_nfc_api.h"

void wise_nfc_init(void)
{
    hal_intf_module_clk_enable(NFC_MODULE);
    hal_intf_module_clk_enable(EFUSE_MODULE);
}

void wise_nfc_deinit(void)
{
    hal_intf_module_clk_disable(NFC_MODULE);
    hal_intf_module_clk_disable(EFUSE_MODULE);
}

void wise_nfc_config(WISE_NFC_CFG_T *cfg)
{
    //set nfc sram work mode: default/full
    hal_intf_nfc_set_mem_work_mode(cfg->work_mode);

    //set sram lock memory mechanism : mode 1 / 2
    hal_intf_nfc_set_mem_lock_mode(cfg->lock_mode);

    //set interrupt idx 0/1
    hal_intf_nfc_set_interrupt(cfg->int_idx, ENABLE);
}

uint8_t wise_nfc_get_mem_work_mode(void)
{
    return hal_intf_nfc_get_mem_work_mode();
}

void wise_nfc_set_mem_work_mode(NFC_WORK_MODE_E mode)
{
    hal_intf_nfc_set_mem_work_mode(mode);
}

uint8_t wise_nfc_get_mem_lock_mode(void)
{
    return hal_intf_nfc_get_mem_lock_mode();
}

void wise_nfc_set_mem_lock_mode(NFC_MEM_LOCK_MODE_E mode)
{
    hal_intf_nfc_set_mem_lock_mode(mode);
}

uint8_t wise_nfc_is_mem_locked(void)
{
    return hal_intf_nfc_is_mem_locked();
}

void wise_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff)
{
    hal_intf_nfc_block_read_data(block_idx, block_len, rx_data_buff);
}

void wise_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff)
{
    hal_intf_nfc_block_write_data(block_idx, block_len, tx_data_buff);
}

uint8_t wise_nfc_get_dpe_status_info(void)
{
    return hal_intf_nfc_get_dpe_status_info();
}

uint8_t wise_nfc_get_dpe_ctrl_info(void)
{
    return hal_intf_nfc_get_dpe_ctrl_info();
}

void wise_nfc_register_int_callback(NFC_INT_IDX_E int_idx, CALLBACK_T cb,            void *context)
{
    hal_intf_nfc_register_int_callback(int_idx, cb, context);
}

void wise_nfc_unregister_int_callback(NFC_INT_IDX_E int_idx)
{
    hal_intf_nfc_unregister_int_callback(int_idx);
}

void wise_nfc_set_host_locked(uint8_t enable)
{
    hal_intf_nfc_set_host_locked(enable);
}

uint8_t wise_nfc_get_interrupt_idx(void)
{
    return hal_intf_nfc_get_interrupt_idx();
}

void wise_nfc_set_interrupt_idx(NFC_INT_IDX_E int_idx)
{
    hal_intf_nfc_set_interrupt(int_idx, ENABLE);
}

void wise_nfc_switch_pwr_src(uint8_t src)
{
    hal_intf_nfc_switch_pwr_src(src);
}

uint8_t wise_nfc_get_pwr_src_idx(void)
{
    return hal_intf_nfc_get_pwr_src_idx();
}

void wise_nfc_set_wakeup_config(uint8_t pwr_mode)
{
    hal_intf_nfc_set_wakeup_config(pwr_mode);
}

