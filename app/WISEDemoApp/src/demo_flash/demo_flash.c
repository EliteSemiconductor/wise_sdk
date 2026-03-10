/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_flash_shell_app.c
 * @brief Example application demonstrating flash operations via UART shell.
 *
 * @ingroup WISE_EXAMPLE_APP_FLASH_SHELL
 *
 * This example application provides a UART-based interactive shell to
 * perform flash operations using WISE_FLASH_API, including:
 * - Flash read (dump memory contents)
 * - Flash write (pattern fill)
 * - Flash sector erase
 * - Flash information and UID query
 *
 * The shell prompt is configured as "FLASH> " and communicates through
 * the specified UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"
#include "wise_flash_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_FLASH_SHELL Flash Shell Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: Flash control through shell commands.
 *
 * This demo integrates a shell backend over UART and exposes flash
 * management commands built on top of Core APIs:
 * - @ref WISE_FLASH  (read/write/erase/info)
 * - @ref WISE_UART   (shell transport)
 * - @ref WISE_CORE   (main processing loop)
 * - @ref WISE_SYS    (system services)
 *
 * Supported shell commands:
 * - read  : Read flash data and dump buffer
 * - write : Write pattern data to flash
 * - erase : Erase flash sector
 * @{
 */

#define DEMO_APP_PROMPT             "FLASH> "

/** Cached flash information retrieved from ::wise_flash_get_info(). */
static WISE_FLASH_INFO_T flashInfo = {0};

/* ========================================================================== */
/* read Command                                                               */
/* ========================================================================== */

/**
 * @brief Shell command: Read flash contents.
 *
 * Usage:
 * @code
 * read [offset_hex] [len_hex]
 * @endcode
 *
 * Reads flash data starting from the specified offset and prints the
 * contents using ::dump_buffer().
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector:
 *                 - argv[1]: offset (hex)
 *                 - argv[2]: length (hex)
 *
 * @return 0 on success, negative value on error.
 */
static int cmd_flash_read(int argc, char **argv)
{
    uint32_t offset = 0, len = 0;

    if (argc != 3) {
        printf("Invalid parameters:\r\n");
        printf("Usage: read [offset_hex] [len_hex]\r\n");
        return -1;
    }

    offset = strtoul(argv[1], NULL, 16);
    if (errno != 0) {
        printf("Invalid offset\r\n");
        return -1;
    }
    len = strtoul(argv[2], NULL, 16);
    if (errno != 0) {
        printf("Invalid length\r\n");
        return -1;
    }

    if (len == 0) {
        printf("invalid length\r\n");
        return -1;
    }

    if ((offset >= flashInfo.flashSize) || ((offset + len) >= flashInfo.flashSize)) {
        printf("out of range\r\n");
        return -1;
    }

    printf("read flash: %08lx-%08lx\r\n", (unsigned long)offset, (unsigned long)len);

    if (len > 0) {
        #define READ_TMP_LEN 256
        uint8_t *tmpBuf = malloc(READ_TMP_LEN);

        if (tmpBuf == NULL) {
            printf("memory not enough\r\n");
            return -1;
        }

        while (len) {
            uint32_t readLen = (len > READ_TMP_LEN) ? READ_TMP_LEN : len;

            printf("flash reading %08lx-%08lx\r\n",
                   (unsigned long)offset,
                   (unsigned long)readLen);

            wise_flash_read(offset, tmpBuf, readLen);
            dump_buffer(tmpBuf, readLen);

            offset += readLen;
            len -= readLen;
        }

        free(tmpBuf);
    }

    return 0;
}

/** Register shell command "read" for flash read operation. */
SHELL_CMD_AUTO(read, cmd_flash_read, "Do flash read");

/* ========================================================================== */
/* write Command                                                              */
/* ========================================================================== */

/**
 * @brief Shell command: Write pattern data to flash.
 *
 * Usage:
 * @code
 * write [offset_hex] [len_hex] [pattern_hex]
 * @endcode
 *
 * Supported patterns:
 * - 0 : 0x00
 * - 1 : 0x55
 * - 2 : 0xAA
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on error.
 */
static int cmd_flash_write(int argc, char **argv)
{
    #define WRITE_BUF_LEN 256
    uint32_t offset = 0, len = 0, ptnIdx = 0;
    uint8_t *writeTmpBuf = NULL;

    if (argc != 4) {
        printf("Invalid parameters:\r\n");
        printf("Usage: write [offset_hex] [len_hex] [pattern_hex]\r\n");
        return -1;
    }

    offset = strtoul(argv[1], NULL, 16);
    if (errno != 0) {
        printf("Invalid offset\r\n");
        return -1;
    }
    len = strtoul(argv[2], NULL, 16);
    if (errno != 0) {
        printf("Invalid length\r\n");
        return -1;
    }
    ptnIdx = strtoul(argv[3], NULL, 16);
    if (errno != 0) {
        printf("Invalid pattern\r\n");
        return -1;
    }

    if (len == 0) {
        printf("invalid length\r\n");
        return -1;
    }

    if ((offset >= flashInfo.flashSize) || ((offset + len) >= flashInfo.flashSize)) {
        printf("out of range\r\n");
        return -1;
    }

    writeTmpBuf = malloc(WRITE_BUF_LEN);
    if (writeTmpBuf == NULL) {
        printf("memory not enough\r\n");
        return -1;
    }

    switch (ptnIdx) {
        case 0: memset(writeTmpBuf, 0x00, WRITE_BUF_LEN); break;
        case 1: memset(writeTmpBuf, 0x55, WRITE_BUF_LEN); break;
        case 2: memset(writeTmpBuf, 0xAA, WRITE_BUF_LEN); break;
        default:
            printf("unknown pattern\r\n");
            free(writeTmpBuf);
            return -1;
    }

    while (len) {
        uint32_t writeLen = (len > WRITE_BUF_LEN) ? WRITE_BUF_LEN : len;

        printf("flash writing %08lx-%08lx...",
               (unsigned long)offset,
               (unsigned long)writeLen);

        wise_flash_write(offset, writeTmpBuf, writeLen);
        printf("finish\r\n");

        len -= writeLen;
        offset += writeLen;
    }

    free(writeTmpBuf);
    return 0;
}

/** Register shell command "write" for flash write operation. */
SHELL_CMD_AUTO(write, cmd_flash_write, "Do flash write");

/* ========================================================================== */
/* erase Command                                                              */
/* ========================================================================== */

/**
 * @brief Shell command: Erase a flash sector.
 *
 * Usage:
 * @code
 * erase [offset_hex]
 * @endcode
 *
 * The offset must be aligned to the flash sector size
 * (::WISE_FLASH_INFO_T::minEraseSize).
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 *
 * @return 0 on success, negative value on error.
 */
static int cmd_flash_erase(int argc, char **argv)
{
    uint32_t offset = 0;
    uint32_t sectorMask = flashInfo.minEraseSize - 1;

    if (argc != 2) {
        printf("Invalid parameters:\r\n");
        printf("Usage: erase [offset_hex]\r\n");
        return -1;
    }

    offset = strtoul(argv[1], NULL, 16);
    if (errno != 0) {
        printf("Invalid offset\r\n");
        return -1;
    }

    if ((offset & sectorMask) != 0) {
        printf("not sector aligned\r\n");
        return -1;
    }

    wise_flash_sector_erase(offset);
    printf("sector erase finished\r\n");

    return 0;
}

/** Register shell command "erase" for flash sector erase operation. */
SHELL_CMD_AUTO(erase, cmd_flash_erase, "Do flash sector erase");

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the flash shell example application.
 *
 * Initializes demo environment, shell backend, and retrieves flash
 * information and UID. Then enters the main loop calling
 * ::wise_main_proc() for system task processing.
 */
void main(void)
{
    uint8_t flashUID[32];
    uint8_t flashUIDLen = 32;
    int i;

    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    wise_flash_get_info(&flashInfo);
    wise_flash_get_uid(flashUID, &flashUIDLen);

    printf("Flash Info:\r\n");
    printf("   total size:   %08lx\r\n", (unsigned long)flashInfo.flashSize);
    printf("   block size:   %08lx\r\n", (unsigned long)flashInfo.blockSize);
    printf("   sector size:  %08lx\r\n", (unsigned long)flashInfo.minEraseSize);

    if (flashUIDLen > 0) {
        printf("   Flash UID:    ");
        for (i = 0; i < flashUIDLen; i++) {
            printf("%02x", flashUID[i]);
        }
        printf("\r\n");
    }

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_FLASH_SHELL */
