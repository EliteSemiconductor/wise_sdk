/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "shell_func.h"


bool parse_u32_dec_hex(const char *s, uint32_t *out)
{
    if (!s || !out) return false;

    uint32_t val = 0;
    int base = 10;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        base = 16;
        s += 2;
    }

    if (*s == '\0') {
        return false;
    }

    while (*s) {
        uint8_t c = (uint8_t)*s++;
        uint8_t digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (base == 16 && c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (base == 16 && c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            return false;
        }

        if (val > (0xFFFFFFFFu - digit) / base)
            return false;

        val = val * base + digit;
    }

    *out = val;
    return true;
}

bool parse_u16_dec_hex(const char *s, uint16_t *out)
{
    uint32_t tmp;
    if (!parse_u32_dec_hex(s, &tmp) || tmp > 0xFFFFu) return false;
    *out = (uint16_t)tmp;
    return true;
}

