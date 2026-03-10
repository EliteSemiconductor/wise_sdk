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
#include "app_shell_cmd_spi.h"

#include "api/wise_spi_api.h"
#include "api/wise_xcvr_api.h"
#include "wise_core.h"

#define SPI_INIT 0x80
#define SPI_TEST_BUF_LEN 48

static uint8_t flag        = 0;
uint32_t intfSel           = 0;
static uint8_t currentRole = 0;
uint8_t testData[SPI_TEST_BUF_LEN];
volatile int8_t spi_transfer_end_status = WISE_FAIL;
extern const uint32_t SPI_CLOCK[];

static WISE_SPI_CONF_T spiCfg[2] = {
    /* Set SPI frame format */
    /*
        spi_clock_mode 0: CPOL=0, CPHA=0
        spi_clock_mode 1: CPOL=0, CPHA=1
        spi_clock_mode 2: CPOL=1, CPHA=0
        spi_clock_mode 3: CPOL=1, CPHA=1
    */
    {.clock_mode     = CLOCK_MODE0,
     .spi_mode       = SPI_MODE_REGULAR,
     .role           = E_SPI_ROLE_MASTER,
     .data_bit_width = 8,
     .addr_len       = 2,
     .clock_sel      = E_SPI_CLOCK_SEL_5M,
     .bit_order      = SPI_MSB_FIRST,
     .data_merge     = 1,
     .addr_fmt       = 0,
     .block_mode     = E_SPI_NONBLOCK_MODE},
    {.clock_mode     = CLOCK_MODE0,
     .spi_mode       = SPI_MODE_REGULAR,
     .role           = E_SPI_ROLE_MASTER,
     .data_bit_width = 8,
     .addr_len       = 2,
     .clock_sel      = E_SPI_CLOCK_SEL_5M,
     .bit_order      = SPI_MSB_FIRST,
     .data_merge     = 1,
     .addr_fmt       = 0,
     .block_mode     = E_SPI_NONBLOCK_MODE},
};

static int _initCmd(int argc, char **argv);
static int _masterCmd(int argc, char **argv);
static int _writeCmd(int argc, char **argv);
static int _readCmd(int argc, char **argv);
static int _regCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "SPI function", _helpCmd},
    {"init", "SPI function", _initCmd},
    {"master", "SPI function", _masterCmd},
    {"write", "master write nbytes only", _writeCmd},
    {"read", "SPI function", _readCmd},
    {"reg", "control transceiver interface", _regCmd},
};

