/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_AES_H
#define __HAL_DRV_AES_H

#include <stdint.h>
#include "hal_intf_crypto.h"
#include "hdl/aes_er8130.h"
int32_t hal_drv_aes_set_init_vect(const uint8_t *iv_or_cnt);
int32_t hal_drv_aes_mode_and_dir_config(uint32_t aes_direction, uint32_t aes_mode);
int32_t hal_drv_aes_configure(uint32_t aes_direction, uint32_t aes_mode, uint32_t swap_mode, uint8_t key_src, uint32_t key_size,
                              const uint8_t *key_bytes, uint32_t key_num, const uint8_t *iv_or_cnt);
int32_t hal_drv_aes_start(uint32_t len, uint32_t trig_mode);

uint32_t hal_drv_aes_ccm_decryption_result(void);
void hal_drv_aes_wait_finished(void);
void hal_drv_aes_wait_tag_finished(void);
int8_t hal_drv_aes_read_tag(uint32_t tags[4]);
int hal_drv_aes_set_ccm_info(const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len);
#endif //__HAL_DRV_AES_H
