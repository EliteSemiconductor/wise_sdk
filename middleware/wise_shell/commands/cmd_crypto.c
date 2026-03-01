/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#include "wise_core.h"
#include "util.h"
#include <stddef.h>
#include <stdint.h>
#if SHELL_CRYPTO
#include "api/wise_tick_api.h"
#include "api/wise_crypto_api.h"
#include "cmd_crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

static int _aes_ecb_Cmd(int argc, char **argv);
static int _aes_cbc_Cmd(int argc, char **argv);
static int _aes_ctr_Cmd(int argc, char **argv);
static int _aes_cbcmac_Cmd(int argc, char **argv);
static int _aes_ccm_Cmd(int argc, char **argv);
static int _sha_generate_Cmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"aes_ecb", "AES ECB demo. Usage: aes_ecb [enc|dec]", _aes_ecb_Cmd},
    {"aes_cbc", "AES CBC demo. Usage: aes_cbc [enc|dec]", _aes_cbc_Cmd},
    {"aes_ctr", "AES CTR demo. Usage: aes_ctr [enc|dec]", _aes_ctr_Cmd},
    {"aes_cbcmac", "AES CBC-MAC demo. Usage: aes_cbcmac [gen|tag]", _aes_cbcmac_Cmd},
    {"aes_ccm", "AES CCM demo. Usage: aes_ccm [enc|dec]", _aes_ccm_Cmd},
    {"sha", "SHA demo. Usage: sha [224|256]", _sha_generate_Cmd},
};

int CRYPTOCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static void print_bytes_as_words(const char *label, const uint8_t *data, size_t len)
{
    printf("%s:\n", label);
    for (size_t i = 0; i < len; i += 4) {
        uint32_t word = (uint32_t)data[i] | ((uint32_t)data[i + 1] << 8) | ((uint32_t)data[i + 2] << 16) | ((uint32_t)data[i + 3] << 24);
        printf("%02X %02X %02X %02X => 0x%08" PRIX32 "\n", data[i], data[i + 1], data[i + 2], data[i + 3], word);
    }
}

