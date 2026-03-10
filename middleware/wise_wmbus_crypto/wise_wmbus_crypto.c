#include <string.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_wmbus_crypto.h"

#ifdef WMBUS_SW_CRYPTO
#undef WMBUS_SW_CRYPTO
#endif
#define WMBUS_SW_CRYPTO         1 //currently force SW crypto

#define AES_KEY_LEN             16

#if (defined WMBUS_SW_CRYPTO) && (WMBUS_SW_CRYPTO == 1)
static uint8_t *pAESKey; //the aes key should be stored 32-bit aligned for mbedTls
#else
static uint8_t pAESKey[AES_KEY_LEN] DMA_ATTR;
#endif

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

uint8_t wise_wmbus_crypto_encrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length, uint8_t encryptedBlocks)
{
    if (input == NULL || output == NULL || iv == NULL) 
    {
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
    
    wise_wmbus_aes128_cbc_encrypt(pAESKey, iv, input, encryptedBlocks * 16, output);

    return encryptedBlocks;
}

void wise_wmbus_crypto_decrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length)
{
    if (input == NULL || output == NULL || iv == NULL) 
    {
        return;
    }
    
    wise_wmbus_aes128_cbc_decrypt(pAESKey, iv, input, length, output);
}

#if (defined WMBUS_SW_CRYPTO) && (WMBUS_SW_CRYPTO == 1)
static mbedtls_aes_context aesCBCCtx;

void wise_wmbus_crypto_init()
{
    static uint8_t inited = 0;

    if(inited)
        return;
    
    pAESKey = malloc(AES_KEY_LEN);
    memset(pAESKey, 0, AES_KEY_LEN);
    
    mbedtls_aes_init(&aesCBCCtx);
    inited = 1;
}

void wise_wmbus_crypto_deinit()
{
    if(pAESKey)
    {
        free(pAESKey);
        pAESKey = NULL;
    }
}

void wise_wmbus_crypto_set_key(uint8_t* newKey)
{
    memcpy(pAESKey, newKey, AES_KEY_LEN);
}

int wise_wmbus_crypto_cmac_gen(const uint8_t key[16], const uint8_t *buf, size_t len, uint8_t out16[16])
{
    const mbedtls_cipher_info_t *info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    /*
    debug_print("key %p:\n", key);
    dump_buffer(key, 16);

    debug_print("in %p:\n", buf);
    dump_buffer(buf, 16);

    debug_print("out %p\n", out16);
    */
    
    return mbedtls_cipher_cmac(info, key, 128, buf, len, out16);
}

int wise_wmbus_aes128_cbc_encrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *pt, size_t len, uint8_t *ct)
{
    int rc;
    
    mbedtls_aes_init(&aesCBCCtx);
    mbedtls_aes_setkey_enc(&aesCBCCtx, key, 128);
    rc = mbedtls_aes_crypt_cbc(&aesCBCCtx, MBEDTLS_AES_ENCRYPT, len, iv, pt, ct);
    mbedtls_aes_free(&aesCBCCtx);

    return rc;
}

int wise_wmbus_aes128_cbc_decrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *ct, size_t len, uint8_t *pt)
{
    int rc;
    
    if(len & 0xF) 
        return -1;
    
    mbedtls_aes_init(&aesCBCCtx);
    mbedtls_aes_setkey_dec(&aesCBCCtx, key, 128);
    rc = mbedtls_aes_crypt_cbc(&aesCBCCtx, MBEDTLS_AES_DECRYPT, len, (unsigned char*)iv, ct, pt);
    mbedtls_aes_free(&aesCBCCtx);
    
    return rc;
}

#else
#include "wise_crypto_api.h"

#define CMAC_BLOCK_SIZE              (16U)
#define CMAC_RB                      (0x87U)

WISE_AES_KEY_CONFIG_T mbusAESKeyCfg = 
{
    .key_size  = AES_KEY_128,
    .key_src   = AES_KEY_SRC_FROM_USER,
    .key_num   = 0,
    .key_bytes = (uint32_t *)NULL,
    .swap_mode = AES_SWAP_IN_OUT,
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
    static uint8_t inited = 0;

    if(inited)
        return;
    
    debug_print("WMbus HW Crypto init\n");
    
    //pAESKey = malloc(AES_KEY_LEN);
    memset(pAESKey, 0, AES_KEY_LEN);
    mbusAESKeyCfg.key_bytes = pAESKey;
    
    wise_crypto_init();

    inited = 1;
}

