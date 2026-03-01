/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_efuse_api.h"

void wise_efuse_init(void)
{
    hal_intf_efuse_init();
}

#define EFUSE_MAX_READ_BYTES (64u)
static inline bool _is_para_valid(uint32_t addr, const void *buf, uint32_t byte_len)
{
    bool valid  = true;

    if ((addr & 0x3u) != 0u) {
        valid = false;
        debug_print("Address must be 4-byte aligned.\n");
    }

    if (buf == NULL) {
        valid = false;
        debug_print("Buffer is NULL.\n");
    }

    if (byte_len == 0u) {
        valid = false;
        debug_print("Length must be non-zero.\n");
    }

    if ((byte_len & 0x3u) != 0u) {
        valid = false;
        debug_print("Length must be a multiple of 4 bytes.\n");
    }

    if (byte_len > EFUSE_MAX_READ_BYTES) {
        valid = false;
        debug_print("Length cannot exceed %lu bytes.\n", (unsigned long)EFUSE_MAX_READ_BYTES);
    }

    return valid;
}

WISE_STATUS wise_efuse_read(uint32_t addr, uint8_t *buf, uint32_t byte_len)
{
    EFUSE_CONF_S efuse_conf = {0};
    uint32_t i;
    const uint32_t word_cnt = (byte_len >> 2u);

    if(!_is_para_valid(addr, buf, byte_len)) {
        return WISE_FAIL;
    }

    efuse_conf.base_idx = EFUSE_BASE_NFC;

    for (i = 0; i < word_cnt; i++) {
        efuse_conf.addr = addr + (i << 2u);
        efuse_conf.buf = &buf[i << 2u];

        if (hal_intf_efuse_read(&efuse_conf) != WISE_SUCCESS) {
            debug_print("efuse read fail: addr=0x%08lx, i=%lu\n", (unsigned long)efuse_conf.addr, (unsigned long)i);
            return WISE_FAIL;
        }
    }

    return WISE_SUCCESS;
}

WISE_STATUS wise_efuse_write(uint32_t addr, uint8_t *buf, uint32_t byte_len)
{
    EFUSE_CONF_S efuse_conf = {0};
    uint32_t i;
    const uint32_t word_cnt = (byte_len >> 2u);
    
    if (!_is_para_valid(addr, buf, byte_len)) {
        return WISE_FAIL;
    }
    
    efuse_conf.base_idx = EFUSE_BASE_NFC;

    for (i = 0; i < word_cnt; i++) {
        efuse_conf.addr = addr + (i << 2u);
        efuse_conf.buf = &buf[i << 2u];     

        if (hal_intf_efuse_write(&efuse_conf) != WISE_SUCCESS) {
            debug_print("efuse write fail: addr=0x%08lx, val=0x%08lx, i=%lu\n", 
                        (unsigned long)efuse_conf.addr, 
                        (unsigned long)buf[i], 
                        (unsigned long)i);
            return WISE_FAIL;
        }
    }
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_efuse_boot_cfg_read(uint32_t addr, uint8_t *buf, uint32_t byte_len)
{
    EFUSE_CONF_S efuse_conf = {0};
    uint32_t i;
    const uint32_t word_cnt = (byte_len >> 2u);

    if(!_is_para_valid(addr, buf, byte_len)) {
        return WISE_FAIL;
    }

    efuse_conf.base_idx = EFUSE_BASE_MCU;

    for (i = 0; i < word_cnt; i++) {
        efuse_conf.addr = addr + (i << 2u);
        efuse_conf.buf = &buf[i << 2u];

        if (hal_intf_efuse_read(&efuse_conf) != WISE_SUCCESS) {
            debug_print("efuse read fail: addr=0x%08lx, i=%lu\n", (unsigned long)efuse_conf.addr, (unsigned long)i);
            return WISE_FAIL;
        }
    }

    return WISE_SUCCESS;
}

WISE_STATUS wise_efuse_boot_cfg_write(uint32_t addr, uint8_t *buf, uint32_t byte_len)
{
    EFUSE_CONF_S efuse_conf = {0};
    uint32_t i;
    const uint32_t word_cnt = (byte_len >> 2u);
    
    if (!_is_para_valid(addr, buf, byte_len)) {
        return WISE_FAIL;
    }
    
    efuse_conf.base_idx = EFUSE_BASE_MCU;

    for (i = 0; i < word_cnt; i++) {
        efuse_conf.addr = addr + (i << 2u);
        efuse_conf.buf = &buf[i << 2u];     

        if (hal_intf_efuse_write(&efuse_conf) != WISE_SUCCESS) {
            debug_print("efuse write fail: addr=0x%08lx, val=0x%08lx, i=%lu\n", 
                        (unsigned long)efuse_conf.addr, 
                        (unsigned long)buf[i], 
                        (unsigned long)i);
            return WISE_FAIL;
        }
    }
    
    return WISE_SUCCESS;
}

void wise_efuse_get_chip_unique(uint8_t* id_arr)
{
    hal_intf_efuse_get_chip_unique(id_arr);
}