int SPICommandHandler(int argc, char **argv)
{
    return commonCommandHandler(
        argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

void my_spi_ch0_callback(uint8_t spi_channel, uint32_t spi_event, void *context)
{
    if (spi_event == WISE_SPI_EVENT_TRANSFER_DONE) {
        spi_transfer_end_status = WISE_SUCCESS;
        printf("SPI Channel %d Transfer Done\n", spi_channel);
    }
    if (spi_event == WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT) {
        printf("SPI Channel %d Get CMD 0x%02x\n", spi_channel,
               wise_spi_get_cmd(spi_channel));
    }
}

void my_spi_ch1_callback(uint8_t spi_channel, uint32_t spi_event, void *context)
{
    if (spi_event == WISE_SPI_EVENT_TRANSFER_DONE) {
        spi_transfer_end_status = WISE_SUCCESS;
        printf("SPI Channel %d Transfer Done\n", spi_channel);
    }
    if (spi_event == WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT) {
        printf("SPI Channel %d Get CMD 0x%02x\n", spi_channel,
               wise_spi_get_cmd(spi_channel));
    }
}

static void _spi_io_config(uint8_t spiChannel, WISE_SPI_MODE_T spiMode)
{
    WISE_GPIO_CFG_T spi_io_cfg[2][6] = 
    {
        {
            // SPI 0
            {10, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // SCK
#ifdef ES_DEVICE_TRX_RADIO
            {11, MODE_GPIO, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // CS
#else
            {11, MODE_PERI_1, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // CS
#endif
            {12, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // MOSI
            {13, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // MISO
            {14, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // HOLD
            {15, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE} // WP
        },
        {
            // SPI 1
            {6, MODE_PERI_2, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // SCK
            {7, MODE_PERI_2, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // CS
            {8, MODE_PERI_2, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // MOSI
            {9, MODE_PERI_2, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // MISO
            {10, MODE_PERI_2, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}, // HOLD
            {11, MODE_PERI_2, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE} // WP
        }
    };
    int num_pins = (spiMode == SPI_MODE_QUAD) ? 6 : (spiMode == SPI_MODE_3WIRE ? 3 : 4);

    for (int i = 0; i < num_pins; i++) 
    {
        wise_gpio_cfg(&spi_io_cfg[spiChannel][i]);
    }
}

static int _initCmd(int argc, char **argv)
{
    if (argc == 3) {
        intfSel = strtoul(argv[2], NULL, 10);
        if (intfSel > 1) {
            goto invalid;
        }
        if (!(flag & SPI_INIT)) {
            wise_spi_init();
            if (intfSel == 0) {
                if (wise_spi_register_event_callback(0, my_spi_ch0_callback,
                                                     NULL) == WISE_SUCCESS) {
                    printf("spi %d callback registered\n", intfSel);
                    spi_transfer_end_status = WISE_FAIL;
                } else {
                    printf("spi callback register failed for channel %d\n",
                           intfSel);
                }
            } else {
                if (wise_spi_register_event_callback(1, my_spi_ch1_callback,
                                                     NULL) == WISE_SUCCESS) {
                    printf("spi %d callback registered\n", intfSel);
                    spi_transfer_end_status = WISE_FAIL;
                } else {
                    printf("spi callback register failed for channel %d\n",
                           intfSel);
                }
            }
            flag |= SPI_INIT;
        }
        printf("spi %ld init\n", intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage : spi init [0/1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _masterCmd(int argc, char **argv)
{
    if (argc == 4) {
        intfSel = strtoul(argv[3], NULL, 10);
        if ((strcmp(argv[2], "open") != 0) || (intfSel > 1)) {
            goto invalid;
        }
        if (flag & (1 << intfSel)) {
            wise_spi_close(intfSel);
        }

        _spi_io_config(intfSel, spiCfg[intfSel].spi_mode);
        
        currentRole = E_SPI_ROLE_MASTER;
        // spiCfg[intfSel].block_mode = E_SPI_BLOCK_MODE ;
        if (WISE_SUCCESS ==
            wise_spi_master_open(intfSel, &spiCfg[intfSel])) {
            flag |= (1 << intfSel);
            printf("SPI master %ld is enabled\n", intfSel);
        } else {
            printf("Failed to open SPI %ld\n", intfSel);
        }
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage : spi master open spi[0/1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _writeCmd(int argc, char **argv)
{
    uint32_t rwLen = 0;
    uint32_t i;

    if (argc == 4) {
        intfSel = strtoul(argv[2], NULL, 10);
        if (intfSel > 1) {
            goto invalid;
        }
        if (!(flag & (1 << intfSel))) {
            printf("plz init spi first\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
        rwLen = strtoul(argv[3], NULL, 16);
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        } else if (rwLen < 1) {
            goto invalid;
        }
        if (currentRole != E_SPI_ROLE_MASTER) {
            printf("plz oprate in MASTER mode\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        for (i = 0; i < rwLen; i++) {
            testData[i] = i + 1;
        }
        spi_transfer_end_status = WISE_FAIL;
        if (rwLen == 1) {
            wise_spi_master_write_byte(intfSel, testData[0]);
        } else {
            wise_spi_master_write(intfSel, testData, rwLen);
        }
        while (spi_transfer_end_status == WISE_FAIL) {
            ;
        }
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    printf("usage : spi write spi[0/1] [hex_len]\n");
    printf("The SPI will transmit a sequence of numbers until the specified "
           "length:\n"
           "0x01, 0x02, 0x03...\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _readCmd(int argc, char **argv)
{
    uint32_t rwLen = 0;

    if (argc == 4) {
        intfSel = strtoul(argv[2], NULL, 10);
        if (intfSel > 1) {
            goto invalid;
        }
        if (!(flag & (1 << intfSel))) {
            printf("plz init spi first\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
        rwLen = strtoul(argv[3], NULL, 16);
        ;
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        }
        if (currentRole != E_SPI_ROLE_MASTER) {
            printf("plz oprate in MASTER mode\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }

        memset(testData, 0, rwLen);

        spi_transfer_end_status = WISE_FAIL;
        if (rwLen == 1) {
            wise_spi_master_read_byte(intfSel, testData);
        } else {
            wise_spi_master_read(intfSel, testData, rwLen);
        }

        while (spi_transfer_end_status == WISE_FAIL) {
            ;
        }

        printf("SPI %ld read %ld bytes\n", intfSel, rwLen);
        dump_buffer(testData, rwLen);

        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage : spi read spi[0/1] [hex_len]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _regCmd(int argc, char **argv)
{
    uint32_t reg      = 0;
    uint32_t value    = 0;
    uint32_t rd_value = 0;

    if (argc == 4) {
        reg                     = strtoul(argv[2], NULL, 16);
        value                   = strtoul(argv[3], NULL, 16);
        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_write(intfSel, reg, &value);
        while (spi_transfer_end_status == WISE_FAIL) {
            ;
        }
        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_read(intfSel, reg, &rd_value);
        while (spi_transfer_end_status == WISE_FAIL) {
            ;
        }
        printf("w %04lx %08lx=> %08x\n", reg, value, rd_value);

    } else if (argc == 3) {
        reg                     = strtoul(argv[2], NULL, 16);
        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_read(intfSel, reg, &rd_value);
        while (spi_transfer_end_status == WISE_FAIL) {
            ;
        }
        printf("r %04lx=> %08lx\n", reg, rd_value);
    } else {
        // handle unexpect input behavior
        printf("input error!\n");
        printf("Usage:spi reg addr_hex value_hex for write\n");
        printf("Usage:spi reg addr_hex for read\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}
