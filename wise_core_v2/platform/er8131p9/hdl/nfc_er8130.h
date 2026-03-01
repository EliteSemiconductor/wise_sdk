/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __NFC_ER8130_H__
#define __NFC_ER8130_H__

#include "esmt_chip_specific.h"
#include "types.h"

uint8_t nfc_get_mem_work_mode_er8130(void);
void nfc_set_mem_work_mode_er8130(uint8_t work_mode);
uint8_t nfc_get_mem_lock_mode_er8130(void);
void nfc_set_mem_lock_mode_er8130(uint8_t lock_mode);
uint8_t nfc_is_mem_locked_er8130(void);
void nfc_block_read_data_er8130(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff);
void nfc_block_write_data_er8130(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff);
uint8_t nfc_get_dpe_status_info_er8130(void);
uint8_t nfc_get_dpe_ctrl_info_er8130(void);
uint8_t nfc_get_interrupt_idx_er8130(void);
void nfc_set_interrupt_er8130(uint8_t int_idx, uint8_t enable);
uint8_t nfc_get_dpe_status_int_mask_er8130(void);
void nfc_clear_int_mask_er8130(void);
void nfc_set_host_locked_er8130(uint8_t enable);

#define NFC_GET_MEM_WORK_MODE()                                 nfc_get_mem_work_mode_er8130()
#define NFC_SET_MEM_WORK_MODE(work_mode)                        nfc_set_mem_work_mode_er8130(work_mode)
#define NFC_GET_MEM_LOCK_MODE()                                 nfc_get_mem_lock_mode_er8130()
#define NFC_SET_MEM_LOCK_MODE(lock_mode)                        nfc_set_mem_lock_mode_er8130(lock_mode)
#define NFC_IS_MEM_LOCKED()                                     nfc_is_mem_locked_er8130()
#define NFC_BLOCK_READ_DATA(block_idx, block_len, rx_data_buff) nfc_block_read_data_er8130(block_idx, block_len, rx_data_buff)
#define NFC_BLOCK_WRITE_DATA(block_idx, block_len, tx_data_buff) nfc_block_write_data_er8130(block_idx, block_len, tx_data_buff)
#define NFC_GET_DPE_STATUS_INFO()                               nfc_get_dpe_status_info_er8130();
#define NFC_GET_DPE_CTRL_INFO()                                 nfc_get_dpe_ctrl_info_er8130();
#define NFC_GET_INTERRUPT_IDX()                                 nfc_get_interrupt_idx_er8130();
#define NFC_SET_INTERRUPT(int_idx, enable)                      nfc_set_interrupt_er8130(int_idx, enable)
#define NFC_GET_DPE_STATUS_INT_MASK()                           nfc_get_dpe_status_int_mask_er8130();
#define NFC_CLEAR_INT_MASK()                                    nfc_clear_int_mask_er8130();
#define NFC_SET_HOST_LOCKED(enable)                             nfc_set_host_locked_er8130(enable);

#endif /* __GPIO_ER8130_H */
