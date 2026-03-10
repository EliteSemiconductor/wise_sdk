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
#include "app_shell_cmd_i2c.h"

#include "api/wise_i2c_api.h"
#include "wise_core.h"

#define DMA_DISALE 0
#define DMA_TX_EN 0x01
#define DMA_RX_EN 0x02

static uint8_t testArray[32]            = {0};
static uint8_t recvArray[32]            = {0};
volatile int8_t i2c_transfer_end_status = WISE_FAIL;
static int _masterInitCmd(int argc, char **argv);
static int _slaveInitCmd(int argc, char **argv);
static int _sendCmd(int argc, char **argv);
static int _recvCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"help", "function list", _helpCmd},
    {"master", "i2c master init - type 'i2c master' to see how to use", _masterInitCmd},
    {"slave", "i2c slave init - type 'i2c slave' to see how to use", _slaveInitCmd},
    {"send", "i2c send [i2cIntf] [Data Count bytes]", _sendCmd},
    {"recv", "i2c recv [i2cIntf] [Data Count bytes]", _recvCmd},
};

int I2CCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

void my_i2c_ch0_callback(uint8_t i2c_channel, uint32_t i2c_event, void *context)
{
    if (i2c_event == WISE_I2C_EVENT_TRANSFER_DONE) {
        i2c_transfer_end_status = WISE_SUCCESS;
        printf("I2C Channel %d Transfer Done\n", i2c_channel);
    }
    if (i2c_event == WISE_I2C_EVENT_ADDRESS_HIT) {
        printf("I2C Channel %d Address Hit\n", i2c_channel);
    }
}

void my_i2c_ch1_callback(uint8_t i2c_channel, uint32_t i2c_event, void *context)
{
    if (i2c_event == WISE_I2C_EVENT_TRANSFER_DONE) {
        i2c_transfer_end_status = WISE_SUCCESS;
        printf("I2C Channel %d Transfer Done\n", i2c_channel);
    }
    if (i2c_event == WISE_I2C_EVENT_ADDRESS_HIT) {
        printf("I2C Channel %d Address Hit\n", i2c_channel);
    }
}