void wise_wmbus_crypto_set_key(uint8_t* newKey)
{  
    memcpy(pAESKey, newKey, AES_KEY_LEN);
    mbusAESKeyCfg.key_bytes = pAESKey;

    if (wise_aes_key_config(&mbusAESKeyCfg) != WISE_SUCCESS) 
    {
        debug_print("set aes key fail\n");
    }
}

static void _cmac_xor_128(uint8_t out[16], const uint8_t a[16], const uint8_t b[16])
{
    size_t i;

    for(i = 0; i < 16; i++)
    {
        out[i] = a[i] ^ b[i];
    }
}

static inline void _cmac_lshift_1(uint8_t out[16], const uint8_t in[16])
{
    int i;
    uint8_t overflow = 0;
    uint8_t next_overflow;

    for(i = 15; i >= 0; i--)
    {
        next_overflow = (uint8_t)((in[i] & 0x80U) ? 1U : 0U);
        out[i] = (uint8_t)((in[i] << 1) | overflow);
        overflow = next_overflow;
    }
}

static void _cmac_gen_subkey(uint8_t out[16], const uint8_t in[16])
{
    uint8_t msb_set;
    size_t i;

    msb_set = (uint8_t)((in[0] & 0x80U) ? 1U : 0U);

    _cmac_lshift_1(out, in);

    if(msb_set != 0U)
    {
        out[15] ^= CMAC_RB;
    }

    /* keep compiler happy if size_t needed later */
    (void)i;
}

static void _cmac_padding(uint8_t out[16], const uint8_t *in, size_t len)
{
    size_t i;

    for(i = 0; i < 16; i++)
    {
        out[i] = 0x00U;
    }

    for(i = 0; i < len; i++)
    {
        out[i] = in[i];
    }

    out[len] = 0x80U;
}

static int _cmac_enc_1_block(const uint8_t key[16], const uint8_t iv[16], const uint8_t in[16], uint8_t out[16])
{
    return wise_wmbus_aes128_cbc_encrypt(key, iv, in, 16, out);
}

DMA_ATTR uint8_t aesCMACBufPool[6][16]; //6 blocks of 16-byte buffer in DMA RAM area
int _wmbus_hybrid_crypto_cmac_gen(const uint8_t key[16], const uint8_t *buf, size_t len, uint8_t out16[16])
{
    int ret;
    size_t nblocks;
    size_t last_len;
    size_t i;
    uint8_t tmp[CMAC_BLOCK_SIZE];
    //uint8_t zero_iv[16];
    uint8_t k1[CMAC_BLOCK_SIZE];
    uint8_t k2[CMAC_BLOCK_SIZE];
    /*
    STATIC_DMA uint8_t zero_block[16];
    STATIC_DMA uint8_t l[16];
    STATIC_DMA uint8_t x[16];
    STATIC_DMA uint8_t y[16];
    STATIC_DMA uint8_t last_block[16];
    */
    uint8_t* zero_block = aesCMACBufPool[0];
    uint8_t* l = aesCMACBufPool[1];
    uint8_t* x = aesCMACBufPool[2];
    uint8_t* y = aesCMACBufPool[3];
    uint8_t* last_block = aesCMACBufPool[4];
    uint8_t* zero_iv = aesCMACBufPool[5];
    
    const uint8_t *p;

    if((key == NULL) || (out16 == NULL))
    {
        return WISE_FAIL;
    }

    if((buf == NULL) && (len != 0U))
    {
        return WISE_FAIL;
    }

    memset(zero_iv, 0, CMAC_BLOCK_SIZE);
    memset(zero_block, 0, CMAC_BLOCK_SIZE);
    memset(x, 0, CMAC_BLOCK_SIZE);

    /*
     * Step 1: L = AES-128(K, 0^128)
     * Use CBC encrypt with IV = 0 and one zero block.
     */
    ret = _cmac_enc_1_block(key, zero_iv, zero_block, l);
    if(ret != WISE_SUCCESS)
    {
        return ret;
    }

    /* Step 2: Generate K1, K2 */
    _cmac_gen_subkey(k1, l);
    _cmac_gen_subkey(k2, k1);

    /*
     * Step 3: Determine block count
     * For len == 0, CMAC still processes one padded block.
     */
    if(len == 0U)
    {
        nblocks = 1U;
        last_len = 0U;
    }
    else
    {
        nblocks = (len + CMAC_BLOCK_SIZE - 1U) / CMAC_BLOCK_SIZE;
        last_len = len % CMAC_BLOCK_SIZE;
    }

    /*
     * Step 4: Process all blocks except the last one
     * X starts from 0, and each step is:
     * X = AES(K, M_i XOR X)
     * which is equivalent to CBC encrypting one block with IV = X.
     */
    p = buf;
    for(i = 0; i < (nblocks - 1U); i++)
    {
        ret = _cmac_enc_1_block(key, x, p, y);
        if(ret != WISE_SUCCESS)
        {
            return ret;
        }

        memcpy(x, y, 16U);
        p += 16U;
    }

    /*
     * Step 5: Build final block
     */
    if((len != 0U) && (last_len == 0U))
    {
        /* Last block is complete */
        _cmac_xor_128(last_block, &buf[(nblocks - 1U) * 16U], k1);
    }
    else
    {
        /* Last block is partial or message length is zero */
        if(last_len == 0U)
        {
            _cmac_padding(tmp, NULL, 0U);
        }
        else
        {
            _cmac_padding(tmp, &buf[(nblocks - 1U) * 16U], last_len);
        }

        _cmac_xor_128(last_block, tmp, k2);
    }

    /*
     * Step 6: Final CMAC
     * T = AES(K, last_block XOR X)
     * equivalent to CBC encrypt one block with IV = X.
     */
    ret = _cmac_enc_1_block(key, x, last_block, out16);
    if(ret != WISE_SUCCESS)
    {
        return ret;
    }

    return WISE_SUCCESS;
}

