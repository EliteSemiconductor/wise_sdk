/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_EFUSE
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "api/wise_efuse_api.h"
#include "cmd_efuse.h"

static int _efuseInitCmd(int argc, char **argv);
static int _readCmd(int argc, char **argv);
static int _writeCmd(int argc, char **argv);


static const struct shellCommand m_EFUSECmdlist[] = {
    {"help", "efuse function", _helpCmd},
    {"init", "init efuse system", _efuseInitCmd},
    {"read", "read efuse address", _readCmd},
    {"write", "write efuse address", _writeCmd},
};

int EFUSECommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_EFUSECmdlist, (sizeof(m_EFUSECmdlist) / sizeof(struct shellCommand)));
}

static int _efuseInitCmd(int argc, char **argv)
{
    wise_efuse_init();

    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _readCmd(int argc, char **argv)
{
    uint32_t addr;
    uint32_t val;

    if (argc < 4) {
        goto error;
    }

    addr = strtoul(argv[2], NULL, 16);

    if (strcmp(argv[2], "mcu") == 0) {
        val = wise_efuse_mcu_read_word(addr);
    } else if (strcmp(argv[2], "nfc") == 0) {
        val = wise_efuse_nfc_read_word(addr);
    } else {
        goto error;
    }

    debug_print("r efuse [0x%lx] = 0x%08lx\n", addr, val);

    return SHLCMD_HIST | SHLCMD_DONE;

error:
    printf("Usage: efuse [mcu/nfc] read [addr(HEX)]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _writeCmd(int argc, char **argv)
{
    uint32_t addr;
    uint32_t val;

    if (argc < 5) {
        goto error;
    }

    addr = strtoul(argv[2], NULL, 16);
    val = strtoul(argv[2], NULL, 16);

    if (strcmp(argv[2], "mcu") == 0) {
        wise_efuse_mcu_write_word(addr, val);
        debug_print("w efuse [0x%lx] = [0x%08lx]\n", addr, wise_efuse_mcu_read_word(addr));
    } else if (strcmp(argv[2], "nfc") == 0) {
        wise_efuse_nfc_write_word(addr, val);
        debug_print("w efuse [0x%lx] = [0x%08lx]\n", addr, wise_efuse_mcu_read_word(addr));
    } else {
        goto error;
    }

    
    
    return SHLCMD_HIST | SHLCMD_DONE;
    
error:
    printf("Usage: efuse [mcu/nfc] write [addr(HEX)] [value(HEX)]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif /* SHELL_NFC */
