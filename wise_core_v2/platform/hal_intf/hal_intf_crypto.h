/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_CRYPTO_H__
#define __HAL_INTF_CRYPTO_H__

#include "esmt_chip_specific.h"
#include <stdint.h>

#ifdef CHIP_HAS_CRYPTO
#include "types.h"
#include "hal_intf_pmu.h"
#include "drv/hal_drv_dma.h"
#include "drv/hal_drv_aes.h"

#ifdef CHIP_CRYPTO_HAS_AES
int32_t hal_intf_aes_cfg_init_vect(const uint8_t *iv_or_cnt);
int32_t hal_intf_aes_mode_and_dir_config(uint32_t aes_direction, uint32_t aes_mode);
int32_t hal_intf_aes_configure(uint32_t aes_direction, uint32_t aes_mode, uint32_t swap_mode, uint8_t key_src, uint32_t key_size,
                               const uint8_t *key_bytes, uint32_t key_num, const uint8_t *iv_or_cnt);
int32_t hal_intf_aes_start(uint32_t len, uint32_t trig_mode);
uint32_t hal_intf_aes_ccm_decryption_result(void);
void hal_intf_aes_wait_finished(void);
void hal_intf_aes_wait_tag_finished(void);
int32_t hal_intf_aes_read_tag(uint32_t tags[4]);
// -------------------- ECB --------------------
int32_t hal_intf_aes_ecb_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num);
int32_t hal_intf_aes_ecb_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num);
// -------------------- CBC --------------------
int32_t hal_intf_aes_cbc_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *iv);
int32_t hal_intf_aes_cbc_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *iv);
// -------------------- CTR --------------------
int32_t hal_intf_aes_ctr_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *counter);
int32_t hal_intf_aes_ctr_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *counter);
// -------------------- CBC-MAC --------------------
int32_t hal_intf_aes_cbcmac_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                      const uint8_t *iv);
// -------------------- CCM --------------------
int32_t hal_intf_aes_ccm_init(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num, bool encrypt);

int32_t hal_intf_aes_ccm_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len);

int32_t hal_intf_aes_ccm_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len);
#endif //CHIP_CRYPTO_HAS_AES

#ifdef CHIP_CRYPTO_HAS_SHA

#define SHA_DMA_CHANNEL 3 //kevinyang, temporarily ugly hard code here

#define SHA_BIT_224 (1U << 0)
#define SHA_BIT_256 (1U << 1)
#define SHA_NO_SWAP (1U << 2)
#define SHA_IN_SWAP (1U << 3)

int32_t hal_intf_sha_generate(uint8_t mode, uint32_t dataAddr, uint32_t dataSize, uint8_t *outputSha);
#endif //CHIP_CRYPTO_HAS_SHA

#endif //CHIP_HAS_CRYPTO

#endif //__HAL_INTF_CRYPTO_H__
