/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __SHA_ER8130_H
#define __SHA_ER8130_H

#include "esmt_chip_specific.h"
#include "types.h"
#include <stdint.h>

#define SHA_DIGEST_NUM                                  8

void sha_set_config_er8130(uint8_t mode, uint8_t swap);
void sha_get_digest_er8130(uint32_t *digest, uint64_t reqLen);

#endif

