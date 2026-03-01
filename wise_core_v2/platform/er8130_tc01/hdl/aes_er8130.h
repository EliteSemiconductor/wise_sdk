/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __AES_ER813X_H
#define __AES_ER813X_H

#include "esmt_chip_specific.h"
#include "types.h"

typedef enum {
    DRV_AES_KEY_SRC_FROM_EFUSE = 0,
    DRV_AES_KEY_SRC_FROM_USER  = 1,
} DRV_AES_KEY_SOURCE_T;

typedef enum {
    DRV_AES_ENCRYPT,
    DRV_AES_DECRYPT,
} DRV_AES_DIR_T;

typedef enum {
    DRV_AES_MODE_ECB     = 0,
    DRV_AES_MODE_CBC     = 1,
    DRV_AES_MODE_CTR     = 2,
    DRV_AES_MODE_CBC_MAC = 3,
    DRV_AES_MODE_CCM     = 4
} DRV_AES_MODE_T;

#define AES_KEY_N_ADDR(n) ((uint32_t)&(AES->KEY[0]) + ((n) * 4U))
#define AES_IV_N_ADDR(n) ((uint32_t)&(AES->IV[0]) + ((n) * 4U))
#define AES_TAG_N_ADDR(n) ((uint32_t)&(AES->TAG[0]) + ((n) * 4U))

uint32_t aes_ccm_decryption_result_er8130(void);
void aes_wait_finished_er8130(void);
void aes_wait_tag_finished_er8130(void);
void aes_set_mode_and_dir_er8130(uint8_t cpt_dir, uint8_t mode);
void aes_set_config_er8130(uint8_t cpt_dir, uint8_t mode, uint8_t in_swap, uint8_t out_swap);
int8_t aes_set_key_from_csr(const uint8_t *key_bytes, uint32_t key_byte_count);
int8_t aes_set_key_from_store(uint8_t key_size, uint8_t key_num);
int8_t aes_set_key_er8130(uint8_t key_source, uint8_t key_size, const uint8_t *key_bytes, uint32_t key_num);
int8_t aes_start_er8130(uint32_t len, uint32_t trig_mode);
uint8_t aes_get_mode_er8130(void);
void aes_set_init_vect_er8130(const uint8_t iv[16]);
int8_t aes_read_tag_er8130(uint32_t tags[4]);
void aes_set_ccm_ctrl_er8130(uint8_t ccm_L, uint8_t ccm_M);
void aes_set_auth_len_er8130(uint32_t auth_len);
#endif //__AES_ER813X_H
