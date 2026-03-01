/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_drv_aes.h"
#include "hdl/aes_er8130.h"
#include <stdint.h>

int32_t hal_drv_aes_mode_and_dir_config(uint32_t aes_direction, uint32_t aes_mode)
{
    aes_set_mode_and_dir_er8130(aes_direction, aes_mode);

    return HAL_NO_ERR;
}

int32_t hal_drv_aes_set_init_vect(const uint8_t *iv_or_cnt)
{
    //uint8_t aes_mode = aes_get_mode_er8130();
    //if (aes_mode != DRV_AES_MODE_ECB || aes_mode != DRV_AES_MODE_CCM) {
    aes_set_init_vect_er8130(iv_or_cnt);
    return HAL_NO_ERR;
    //}
    //return HAL_ERR;
}

int32_t hal_drv_aes_configure(uint32_t aes_direction, uint32_t aes_mode, uint32_t swap_mode, uint8_t key_src, uint32_t key_size,
                              const uint8_t *key_bytes, uint32_t key_num, const uint8_t *iv_or_cnt)
{
    uint8_t in_swap  = 0;
    uint8_t out_swap = 0;

    switch (swap_mode) {
    case 0:
        in_swap  = 0;
        out_swap = 0;
        break;
    case 1:
        in_swap  = 1;
        out_swap = 0;
        break;
    case 2:
        in_swap  = 0;
        out_swap = 1;
        break;
    case 3:
        in_swap  = 1;
        out_swap = 1;
        break;
    }
    aes_set_config_er8130(aes_direction, aes_mode, in_swap, out_swap);
    aes_set_key_er8130(key_src, key_size, key_bytes, key_num);
    //if (aes_mode != DRV_AES_MODE_ECB || aes_mode != DRV_AES_MODE_CCM) {
    aes_set_init_vect_er8130(iv_or_cnt);
    //}

    return HAL_NO_ERR;
}

int hal_drv_aes_set_ccm_info(const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len)
{
    if (!nonce || nonce_len < 7 || nonce_len > 13) {
        return HAL_ERR;
    }

    if (tag_len < 4 || tag_len > 16 || (tag_len % 2) != 0) {
        return HAL_ERR;
    }

    const uint8_t L_field = 14 - nonce_len; // q - 1 = 14 - nonce_len
    const uint8_t M_field = ((tag_len >> 1) - 1);

    aes_set_ccm_ctrl_er8130(L_field, M_field);
    aes_set_auth_len_er8130(auth_len);

    /* Formatting of the Counter Blocks */

    // Ctr0 block（default i = 0）
    uint8_t ctr_block[16] = {0};

    ctr_block[0] = (L_field) & 0x07;         // Flags: bits [2:0] = (q−1), bits [7:3] = 0
    memcpy(&ctr_block[1], nonce, nonce_len); // fill nonce

    // fill counter_i = 0 (big-endian q-byte)
    for (uint8_t j = 0; j < L_field; j++) {
        ctr_block[15 - j] = 0x00;
    }
    /* Formatting of the Counter Blocks */

    // write to IV register
    uint32_t reg_addr = AES_IV_N_ADDR(0);
    for (int i = 0; i < 4; i++) {
        uint32_t val = ((uint32_t)ctr_block[i * 4 + 0] << 24) | ((uint32_t)ctr_block[i * 4 + 1] << 16) | ((uint32_t)ctr_block[i * 4 + 2] << 8) |
                       ((uint32_t)ctr_block[i * 4 + 3]);
        REG_W32(reg_addr, val);
        reg_addr += 4;
    }

    return HAL_NO_ERR;
}

void hal_drv_aes_wait_finished(void)
{
    aes_wait_finished_er8130();
}

uint32_t hal_drv_aes_ccm_decryption_result(void)
{
    return aes_ccm_decryption_result_er8130();
}

void hal_drv_aes_wait_tag_finished(void)
{
    aes_wait_tag_finished_er8130();
}

int32_t hal_drv_aes_start(uint32_t len, uint32_t trig_mode)
{
    return aes_start_er8130(len, trig_mode);
}

int8_t hal_drv_aes_read_tag(uint32_t tags[4])
{
    return aes_read_tag_er8130(tags);
}
