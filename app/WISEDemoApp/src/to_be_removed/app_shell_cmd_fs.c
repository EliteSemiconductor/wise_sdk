/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include "wise_core.h"
#include "app_shell_cmd_fs.h"
#include "wise_file_system.h"

uint8_t cmdTmpBuf[256];

static int _formatCmd(int argc, char **argv);
static int _infoCmd(int argc, char **argv);
static int _dumpCmd(int argc, char **argv);
static int _userCmd(int argc, char **argv);

static const struct shellCommand m_fsCmdList[] = {
    {"help", "file system function", _helpCmd},
    {"info", "file system info", _infoCmd},
    {"dump", "file system dump", _dumpCmd},
    {"user", "file system user cmd", _userCmd},
};

int FileSystemCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_fsCmdList,
                                sizeof(m_fsCmdList) /
                                    sizeof(struct shellCommand));
}

static int _infoCmd(int argc, char **argv)
{
    if (argc == 2) {
        wise_fs_debug_info();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        CLI_PRINTF("input error!\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}

static int _dumpCmd(int argc, char **argv)
{
    uint32_t offset;
    uint32_t size;
    uint32_t parIdx = 0;

    if (argc == 5) {
        WISE_FS_INFO_T fsInfo;

        parIdx = strtoul(argv[2], NULL, 16);
        offset = strtoul(argv[3], NULL, 16);
        size   = strtoul(argv[4], NULL, 16);

        wise_fs_get_fs_info(&fsInfo);

        if (parIdx >= fsInfo.parNum) {
            CLI_PRINTF("Invalid partition index\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        offset = fsInfo.partitions[parIdx].offset + offset;

        while (size) {
            uint32_t readLen = size > 256 ? 256 : size;

            wise_flash_read(offset, &cmdTmpBuf[0], readLen);
            dump_buffer(&cmdTmpBuf[0], readLen);

            size   -= readLen;
            offset += readLen;
        }

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        CLI_PRINTF("input error!\n");
        CLI_PRINTF("usage: fs dump parIdx(hex) offset(hex) size(hex)\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}

static int _userCmd(int argc, char **argv)
{
    if (argc == 3 || argc == 4) {
        if ((strcmp(argv[2], "erase") == 0) && (argc == 4)) {
            if (strcmp(argv[3], "0") == 0) {
                wise_fs_user_data_erase();
                wise_fs_reload();
            } else {
                WISE_FS_INFO_T fsInfo;
                PAR_INFO_T parInfo;
                wise_fs_get_fs_info(&fsInfo);
                wise_fs_get_par_info(FS_SIG_USER, &parInfo);
                debug_print("erase page 0\n");
                wise_fs_user_page_erase(0);
                debug_print("erase page 1\n");
                wise_fs_user_page_erase(fsInfo.eraseSize);
            }
            debug_print("User data erased.\n");
        } else if ((strcmp(argv[2], "test") == 0) && (argc == 3)) {
            volatile uint32_t testData = 0xabcd1234;
            volatile uint32_t readData = 0;

            debug_print("erasing user page...\n");
            wise_fs_user_page_erase(0);

            debug_print("writing user data... %08lx\n", testData);
            wise_fs_user_data_write(0, 4, (uint8_t *)&testData);

            debug_print("reading user data...\n");
            wise_fs_user_data_read(0, 4, (uint8_t *)&readData);
            debug_print("result=%08lx\n", readData);

            return SHLCMD_HIST | SHLCMD_DONE;
        } else {
            CLI_PRINTF("input error!\n");
            CLI_PRINTF("usage: fs user erase [0/1]\n");
            CLI_PRINTF("       fs user test\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    } else {
        CLI_PRINTF("input error!\n");
        CLI_PRINTF("usage: fs user erase [0/1]\n");
        CLI_PRINTF("       fs user test\n");
    }
    return SHLCMD_DONE;
}
