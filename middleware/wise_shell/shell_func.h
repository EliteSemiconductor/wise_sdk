/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __SHELL_FUNC_H_
#define __SHELL_FUNC_H_

#include <stdint.h>
#include <stdbool.h>

bool parse_u32_dec_hex(const char *s, uint32_t *out);
bool parse_u16_dec_hex(const char *s, uint16_t *out);

#endif
