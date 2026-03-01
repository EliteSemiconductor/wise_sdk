/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "es_platform_components.h"
#include "wise_spi_api.h"
#include "wise_gpio_api.h"
#if SHELL_SPI
#include "cmd_spi.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell_func.h"
#include "functions/fmt_print.h"

/* ================== Config ================== */
#define SPI_INIT 0x80
#define SPI_TEST_BUF_LEN 0x200
#define SPI_TRANS_TEST_BUF_LEN 0x20

/* ================== Globals ================== */
static uint8_t flag             = 0;
volatile static uint8_t intfSel = 0;
static uint8_t currentRole      = 0;

volatile uint32_t call_back_handler = 0xff;
volatile uint8_t slave_cmd_handler;
volatile uint8_t slave_cmd;

uint8_t testData[SPI_TEST_BUF_LEN] __attribute__((aligned(4)));
uint8_t testDataTrans[SPI_TRANS_TEST_BUF_LEN] __attribute__((aligned(4)));
volatile int8_t spi_transfer_end_status = WISE_FAIL;

extern const uint32_t SPI_CLOCK[];

static WISE_SPI_CONF_T spiCfg[2] = {
    {.clock_mode     = CLOCK_MODE0,
     .spi_mode       = SPI_MODE_REGULAR,
     .role           = E_SPI_ROLE_MASTER,
     .data_bit_width = 8,
     .addr_len       = 2,
     .clock_sel      = E_SPI_CLOCK_SEL_100K,
     .bit_order      = SPI_MSB_FIRST,
     .data_merge     = 1,
     .addr_fmt       = 0,
     .block_mode     = E_SPI_NONBLOCK_MODE,
     .dma_enable     = 0},
    {.clock_mode     = CLOCK_MODE0,
     .spi_mode       = SPI_MODE_REGULAR,
     .role           = E_SPI_ROLE_MASTER,
     .data_bit_width = 8,
     .addr_len       = 2,
     .clock_sel      = E_SPI_CLOCK_SEL_5M,
     .bit_order      = SPI_MSB_FIRST,
     .data_merge     = 1,
     .addr_fmt       = 0,
     .block_mode     = E_SPI_NONBLOCK_MODE,
     .dma_enable     = 0},
};

/* ================== Prototypes ================== */
static int _spiInitCmd(int argc, char **argv);
static int _masterCmd(int argc, char **argv);
static int _slaveCmd(int argc, char **argv);
static int _clockCmd(int argc, char **argv);
static int _confCmd(int argc, char **argv);
static int _closeCmd(int argc, char **argv);
static int _regCmd(int argc, char **argv);

static int _m_raw_write_Cmd(int argc, char **argv);
static int _m_raw_read_Cmd(int argc, char **argv);

static int _m2s_send_Cmd(int argc, char **argv);
static int _m2s_read_Cmd(int argc, char **argv);
static int _m2s_user_Cmd(int argc, char **argv);

/* ================== Command Table ================== */
static const struct shellCommand m_CmdList[] = {
    {"help", "show SPI help", _helpCmd},
    {"init", "init SPI core & register callbacks", _spiInitCmd},
    {"master", "open SPI in MASTER role", _masterCmd},
    {"slave", "open SPI in SLAVE role (listen mode)", _slaveCmd},
    {"clock", "set SPI clock by selector index", _clockCmd},
    {"conf", "show SPI configuration", _confCmd},
    {"close", "close SPI channel", _closeCmd},
    {"reg", "read/write transceiver register", _regCmd},

    /* RAW: Without SPI CMD */
    {"m_raw_write", "MASTER write N bytes (no opcode)", _m_raw_write_Cmd},
    {"m_raw_read", "MASTER read  N bytes (no opcode)", _m_raw_read_Cmd},

    /* MASTER <-> SLAVE: With SPI CMD */
    {"m2s_send", "MASTER->SLAVE write N bytes to slave FIFO", _m2s_send_Cmd},
    {"m2s_read", "MASTER<-SLAVE read  N bytes from slave FIFO", _m2s_read_Cmd},
    {"m2s_usercmd", "send USER_CMD+base; slave fills FIFO with base..", _m2s_user_Cmd},
};

int SPICommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

/* ================== Utilities ================== */
static int _check_spi_channel(int argc, char **argv, int idx, int require_init, int require_master)
{
    if (argc <= idx) {
        return -10;
    }
    long sel = strtol(argv[idx], NULL, 10);
    if (sel < 0 || sel > 1) {
        return -1;
    }

    if (require_init && !(flag & (1 << sel))) {
        printf("plz init spi first\n");
        return -2;
    }
    if (require_master && currentRole != E_SPI_ROLE_MASTER) {
        printf("plz operate in MASTER mode\n");
        return -3;
    }
    return (int)sel;
}

static void _spi_xfer_and_wait(uint8_t ch, WISE_SPI_XFER_MSG_T *msg)
{
    spi_transfer_end_status = WISE_FAIL;
    wise_spi_msg_xfer(ch, msg);
    while (spi_transfer_end_status == WISE_FAIL) {
        __NOP();
    }
}

/* ================== Callbacks ================== */
void my_spi_ch0_callback(uint8_t spi_channel, uint32_t spi_event, void *context)
{
    if (spi_event == WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT) {
        call_back_handler = spi_event;
    }
    if (spi_event == WISE_SPI_EVENT_TRANSFER_DONE) {
        spi_transfer_end_status = WISE_SUCCESS;
        printf("SPI Channel %d Transfer Done\n", spi_channel);
    }
}

void my_spi_ch1_callback(uint8_t spi_channel, uint32_t spi_event, void *context)
{
    if (spi_event == WISE_SPI_EVENT_TRANSFER_DONE) {
        spi_transfer_end_status = WISE_SUCCESS;
        printf("SPI Channel %d Transfer Done\n", spi_channel);
    }
    if (spi_event == WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT) {
        printf("SPI Channel %d Get CMD 0x%02x\n", spi_channel, wise_spi_get_cmd(spi_channel));
    }
}

