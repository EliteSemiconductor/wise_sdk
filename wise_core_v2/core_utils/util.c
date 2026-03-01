/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "util_debug_log.h"

void print_array(int arr[], int n)
{
    for (int i = 0; i < n; i++) {
        debug_printf("[%d] = 0x%08x \n", i, arr[i]);
    }
    debug_printf("\n");
}
void dump_buffer(unsigned char *buffer, int len)
{
    uint16_t offset = 0;
    while (len > 0) {
        int lineLen = (len > 16) ? 16 : len;
        printf("%04x: ", offset);

        for (int i = 0; i < lineLen; i++) {
            printf("%02x ", buffer[offset + i]);
            if (i == 7) {
                printf("  ");
            }
        }

        printf("\n");
        offset += lineLen;
        len    -= lineLen;
    }
    printf("\n");
    fflush(stdout);
}
#if 0
void dump_buffer(unsigned char *buffer, int len)
{
    uint16_t lineLen;
    volatile unsigned char *ptr = buffer;
    uint16_t lineIndex          = 0;

    while (len) {
        uint16_t i;

        if (len > 16) {
            lineLen = 16;
        } else {
            lineLen = len;
        }

        printf("%04x: ", lineIndex << 4);
        for (i = 0; i < lineLen; i++) {
            printf("%02x ", *ptr);
            if (i == 7) {
                printf("   ");
            }

            ptr++;
        }
        printf("\n");

        len -= lineLen;
        lineIndex++;
    }

    printf("\n");

    fflush(stdout);
}
#endif
uint32_t bit_reverse(uint32_t val, uint8_t bytes)
{
    uint32_t outputVal = 0;
    uint8_t bits       = (bytes << 3);
    int i;

    if ((val != 0) && (bytes != 0)) {
        for (i = 0; i < bits; i++) {
            outputVal |= (((val >> (bits - i - 1)) & 1) << i);
        }
    }

    return outputVal;
}

static unsigned short crc16_kermit_table[256] = {
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7, 0x1081, 0x0108,
    0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876, 0x2102, 0x308B, 0x0210, 0x1399,
    0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5, 0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E,
    0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974, 0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
    0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3, 0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44,
    0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72, 0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5,
    0xA96A, 0xB8E3, 0x8A78, 0x9BF1, 0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862,
    0x9AF9, 0x8B70, 0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E, 0xA50A, 0xB483,
    0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD, 0xB58B, 0xA402, 0x9699, 0x8710,
    0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C, 0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1,
    0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB, 0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
    0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A, 0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF,
    0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9, 0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C,
    0x3DE3, 0x2C6A, 0x1EF1, 0x0F78};

uint16_t crc16_kermit(uint8_t *buf, int length, uint16_t startVal)
{
    uint16_t crc = startVal;

    for (int i = 0; i < length; ++i) {
        uint8_t index = (uint8_t)(crc ^ buf[i]);
        crc           = (uint16_t)((crc >> 8) ^ crc16_kermit_table[index]);
    }
    return crc;
}

#if 0
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;

void init_rand(uint32_t x)
{
    int i;

    Q[0] = x;
    Q[1] = x + PHI;
    Q[2] = x + PHI + PHI;

    for (i = 3; i < 4096; i++)
            Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t rand_cmwc(void)
{
    uint64_t t, a = 18782LL;
    static uint32_t i = 4095;
    uint32_t x, r = 0xfffffffe;
    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;
    if (x < c) {
            x++;
            c++;
    }
    return (Q[i] = r - x);
}
#else
static uint32_t randSeed = 0;
void init_rand(uint32_t x)
{
    randSeed = x;
}

uint32_t rand_cmwc(void)
{
    randSeed = (214013 * randSeed + 2531011);
    return (randSeed >> 16) & 0x7FFF;
}

#endif

void wise_swap_be32_to_le32(uint8_t *dst, const uint8_t *src, uint32_t byte_len)
{
    uint32_t i = 0;

    while (i < byte_len) {
        uint8_t tmp[4] = {0};

        for (int j = 0; j < 4 && (i + j) < byte_len; j++) {
            tmp[j] = src[i + j];
        }

        uint32_t val = LOAD_BE_32(tmp);

        dst[i + 0] = (val >> 0) & 0xFF;
        dst[i + 1] = (val >> 8) & 0xFF;
        dst[i + 2] = (val >> 16) & 0xFF;
        dst[i + 3] = (val >> 24) & 0xFF;

        i += 4;
    }
}
