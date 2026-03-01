/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_crypto.h"
#include "hal_intf_dma.h"
#include "types.h"
#include <stdint.h>

#ifdef CHIP_HAS_CRYPTO
#include "drv/hal_drv_sha.h"

#ifdef CHIP_CRYPTO_HAS_AES
int32_t hal_intf_aes_cfg_init_vect(const uint8_t *iv_or_cnt)
{
    return hal_drv_aes_set_init_vect(iv_or_cnt);
}

int32_t hal_intf_aes_configure(uint32_t aes_direction, uint32_t aes_mode, uint32_t swap_mode, uint8_t key_src, uint32_t key_size,
                               const uint8_t *key_bytes, uint32_t key_num, const uint8_t *iv_or_cnt)
{
    uint32_t enable_module_clock = 0;

    if (!hal_intf_pmu_module_clk_is_enabled(DMA_MODULE)) {
        enable_module_clock |= DMA_MODULE;
    }
    if (!hal_intf_pmu_module_clk_is_enabled(AES_MODULE)) {
        enable_module_clock |= AES_MODULE;
    }

    hal_intf_pmu_module_clk_enable(enable_module_clock);

    return hal_drv_aes_configure(aes_direction, aes_mode, swap_mode, key_src, key_size, key_bytes, key_num, iv_or_cnt);
}

int32_t hal_intf_aes_mode_and_dir_config(uint32_t aes_direction, uint32_t aes_mode)
{
    return hal_drv_aes_mode_and_dir_config(aes_direction, aes_mode);
}
int32_t hal_intf_aes_start(uint32_t len, uint32_t trig_mode)
{
    return hal_drv_aes_start(len, trig_mode);
}

uint32_t hal_intf_aes_ccm_decryption_result(void)
{
    return hal_drv_aes_ccm_decryption_result();
}

void hal_intf_aes_wait_finished(void)
{
    hal_drv_aes_wait_finished();
}

void hal_intf_aes_wait_tag_finished(void)
{
    hal_drv_aes_wait_tag_finished();
}

int32_t hal_intf_aes_read_tag(uint32_t tags[4])
{
    return hal_drv_aes_read_tag(tags);
}
// -------------------- ECB --------------------
int32_t hal_intf_aes_ecb_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num)
{
    return hal_intf_aes_configure(DRV_AES_ENCRYPT, DRV_AES_MODE_ECB, swap_mode, key_src, key_size, key_bytes, key_num, NULL);
}

int32_t hal_intf_aes_ecb_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num)
{
    return hal_intf_aes_configure(DRV_AES_DECRYPT, DRV_AES_MODE_ECB, swap_mode, key_src, key_size, key_bytes, key_num, NULL);
}

// -------------------- CBC --------------------
int32_t hal_intf_aes_cbc_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *iv)
{
    return hal_intf_aes_configure(DRV_AES_ENCRYPT, DRV_AES_MODE_CBC, swap_mode, key_src, key_size, key_bytes, key_num, iv);
}

int32_t hal_intf_aes_cbc_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *iv)
{
    return hal_intf_aes_configure(DRV_AES_DECRYPT, DRV_AES_MODE_CBC, swap_mode, key_src, key_size, key_bytes, key_num, iv);
}

// -------------------- CTR --------------------
int32_t hal_intf_aes_ctr_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *counter)
{
    return hal_intf_aes_configure(DRV_AES_ENCRYPT, DRV_AES_MODE_CTR, swap_mode, key_src, key_size, key_bytes, key_num, counter);
}

int32_t hal_intf_aes_ctr_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *counter)
{
    return hal_intf_aes_configure(DRV_AES_DECRYPT, DRV_AES_MODE_CTR, swap_mode, key_src, key_size, key_bytes, key_num, counter);
}

// -------------------- CBC-MAC --------------------
int32_t hal_intf_aes_cbcmac_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                      const uint8_t *iv)
{
    return hal_intf_aes_configure(DRV_AES_ENCRYPT, DRV_AES_MODE_CBC_MAC, swap_mode, key_src, key_size, key_bytes, key_num, iv);
}
// -------------------- CCM --------------------
int32_t hal_intf_aes_ccm_init(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num, bool encrypt)
{
    return hal_intf_aes_configure(encrypt ? DRV_AES_ENCRYPT : DRV_AES_DECRYPT, DRV_AES_MODE_CCM, swap_mode, key_src, key_size, key_bytes, key_num,
                                  NULL);
}

int32_t hal_intf_aes_ccm_set_info(const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len)
{
    return hal_drv_aes_set_ccm_info(nonce, nonce_len, tag_len, auth_len);
}

int32_t hal_intf_aes_ccm_encrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len)
{
    int ret = hal_intf_aes_configure(DRV_AES_ENCRYPT, DRV_AES_MODE_CCM, swap_mode, key_src, key_size, key_bytes, key_num, NULL);
    if (ret != HAL_NO_ERR) {
        return ret;
    }

    hal_drv_aes_set_ccm_info(nonce, nonce_len, tag_len, auth_len);
    return HAL_NO_ERR;
}

int32_t hal_intf_aes_ccm_decrypt_configure(uint32_t swap_mode, uint8_t key_src, uint32_t key_size, const uint8_t *key_bytes, uint32_t key_num,
                                           const uint8_t *nonce, uint8_t nonce_len, uint8_t tag_len, uint32_t auth_len)
{
    int ret = hal_intf_aes_configure(DRV_AES_DECRYPT, DRV_AES_MODE_CCM, swap_mode, key_src, key_size, key_bytes, key_num, NULL);
    if (ret != HAL_NO_ERR) {
        return ret;
    }

    hal_drv_aes_set_ccm_info(nonce, nonce_len, tag_len, auth_len);
    return HAL_NO_ERR;
}
#endif
#ifdef CHIP_CRYPTO_HAS_SHA
int32_t hal_intf_sha_generate(uint8_t mode, uint32_t dataAddr, uint32_t dataSize, uint8_t *outputSha)
{
    uint32_t enableClock = 0;
    uint8_t is256        = (mode & SHA_BIT_256) ? 1 : 0;
    uint8_t isSwap       = (mode & SHA_IN_SWAP) ? 1 : 0;

    if (!hal_intf_pmu_module_clk_is_enabled(DMA_MODULE)) {
        enableClock |= DMA_MODULE;
    }
    if (!hal_intf_pmu_module_clk_is_enabled(SHA_MODULE)) {
        enableClock |= SHA_MODULE;
    }

    hal_intf_pmu_module_clk_enable(enableClock);

    HAL_DMA_DATA_CFG_T dma_cfg = {.input = (const void *)dataAddr, .output = NULL, .input_length = dataSize, .output_length = 0};
    if (HAL_NO_ERR != hal_intf_sha_dma_update(&dma_cfg)) {
        return HAL_ERR;
    }

    hal_drv_sha_set_config(is256, isSwap);
    hal_drv_sha_get_digest((uint32_t *)outputSha, dataSize);

    //hal_intf_pmu_module_clk_disable(enableClock);
    return HAL_NO_ERR;
}
#endif //CHIP_CRYPTO_HAS_SHA

#endif //CHIP_HAS_CRYPTO
