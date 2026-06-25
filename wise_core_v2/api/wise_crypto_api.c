/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_crypto_api.h"
#include "hal_intf_crypto.h"
#include "hal_intf_dma.h"
#include "util.h"
#include "wise_core.h"
#include <stdint.h>

static uint8_t cryptoInited = 0;

WISE_STATUS wise_crypto_init()
{
#ifdef CHIP_HAS_CRYPTO
    if(cryptoInited == 0)
    {
        hal_intf_crypto_dma_init();
        cryptoInited = 1;
    }
    return WISE_SUCCESS;
#else
    return WISE_FAIL;
#endif
}

void wise_crypto_deinit()
{
}

WISE_STATUS wise_aes_start(uint32_t len, AES_TRIG_MODE_T trig_mode)
{
#ifdef CHIP_CRYPTO_HAS_AES
    return hal_intf_aes_start(len, trig_mode);
#else
    return WISE_FAIL;
#endif
}

WISE_STATUS wise_aes_key_config(const WISE_AES_KEY_CONFIG_T *key_cfg)
{
#ifdef CHIP_CRYPTO_HAS_AES
    return hal_intf_aes_configure(0, 0, key_cfg->swap_mode, key_cfg->key_src, key_cfg->key_size, key_cfg->key_bytes, key_cfg->key_num);
#else
    return WISE_FAIL;
#endif
}

WISE_STATUS wise_aes_exec(WISE_AES_CONFIG_CTX_T *ctx)
{
    int ret = WISE_FAIL;

#ifdef CHIP_CRYPTO_HAS_AES
    hal_intf_aes_mode_and_dir_config(ctx->aes_direction, ctx->aes_mode);

    HAL_AES_DMA_SETUP_T dma_cfg = {.data_io = {.input         = ctx->data_cfg.input,
                                               .output        = ctx->data_cfg.output,
                                               .input_length  = ctx->data_cfg.length,
                                               .output_length = ctx->data_cfg.length},
                                   .auth_in = NULL};

    ret = hal_intf_aes_cfg_init_vect(ctx->data_cfg.iv_or_cnt);
    if (ret != WISE_SUCCESS) {
        return ret;
    }

    ret = hal_intf_aes_dma_update(&dma_cfg);
    if (ret != WISE_SUCCESS) {
        return ret;
    }
    hal_intf_aes_start(ctx->data_cfg.length, AES_DMA_DATA_TRIG);
    if (ctx->aes_mode == AES_MODE_CBC_MAC) {
        hal_intf_aes_wait_tag_finished();
        hal_intf_aes_read_tag((uint32_t *)ctx->data_cfg.output);
    } else {
        hal_intf_aes_wait_finished();
    }
    return WISE_SUCCESS;
#else
    return WISE_FAIL;
#endif
}

