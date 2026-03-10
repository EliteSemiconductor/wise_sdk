/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#include "app_shell_cmd_trng.h"

uint8_t trngInited = 0;

static int _trngInitCmd(int argc, char **argv);
static int _trngGenCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "TRNG help function", _helpCmd},
    {"init", "TRNG init function", _trngInitCmd},
    {"gen", "TRNG generate function", _trngGenCmd},
};

int TRNGCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

static int _trngInitCmd(int argc, char **argv)
{
    if (argc != 2) {
        goto invalid;
    }

    if (trngInited == 0) {
        wise_trng_init();
        wise_trng_enable();
        trngInited = 1;

        printf("TRNG inited.\n");
    } else {
        printf("TRNG already inited\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage : trng init\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _trngGenCmd(int argc, char **argv)
{
#define TRNG_TMP_BUF_LEN 64
#define OUTPUT_ARRAY_FORMAT
    int genLen      = 0;
    uint8_t *tmpBuf = NULL;
    if (argc != 3) {
        goto invalid;
    }

    genLen = strtoul(argv[2], NULL, 10);
    if (genLen >= 0) {
        int segLen = 0;
        int i;

        tmpBuf = malloc(TRNG_TMP_BUF_LEN);

#ifdef OUTPUT_ARRAY_FORMAT
        printf("unsigned char trngData[] = {\n");
#endif

        while (genLen > 0) {
            if (genLen >= TRNG_TMP_BUF_LEN) {
                segLen = TRNG_TMP_BUF_LEN;
            } else {
                segLen = genLen;
            }

            memset(tmpBuf, 0, TRNG_TMP_BUF_LEN);

            wise_trng_generate(tmpBuf, segLen);
            for (i = 0; i < segLen; i++) {
#ifdef OUTPUT_ARRAY_FORMAT
                printf("0x%02X, ", tmpBuf[i]);
                if ((i & 0xf) == 0xf) {
                    printf("\n");
                }
#else
                printf("%02X", tmpBuf[i]);
                if ((i & 0x1f) == 0x1f) {
                    printf("\n");
                }
#endif
            }

            genLen -= segLen;
        }

        printf("\n");

#ifdef OUTPUT_ARRAY_FORMAT
        printf("};\n");
#endif

        free(tmpBuf);
    } else {
        goto invalid;
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage : trng gen [num_bytes]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}