/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hdl/trng_er8130.h"

void trng_sw_rst_er8130(void)
{
    REG_W32(TRNG_SW_RST_ADDR, TRNG_RNG_SW_RESET_MASK);
}

uint8_t trng_get_busy_er8130(void)
{
    return ((REG_R32(TRNG_BUSY_ADDR) & TRNG_BSY_MASK) >> TRNG_BSY_POS);
}

uint32_t trng_get_status_er8130(void)
{
    return REG_R32(TRNG_ISR_ADDR);
}

void trng_clear_status_er8130(uint32_t status)
{
    REG_W32(TRNG_ICR_ADDR, status);
}

void trng_config_er8130(void)
{
    uint32_t reg;

    reg = ((TRNG_ENTROPY_INVERTER_SHORTEST << TRNG_RND_SRC_SEL_POS) & TRNG_RND_SRC_SEL_MASK);
    REG_W32(TRNG_CONFIG_ADDR, reg);
    REG_W32(TRNG_SAMPLE_CNT1_ADDR, TRNG_SAMPLE_CNT);
}

void trng_start_er8130(void)
{
    REG_W32(TRNG_RND_SRC_EN_ADDR, ((0x01 << TRNG_RND_SRC_EN_POS) & TRNG_RND_SRC_EN_MASK));
}

uint8_t trng_get_ehr_data_er8130(uint32_t *ehr_buf)
{
    uint8_t i = 0;

    if (REG_R32(TRNG_VALID_ADDR) & TRNG_VALID_EHR_VLD_MASK) {
        for (i = 0; i < TRNG_EHR_DATA_NUM; i++) {
            ehr_buf[i] = REG_R32(TRNG_EHR_DATA0_ADDR + (i * sizeof(uint32_t)));
        }
        return 24;
    }

    return 0;
}