WISE_STATUS wise_aes_read_tags(uint32_t tags[4])
{
    int ret = WISE_FAIL;

#ifdef CHIP_CRYPTO_HAS_AES
    ret = hal_intf_aes_read_tag(tags);
    return ret;
#else
    return ret;
#endif
}
WISE_STATUS wise_aes_ccm_configure(WISE_AES_CCM_CTX_T *ctx)
{
#ifdef CHIP_CRYPTO_HAS_AES
    if (!ctx || (ctx->ccm_cfg.nonce_len > 13) || (ctx->ccm_cfg.tag_len % 2 != 0) || (ctx->ccm_cfg.auth_len > 0 && ctx->ccm_cfg.auth_data == NULL)) {
        return WISE_FAIL;
    }

    AES_DIR_T dir = ctx->is_encrypt ? AES_ENCRYPT : AES_DECRYPT;
    int ret = hal_intf_aes_configure(dir, AES_MODE_CCM, ctx->key_cfg.swap_mode, ctx->key_cfg.key_src, ctx->key_cfg.key_size, ctx->key_cfg.key_bytes,
                                     ctx->key_cfg.key_num);
    if (ret != HAL_NO_ERR) {
        return WISE_FAIL;
    }

    if (ctx->ccm_cfg.auth_len > 0) {
        ctx->auth_cfg.input        = ctx->ccm_cfg.auth_data;
        ctx->auth_cfg.output       = NULL;
        ctx->auth_cfg.input_length = ctx->ccm_cfg.auth_len;
        ctx->has_auth              = true;
    } else {
        ctx->has_auth = false;
    }

    ret = hal_drv_aes_set_ccm_info(ctx->ccm_cfg.nonce, ctx->ccm_cfg.nonce_len, ctx->ccm_cfg.tag_len, ctx->ccm_cfg.auth_len);
    return (ret == HAL_NO_ERR) ? WISE_SUCCESS : WISE_FAIL;
#else
    return WISE_FAIL;
#endif
}
WISE_STATUS wise_aes_ccm_exec(WISE_AES_CCM_CTX_T *ctx, const WISE_AES_DATA_T *data)
{
#ifdef CHIP_CRYPTO_HAS_AES
    int ret = WISE_FAIL;

    if (!ctx || !data || data->input == NULL || data->output == NULL || (!ctx->is_encrypt && data->length < ctx->ccm_cfg.tag_len)) {
        return WISE_FAIL;
    }

    uint32_t payload_len = ctx->is_encrypt ? data->length : (data->length - ctx->ccm_cfg.tag_len);

    HAL_AES_DMA_SETUP_T dma_cfg = {
        .data_io =
            {
                .input         = data->input,
                .output        = data->output,
                .input_length  = data->length,
                .output_length = ctx->is_encrypt ? (payload_len + ctx->ccm_cfg.tag_len) : payload_len,
            },
        .auth_in = ctx->has_auth ? &ctx->auth_cfg : NULL,
    };

    //ret = hal_intf_aes_dma_setup(&dma_cfg);
    ret = hal_intf_aes_dma_update(&dma_cfg);
    if (ret != WISE_SUCCESS) {
        return ret;
    }

    if (ctx->is_encrypt) {
        if (ctx->ccm_cfg.auth_len > 0) {
            hal_intf_aes_start(data->length, AES_DMA_AUTH_TRIG);
        }

        hal_intf_aes_wait_tag_finished();

        if (data->length > 0) {
            hal_intf_dma_aes_in_channel_trigger();
            hal_intf_aes_start(data->length, AES_DMA_DATA_TRIG);
        }

        hal_intf_aes_wait_finished();
        return WISE_SUCCESS;

    } else {
        hal_intf_aes_start(data->length, AES_DMA_DATA_TRIG);
        hal_intf_aes_wait_tag_finished();

        if (data->length != ctx->ccm_cfg.tag_len) {
            hal_intf_dma_aes_in_channel_trigger();
        }

        if (ctx->ccm_cfg.auth_len > 0) {
            hal_intf_aes_start(data->length, AES_DMA_AUTH_TRIG);
        } else {
            hal_intf_aes_start(data->length, AES_DMA_DATA_TRIG);
        }

        hal_intf_aes_wait_finished();

        return (hal_intf_aes_ccm_decryption_result() == WISE_SUCCESS) ? WISE_SUCCESS : WISE_FAIL;
    }
#else
    return WISE_FAIL;
#endif
}

WISE_STATUS wise_sha_generate(E_SHA_MODE_T mode, uint32_t dataAddr, uint32_t dataLen, uint8_t *outputSha)
{
#ifdef CHIP_CRYPTO_HAS_SHA
    uint8_t shaMode;

    if ((!outputSha) || (!dataAddr) || (!dataLen)) {
        return WISE_FAIL;
    }

    switch (mode) {
    case E_SHA_MODE_224:
        shaMode = (SHA_BIT_224 | SHA_IN_SWAP);
        break;

    case E_SHA_MODE_256:
        shaMode = (SHA_BIT_256 | SHA_IN_SWAP);
        break;

    default:
        return WISE_FAIL;
    }

    if (HAL_NO_ERR != hal_intf_sha_generate(shaMode, dataAddr, dataLen, outputSha)) {
        return WISE_FAIL;
    }

    return WISE_SUCCESS;
#else
    return WISE_FAIL;
#endif
}
