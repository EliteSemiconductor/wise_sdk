/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_trng_api.h"

static uint8_t trngBuffer[TRNG_BLOCK_SIZE];
static uint8_t readIndex    = 0;
static uint8_t validDataLen = 0;

void wise_trng_init(void)
{
    memset(&trngBuffer[0], 0, TRNG_BLOCK_SIZE);
}

void wise_trng_enable(void)
{
    readIndex    = 0;
    validDataLen = 0;

    hal_intf_module_clk_enable(TRNG_MODULE);
    hal_intf_trng_config();

    hal_intf_trng_start();
}

void wise_trng_disable(void)
{
    hal_intf_module_clk_disable(TRNG_MODULE);
}

void wise_trng_generate(uint8_t *buffer, uint32_t len)
{
    while (len) {
        uint32_t blockLen = 0; // len > TRNG_BLOCK_SIZE ? TRNG_BLOCK_SIZE : len;

        if (validDataLen == 0) {
            validDataLen = hal_intf_trng_get_ehr_data((uint32_t *)trngBuffer);
            //wise_tick_delay_ms(5); // kevinyang, 20240902, necessary workaround?

            // debug_print("gen:\n");
            // dump_buffer(trngBuffer, TRNG_BLOCK_SIZE);

            readIndex    = 0;
            //validDataLen = TRNG_BLOCK_SIZE;
        }

        if (validDataLen > 0) {
            blockLen = len > validDataLen ? validDataLen : len;
            memcpy(buffer, &trngBuffer[readIndex], blockLen);

            buffer       += blockLen;
            len          -= blockLen;
            readIndex    += blockLen;
            validDataLen -= blockLen;
        }
    }
}
