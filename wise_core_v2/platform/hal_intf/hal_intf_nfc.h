/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_NFC_H
#define __HAL_INTF_NFC_H

#include "esmt_chip_specific.h"
#include "types.h"
#include "drv/hal_drv_nfc.h"

enum {
    NFC_PWR_SRC_MCU     = 0, 
    NFC_PWR_SRC_READER  = 1,    
};

uint8_t hal_intf_nfc_get_interrupt_idx(void);
void hal_intf_nfc_set_interrupt(uint8_t int_idx, uint8_t enable);
uint8_t hal_intf_nfc_get_mem_work_mode(void);
void hal_intf_nfc_set_mem_work_mode(uint8_t work_mode);
uint8_t hal_intf_nfc_get_mem_lock_mode(void);
void hal_intf_nfc_set_mem_lock_mode(uint8_t lock_mode);
uint8_t hal_intf_nfc_is_mem_locked(void);
void hal_intf_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff);
void hal_intf_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff);
uint8_t hal_intf_nfc_get_dpe_status_info(void);
uint8_t hal_intf_nfc_get_dpe_ctrl_info(void);
void hal_intf_nfc_register_int_callback(uint8_t int_idx, CALLBACK_T cb, void *context);
void hal_intf_nfc_unregister_int_callback(uint8_t int_idx);
void hal_intf_nfc_set_host_locked(uint8_t enable);
void hal_intf_nfc_switch_pwr_src(uint8_t src);
uint8_t hal_intf_nfc_get_pwr_src_idx(void);
void hal_intf_nfc_set_wakeup_config(uint8_t pwr_mode);

#endif
