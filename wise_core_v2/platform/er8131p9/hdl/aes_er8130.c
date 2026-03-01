/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/aes_er8130.h"
#include "cmsis/include/regfiles/aes_reg_er8130.h"
#include "types.h"
#include <stdint.h>

static inline uint8_t _aes_chk_writen_key_status(uint8_t area_mask)
{
    return (REG_R32(AES_KEY_WRITTEN_AREA_ADDR) & area_mask);
}

static inline void _aes_wait_write_key_finished(void)
{
    while ((REG_R32(AES_STATUS_ADDR) & AES_KEY_WRITE_BUSY_MASK))
        ;
}

static inline uint8_t _aes_chk_write_key_status(void)
{
    return (REG_R32(AES_INT_ST_ADDR) & AES_INT_ST_KEY_WR_ERR_MASK);
}

static inline uint8_t _aes_chk_read_key_status(void)
{
    return (REG_R32(AES_INT_ST_ADDR) & AES_INT_ST_KEY_RD_ERR_MASK);
}

static inline void _aes_wait_load_key_complete(void)
{
    while (!(REG_R32(AES_STATUS_ADDR) & AES_KEY_READY_MASK))
        ;
}

void aes_wait_finished_er8130(void)
{
    while (!(REG_R32(AES_INT_ST_ADDR) & AES_INT_ST_FSH_MASK))
        ;
    REG_W32(AES_INT_ST_ADDR, AES_INT_ST_FSH_MASK);
}

uint32_t aes_ccm_decryption_result_er8130(void)
{
    return (REG_R32(AES_INT_ST_ADDR) & AES_INT_ST_CCM_INVALID_MASK);
}

void aes_wait_tag_finished_er8130(void)
{
    while (!(REG_R32(AES_INT_ST_ADDR) & AES_INT_ST_TAG_MASK))
        ;
    REG_W32(AES_INT_ST_ADDR, AES_INT_ST_TAG_MASK);
}

void aes_set_config_er8130(uint8_t cpt_dir, uint8_t mode, uint8_t in_swap, uint8_t out_swap)
{
    uint32_t reg = REG_R32(AES_CTL_ADDR) & ~(AES_DIR_MASK + AES_MODE_MASK + AES_IN_SWAP_MASK);

    reg |= (((cpt_dir << AES_DIR_POS) & AES_DIR_MASK) | ((mode << AES_MODE_POS) & AES_MODE_MASK) | ((in_swap << AES_IN_SWAP_POS) & AES_IN_SWAP_MASK) |
            ((out_swap << AES_OUT_SWAP_POS) & AES_OUT_SWAP_MASK));
    REG_W32(AES_CTL_ADDR, reg);
}

void aes_set_mode_and_dir_er8130(uint8_t cpt_dir, uint8_t mode)
{
    uint32_t reg = REG_R32(AES_CTL_ADDR) & ~(AES_DIR_MASK | AES_MODE_MASK);

    reg |= (((cpt_dir << AES_DIR_POS) & AES_DIR_MASK) | ((mode << AES_MODE_POS) & AES_MODE_MASK));
    REG_W32(AES_CTL_ADDR, reg);
}

int8_t aes_set_key_from_csr(const uint8_t *key_bytes, uint32_t key_byte_count)
{
    for (uint32_t i = 0; i < CHIP_CRYPTO_AES_MAX_KEY_WORDS; i++) {
        uint32_t val = 0U;

        if ((i * 4) < key_byte_count) {
            // Convert big-endian data to little-endian
            val  = (i * 4 + 0 < key_byte_count) ? key_bytes[i * 4 + 3] << 0 : 0U;
            val |= (i * 4 + 1 < key_byte_count) ? key_bytes[i * 4 + 2] << 8 : 0U;
            val |= (i * 4 + 2 < key_byte_count) ? key_bytes[i * 4 + 1] << 16 : 0U;
            val |= (i * 4 + 3 < key_byte_count) ? key_bytes[i * 4 + 0] << 24 : 0U;
        }

        REG_W32(AES_KEY_N_ADDR(i), val);
    }

    return HAL_NO_ERR;
}

