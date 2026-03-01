/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdint.h>
#include "esmt_chip_specific.h"
#include "hdl/sha_er8130.h"

static inline void _sha_set_length(uint64_t len);
static inline void _sha_trigger_dma(void);
static inline void _sha_wait_finished(void);

void sha_set_config_er8130(uint8_t mode, uint8_t swap)
{
    uint32_t reg;

    reg  = REG_R32(SHA_CTL_ADDR) & ~(SHA_SHA_MODE_MASK + SHA_SHA_IN_SWAP_MASK);
    reg |= (((mode << SHA_SHA_MODE_POS) & SHA_SHA_MODE_MASK) | ((swap << SHA_SHA_IN_SWAP_POS) & SHA_SHA_IN_SWAP_MASK));
    REG_W32(SHA_CTL_ADDR, reg);
}

static inline void _sha_set_length(uint64_t len)
{
    //	if(len <= UINT32_MAX)
    //		SHA_W32(SHA_IN_LEN_0_ADDR, (uint32_t)len);
    //	else
    //		SHA_W32(SHA_IN_LEN_1_ADDR, (uint32_t)(len >> 32));

    REG_W32(SHA_IN_LEN_0_ADDR, (uint32_t)((len <= UINT32_MAX) ? (len) : (len >> 32)));
}

static inline void _sha_set_interrupt(void)
{
    REG_W32(SHA_INT_EN_ADDR, SHA_INT_EN_FSH_MASK);
}

static inline void _sha_trigger_dma(void)
{
    REG_W32(SHA_DMA_ADDR, SHA_W1T_SHA_DMA_DATA_MASK);
}

static inline void _sha_wait_finished(void)
{
    while (!(REG_R32(SHA_INT_ST_ADDR) & SHA_INT_ST_FSH_MASK)) {
        ;
    }

    REG_W32(SHA_INT_ST_ADDR, SHA_INT_ST_FSH_MASK);
}

void sha_get_digest_er8130(uint32_t *digest, uint64_t req_len)
{
    uint8_t out;

    _sha_set_length(req_len);
    _sha_set_interrupt();
    _sha_trigger_dma();
    _sha_wait_finished();
    for (out = 0; out < SHA_DIGEST_NUM; out++) {
        digest[out] = REG_R32(SHA_DIGEST_OUT_0_ADDR + (out * 4));
    }
}
