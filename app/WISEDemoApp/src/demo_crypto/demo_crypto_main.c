/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_tick_api.h"
#include "util.h"
#include "wise_crypto_api.h"

#define DEMO_APP_PROMPT             "CRYPTO> "
#define CRYPTO_BUF_LEN              64

STATIC_DMA_DATA const uint8_t cryptoKey[] = 
{
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

DMA_ATTR uint8_t cryptoInputBuf[CRYPTO_BUF_LEN];
DMA_ATTR uint8_t cryptoEncBuf[CRYPTO_BUF_LEN];
DMA_ATTR uint8_t cryptoDecBuf[CRYPTO_BUF_LEN];

void _init_test_pattern()
{
    int i;

    for(i = 0; i < CRYPTO_BUF_LEN; i++)
    {
        cryptoInputBuf[i] = i;
    }

    memset(cryptoEncBuf, 0, CRYPTO_BUF_LEN);
    memset(cryptoDecBuf, 0, CRYPTO_BUF_LEN);
}

void main(void)
{
    WISE_AES_KEY_CONFIG_T cryptoKeyCfg = {0};
    WISE_AES_CONFIG_CTX_T cryptoAESCtx = {0};
    
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    _init_test_pattern();
    
    //crypto engine init
    wise_crypto_init();

    //setup crypto KEY
    cryptoKeyCfg.key_size = AES_KEY_128;
    cryptoKeyCfg.key_src = AES_KEY_SRC_FROM_USER;
    cryptoKeyCfg.key_num = 0; //NA for AES_KEY_SRC_FROM_USER mode
    cryptoKeyCfg.key_bytes = cryptoKey;
    cryptoKeyCfg.swap_mode = AES_SWAP_NONE;
    cryptoKeyCfg.iv_or_cnt = NULL;
    
    if (wise_aes_key_config(&cryptoKeyCfg) != WISE_SUCCESS)
    {
        debug_print("set aes ECB key fail\n");
        goto demo_fin;
    }

    //execute encryption
    cryptoAESCtx.aes_mode = AES_MODE_ECB;
    cryptoAESCtx.aes_direction = AES_ENCRYPT;
    cryptoAESCtx.data_cfg.input = cryptoInputBuf;
    cryptoAESCtx.data_cfg.output = cryptoEncBuf;
    cryptoAESCtx.data_cfg.length = CRYPTO_BUF_LEN;
    cryptoAESCtx.data_cfg.iv_or_cnt = NULL;

    if(wise_aes_exec(&cryptoAESCtx) != WISE_SUCCESS)
    {
        debug_print("AES ECB encrypt failed\n");
        goto demo_fin;
    }

    //execute decryption
    cryptoAESCtx.aes_mode = AES_MODE_ECB;
    cryptoAESCtx.aes_direction = AES_DECRYPT;
    cryptoAESCtx.data_cfg.input = cryptoEncBuf;
    cryptoAESCtx.data_cfg.output = cryptoDecBuf;
    cryptoAESCtx.data_cfg.length = CRYPTO_BUF_LEN;
    cryptoAESCtx.data_cfg.iv_or_cnt = NULL;

    if(wise_aes_exec(&cryptoAESCtx) != WISE_SUCCESS)
    {
        debug_print("AES ECB decrypt failed\n");
        goto demo_fin;
    }

    debug_print("AES ECB key:\n");
    dump_buffer(cryptoKey, 16);

    debug_print("Data pattern:\n");
    dump_buffer(cryptoInputBuf, CRYPTO_BUF_LEN);

    debug_print("Encrypted:\n");
    dump_buffer(cryptoEncBuf, CRYPTO_BUF_LEN);

    debug_print("Decrypted:\n");
    dump_buffer(cryptoDecBuf, CRYPTO_BUF_LEN);

demo_fin:
    while (1) 
    {
        wise_main_proc();
    }
}