int8_t aes_set_key_from_store(uint8_t key_size, uint8_t key_num)
{
    uint32_t reg     = REG_R32(AES_CTL_ADDR) & ~(AES_KEY_SOURCE_MASK + AES_KEY_SIZE_MASK);
    uint8_t key_mask = 1U << key_num;

    // For key_size 192 or 256 bits, restrict use of store areas 1, 3, 5 only
    if ((key_size != 0U) && ((key_num == 1) || (key_num == 3) || (key_num == 5))) {
        return HAL_ERR;
    }
    reg |= (((key_size << AES_KEY_SIZE_POS) & AES_KEY_SIZE_MASK) | ((DRV_AES_KEY_SRC_FROM_EFUSE << AES_KEY_SOURCE_POS) & AES_KEY_SOURCE_MASK));
    REG_W32(AES_CTL_ADDR, reg);

    if (_aes_chk_writen_key_status(key_mask)) { // check whether the key is loaded
        return HAL_ERR;
    }

    REG_W32(AES_KEY_STORE_SIZE_ADDR, key_size);
    REG_W32(AES_KEY_WRITE_AREA_ADDR, key_num);
    REG_W32(AES_DMA_ADDR, AES_W1T_AES_KEY_WRITE_MASK);

    _aes_wait_write_key_finished();

    if (_aes_chk_write_key_status() ||               // check no writing error
        _aes_chk_writen_key_status(key_mask) == 0) { // check the key has been written
        return HAL_ERR;
    }

    return HAL_NO_ERR;
}

int8_t aes_set_key_er8130(uint8_t key_source, uint8_t key_size, const uint8_t *key_bytes, uint32_t key_num)
{
    uint32_t reg  = REG_R32(AES_CTL_ADDR);
    reg          &= ~(AES_KEY_SOURCE_MASK | AES_KEY_SIZE_MASK);
    reg          |= ((key_source << AES_KEY_SOURCE_POS) & AES_KEY_SOURCE_MASK) | ((key_size << AES_KEY_SIZE_POS) & AES_KEY_SIZE_MASK);
    REG_W32(AES_CTL_ADDR, reg);

    if (key_source == DRV_AES_KEY_SRC_FROM_USER) {
        /**
         * KEY_SIZE = 0 => 128bits  = 16 bytes
         * KEY_SIZE = 1 => 192bits  = 24 bytes
         * KEY_SIZE = 2 => 256bits  = 32 bytes
         * */
        return aes_set_key_from_csr(key_bytes, 16 + (key_size << 3));
    } else {
        return aes_set_key_from_store(key_size, key_num);
    }
}

int8_t aes_start_er8130(uint32_t len, uint32_t trig_mode)
{
    uint8_t key_src  = (REG_R32(AES_CTL_ADDR) & AES_KEY_SOURCE_MASK) >> AES_KEY_SOURCE_POS;
    uint8_t key_size = (REG_R32(AES_CTL_ADDR) & AES_KEY_SIZE_MASK) >> AES_KEY_SIZE_POS;

    if (key_src == DRV_AES_KEY_SRC_FROM_EFUSE) {
        REG_W32(AES_KEY_STORE_SIZE_ADDR, key_size);
        REG_W32(AES_KEY_READ_AREA_ADDR, REG_R32(AES_KEY_WRITE_AREA_ADDR));
        REG_W32(AES_DMA_ADDR, AES_W1T_AES_KEY_READ_MASK);
        if (_aes_chk_read_key_status()) {
            return HAL_ERR;
        }
        _aes_wait_load_key_complete();
    }

    REG_W32(AES_DATA_LEN_0_ADDR, len);
    REG_W32(AES_DMA_ADDR, (0x1 << trig_mode));
    return HAL_NO_ERR;
}

uint8_t aes_get_mode_er8130(void)
{
    uint32_t reg = REG_R32(AES_CTL_ADDR);
    return ((reg & AES_MODE_MASK) >> AES_MODE_POS);
}

void aes_set_init_vect_er8130(const uint8_t iv[16])
{
    for (uint32_t i = 0; i < 4; i++) {
        uint32_t val = 0;
        // Convert big-endian data to little-endian
        val  = iv[i * 4 + 3] << 0;
        val |= iv[i * 4 + 2] << 8;
        val |= iv[i * 4 + 1] << 16;
        val |= iv[i * 4 + 0] << 24;

        REG_W32(AES_IV_N_ADDR(i), val);
    }
}

int8_t aes_read_tag_er8130(uint32_t tags[4])
{
    for (uint32_t i = 0U; i < 4U; i++) {
        tags[i] = REG_R32(AES_TAG_N_ADDR(i));
    }

    return HAL_NO_ERR;
}

void aes_set_ccm_ctrl_er8130(uint8_t ccm_L, uint8_t ccm_M)
{
    uint32_t reg = REG_R32(AES_CTL_ADDR);

    reg &= ~(AES_CCM_L_MASK | AES_CCM_M_MASK);
    reg |= ((ccm_L << AES_CCM_L_POS) & AES_CCM_L_MASK);
    reg |= ((ccm_M << AES_CCM_M_POS) & AES_CCM_M_MASK);
    REG_W32(AES_CTL_ADDR, reg);
}

void aes_set_auth_len_er8130(uint32_t auth_len)
{
    REG_W32(AES_AUTH_LEN_ADDR, auth_len);
}
