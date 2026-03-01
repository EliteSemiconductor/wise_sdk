/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_nfc.h"

#ifdef CHIP_HAS_NFC
uint8_t hal_intf_nfc_get_interrupt_idx(void)
{
    return hal_drv_nfc_get_interrupt_idx();
}

void hal_intf_nfc_set_interrupt(uint8_t int_idx, uint8_t enable)
{
    hal_drv_nfc_set_interrupt(int_idx, enable);
}

uint8_t hal_intf_nfc_get_mem_work_mode(void)
{
    return hal_drv_nfc_get_mem_work_mode();
}

void hal_intf_nfc_set_mem_work_mode(uint8_t work_mode)
{
    hal_drv_nfc_set_mem_work_mode(work_mode);
}

uint8_t hal_intf_nfc_get_mem_lock_mode(void)
{
    return hal_drv_nfc_get_mem_lock_mode();
}

void hal_intf_nfc_set_mem_lock_mode(uint8_t lock_mode)
{
    hal_drv_nfc_set_mem_lock_mode(lock_mode);
}

uint8_t hal_intf_nfc_is_mem_locked(void)
{
    return hal_drv_nfc_is_mem_locked();
}

void hal_intf_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff)
{
    hal_drv_nfc_block_read_data(block_idx, block_len, rx_data_buff);
}

void hal_intf_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff)
{
    hal_drv_nfc_block_write_data(block_idx, block_len, tx_data_buff);
}

uint8_t hal_intf_nfc_get_dpe_status_info(void)
{
    return hal_drv_nfc_get_dpe_status_info();
}

uint8_t hal_intf_nfc_get_dpe_ctrl_info(void)
{
    return hal_drv_nfc_get_dpe_ctrl_info();
}

void hal_intf_nfc_register_int_callback(uint8_t int_idx, CALLBACK_T cb,              void *context)
{
    hal_drv_nfc_register_int_callback(int_idx, cb, context);
}

void hal_intf_nfc_unregister_int_callback(uint8_t int_idx)
{
    hal_drv_nfc_unregister_int_callback(int_idx);
}

void hal_intf_nfc_set_host_locked(uint8_t enable)
{
    hal_drv_nfc_set_host_locked(enable);
}

void hal_intf_nfc_switch_pwr_src(uint8_t src)
{
    hal_drv_nfc_switch_pwr_src(src);
}

uint8_t hal_intf_nfc_get_pwr_src_idx(void)
{
    return hal_drv_nfc_get_pwr_src_idx();
}

void hal_intf_nfc_set_wakeup_config(uint8_t pwr_mode)
{
#ifdef CHIP_NFC_SUPPORT_WAKEUP
    hal_drv_nfc_set_wakeup_config(pwr_mode);
#endif
}

#else
uint8_t hal_intf_nfc_get_interrupt_idx(void){return 0;}
void hal_intf_nfc_set_interrupt(uint8_t int_idx, uint8_t enable){}
uint8_t hal_intf_nfc_get_mem_work_mode(void){return 0;}
void hal_intf_nfc_set_mem_work_mode(uint8_t work_mode){}
uint8_t hal_intf_nfc_get_mem_lock_mode(void){return 0;}
void hal_intf_nfc_set_mem_lock_mode(uint8_t lock_mode){}
uint8_t hal_intf_nfc_is_mem_locked(void){return 0;}
void hal_intf_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff){}
void hal_intf_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff){}
uint8_t hal_intf_nfc_get_dpe_status_info(void){return 0;}
uint8_t hal_intf_nfc_get_dpe_ctrl_info(void){return 0;}
void hal_intf_nfc_register_int_callback(uint8_t int_idx, CALLBACK_T cb,              void *context){}
void hal_intf_nfc_unregister_int_callback(uint8_t int_idx){}
void hal_intf_nfc_set_host_locked(uint8_t enable){}
void hal_intf_nfc_switch_pwr_src(uint8_t src){}
uint8_t hal_intf_nfc_get_pwr_src_idx(void){return 0;}
void hal_intf_nfc_set_wakeup_config(uint8_t pwr_mode);

#endif
