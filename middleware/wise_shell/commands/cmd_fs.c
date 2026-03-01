/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#if SHELL_FILESYSTEM
#include "cmd_fs.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t cmdTmpBuf[256];

static int _formatCmd(int argc, char **argv);
static int _infoCmd(int argc, char **argv);
static int _dumpCmd(int argc, char **argv);
static int _userCmd(int argc, char **argv);
static int _patchCmd(int argc, char **argv);

static const struct shellCommand m_fsCmdList[] = {
    {"help", "file system function", _helpCmd},
    {"format", "file system format", _formatCmd},
    {"info", "file system info", _infoCmd},
    {"dump", "file system dump", _dumpCmd},
    {"user", "file system user cmd", _userCmd},
    {"patch", "file system patch cmd", _patchCmd},
};

int FileSystemCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_fsCmdList,
                                sizeof(m_fsCmdList) /
                                    sizeof(struct shellCommand));
}

static int _formatCmd(int argc, char **argv)
{
    if (argc == 2) {
        wise_fs_create_default();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        CLI_PRINTF("input error!\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
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

    if (argc == 4) {
        offset = strtoul(argv[2], NULL, 16);
        size   = strtoul(argv[3], NULL, 16);
#if defined(CONFIG_ER8130_TC04)
        while (size) {
            uint32_t readLen = size > 256 ? 256 : size;

            wise_flash_read(offset, &cmdTmpBuf[0], readLen);
            dump_buffer(&cmdTmpBuf[0], readLen);

            size   -= readLen;
            offset += readLen;
        }
#endif
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        CLI_PRINTF("input error!\n");
        CLI_PRINTF("usage: fs dump addr(hex) size(hex)\n");
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

#if 0
			uint32_t testOffset = 4091;
			uint32_t testLen = 85;
			uint32_t i;

			wise_fs_user_data_read(testOffset, testLen, &cmdTmpBuf[0]);
			debug_print("orig buffer\n");
			dump_buffer(&cmdTmpBuf[0], testLen);

			for (i = 0; i < testLen; i++) {
				cmdTmpBuf[i] = i;
			}

			debug_print("updating\n");
			wise_fs_user_data_update(testOffset, testLen, &cmdTmpBuf[0]);
			debug_print("finish\n");

			wise_fs_user_data_read(testOffset, 256, &cmdTmpBuf[0]);
			dump_buffer(&cmdTmpBuf[0], 256);
#endif

#if 0
			for (i = 0; i < testLen; i++) {
				cmdTmpBuf[i] = i + 40;
			}

			debug_print("updating 2\n");
			wise_fs_user_data_update(testOffset + testLen, testLen, &cmdTmpBuf[0]);
			debug_print("finish\n");

			wise_fs_user_data_read(0, 256, &cmdTmpBuf[0]);
			dump_buffer(&cmdTmpBuf[0], 256);
#endif
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

static int _patchCmd(int argc, char **argv)
{
    if (argc == 3) {
        if (strcmp(argv[2], "erase") == 0) {
            wise_fs_patch_erase();
            debug_print("patch erase\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        } else if (strcmp(argv[2], "test") == 0) {
            uint32_t patchOffset = 0;
            int32_t patchLen     = 0;
            int i;
            int testLen = 241;

            debug_print("patch test start\n");
            if (WISE_SUCCESS !=
                wise_fs_patch_get_info(&patchOffset, &patchLen)) {
                debug_print("no patch available\n");
            }

            for (i = 0; i < testLen; i++) {
                cmdTmpBuf[i] = i;
            }

            wise_fs_patch_update(testLen, &cmdTmpBuf[0]);

            if (WISE_SUCCESS !=
                wise_fs_patch_get_info(&patchOffset, &patchLen)) {
                debug_print("patch update fail\n");
            } else {
                debug_print("patch @%08lx-%08lx\n", patchOffset, patchLen);
                memset(&cmdTmpBuf[0], 0, 256);

                if (WISE_SUCCESS ==
                    wise_fs_patch_read(patchLen, &cmdTmpBuf[0])) {
                    dump_buffer(&cmdTmpBuf[0], patchLen);
                }
            }
            return SHLCMD_HIST | SHLCMD_DONE;
        } else {
            CLI_PRINTF("input error!\n");
            CLI_PRINTF("usage: fs patch erase\n");
            CLI_PRINTF("       fs patch test\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    } else {
        CLI_PRINTF("input error!\n");
        CLI_PRINTF("usage: fs patch erase\n");
        CLI_PRINTF("       fs patch test\n");
    }
    return SHLCMD_DONE;
}
#endif /* SHELL_FILESYSTEM */