int8_t check_ciphertext(const uint8_t *ciphertext, const uint8_t *expected_bytes, size_t length)
{
    int all_match = 1;

    for (size_t i = 0; i < length; i += 4) {
        uint32_t word_actual = ((uint32_t)ciphertext[i]) | ((uint32_t)ciphertext[i + 1] << 8) | ((uint32_t)ciphertext[i + 2] << 16) |
                               ((uint32_t)ciphertext[i + 3] << 24);

        uint32_t word_expected = ((uint32_t)expected_bytes[i]) | ((uint32_t)expected_bytes[i + 1] << 8) | ((uint32_t)expected_bytes[i + 2] << 16) |
                                 ((uint32_t)expected_bytes[i + 3] << 24);

        if (word_actual != word_expected) {
            printf("Mismatch @[%02u]: %02X %02X %02X %02X => 0x%08lX (expected: 0x%08lX)\n", i / 4, ciphertext[i], ciphertext[i + 1],
                   ciphertext[i + 2], ciphertext[i + 3], word_actual, word_expected);
            all_match = 0;
        } else {
            printf("Match    @[%02u]: %02X %02X %02X %02X => 0x%08lX\n", i / 4, ciphertext[i], ciphertext[i + 1], ciphertext[i + 2],
                   ciphertext[i + 3], word_actual);
        }
    }

    return all_match ? 1 : 0;
}
#if 0
static int _aes_ecb_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_ecb [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_ecb [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    const uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    const uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    const uint8_t golden_ciphertext[64] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97,
                                           0xF5, 0xD3, 0xD5, 0x85, 0x03, 0xB9, 0x69, 0x9D, 0xE7, 0x85, 0x89, 0x5A, 0x96, 0xFD, 0xBA, 0xAF,
                                           0x43, 0xB1, 0xCD, 0x7F, 0x59, 0x8E, 0xCE, 0x23, 0x88, 0x1B, 0x00, 0xE3, 0xED, 0x03, 0x06, 0x88,
                                           0x7B, 0x0C, 0x78, 0x5E, 0x27, 0xE8, 0xAD, 0x3F, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5D, 0xD4};

    uint8_t plaintext_le[64];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    uint8_t golden_ciphertext_le[64];
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = (is_encrypt ? golden_ciphertext_le : plaintext_le);

    uint8_t output[64] = {0};

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size = AES_KEY_128, .key_src = AES_KEY_SRC_FROM_USER, .key_num = 0, .key_bytes = aes_key, .swap_mode = AES_SWAP_NONE};

    WISE_AES_DATA_T data_cfg = {.input = input_data, .output = output, .length = sizeof(output)};

    status = is_encrypt ? wise_aes_ecb_encrypt(&key_cfg, &data_cfg) : wise_aes_ecb_decrypt(&key_cfg, &data_cfg);

    if (status != WISE_SUCCESS) {
        printf("AES ECB %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES ECB %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#else
static int _aes_ecb_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_ecb [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_ecb [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    STATIC_DMA_DATA uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

     STATIC_DMA_DATA uint8_t golden_ciphertext[64] = {
        0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97, 0xF5, 0xD3, 0xD5, 0x85, 0x03, 0xB9,
        0x69, 0x9D, 0xE7, 0x85, 0x89, 0x5A, 0x96, 0xFD, 0xBA, 0xAF, 0x43, 0xB1, 0xCD, 0x7F, 0x59, 0x8E, 0xCE, 0x23, 0x88, 0x1B, 0x00, 0xE3,
        0xED, 0x03, 0x06, 0x88, 0x7B, 0x0C, 0x78, 0x5E, 0x27, 0xE8, 0xAD, 0x3F, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5D, 0xD4};

    STATIC_DMA uint8_t plaintext_le[64];
    STATIC_DMA uint8_t golden_ciphertext_le[64];
    STATIC_DMA uint8_t output[64];
  
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = is_encrypt ? golden_ciphertext_le : plaintext_le;

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size  = AES_KEY_128,
        .key_src   = AES_KEY_SRC_FROM_USER,
        .key_num   = 0,
        .key_bytes = aes_key,
        .swap_mode = AES_SWAP_NONE,
        .iv_or_cnt = NULL,
    };

    WISE_AES_CONFIG_CTX_T ctx = {
        .aes_mode      = AES_MODE_ECB,
        .aes_direction = is_encrypt ? AES_ENCRYPT : AES_DECRYPT,
        .data_cfg =
            {
                .input  = input_data,
                .output = output,
                .length = sizeof(output),
            },
    };

    status = wise_aes_key_config(&key_cfg);
    if (status != WISE_SUCCESS) {
        printf("Key config failed\n");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    status = wise_aes_exec(&ctx);
    if (status != WISE_SUCCESS) {
        printf("AES ECB %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES ECB %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif

#if 0
static int _aes_cbc_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_cbc [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_cbc [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    const uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    const uint8_t aes_iv[16]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    const uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    const uint8_t golden_ciphertext[64] = {0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46, 0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D,
                                           0x50, 0x86, 0xCB, 0x9B, 0x50, 0x72, 0x19, 0xEE, 0x95, 0xDB, 0x11, 0x3A, 0x91, 0x76, 0x78, 0xB2,
                                           0x73, 0xBE, 0xD6, 0xB8, 0xE3, 0xC1, 0x74, 0x3B, 0x71, 0x16, 0xE6, 0x9E, 0x22, 0x22, 0x95, 0x16,
                                           0x3F, 0xF1, 0xCA, 0xA1, 0x68, 0x1F, 0xAC, 0x09, 0x12, 0x0E, 0xCA, 0x30, 0x75, 0x86, 0xE1, 0xA7};

    uint8_t plaintext_le[64];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    uint8_t golden_ciphertext_le[64];
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = (is_encrypt ? golden_ciphertext_le : plaintext_le);

    uint8_t output[64] = {0};

    WISE_AES_KEY_CONFIG_T key_cfg = {.key_size  = AES_KEY_128,
                                     .key_src   = AES_KEY_SRC_FROM_USER,
                                     .key_num   = 0,
                                     .key_bytes = aes_key,
                                     .swap_mode = AES_SWAP_NONE,
                                     .iv_or_cnt = aes_iv};

    WISE_AES_DATA_T data_cfg = {.input = input_data, .output = output, .length = sizeof(output)};

    status = is_encrypt ? wise_aes_cbc_encrypt(&key_cfg, &data_cfg) : wise_aes_cbc_decrypt(&key_cfg, &data_cfg);

    if (status != WISE_SUCCESS) {
        printf("AES CBC %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
    print_bytes_as_words("IV", (const uint8_t *)aes_iv, sizeof(aes_iv));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES CBC %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#else
static int _aes_cbc_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_cbc [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_cbc [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    STATIC_DMA_DATA uint8_t aes_iv[16]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    STATIC_DMA_DATA uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    STATIC_DMA_DATA uint8_t golden_ciphertext[64] = {0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46, 0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D,
                                           0x50, 0x86, 0xCB, 0x9B, 0x50, 0x72, 0x19, 0xEE, 0x95, 0xDB, 0x11, 0x3A, 0x91, 0x76, 0x78, 0xB2,
                                           0x73, 0xBE, 0xD6, 0xB8, 0xE3, 0xC1, 0x74, 0x3B, 0x71, 0x16, 0xE6, 0x9E, 0x22, 0x22, 0x95, 0x16,
                                           0x3F, 0xF1, 0xCA, 0xA1, 0x68, 0x1F, 0xAC, 0x09, 0x12, 0x0E, 0xCA, 0x30, 0x75, 0x86, 0xE1, 0xA7};

    STATIC_DMA uint8_t plaintext_le[64];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    STATIC_DMA uint8_t golden_ciphertext_le[64];
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = is_encrypt ? golden_ciphertext_le : plaintext_le;

    STATIC_DMA uint8_t output[64] = {0};

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size  = AES_KEY_128,
        .key_src   = AES_KEY_SRC_FROM_USER,
        .key_num   = 0,
        .key_bytes = aes_key,
        .swap_mode = AES_SWAP_NONE,
        .iv_or_cnt = aes_iv,
    };

    WISE_AES_CONFIG_CTX_T ctx = {
        .aes_mode      = AES_MODE_CBC,
        .aes_direction = is_encrypt ? AES_ENCRYPT : AES_DECRYPT,
        .data_cfg =
            {
                .input     = input_data,
                .output    = output,
                .length    = sizeof(output),
                .iv_or_cnt = aes_iv,
            },
    };

    status = wise_aes_key_config(&key_cfg);
    if (status != WISE_SUCCESS) {
        printf("AES key config failed\n");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    status = wise_aes_exec(&ctx);
    if (status != WISE_SUCCESS) {
        printf("AES CBC %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", aes_key, sizeof(aes_key));
    print_bytes_as_words("IV", aes_iv, sizeof(aes_iv));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES CBC %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif
#if 0
static int _aes_ctr_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_ctr [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_ctr [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    const uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    const uint8_t aes_cnt[16] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};

    const uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    const uint8_t golden_ciphertext[64] = {0x87, 0x4D, 0x61, 0x91, 0xB6, 0x20, 0xE3, 0x26, 0x1B, 0xEF, 0x68, 0x64, 0x99, 0x0D, 0xB6, 0xCE,
                                           0x98, 0x06, 0xF6, 0x6B, 0x79, 0x70, 0xFD, 0xFF, 0x86, 0x17, 0x18, 0x7B, 0xB9, 0xFF, 0xFD, 0xFF,
                                           0x5A, 0xE4, 0xDF, 0x3E, 0xDB, 0xD5, 0xD3, 0x5E, 0x5B, 0x4F, 0x09, 0x02, 0x0D, 0xB0, 0x3E, 0xAB,
                                           0x1E, 0x03, 0x1D, 0xDA, 0x2F, 0xBE, 0x03, 0xD1, 0x79, 0x21, 0x70, 0xA0, 0xF3, 0x00, 0x9C, 0xEE};

    uint8_t plaintext_le[64];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    uint8_t golden_ciphertext_le[64];
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));
    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = (is_encrypt ? golden_ciphertext_le : plaintext_le);

    uint8_t output[64] = {0};

    WISE_AES_KEY_CONFIG_T key_cfg = {.key_size  = AES_KEY_128,
                                     .key_src   = AES_KEY_SRC_FROM_USER,
                                     .key_num   = 0,
                                     .key_bytes = aes_key,
                                     .swap_mode = AES_SWAP_NONE,
                                     .iv_or_cnt = aes_cnt};

    WISE_AES_DATA_T data_cfg = {.input = input_data, .output = output, .length = sizeof(output)};

    status = is_encrypt ? wise_aes_ctr_encrypt(&key_cfg, &data_cfg) : wise_aes_ctr_decrypt(&key_cfg, &data_cfg);

    if (status != WISE_SUCCESS) {
        printf("AES CTR %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
    print_bytes_as_words("CNT", (const uint8_t *)aes_cnt, sizeof(aes_cnt));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES CTR %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#else
static int _aes_ctr_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_ctr [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: aes_ctr [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    STATIC_DMA_DATA uint8_t aes_cnt[16] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};

    STATIC_DMA_DATA uint8_t plaintext[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                   0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                   0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                   0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    STATIC_DMA_DATA uint8_t golden_ciphertext[64] = {0x87, 0x4D, 0x61, 0x91, 0xB6, 0x20, 0xE3, 0x26, 0x1B, 0xEF, 0x68, 0x64, 0x99, 0x0D, 0xB6, 0xCE,
                                           0x98, 0x06, 0xF6, 0x6B, 0x79, 0x70, 0xFD, 0xFF, 0x86, 0x17, 0x18, 0x7B, 0xB9, 0xFF, 0xFD, 0xFF,
                                           0x5A, 0xE4, 0xDF, 0x3E, 0xDB, 0xD5, 0xD3, 0x5E, 0x5B, 0x4F, 0x09, 0x02, 0x0D, 0xB0, 0x3E, 0xAB,
                                           0x1E, 0x03, 0x1D, 0xDA, 0x2F, 0xBE, 0x03, 0xD1, 0x79, 0x21, 0x70, 0xA0, 0xF3, 0x00, 0x9C, 0xEE};

    STATIC_DMA uint8_t plaintext_le[64];
    STATIC_DMA uint8_t golden_ciphertext_le[64];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *input_data  = is_encrypt ? plaintext_le : golden_ciphertext_le;
    const uint8_t *golden_data = is_encrypt ? golden_ciphertext_le : plaintext_le;

    STATIC_DMA uint8_t output[64] = {0};

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size  = AES_KEY_128,
        .key_src   = AES_KEY_SRC_FROM_USER,
        .key_num   = 0,
        .key_bytes = aes_key,
        .swap_mode = AES_SWAP_NONE,
        .iv_or_cnt = aes_cnt,
    };

    WISE_AES_CONFIG_CTX_T ctx = {
        .aes_mode      = AES_MODE_CTR,
        .aes_direction = is_encrypt ? AES_ENCRYPT : AES_DECRYPT,
        .data_cfg =
            {
                .input     = input_data,
                .output    = output,
                .length    = sizeof(output),
                .iv_or_cnt = aes_cnt,
            },
    };

    status = wise_aes_key_config(&key_cfg);
    if (status != WISE_SUCCESS) {
        printf("AES key config failed\n");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    status = wise_aes_exec(&ctx);
    if (status != WISE_SUCCESS) {
        printf("AES CTR %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("AES Key", aes_key, sizeof(aes_key));
    print_bytes_as_words("CNT", aes_cnt, sizeof(aes_cnt));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext", input_data, sizeof(output));
    print_bytes_as_words(is_encrypt ? "Ciphertext" : "Plaintext", output, sizeof(output));

    int result = check_ciphertext(output, golden_data, sizeof(output));
    printf("\nAES CTR %s %s!\n", is_encrypt ? "encryption" : "decryption", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif

#if 0
static int _aes_cbcmac_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_cbcmac [gen|tag]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_generate = (strcmp(argv[2], "gen") == 0);
    bool is_checktag = (strcmp(argv[2], "tag") == 0);
    if (!is_generate && !is_checktag) {
        printf("Invalid mode: %s\nUsage: aes_cbcmac [gen|tag]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    const uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    const uint8_t aes_iv[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    const uint8_t cbcmac_input[64]      = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                           0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                           0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                           0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};
    const uint8_t cbcmac_golden_tag[16] = {0xA7, 0x35, 0x6E, 0x12, 0x07, 0xBB, 0x40, 0x66, 0x39, 0xE5, 0xE5, 0xCE, 0xB9, 0xA9, 0xED, 0x93};

    uint8_t cbcmac_input_le[64];
    wise_swap_be32_to_le32(cbcmac_input_le, cbcmac_input, sizeof(cbcmac_input));
    uint8_t cbcmac_golden_tag_le[16];
    wise_swap_be32_to_le32(cbcmac_golden_tag_le, cbcmac_golden_tag, sizeof(cbcmac_golden_tag));

    uint32_t tag_u32[4] = {0};
    uint8_t *tag        = (uint8_t *)tag_u32;

    WISE_AES_KEY_CONFIG_T key_cfg = {.key_size  = AES_KEY_128,
                                     .key_src   = AES_KEY_SRC_FROM_USER,
                                     .key_num   = 0,
                                     .key_bytes = aes_key,
                                     .swap_mode = AES_SWAP_NONE,
                                     .iv_or_cnt = aes_iv};

    WISE_AES_DATA_T data_cfg = {.input = cbcmac_input_le, .output = tag, .length = sizeof(cbcmac_input_le)};

    if (is_generate) {
        status = wise_aes_cbcmac_generate(&key_cfg, &data_cfg);
        if (status != WISE_SUCCESS) {
            printf("AES CBC-MAC generation failed\n");
            wise_crypto_deinit();
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    if (is_checktag) {
        status = wise_aes_read_tags(tag_u32);
        if (status != WISE_SUCCESS) {
            printf("Failed to read AES tag\n");
            wise_crypto_deinit();
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }
    if (is_generate) {
        print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
        print_bytes_as_words("IV", (const uint8_t *)aes_iv, sizeof(aes_iv));
        print_bytes_as_words("CBCMAC_INPUT", cbcmac_input, sizeof(cbcmac_input));
    }
    print_bytes_as_words("CBCMAC_OUTPUT", tag, sizeof(cbcmac_golden_tag));

    int result = check_ciphertext(tag, cbcmac_golden_tag_le, sizeof(cbcmac_golden_tag_le));
    printf("\nAES CBC-MAC %s %s!\n", is_checktag ? "TAG Check" : "TAG Generate", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#else
static int _aes_cbcmac_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: aes_cbcmac [gen|tag]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_generate = (strcmp(argv[2], "gen") == 0);
    bool is_checktag = (strcmp(argv[2], "tag") == 0);
    if (!is_generate && !is_checktag) {
        printf("Invalid mode: %s\nUsage: aes_cbcmac [gen|tag]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    STATIC_DMA_DATA uint8_t aes_iv[16] = {0};

    STATIC_DMA_DATA uint8_t cbcmac_input[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
                                      0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
                                      0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
                                      0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

    STATIC_DMA_DATA uint8_t cbcmac_golden_tag[16] = {0xA7, 0x35, 0x6E, 0x12, 0x07, 0xBB, 0x40, 0x66, 0x39, 0xE5, 0xE5, 0xCE, 0xB9, 0xA9, 0xED, 0x93};

    STATIC_DMA uint8_t cbcmac_input_le[64];
    wise_swap_be32_to_le32(cbcmac_input_le, cbcmac_input, sizeof(cbcmac_input));
    STATIC_DMA uint8_t cbcmac_golden_tag_le[16];
    wise_swap_be32_to_le32(cbcmac_golden_tag_le, cbcmac_golden_tag, sizeof(cbcmac_golden_tag));

    STATIC_DMA uint32_t tag_u32[4] = {0};
    uint8_t *tag        = (uint8_t *)tag_u32;

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size  = AES_KEY_128,
        .key_src   = AES_KEY_SRC_FROM_USER,
        .key_num   = 0,
        .key_bytes = aes_key,
        .swap_mode = AES_SWAP_NONE,
        .iv_or_cnt = aes_iv,
    };

    WISE_AES_CONFIG_CTX_T ctx = {
        .aes_mode      = AES_MODE_CBC_MAC,
        .aes_direction = AES_ENCRYPT,
        .data_cfg =
            {
                .input     = cbcmac_input_le,
                .output    = tag,
                .length    = sizeof(cbcmac_input_le),
                .iv_or_cnt = aes_iv,
            },
    };

    if (is_generate) {
        status = wise_aes_key_config(&key_cfg);
        if (status != WISE_SUCCESS) {
            printf("AES key config failed\n");
            wise_crypto_deinit();
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        status = wise_aes_exec(&ctx);
        if (status != WISE_SUCCESS) {
            printf("AES CBC-MAC generation failed\n");
            wise_crypto_deinit();
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    if (is_checktag) {
        status = wise_aes_read_tags(tag_u32);
        if (status != WISE_SUCCESS) {
            printf("Failed to read AES tag\n");
            wise_crypto_deinit();
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    if (is_generate) {
        print_bytes_as_words("AES Key", aes_key, sizeof(aes_key));
        print_bytes_as_words("IV", aes_iv, sizeof(aes_iv));
        print_bytes_as_words("CBCMAC_INPUT", cbcmac_input, sizeof(cbcmac_input));
    }

    print_bytes_as_words("CBCMAC_OUTPUT", tag, sizeof(cbcmac_golden_tag));

    int result = check_ciphertext(tag, cbcmac_golden_tag_le, sizeof(cbcmac_golden_tag_le));
    printf("\nAES CBC-MAC %s %s!\n", is_checktag ? "TAG Check" : "TAG Generate", result ? "PASS" : "FAIL");

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif

#if 0
static int _aes_ccm_Cmd(int argc, char **argv)
{
    int decrypto_result = WISE_FAIL;
    if (argc < 3) {
        printf("Usage: crypto aes_ccm [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: crypto aes_ccm [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    const uint8_t aes_key[16] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};

    const uint8_t aes_nonce[8] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

    const uint8_t auth_data[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    const uint8_t plaintext[16] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};

    const uint8_t golden_ciphertext[16 + 6 + 2] = {
        // 16 bytes ciphertext + 6 bytes authentication tag + 2 bytes alignment/padding
        0xD2, 0xA1, 0xF0, 0xE0, // Chipertext
        0x51, 0xEA, 0x5F, 0x62, // Chipertext
        0x08, 0x1A, 0x77, 0x92, // Chipertext
        0x07, 0x3D, 0x59, 0x3D, // Chipertext
        0x1F, 0xC6, 0x4F, 0xBF, // TAG
        0xAC, 0xCD, 0x00, 0x00  // TAG
    };

    uint32_t payload_len = sizeof(plaintext);
    uint32_t tag_len     = 6;

    uint8_t input_buf[sizeof(golden_ciphertext)]  = {0};
    uint8_t output_buf[sizeof(golden_ciphertext)] = {0};
    uint8_t auth_data_le[16];
    wise_swap_be32_to_le32(auth_data_le, auth_data, sizeof(auth_data));
    uint8_t plaintext_le[16];
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    uint8_t golden_ciphertext_le[24];
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *golden_data = is_encrypt ? golden_ciphertext_le : plaintext_le;
    if (is_encrypt) {
        memcpy(input_buf, plaintext_le, payload_len);
    } else {
        memcpy(input_buf, golden_ciphertext_le, payload_len + tag_len + 2);
    }

    WISE_AES_KEY_CONFIG_T key_cfg = {
        .key_size  = AES_KEY_128,
        .key_src   = AES_KEY_SRC_FROM_USER,
        .key_num   = 0,
        .key_bytes = aes_key,
        .swap_mode = AES_SWAP_NONE,
    };

    WISE_AES_CCM_EXTRA_T ccm_cfg = {
        .nonce     = aes_nonce,
        .nonce_len = sizeof(aes_nonce),
        .tag_len   = tag_len,
        .auth_data = auth_data_le,
        .auth_len  = sizeof(auth_data),
    };

    WISE_AES_DATA_T data_cfg = {
        .input  = input_buf,
        .output = output_buf,
        .length = is_encrypt ? payload_len : (payload_len + tag_len),
    };

    status = is_encrypt ? wise_aes_ccm_encrypt(&key_cfg, &data_cfg, &ccm_cfg) : wise_aes_ccm_decrypt(&key_cfg, &data_cfg, &ccm_cfg);

    if (status != WISE_SUCCESS) {
        printf("AES CCM %s failed\n", is_encrypt ? "encryption" : "decryption");
        wise_crypto_deinit();
        ;
        //return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        decrypto_result = WISE_SUCCESS;
    }

    // Dump info
    print_bytes_as_words("AES Key", (const uint8_t *)aes_key, sizeof(aes_key));
    print_bytes_as_words("Nonce", aes_nonce, sizeof(aes_nonce));
    print_bytes_as_words("AAD", (const uint8_t *)auth_data, sizeof(auth_data));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext+Tag", input_buf, is_encrypt ? payload_len : (payload_len + tag_len + 2));
    print_bytes_as_words(is_encrypt ? "Ciphertext+Tag" : "Plaintext", output_buf, is_encrypt ? (payload_len + tag_len) : payload_len);

    // Compare with golden
    int result = check_ciphertext(output_buf, golden_data, is_encrypt ? (payload_len + tag_len) : payload_len);

    const char *mode_str   = is_encrypt ? "encryption" : "decryption";
    const char *result_str = "FAIL";

    if (decrypto_result == WISE_SUCCESS) {
        result_str = result ? "PASS" : "FAIL";
    }

    printf("\nAES CCM %s %s!\n", mode_str, result_str);

    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#else
static int _aes_ccm_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: crypto aes_ccm [enc|dec]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_encrypt = (strcmp(argv[2], "enc") == 0);
    bool is_decrypt = (strcmp(argv[2], "dec") == 0);
    if (!is_encrypt && !is_decrypt) {
        printf("Invalid mode: %s\nUsage: crypto aes_ccm [enc|dec]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (wise_crypto_init() != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t aes_key[16]           = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
    STATIC_DMA_DATA uint8_t aes_nonce[8]          = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    STATIC_DMA_DATA uint8_t auth_data[16]         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    STATIC_DMA_DATA uint8_t plaintext[16]         = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};
    STATIC_DMA_DATA uint8_t golden_ciphertext[24] = {0xD2, 0xA1, 0xF0, 0xE0, 0x51, 0xEA, 0x5F, 0x62, 0x08, 0x1A, 0x77, 0x92,
                                           0x07, 0x3D, 0x59, 0x3D, 0x1F, 0xC6, 0x4F, 0xBF, 0xAC, 0xCD, 0x00, 0x00};

    STATIC_DMA_DATA uint32_t payload_len = sizeof(plaintext);
    STATIC_DMA_DATA uint32_t tag_len     = 6;

    STATIC_DMA uint8_t input_buf[sizeof(golden_ciphertext)]  = {0};
    STATIC_DMA uint8_t output_buf[sizeof(golden_ciphertext)] = {0};
    STATIC_DMA uint8_t auth_data_le[16];
    STATIC_DMA uint8_t plaintext_le[16];
    STATIC_DMA uint8_t golden_ciphertext_le[24];
    wise_swap_be32_to_le32(auth_data_le, auth_data, sizeof(auth_data));
    wise_swap_be32_to_le32(plaintext_le, plaintext, sizeof(plaintext));
    wise_swap_be32_to_le32(golden_ciphertext_le, golden_ciphertext, sizeof(golden_ciphertext));

    const uint8_t *golden_data = is_encrypt ? golden_ciphertext_le : plaintext_le;

    if (is_encrypt) {
        memcpy(input_buf, plaintext_le, payload_len);
    } else {
        memcpy(input_buf, golden_ciphertext_le, payload_len + tag_len + 2);
    }

    WISE_AES_CCM_CTX_T ctx = {
        .key_cfg =
            {
                .key_size  = AES_KEY_128,
                .key_src   = AES_KEY_SRC_FROM_USER,
                .key_bytes = aes_key,
                .key_num   = 0,
                .swap_mode = AES_SWAP_NONE,
            },
        .ccm_cfg =
            {
                .nonce     = aes_nonce,
                .nonce_len = sizeof(aes_nonce),
                .tag_len   = tag_len,
                 .auth_data = auth_data_le,
                .auth_len  = sizeof(auth_data),
            },
        .is_encrypt = is_encrypt,
    };

    if (wise_aes_ccm_configure(&ctx) != WISE_SUCCESS) {
        printf("CCM configure failed\n");
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_AES_DATA_T data_cfg = {
        .input  = input_buf,
        .output = output_buf,
        .length = is_encrypt ? payload_len : (payload_len + tag_len),
    };

    WISE_STATUS status = wise_aes_ccm_exec(&ctx, &data_cfg);

    if (status != WISE_SUCCESS) {
        printf("AES CCM %s failed\n", is_encrypt ? "encryption" : "decryption");
    }

    // Dump info
    print_bytes_as_words("AES Key", aes_key, sizeof(aes_key));
    print_bytes_as_words("Nonce", aes_nonce, sizeof(aes_nonce));
    print_bytes_as_words("AAD", auth_data, sizeof(auth_data));
    print_bytes_as_words(is_encrypt ? "Plaintext" : "Ciphertext+Tag", input_buf, data_cfg.length);
    print_bytes_as_words(is_encrypt ? "Ciphertext+Tag" : "Plaintext", output_buf, is_encrypt ? (payload_len + tag_len) : payload_len);

    const char *mode_str   = is_encrypt ? "encryption" : "decryption";
    const char *result_str = "FAIL";
    if (status == WISE_SUCCESS) {
        int ok     = check_ciphertext(output_buf, golden_data, is_encrypt ? (payload_len + tag_len) : payload_len);
        result_str = ok ? "PASS" : "FAIL";
    }

    printf("\nAES CCM %s %s!\n", mode_str, result_str);
    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif
static int _sha_generate_Cmd(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: sha [224|256]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    E_SHA_MODE_T mode;
    if (strcmp(argv[2], "224") == 0) {
        mode = E_SHA_MODE_224;
    } else if (strcmp(argv[2], "256") == 0) {
        mode = E_SHA_MODE_256;
    } else {
        printf("Invalid SHA mode: %s\nUsage: sha [224|256]\n", argv[2]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    WISE_STATUS status = wise_crypto_init();
    if (status != WISE_SUCCESS) {
        printf("Crypto init failed\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    STATIC_DMA_DATA uint8_t plaintext[] = {
        0x61, 0x62, 0x63, // "abc"
        0x64, 0x65, 0x66, // "def"
        0x67, 0x68, 0x69  // "ghi"
    };
    const uint32_t dataAddr = (uint32_t)(uintptr_t)plaintext;
    const uint32_t dataLen  = sizeof(plaintext);

    STATIC_DMA uint8_t outputSha[32] = {0}; // SHA-224=28 bytes, SHA-256=32 bytes

    status = wise_sha_generate(mode, dataAddr, dataLen, outputSha);
    if (status != WISE_SUCCESS) {
        printf("SHA-%u generation failed\n", (mode == E_SHA_MODE_224) ? 224 : 256);
        wise_crypto_deinit();
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    print_bytes_as_words("Input", plaintext, dataLen);
    print_bytes_as_words((mode == E_SHA_MODE_224) ? "SHA-224" : "SHA-256", outputSha, (mode == E_SHA_MODE_224) ? 28 : 32);
    wise_crypto_deinit();
    return SHLCMD_HIST | SHLCMD_DONE;
}
#endif
