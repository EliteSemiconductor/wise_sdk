/**
 * @file wise_wmbus_crypto.h
 * @brief wise_wmbus_crypto.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */

#ifndef __WISE_WMBUS_CRYPTO_H 
#define __WISE_WMBUS_CRYPTO_H

#include <stdint.h>
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"

typedef enum {
    DC_KENC = 0x00, /* Encryption from the meter (Kenc) */
    DC_KMAC = 0x01, /* MAC from the meter (Kmac)       */
    DC_LENC = 0x10, /* Encryption to the meter (Lenc)  */
    DC_LMAC = 0x11  /* MAC to the meter (Lmac)         */
} mode7_dc_t;

void wise_wmbus_crypto_init();
void wise_wmbus_crypto_deinit();
void wise_wmbus_crypto_set_key(uint8_t* key);
void wise_wmbus_crypto_get_key(uint8_t* key);
uint8_t wise_wmbus_crypto_encrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length, uint8_t encryptedBlocks);
void wise_wmbus_crypto_decrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length);

int wise_wmbus_crypto_cmac_gen(const uint8_t key[16], const uint8_t *buf, size_t len, uint8_t out16[16]);
int wise_wmbus_aes128_cbc_encrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *input, size_t len, uint8_t *output);
int wise_wmbus_aes128_cbc_decrypt(const uint8_t key[16], const uint8_t iv[16], const uint8_t *input, size_t len, uint8_t *output);

#endif