int wise_wmbus_crypto_cmac_gen(const uint8_t key[16], const uint8_t *buf, size_t len, uint8_t out16[16])
{
    return _wmbus_hybrid_crypto_cmac_gen(key, buf, len, out16);
}

int wise_wmbus_aes128_cbc_encrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *input, size_t length, uint8_t *output)
{
    /*
    debug_print("cbc_encrypt\n");
    debug_print("key %p:\n", key);
    dump_buffer(key, 16);

    debug_print("iv %p:\n", iv);
    dump_buffer(iv, 16);

    debug_print("input %p:\n", input);
    dump_buffer(input, 16);

    debug_print("output %p\n", output);
    */
    
    memcpy(pAESKey, key, AES_KEY_LEN);
    mbusAESKeyCfg.key_bytes = pAESKey;
    wise_aes_key_config(&mbusAESKeyCfg);

    wmbusAESCBCCtx.aes_direction = AES_ENCRYPT;
    wmbusAESCBCCtx.data_cfg.input = input;
    wmbusAESCBCCtx.data_cfg.output = output;
    wmbusAESCBCCtx.data_cfg.length = length;
    wmbusAESCBCCtx.data_cfg.iv_or_cnt = iv;
    
    if(WISE_SUCCESS != wise_aes_exec(&wmbusAESCBCCtx))
    {
        debug_print("wmbus dec failed\n");
        return WISE_FAIL;
    }
    
    return WISE_SUCCESS;
}

int wise_wmbus_aes128_cbc_decrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *input, size_t length, uint8_t *output)
{
    if(length & 0xF) 
        return WISE_FAIL;

    memcpy(pAESKey, key, 16);
    mbusAESKeyCfg.key_bytes = pAESKey;
    wise_aes_key_config(&mbusAESKeyCfg);

    wmbusAESCBCCtx.aes_direction = AES_DECRYPT;
    wmbusAESCBCCtx.data_cfg.input = input;
    wmbusAESCBCCtx.data_cfg.output = output;
    wmbusAESCBCCtx.data_cfg.length = length;
    wmbusAESCBCCtx.data_cfg.iv_or_cnt = iv;

    if(WISE_SUCCESS != wise_aes_exec(&wmbusAESCBCCtx))
    {
        debug_print("wmbus dec failed\n");
        return WISE_FAIL;
    }
    
    return WISE_SUCCESS;
}

#endif