/* ================== IO Config ================== */
static void _spi_io_config(uint8_t spiChannel, WISE_SPI_MODE_T spiMode)
{
    if (spiChannel > 1) {
        printf("[SPI] invalid channel: %u\n", spiChannel);
        return;
    }

    int num_pins = (spiMode == SPI_MODE_QUAD)  ? 6 :
                   (spiMode == SPI_MODE_3WIRE) ? 3 : 4;

    bool need_tcxo = false;
    bool need_swd  = false;

#if ES_DEVICE_PIO
    /* { func, pin } */
    const uint8_t spi_pio_cfg[2][6][2] = {
        {   /* SPI 0 */
            { MODE_PIO_FUNC_SPI0_CLK,  10 },
#ifdef ES_DEVICE_TRX_RADIO
            { MODE_PIO_FUNC_GPIO,      11 },
#else
            { MODE_PIO_FUNC_SPI0_CS,   11 },
#endif
            { MODE_PIO_FUNC_SPI0_MOSI, 12 },
            { MODE_PIO_FUNC_SPI0_MISO, 13 },
            { MODE_PIO_FUNC_SPI0_HOLD, 14 },
            { MODE_PIO_FUNC_SPI0_WP,   15 },
        },
        {   /* SPI 1 */
            { MODE_PIO_FUNC_SPI1_CLK,  6  },
            { MODE_PIO_FUNC_SPI1_CS,   7  },
            { MODE_PIO_FUNC_SPI1_MOSI, 8  },
            { MODE_PIO_FUNC_SPI1_MISO, 9  },
            { MODE_PIO_FUNC_SPI1_HOLD, 10 },
            { MODE_PIO_FUNC_SPI1_WP,   11 },
        }
    };

    printf("[SPI%u][PIO] spiMode=%d, numPins=%d\n", spiChannel, spiMode, num_pins);

    /* First Pass: check which pins will be used */
    for (int i = 0; i < num_pins; i++) {
        uint8_t pin = spi_pio_cfg[spiChannel][i][1];

        if (pin == 10) need_tcxo = true;
        if (pin == 14 || pin == 15) need_swd = true;
    }

#else
    /* non-PIO */
    WISE_GPIO_CFG_T spi_io_cfg[2][6] = {{
                                            {10, MODE_PERI_1, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#ifdef ES_DEVICE_TRX_RADIO
                                            {11, MODE_GPIO,   GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#else
                                            {11, MODE_PERI_1, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
#endif
                                            {12, MODE_PERI_1, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {13, MODE_PERI_1, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {14, MODE_PERI_1, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {15, MODE_PERI_1, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}
                                        },
                                        {
                                            {6,  MODE_PERI_2, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {7,  MODE_PERI_2, GPIO_DIR_OUTPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {8,  MODE_PERI_2, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {9,  MODE_PERI_2, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {10, MODE_PERI_2, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
                                            {11, MODE_PERI_2, GPIO_DIR_INPUT,  GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}
                                        }};

    printf("[SPI%u][GPIO] spiMode=%d, numPins=%d\n", spiChannel, spiMode, num_pins);

    /* First Pass: check which pins will be used */
    for (int i = 0; i < num_pins; i++) {
        uint8_t pin = spi_io_cfg[spiChannel][i].pin;

        if (pin == 10) need_tcxo = true;
        if (pin == 14 || pin == 15) need_swd = true;
    }
#endif

    /* Now safely disable SWD / TCXO only if pins are used */
    if (need_swd) {
        wise_sys_swd_config(false);
        printf("  [CFG] SWD disabled (pin14/15 in use)\n");
    }

    if (need_tcxo) {
        wise_sys_tcxo_config(false);
        printf("  [CFG] TCXO disabled (pin10 in use)\n");
    }

    /* Second Pass: apply pin configuration */
#if ES_DEVICE_PIO
    for (int i = 0; i < num_pins; i++) {
        uint8_t func = spi_pio_cfg[spiChannel][i][0];
        uint8_t pin  = spi_pio_cfg[spiChannel][i][1];

        wise_gpio_func_cfg(pin, func);

        printf("  Pin %2u -> %-12s (func=%u)\n",
               pin, gpio_pin_func_to_str(func), func);
    }
#else
    for (int i = 0; i < num_pins; i++) {
        wise_gpio_cfg(&spi_io_cfg[spiChannel][i]);
        printf("  Pin %2u -> mode=%d, dir=%d\n",
               spi_io_cfg[spiChannel][i].pin,
               spi_io_cfg[spiChannel][i].mode,
               spi_io_cfg[spiChannel][i].dir);
    }
#endif
}


/* ================== Small Helpers ================== */
static void _spi_slave_listen_once(uint8_t sel, uint16_t recv_count)
{
    memset(testData, 0, SPI_TEST_BUF_LEN);

    WISE_SPI_XFER_MSG_T trans_msg = {0};
    trans_msg.trans_mode          = SPI_TM_READ_ONLY;
    trans_msg.dummy_len           = 1;
    trans_msg.rx_data_buff        = testData;
    trans_msg.rx_unit_count       = recv_count;

    call_back_handler       = 0xff;
    spi_transfer_end_status = WISE_FAIL;

    wise_spi_reset(sel, WISE_SPI_RESET_RX);
    wise_spi_msg_xfer(sel, &trans_msg);
    wise_spi_slave_set_ready(sel);

    printf("SPI %d slave Listening...\n", sel);
    printf("[Info] Press Ctrl+G to leave slave listen mode.\n");
}

static void _wait_for_transfer_done(void)
{
    while (spi_transfer_end_status == WISE_FAIL) {
        __NOP();
    }
}

/* ================== Commands ================== */
static int _spiInitCmd(int argc, char **argv)
{
    if (argc == 3) {
        int sel = _check_spi_channel(argc, argv, 2, 0, 0);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        if (!(flag & SPI_INIT)) {
            wise_spi_init();
            if (intfSel == 0) {
                if (wise_spi_register_event_callback(0, my_spi_ch0_callback, NULL) == WISE_SUCCESS) {
                    printf("spi %d callback registered\n", intfSel);
                    spi_transfer_end_status = WISE_FAIL;
                } else {
                    printf("spi callback register failed for channel %d\n", intfSel);
                }
            } else {
                if (wise_spi_register_event_callback(1, my_spi_ch1_callback, NULL) == WISE_SUCCESS) {
                    printf("spi %d callback registered\n", intfSel);
                    spi_transfer_end_status = WISE_FAIL;
                } else {
                    printf("spi callback register failed for channel %d\n", intfSel);
                }
            }
            flag |= SPI_INIT;
        }
        printf("spi %d init\n", intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi init [0|1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _masterCmd(int argc, char **argv)
{
    if (argc == 4) {
        if (strcmp(argv[2], "open") != 0) {
            goto invalid;
        }

        int sel = _check_spi_channel(argc, argv, 3, 0, 0);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        _spi_io_config(intfSel, spiCfg[intfSel].spi_mode);
        currentRole = E_SPI_ROLE_MASTER;

        if (WISE_SUCCESS == wise_spi_master_open(intfSel, &spiCfg[intfSel])) {
            flag |= (1 << intfSel);
            printf("SPI master %d is enabled\n", intfSel);
        } else {
            printf("Failed to open SPI %d\n", intfSel);
        }
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi master open [0|1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _slaveCmd(int argc, char **argv)
{
    uint16_t recv_count  = 0;
    uint16_t trans_count = 0;

    if (argc != 4) {
        goto invalid;
    }
    if (strcmp(argv[2], "open") != 0) {
        goto invalid;
    }

    int sel = _check_spi_channel(argc, argv, 3, 0, 0);
    if (sel < 0) {
        goto invalid;
    }
    intfSel = (uint8_t)sel;

    _spi_io_config(intfSel, spiCfg[intfSel].spi_mode);
    currentRole = E_SPI_ROLE_SLAVE;

    if (WISE_SUCCESS != wise_spi_slave_open(intfSel, &spiCfg[intfSel])) {
        printf("Failed to open SPI %d\n", intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    flag              |= (1 << intfSel);
    slave_cmd_handler  = 1;
    _spi_slave_listen_once(intfSel, SPI_TEST_BUF_LEN);

    while (slave_cmd_handler) {
        extern void _wise_uart_dispatch_task(void);
        _wise_uart_dispatch_task();

        if (call_back_handler != WISE_SPI_EVENT_SLAVE_COMMAND_INTERRUPT) {
            continue;
        }

        slave_cmd = wise_spi_get_cmd(intfSel);
        printf("SPI Channel %d Get CMD 0x%02x\n", intfSel, slave_cmd);
        call_back_handler = 0xff;

        switch (slave_cmd) {
        case WRITE_DATA_SINGLE_IO:
        case WRITE_DATA_DUAL_IO:
        case WRITE_DATA_QUAD_IO:
            _wait_for_transfer_done();
            recv_count = wise_spi_slave_get_recv_count(intfSel);
            if (recv_count > 0) {
                printf("SPI %d read %d counts\n", intfSel, recv_count);
                dump_buffer((uint8_t *)testData, recv_count);
            }
            break;

        case READ_DATA_SINGLE_IO:
        case READ_DATA_DUAL_IO:
        case READ_DATA_QUAD_IO:
            _wait_for_transfer_done();
            printf("SPI %d send %d counts\n", intfSel, trans_count);
            dump_buffer((uint8_t *)testDataTrans, trans_count);
            break;

        case USER_CMD:
            _wait_for_transfer_done();
            recv_count = wise_spi_slave_get_recv_count(intfSel);
            if (recv_count > 0) {
                printf("SPI %d read %d counts\n", intfSel, recv_count);
                dump_buffer((uint8_t *)testData, recv_count);

                /* base = first byte; fill TX FIFO as base, base+1, ... */
                uint8_t base = testData[0];
                trans_count  = SPI_TRANS_TEST_BUF_LEN;
                for (uint16_t i = 0; i < trans_count; i++) {
                    testDataTrans[i] = (uint8_t)(base + i);
                }
                wise_spi_data_prepare(intfSel, testDataTrans, trans_count);
            }
            break;

        default:
            break;
        }

        slave_cmd = 0x00;
        _spi_slave_listen_once(intfSel, SPI_TEST_BUF_LEN);
    }

    printf("exit spi %d slave handler\n", intfSel);
    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    printf("usage:  spi slave open [0|1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* -------- Master RAW (no opcode) -------- */
static int _m_raw_write_Cmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 1, 1);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t rwLen = 0;
        if (!parse_u32_dec_hex(argv[3], &rwLen) || rwLen < 1) {
            goto invalid;
        }
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        }

        for (uint32_t i = 0; i < rwLen; i++) {
            testData[i] = (uint8_t)(i + 1);
        }

        if (rwLen == 1) {
            spi_transfer_end_status = WISE_FAIL;
            wise_spi_master_write_byte(intfSel, testData[0]);
            while (spi_transfer_end_status == WISE_FAIL) {
                __NOP();
            }
        } else {
            spi_transfer_end_status = WISE_FAIL;
            wise_spi_master_write(intfSel, testData, rwLen);
            while (spi_transfer_end_status == WISE_FAIL) {
                __NOP();
            }
        }
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    printf("usage:  spi m_raw_write [0|1] [len]\n");
    printf("  MASTER writes N bytes without SPI opcode (raw clocking).\n");
    printf("  Data pattern: 0x01, 0x02, 0x03 ...\n");
    printf("  Example: spi m_raw_write 0 0x20   # dec or 0xHEX\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _m_raw_read_Cmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 1, 1);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t rwLen = 0;
        if (!parse_u32_dec_hex(argv[3], &rwLen)) {
            goto invalid;
        }
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        }

        memset(testData, 0, rwLen);

        if (rwLen == 1) {
            spi_transfer_end_status = WISE_FAIL;
            wise_spi_master_read_byte(intfSel, (uint8_t *)testData);
            while (spi_transfer_end_status == WISE_FAIL) {
                __NOP();
            }
        } else {
            spi_transfer_end_status = WISE_FAIL;
            wise_spi_master_read(intfSel, testData, rwLen);
            while (spi_transfer_end_status == WISE_FAIL) {
                __NOP();
            }
        }

        printf("SPI %d read %lu bytes\n", intfSel, (unsigned long)rwLen);
        dump_buffer((uint8_t *)testData, rwLen);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi m_raw_read [0|1] [len]\n");
    printf("  MASTER reads N bytes without SPI opcode (raw clocking).\n");
    printf("  Example: spi m_raw_read 0 16      # dec or 0xHEX\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* -------- Master with protocol (to Slave) -------- */
static int _m2s_send_Cmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 1, 1);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t rwLen = 0;
        if (!parse_u32_dec_hex(argv[3], &rwLen) || rwLen < 1) {
            goto invalid;
        }
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        }

        for (uint32_t i = 0; i < rwLen; i++) {
            testData[i] = (uint8_t)(i + 1);
        }

        WISE_SPI_XFER_MSG_T trans_msg  = (WISE_SPI_XFER_MSG_T){0};
        trans_msg.msg_fmt             |= SPI_MSG_FMT_CMD_EN;
        trans_msg.trans_mode           = SPI_TM_DMY_WRITE;
        trans_msg.tx_data_buff         = testData;
        trans_msg.tx_unit_count        = rwLen;

        if (spiCfg[intfSel].spi_mode == SPI_MODE_QUAD) {
            trans_msg.cmd       = WRITE_DATA_QUAD_IO;
            trans_msg.dummy_len = 4;
        } else if (spiCfg[intfSel].spi_mode == SPI_MODE_DUAL) {
            trans_msg.cmd       = WRITE_DATA_DUAL_IO;
            trans_msg.dummy_len = 1;
        } else {
            trans_msg.cmd       = WRITE_DATA_SINGLE_IO;
            trans_msg.dummy_len = 1;
        }

        _spi_xfer_and_wait(intfSel, &trans_msg);

        printf("send data : \n");
        dump_buffer((uint8_t *)testData, rwLen);
        printf("SPI %d transmission complete\n", intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    printf("usage:  spi m2s_send [0|1] [len]\n");
    printf("  MASTER -> SLAVE: write N bytes into slave FIFO (opcode depends on IO mode).\n");
    printf("  Data pattern: 0x01, 0x02, 0x03 ...\n");
    printf("  Example: spi m2s_send 0 0x40      # dec or 0xHEX\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _m2s_read_Cmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 1, 1);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t rwLen = 0;
        if (!parse_u32_dec_hex(argv[3], &rwLen)) {
            goto invalid;
        }
        if (rwLen > SPI_TEST_BUF_LEN) {
            rwLen = SPI_TEST_BUF_LEN;
        }

        memset(testData, 0, rwLen);

        WISE_SPI_XFER_MSG_T trans_msg  = (WISE_SPI_XFER_MSG_T){0};
        trans_msg.msg_fmt             |= SPI_MSG_FMT_CMD_EN;
        trans_msg.trans_mode           = SPI_TM_DMY_READ;
        trans_msg.rx_data_buff         = testData;
        trans_msg.rx_unit_count        = rwLen;

        if (spiCfg[intfSel].spi_mode == SPI_MODE_QUAD) {
            trans_msg.cmd       = READ_DATA_QUAD_IO;
            trans_msg.dummy_len = 4;
        } else if (spiCfg[intfSel].spi_mode == SPI_MODE_DUAL) {
            trans_msg.cmd       = READ_DATA_DUAL_IO;
            trans_msg.dummy_len = 1;
        } else {
            trans_msg.cmd       = READ_DATA_SINGLE_IO;
            trans_msg.dummy_len = 1;
        }

        _spi_xfer_and_wait(intfSel, &trans_msg);

        printf("SPI %d read %lu bytes\n", intfSel, (unsigned long)rwLen);
        dump_buffer((uint8_t *)testData, rwLen);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi m2s_read [0|1] [len]\n");
    printf("  MASTER <- SLAVE: read N bytes from slave FIFO (opcode depends on IO mode).\n");
    printf("  Example: spi m2s_read 1 32        # dec or 0xHEX\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _m2s_user_Cmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 1, 1); /* 需要 MASTER 角色 */
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t base = 0;
        if (!parse_u32_dec_hex(argv[3], &base) || base > 0xFF) {
            printf("base must be 0..0xFF\n");
            goto invalid;
        }

        memset(testData, 0, 1);
        testData[0] = (uint8_t)base;

        WISE_SPI_XFER_MSG_T trans_msg  = (WISE_SPI_XFER_MSG_T){0};
        trans_msg.msg_fmt             |= SPI_MSG_FMT_CMD_EN;
        trans_msg.trans_mode           = SPI_TM_DMY_WRITE;
        trans_msg.tx_data_buff         = testData;
        trans_msg.tx_unit_count        = 1;
        trans_msg.cmd                  = USER_CMD;
        trans_msg.dummy_len            = (spiCfg[intfSel].spi_mode == SPI_MODE_QUAD) ? 4 : 1;

        _spi_xfer_and_wait(intfSel, &trans_msg);

        printf("prepared base:\n");
        dump_buffer((uint8_t *)testData, 1);
        printf("SPI %d prepare complete (slave will stage base, base+1, ... into FIFO)\n", intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    printf("usage:  spi m2s_usercmd [0|1] [base]\n");
    printf("  Send USER_CMD + base; slave fills TX FIFO with base, base+1, ... (wrap 0xFF).\n");
    printf("  Example: spi m2s_usercmd 0 0xA5   # dec or 0xHEX\n");
    printf("  - [0|1] : SPI channel index\n");
    printf("  - base  : 0..255\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* -------- Misc -------- */
static int _confCmd(int argc, char **argv)
{
    if (argc == 3) {
        int sel = _check_spi_channel(argc, argv, 2, 0, 0);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        printf("SPI %d config:\n", intfSel);
        printf("    %s\n", (flag & (1 << intfSel)) ? "ENABLE" : "DISABLE");
        printf("    Data width: %d\n", spiCfg[intfSel].data_bit_width);
        printf("    Clock sel: %d  => %lu Hz\n", spiCfg[intfSel].clock_sel, SPI_CLOCK[spiCfg[intfSel].clock_sel]);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi conf [0|1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _clockCmd(int argc, char **argv)
{
    if (argc == 4) {
        int sel = _check_spi_channel(argc, argv, 2, 0, 0);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        uint32_t clock_sel = 0;
        if (!parse_u32_dec_hex(argv[3], &clock_sel)) {
            goto invalid;
        }
        if (clock_sel > E_SPI_CLOCK_SEL_MAX) {
            printf("unsupportd spi clock rate\n");
            goto invalid;
        }
        spiCfg[intfSel].clock_sel = (int)clock_sel;
        printf("set SPI %d clock_sel=%d %luHz\n", intfSel, spiCfg[intfSel].clock_sel, SPI_CLOCK[spiCfg[intfSel].clock_sel]);
        flag &= ~(1 << intfSel);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

invalid:
    printf("usage:  spi clock [0|1] [clock_sel]\n");
    printf("  clock_sel: 0..%d\n", E_SPI_CLOCK_SEL_MAX);
    printf("  Example:   spi clock 0 5\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _closeCmd(int argc, char **argv)
{
    if (argc == 3) {
        int sel = _check_spi_channel(argc, argv, 2, 0, 0);
        if (sel < 0) {
            goto invalid;
        }
        intfSel = (uint8_t)sel;

        if (flag & (1 << intfSel)) {
            wise_spi_close(intfSel);
            flag &= ~(1 << intfSel);
        }
        return SHLCMD_HIST | SHLCMD_DONE;
    }
invalid:
    printf("usage:  spi close [0|1]\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _regCmd(int argc, char **argv)
{
    uint32_t reg      = 0;
    uint32_t value    = 0;
    uint32_t rd_value = 0;

    if (argc == 4) {
        reg   = strtoul(argv[2], NULL, 16);
        value = strtoul(argv[3], NULL, 16);

        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_write(intfSel, reg, &value);
        while (spi_transfer_end_status == WISE_FAIL) {
            __NOP();
        }

        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_read(intfSel, reg, &rd_value);
        while (spi_transfer_end_status == WISE_FAIL) {
            __NOP();
        }

        printf("w %04lx %08lx => %08lx\n", reg, value, rd_value);
    } else if (argc == 3) {
        reg = strtoul(argv[2], NULL, 16);

        spi_transfer_end_status = WISE_FAIL;
        wise_xcvr_register_read(intfSel, reg, &rd_value);
        while (spi_transfer_end_status == WISE_FAIL) {
            __NOP();
        }

        printf("r %04lx => %08lx\n", reg, rd_value);
    } else {
        printf("usage:  spi reg [addr_hex] [value_hex]   # write\n");
        printf("        spi reg [addr_hex]               # read\n");
        printf("Example:\n");
        printf("  spi reg 0x0042 0x00000017\n");
        printf("  spi reg 0x0042\n");
    }

    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif /* SHELL_SPI */
