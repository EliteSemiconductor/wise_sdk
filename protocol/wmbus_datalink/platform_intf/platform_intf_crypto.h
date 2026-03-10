/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __PLATFORM_INTF_CRYPTO_H
#define __PLATFORM_INTF_CRYPTO_H

void platform_crypto_init(void);
void platform_crypto_set_key(uint8_t* newKey);
void platform_crypto_get_key(uint8_t* key);
uint8_t platform_crypto_encrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length, uint8_t encryptedBlocks);
void platform_crypto_decrypt(uint8_t *input, uint8_t *output, uint8_t *iv, uint16_t length);

#endif
