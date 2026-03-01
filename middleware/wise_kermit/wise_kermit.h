/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_kermit.h
 * @brief wise_kermit.h middleware APIs
 *
 * @ingroup WISE_MIDDLEWARE
 *
 * This header belongs to the Middleware layer of the WISE SDK and provides
 * middleware-level interfaces built on top of the Core APIs.
 */


#ifndef __WISE_KERMIT_H
#define __WISE_KERMIT_H

#include "wise.h"

enum {
    E_KERMIT_TARGET_FS = 0,
    E_KERMIT_TARGET_FLASH,
    E_KERMIT_TARGET_RAM,
};

int32_t wise_kermit_init(int8_t target, uint32_t param, int8_t uartIntf);
int32_t wise_kermit_load(uint32_t *rxDataLen, uint16_t *rxDataCrc);

#endif
