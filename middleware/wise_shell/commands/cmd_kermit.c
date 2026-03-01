#include "es_platform_components.h"

#if (defined SHELL_KERMIT) && (SHELL_KERMIT == 1)
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "wise.h"
#include "wise_file_system.h"
#include "wise_kermit.h"
#include "cmd_kermit.h"

int KermitCommandHandler(int argc, char **argv)
{
    uint32_t rxDataLen = 0;
    uint16_t rxDataCrc = 0;
    uint32_t param     = 0;
    uint8_t target     = E_KERMIT_TARGET_FS;

    if (argc != 3) {
        goto help;
    }

    if (0 == strcmp(argv[1], "fs")) {
        target = E_KERMIT_TARGET_FS;
    } else if (0 == strcmp(argv[1], "flash")) {
        target = E_KERMIT_TARGET_FLASH;
    } else if (0 == strcmp(argv[1], "ram")) {
        target = E_KERMIT_TARGET_RAM;
    } else {
        goto help;
    }

    param = strtoul(argv[2], NULL, 16);

    switch (target) {
    case E_KERMIT_TARGET_FS: {
        WISE_FS_INFO_T fsInfo;

        wise_fs_get_fs_info(&fsInfo);
        if ((fsInfo.storageSize == 0) || (fsInfo.eraseSize == 0)) {
            shell_output("Invalid flash info\n");
            goto finish;
        }

        if ((param >= 0) && (param < fsInfo.parNum)) {
            if (param == 0) {
                shell_output("AppLoader cannot be updapted\n");
                goto finish;
            }
        } else {
            shell_output("Invalid parition index\n");
        }
    } break;

    case E_KERMIT_TARGET_FLASH: {
        WISE_FLASH_INFO_T flashInfo;

        wise_flash_get_info(&flashInfo);

        if (param >= flashInfo.flashSize) {
            shell_output("out of flash range\n");
            goto help;
        }
    } break;

    case E_KERMIT_TARGET_RAM:

        break;
    }

    if (WISE_SUCCESS == wise_kermit_init(target, param, STDIO_UART_PORT)) {
        shell_output("Start kermit receiver\n");
        wise_kermit_load(&rxDataLen, &rxDataCrc);

        if (target == E_KERMIT_TARGET_FS) {
            if (WISE_SUCCESS ==
                wise_fs_par_update_info(param, rxDataLen, rxDataCrc)) {
                shell_output("partition %d update finished\n", param);
            } else {
                shell_output("CRC error\n");
            }
        }
    } else {
        shell_output("Failed to start kermit\n");
    }

finish:
    return SHLCMD_HIST | SHLCMD_DONE;

help:
    shell_output(INVALID_PARAM_MESSAGE);
    shell_output("  kermit [fs/flash/ram] [partition/flashAddr/ramAddr]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif
