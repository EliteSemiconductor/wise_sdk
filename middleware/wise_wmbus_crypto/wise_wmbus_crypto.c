#include <string.h>
#include "wise_core.h"
#include "wise_wmbus_crypto.h"

static uint8_t aesKey[16]; //the aes key should be stored 32-bit aligned for mbedTls
uint16_t _add_idle_filler(uint8_t *buffer, uint16_t fillerLength);

uint16_t _add_idle_filler(uint8_t *buffer, uint16_t fillerLength)
{
    if (buffer == NULL) 
    {
      return 0;
    }
    memset(buffer, 0x2f, fillerLength);
    return fillerLength;
}

void wise_wmbus_crypto_get_key(uint8_t* key)
{
    memcpy(key, aesKey, 16);
}

// #ifdef CHIP_RADIO_HAS_W_MBUS_FAKE
#if 1
static mbedtls_aes_context aes_ctx;

void wise_wmbus_crypto_init()
{
    mbedtls_aes_init(&aes_ctx);
}

void wise_wmbus_crypto_set_key(uint8_t* newKey)
{
    memcpy(aesKey, newKey, 16);
}

uint8_t wise_wmbus_crypto_encrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length, uint8_t encryptedBlocks)
{
    if (input == NULL || output == NULL || iv == NULL) 
    {
        return 0;
    }

    mbedtls_aes_setkey_enc(&aes_ctx, aesKey, 128);

    if ( encryptedBlocks == 0 ) 
    {
        encryptedBlocks = (uint8_t)(length / 16);
        if ( length % 16 ) 
        {
            encryptedBlocks++;
        }
    }

    //add 0x2F padding as per EN13757-3
    if ( (uint16_t)encryptedBlocks * 16 > length) 
    {
        _add_idle_filler(input + length, (uint16_t)encryptedBlocks * 16 - length);
    }

    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, (uint16_t)encryptedBlocks * 16, iv, input, output);

    return encryptedBlocks;
}

void wise_wmbus_crypto_decrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length)
{
    if (input == NULL || output == NULL || iv == NULL) 
    {
        return;
    }

    mbedtls_aes_setkey_dec(&aes_ctx, aesKey, 128);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, length, iv, input, output);
}

void put_u32_le(uint8_t b[4], uint32_t v){
    b[0] = (uint8_t)(v & 0xFF);
    b[1] = (uint8_t)((v>>8) & 0xFF);
    b[2] = (uint8_t)((v>>16) & 0xFF);
    b[3] = (uint8_t)((v>>24) & 0xFF);
}

void wise_wmbus_derive_key(uint8_t tag,
                    uint32_t mcr,
                    uint32_t ident,
                    uint8_t out_key16[16])
{
    const mbedtls_cipher_info_t *info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    uint8_t buf[16] = {0};
    // CMAC( Mk, tag || MCR(4B,L) || Ident(4B,L) || 0x07 padding )  16 bytes
    buf[0] = tag;
    put_u32_le(&buf[1], mcr);    //LSB-first
    put_u32_le(&buf[5], ident); //LSB-first
    memset(&buf[9], 0x07, 7);
    //printf("tag=%d\n", tag);
    //dump_raw("buf", buf, 16);
    //dump_raw("aesKey", aesKey, 16);
    mbedtls_cipher_cmac(info, aesKey, 128, buf, sizeof(buf), out_key16);
}

int wise_wmbus_cmac_oneshot(const mbedtls_cipher_info_t *info,
                        const uint8_t key[16],
                        const uint8_t *buf, size_t len,
                        uint8_t out16[16])
{
    return mbedtls_cipher_cmac(info, key, 128, buf, len, out16);
}

int wise_wmbus_aes128_cbc_encrypt(const uint8_t key[16],
                              const uint8_t iv_in[16],
                              const uint8_t *pt, size_t len,
                              uint8_t *ct)
{
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, key, 128);
    int rc = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, len, iv_in, pt, ct);
    mbedtls_aes_free(&ctx);

    return rc;
}

int wise_wmbus_aes128_cbc_decrypt(const uint8_t key[16], const uint8_t iv[16],
                              const uint8_t *ct, size_t len, uint8_t *pt)
{
    if(len % 16) return -1;
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, key, 128);
    int rc = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, len, (unsigned char*)iv, ct, pt);
    mbedtls_aes_free(&aes);
    return rc;
}

#else
#include "wise_crypto_api.h"

WISE_AES_KEY_CONFIG_T mbusAESKeyCfg = 
{
    .key_size  = AES_KEY_128,
    .key_src   = AES_KEY_SRC_FROM_USER,
    .key_num   = 0,
    .key_bytes = (uint32_t *)NULL,
    .swap_mode = AES_SWAP_NONE,
    .iv_or_cnt = (uint32_t *)NULL,
};

WISE_AES_CONFIG_CTX_T wmbusAESCBCCtx = 
{
    .aes_mode      = AES_MODE_CBC,
    .aes_direction = AES_ENCRYPT,
    .data_cfg      = NULL,
};



void wise_wmbus_crypto_init()
{
   wise_crypto_init();
}

void wise_wmbus_crypto_set_key(uint8_t* newKey)
{
    debug_print("WMBUS setup HW AES support\n");
    
    memcpy(aesKey, newKey, 16);
    mbusAESKeyCfg.key_bytes = aesKey;

    if (wise_aes_key_config(&mbusAESKeyCfg) != WISE_SUCCESS) 
    {
        debug_print("set aes key fail\n");
    }
}

uint8_t wise_wmbus_crypto_encrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length, uint8_t encryptedBlocks)
{    
    if (input == NULL || output == NULL || iv == NULL) 
    {
        return 0;
    }

    if(mbusAESKeyCfg.key_bytes == NULL)
    {
        debug_print("wmbus enc empty key\n");
        return 0;
    }
    
    if ( encryptedBlocks == 0 ) 
    {
        encryptedBlocks = (uint8_t)(length / 16);
        if ( length % 16 ) 
        {
            encryptedBlocks++;
        }
    }

    //add 0x2F padding as per EN13757-3
    if ( (uint16_t)encryptedBlocks * 16 > length) 
    {
        _add_idle_filler(input + length, (uint16_t)encryptedBlocks * 16 - length);
    }

    wmbusAESCBCCtx.aes_direction = AES_ENCRYPT;
    wmbusAESCBCCtx.data_cfg.input = input;
    wmbusAESCBCCtx.data_cfg.output = output;
    wmbusAESCBCCtx.data_cfg.length = length;
    wmbusAESCBCCtx.data_cfg.iv_or_cnt = iv;

    if(WISE_SUCCESS != wise_aes_exec(&wmbusAESCBCCtx))
    {
        debug_print("wmbus enc failed\n");
        return 0;
    }
    
    return encryptedBlocks;
}

void wise_wmbus_crypto_decrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length)
{
    if (input == NULL || output == NULL || iv == NULL) 
    {
        return;
    }

    if(mbusAESKeyCfg.key_bytes == NULL)
    {
        debug_print("wmbus dec empty key\n");
        return;
    }

    wmbusAESCBCCtx.aes_direction = AES_DECRYPT;
    wmbusAESCBCCtx.data_cfg.input = input;
    wmbusAESCBCCtx.data_cfg.output = output;
    wmbusAESCBCCtx.data_cfg.length = length;
    wmbusAESCBCCtx.data_cfg.iv_or_cnt = iv;

    if(WISE_SUCCESS != wise_aes_exec(&wmbusAESCBCCtx))
    {
        debug_print("wmbus dec failed\n");
    }
}

#endif
