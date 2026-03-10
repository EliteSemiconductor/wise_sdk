/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <errno.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_tick_api.h"
#include "util.h"
#include "wise_trng_api.h"
#include "demo_app_common.h"

#define DEMO_APP_PROMPT             "TRNG> "
#define TRNG_TMP_BUF_LEN            256

static int cmd_trng_gen(int argc, char **argv)
{
    uint32_t dataLen = 0;
    uint8_t* tempBuf = NULL;

    if(argc > 2)
        goto print_usage;
    
    if(argc == 2) 
    {
        dataLen = strtoul(argv[1], NULL, 10);
        if (errno != 0) 
        {
            goto print_usage;
        }
    }

    if(dataLen == 0)
        dataLen = 16;

    tempBuf = (uint8_t*)malloc(TRNG_TMP_BUF_LEN);

    debug_print("TRNG generated:\n");
    while(dataLen)
    {
        uint32_t blockLen = dataLen > TRNG_TMP_BUF_LEN ? TRNG_TMP_BUF_LEN : dataLen;
        
        wise_trng_generate(tempBuf, blockLen);
        dump_buffer(tempBuf, blockLen);

        dataLen -= blockLen;
    }
    
    free(tempBuf);
    
    return 0;

print_usage:
    debug_print("Invalid parameters:\n");
    debug_print("Usage: gen [length_dec]\n");
    return -1;
}
SHELL_CMD_AUTO(gen, cmd_trng_gen, "generate N bytes of random data");


void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_trng_init();
    wise_trng_enable();

    while (1) 
    {
    	wise_main_proc();
    }
}