static void _i2c_io_config(uint8_t i2c_channel)
{
    WISE_GPIO_CFG_T i2c_io_cfg[2][2] = {{{4, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                         {5, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}},

                                        {{8, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                         {9, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}}};
    
    // I2C GPIO setting
    for (int i = 0; i < 2; i++) {
        wise_gpio_cfg(&i2c_io_cfg[i2c_channel][i]);
    }
}

static int _masterInitCmd(int argc, char **argv)
{
    uint8_t i2cIntf, mode, speed_mode, addressing, dmaEn;
    uint16_t target_address;
    uint8_t is_master = 1;

    if (argc != 9 || strcmp(argv[2], "init") != 0) {
        goto invalid;
    }

    i2cIntf        = strtoul(argv[3], NULL, 10);
    mode           = strtoul(argv[4], NULL, 10);
    speed_mode     = strtoul(argv[5], NULL, 10);
    addressing     = strtoul(argv[6], NULL, 10);
    target_address = strtoul(argv[7], NULL, 16);
    dmaEn          = strtoul(argv[8], NULL, 10);

    if (i2cIntf > 1 || mode > 1 || speed_mode > 2 || addressing > 1 || (addressing == 1 && target_address > 0x3FF) ||
        (addressing == 0 && target_address > 0x7F) || dmaEn > 1) {
        goto invalid;
    }

    uint8_t adjusted_mode = mode;
    if (is_master) {
        adjusted_mode = mode ? 0 : 1;
    }

    printf("I2C %d master config:\n", i2cIntf);
    printf("mode = %s\n", mode ? "Transmitter" : "Receiver");
    printf("speed = %s\n", speed_mode == 0 ? "100 kb/s" : (speed_mode == 1 ? "400 kb/s" : "1 Mb/s"));
    printf("addressing = %s\n", addressing ? "10-bit mode" : "7-bit mode");
    printf("target_address = 0x%x\n", target_address);
    printf("dmaEn = 0x%d\n", dmaEn);

    WISE_I2C_CONF_T i2cCfg = {.i2c_idx        = i2cIntf,
                              .role           = I2C_MASTER,
                              .i2cEn          = I2C_ENABLE,
                              .dmaEn          = dmaEn ? (DMA_TX_EN | DMA_RX_EN) : 0,
                              .speedMode      = speed_mode,
                              .addressing     = addressing,
                              .dir            = adjusted_mode,
                              .target_address = target_address};

    wise_i2c_init();
    wise_i2c_open(i2cIntf);
    if (i2cIntf == 0) {
        if (wise_i2c_register_event_callback(i2cIntf, my_i2c_ch0_callback, NULL) == WISE_SUCCESS) {
            printf("i2c %d callback registered\n", i2cIntf);
            i2c_transfer_end_status = WISE_FAIL;
        } else {
            printf("i2c callback register failed for channel %d\n", i2cIntf);
        }
    } else {
        if (wise_i2c_register_event_callback(i2cIntf, my_i2c_ch1_callback, NULL) == WISE_SUCCESS) {
            printf("i2c %d callback registered\n", i2cIntf);
            i2c_transfer_end_status = WISE_FAIL;
        } else {
            printf("i2c callback register failed for channel %d\n", i2cIntf);
        }
    }

    _i2c_io_config(i2cIntf);
    
    int32_t result = wise_i2c_config(i2cIntf, &i2cCfg);

    if (result == WISE_FAIL) {
        printf("I2C master initialization failed\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage: i2c master init\n"
           "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
           "       mode [0 = Transmitter / 1 = Receiver]\n"
           "       speed_mode [0 = 100 kb/s / 1 = 400 kb/s / 2 = 1 Mb/s]\n"
           "       addressing [0 = 7-bit / 1 = 10-bit]\n"
           "       target_address (in HEX format)\n"
           "       dmaEn [0 = disable / 1 = enable]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _slaveInitCmd(int argc, char **argv)
{
    uint8_t i2cIntf, mode, speed_mode, addressing, dmaEn;
    uint16_t target_address;
    uint8_t is_master = 0;

    if (argc != 9 || strcmp(argv[2], "init") != 0) {
        goto invalid;
    }

    i2cIntf        = strtoul(argv[3], NULL, 10);
    mode           = strtoul(argv[4], NULL, 10);
    speed_mode     = strtoul(argv[5], NULL, 10);
    addressing     = strtoul(argv[6], NULL, 10);
    target_address = strtoul(argv[7], NULL, 16);
    dmaEn          = strtoul(argv[8], NULL, 10);

    if (i2cIntf > 1 || mode > 1 || speed_mode > 2 || addressing > 1 || (addressing == 1 && target_address > 0x3FF) ||
        (addressing == 0 && target_address > 0x7F) || dmaEn > 1) {
        goto invalid;
    }

    uint8_t adjusted_mode = mode;
    if (is_master) {
        adjusted_mode = mode ? 0 : 1;
    }

    printf("I2C %d slave config:\n", i2cIntf);
    printf("mode = %s\n", mode ? "Transmitter" : "Receiver");
    printf("speed = %s\n", speed_mode == 0 ? "100 kb/s" : (speed_mode == 1 ? "400 kb/s" : "1 Mb/s"));
    printf("addressing = %s\n", addressing ? "10-bit mode" : "7-bit mode");
    printf("target_address = 0x%x\n", target_address);
    printf("dmaEn = 0x%d\n", dmaEn);

    WISE_I2C_CONF_T i2cCfg = {.i2c_idx        = i2cIntf,
                              .role           = I2C_SLAVE,
                              .i2cEn          = I2C_ENABLE,
                              .dmaEn          = dmaEn ? (DMA_TX_EN | DMA_RX_EN) : 0,
                              .speedMode      = speed_mode,
                              .addressing     = addressing,
                              .dir            = adjusted_mode,
                              .target_address = target_address};

    wise_i2c_init();
    wise_i2c_open(i2cIntf);
    if (i2cIntf == 0) {
        if (wise_i2c_register_event_callback(i2cIntf, my_i2c_ch0_callback, NULL) == WISE_SUCCESS) {
            printf("i2c %d callback registered\n", i2cIntf);
            i2c_transfer_end_status = WISE_FAIL;
        } else {
            printf("i2c callback register failed for channel %d\n", i2cIntf);
        }
    } else {
        if (wise_i2c_register_event_callback(i2cIntf, my_i2c_ch1_callback, NULL) == WISE_SUCCESS) {
            printf("i2c %d callback registered\n", i2cIntf);
            i2c_transfer_end_status = WISE_FAIL;
        } else {
            printf("i2c callback register failed for channel %d\n", i2cIntf);
        }
    }

    _i2c_io_config(i2cIntf);
    
    int32_t result = wise_i2c_config(i2cIntf, &i2cCfg);

    if (result == WISE_FAIL) {
        printf("I2C slave initialization failed\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage: i2c slave init\n"
           "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
           "       mode [0 = Transmitter / 1 = Receiver]\n"
           "       speed_mode [0 = 100 kb/s / 1 = 400 kb/s / 2 = 1 Mb/s]\n"
           "       addressing [0 = 7-bit / 1 = 10-bit]\n"
           "       target_address (in HEX format)\n"
           "       dmaEn [0 = disable / 1 = enable]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _sendCmd(int argc, char **argv)
{
    uint8_t i2cIntf  = strtoul(argv[2], NULL, 10);
    uint32_t dataCnt = strtoul(argv[3], NULL, 10);
    if (argc != 4 || i2cIntf > 1) {
        goto invalid;
    }

    for (uint8_t i = 0; i < sizeof(testArray) / sizeof(testArray[0]); i++) {
        testArray[i] = i;
    }
    i2c_transfer_end_status = WISE_FAIL;
    int32_t result          = wise_i2c_send_nbyte(i2cIntf, testArray, dataCnt);

    if (result == WISE_FAIL) {
        printf("I2C %d transmission failed\n", i2cIntf);
    } else {
        printf("I2C %d transmission start\n", i2cIntf);
    }

    printf("wait to send...\n");

    while (1) {
        if (i2c_transfer_end_status == WISE_SUCCESS) {
            printf("send data : \n");
            for (uint8_t i = 0; i < dataCnt; i++) {
                printf("%d ", testArray[i]);
            }
            printf("\nI2C %d transmission complete\n", i2cIntf);
            break;
        }
    }

    return SHLCMD_HIST | SHLCMD_DONE;
invalid:
    printf("usage: i2c send [i2cIntf] [wanted n bytes]\n"
           "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
           "       wanted n bytes [DEC]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _recvCmd(int argc, char **argv)
{
    uint8_t i2cIntf  = strtoul(argv[2], NULL, 10);
    uint32_t dataCnt = strtoul(argv[3], NULL, 10);

    if (argc != 4 || i2cIntf > 1) {
        goto invalid;
    }

    i2c_transfer_end_status = WISE_FAIL;
    int32_t result          = wise_i2c_recv_nbyte(i2cIntf, recvArray, dataCnt);

    if (result == WISE_FAIL) {
        printf("I2C %d reception failed\n", i2cIntf);
    } else {
        printf("I2C %d reception start\n", i2cIntf);
    }

    printf("wait to recv...\n");

    while (1) {
        if (i2c_transfer_end_status == WISE_SUCCESS) {
            printf("recv data : \n");
            for (uint8_t i = 0; i < dataCnt; i++) {
                printf("%d ", recvArray[i]);
            }
            printf("\nI2C %d recvive complete\n", i2cIntf);
            break;
        }
    }

    return SHLCMD_HIST | SHLCMD_DONE;
invalid:
    printf("usage: i2c recv [i2cIntf] [wanted n bytes]\n"
           "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
           "       wanted n bytes [DEC]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

