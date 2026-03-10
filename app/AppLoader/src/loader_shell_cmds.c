/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_core.h"
#include "wise.h"
#include "loader_shell_cmds.h"
#include "shell_commands.h"
#include "wise_sys_api.h"
#include "wise_tick_api.h"
#include "wise_file_system.h"
#include "wise_kermit.h"

#define LOADER_CMD_BUFFER_LEN 256
#define INVALID_PARAM_MESSAGE "Invalid parameters\nUsage:\n"

uint8_t cmdTmpBuf[LOADER_CMD_BUFFER_LEN];

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
int FileSystemCommandHandler(int argc, char **argv);
#endif

int KermitCommandhandler(int argc, char **argv);
int ResetCommandHandler(int argc, char **argv);
int DumpCommandHandler(int argc, char **argv);

const struct shellCommand extCmdTable[] = {
    // basic command set
    {"help", "help", _helpCmd},

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    // file system cmds for apploader
    {"fs", "fs cmds", FileSystemCommandHandler},
#endif

    //kermit loader
    {"kermit", "start kermit receiver", KermitCommandhandler},

    //
    {"reset", "do chip reset", ResetCommandHandler},

    //
    {"dump", "dump buffer", DumpCommandHandler},
};

void loader_shell_cmd_init()
{
    shlcmd_set_ext_cmd_table(extCmdTable,
                             sizeof(extCmdTable) / sizeof(struct shellCommand));
}

int loader_shell_cmd_check()
{
    uint32_t checkCount = 3;
    uint32_t hitCounter = 0;

    shell_output("Press 'cccc' to enter console...\n");

    while (checkCount--) {
        uint32_t startTick = wise_tick_get_counter();
        uint8_t inputC;

        while (1) {
            uint32_t now = wise_tick_get_counter();

            if ((now - startTick) >= MS_TO_CLK(1000)) {
                shell_output(".");
                break;
            }

            if ((wise_uart_check_data_len(STDIO_UART_PORT) > 0) &&
                (wise_uart_read_char(STDIO_UART_PORT, &inputC) != WISE_FAIL)) {
                wise_uart_write_char(STDIO_UART_PORT, inputC);

                if (inputC == 'c') {
                    hitCounter++;
                    if (hitCounter >= 3) {
                        return E_LOADER_CONSOLE;
                    }
                }
            }
        }
    }

    return E_LOADER_NORMAL_BOOT;
}

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
//====== FS command implementations =========//
static int _fsFormatCmd(int argc, char **argv)
{
    if (argc == 2) {
        wise_fs_create_default();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        shell_output("input error!\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}

static int _fsInfoCmd(int argc, char **argv)
{
    if (argc == 2) {
        wise_fs_debug_info();
        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        shell_output("input error!\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}

static int _fsDumpCmd(int argc, char **argv)
{
    uint32_t offset;
    uint32_t size;

    if (argc == 4) {
        offset = strtoul(argv[2], NULL, 16);
        size   = strtoul(argv[3], NULL, 16);

        while (size) {
            uint32_t readLen = size > 256 ? 256 : size;

            wise_flash_read(offset, &cmdTmpBuf[0], readLen);
            dump_buffer(&cmdTmpBuf[0], readLen);

            size   -= readLen;
            offset += readLen;
        }

        return SHLCMD_HIST | SHLCMD_DONE;
    } else {
        shell_output("input error!\n");
        shell_output("usage: fs dump addr(hex) size(hex)\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
}


static const struct shellCommand m_fsCmdList[] = {
    {"help", "file system function", _helpCmd},
    {"format", "create default partitions", _fsFormatCmd},
    {"info", "list partition info", _fsInfoCmd},
    {"dump", "dump flash contents", _fsDumpCmd},
};

int FileSystemCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_fsCmdList,
                                sizeof(m_fsCmdList) /
                                    sizeof(struct shellCommand));
}
#endif


int KermitCommandhandler(int argc, char **argv)
{
    uint32_t rxDataLen = 0;
    uint16_t rxDataCrc = 0;
    uint32_t param     = 0;
    uint8_t target     = E_KERMIT_TARGET_FS;
    uint32_t kermitParam = 0;
    
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

    switch (target) 
    {
        case E_KERMIT_TARGET_FS: 
        {
#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
            WISE_FS_INFO_T fsInfo;

            wise_fs_get_fs_info(&fsInfo);
            if ((fsInfo.storageSize == 0) || (fsInfo.eraseSize == 0)) {
                shell_output("Invalid flash info\n");
                goto finish;
            }

            if ((param >= 0) && (param < fsInfo.parNum)) 
            {
                if (param == 0) 
                {
                    shell_output("AppLoader cannot be updapted\n");
                    goto finish;
                }
            } 
            else 
            {
                shell_output("Invalid parition index\n");
                goto finish;
            }

            kermitParam = param;
#else
            shell_output("File system module not enabled\n");
#endif
        }
        break;

        case E_KERMIT_TARGET_FLASH: 
        {
            /*
            WISE_FLASH_INFO_T flashInfo;

            wise_flash_get_info(&flashInfo);

            if (param >= flashInfo.flashSize) {
                shell_output("out of flash range\n");
                goto help;
            }
            */
            shell_output("Kermit loading address - 0x%08x\n", APP_BOOT_ADDR);
            kermitParam = APP_BOOT_ADDR;
        } 
        break;

        case E_KERMIT_TARGET_RAM:
        defualt:
            goto finish;
            break;
    }

    if (WISE_SUCCESS == wise_kermit_init(target, kermitParam, STDIO_UART_PORT)) {
        shell_output("Start kermit receiver\n");
        wise_kermit_load(&rxDataLen, &rxDataCrc);

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
        if (target == E_KERMIT_TARGET_FS) {
            if (WISE_SUCCESS ==
                wise_fs_par_update_info(param, rxDataLen, rxDataCrc)) {
                shell_output("partition %d update finished\n", param);
            } else {
                shell_output("CRC error\n");
            }
        }
#endif
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

int ResetCommandHandler(int argc, char **argv)
{
    wise_sys_chip_reset();
    //wise_sys_reset();

    shell_output("failed to chip reset\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

int DumpCommandHandler(int argc, char **argv)
{
    uint32_t addr   = 0;
    uint32_t length = 0;

    if (argc != 3) {
        goto help;
    }

    addr   = strtoul(argv[1], NULL, 16);
    length = strtoul(argv[2], NULL, 16);

    dump_buffer(addr, length);
    return SHLCMD_HIST | SHLCMD_DONE;

help:
    shell_output(INVALID_PARAM_MESSAGE);
    shell_output("  dump [addr_hex] [length_hex]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}
