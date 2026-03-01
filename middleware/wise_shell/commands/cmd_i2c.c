/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */
#include "es_platform_components.h"
#if SHELL_I2C

#include "cmd_i2c.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "shell_func.h"
#include "api/wise_gpio_api.h"
#include "api/wise_i2c_api.h"
#include "api/wise_sys_api.h"
#include "functions/fmt_print.h"

/* ================== Config ================== */
#define DMA_DISALE 0
#define DMA_TX_EN 0x01
#define DMA_RX_EN 0x02
#define I2C_TEST_BUF_LEN 0xFF
#define I2C_LISTEN_INNER_WAIT_MAX (10000u)
/* ================== Globals ================== */
volatile uint8_t i2c_slave_cmd_handler;
volatile int8_t i2c_transfer_end_status      = WISE_FAIL;
volatile int8_t i2c_transfer_addr_Hit_status = WISE_FAIL;
static uint8_t testArray[I2C_TEST_BUF_LEN]   = {0};
static uint8_t recvArray[I2C_TEST_BUF_LEN]   = {0};
static WISE_I2C_CONF_T g_cfg_shadow[2];
static bool g_cfg_valid[2] = {false, false};
/* ================== Declarations ============ */
static int _masterInitCmd(int argc, char **argv);
static int _slaveInitCmd(int argc, char **argv);
static int _sendCmd(int argc, char **argv);
static int _recvCmd(int argc, char **argv);
static int _slaveListenCmd(int argc, char **argv);
static int _writeWithAddrCmd(int argc, char **argv);
static int _readWithAddrCmd(int argc, char **argv);
static int _memReadCmd(int argc, char **argv);
static int _memWriteCmd(int argc, char **argv);

static const struct shellCommand m_CmdList[] = {
    {"master", "i2c master init - type 'i2c master' to see how to use", _masterInitCmd},
    {"slave", "i2c slave init - type 'i2c slave'  to see how to use", _slaveInitCmd},
    {"send", "i2c send [i2cIntf] [Data Count bytes]", _sendCmd},
    {"recv", "i2c recv [i2cIntf] [Data Count bytes]", _recvCmd},
    {"listen", "i2c listen [i2cIntf]", _slaveListenCmd},
    {"waddr", "i2c waddr [i2cIntf] [addr] [count] [data...]", _writeWithAddrCmd},
    {"raddr", "i2c raddr [i2cIntf] [addr] [count]", _readWithAddrCmd},
    {"rmem", "i2c rmem [i2cIntf] [addr] [reg] [reg_len(1..4)] [count]", _memReadCmd},
    {"wmem", "i2c wmem [i2cIntf] [addr] [reg] [reg_len(1..4)] [data...]", _memWriteCmd},
};

int I2CCommandHandler(int argc, char **argv)
{
    return commonCommandHandler(argc, argv, m_CmdList, sizeof(m_CmdList) / sizeof(struct shellCommand));
}

/* ================== Utilities =============== */
static inline void _u32_to_be_bytes(uint32_t v, uint8_t *out, uint8_t len)
{
    for (uint8_t i = 0; i < len; ++i) {
        out[i] = (uint8_t)((v >> (8 * (len - 1 - i))) & 0xFF);
    }
}
static int _parse_token_bytes(const char *token, uint8_t *out, uint32_t *cursor, uint32_t max_len)
{
    const char *p = token;
    while (*p) {
        while (*p == ',' || *p == ' ' || *p == '\t') {
            p++;
        }
        if (!*p) {
            break;
        }

        const char *q = p;
        while (*q && *q != ',') {
            q++;
        }

        char tmp[16];
        size_t n = (size_t)(q - p);
        if (n >= sizeof(tmp)) {
            return -1;
        }
        memcpy(tmp, p, n);
        tmp[n] = '\0';

        uint32_t v;
        if (!parse_u32_dec_hex(tmp, &v) || v > 0xFF) {
            return -1;
        }
        if (*cursor >= max_len) {
            return -1;
        }

        out[(*cursor)++] = (uint8_t)v;

        p = q;
    }
    return 0;
}

static int _parse_bytes_list(int first_arg_idx, int argc, char **argv, uint8_t *out, uint32_t *out_len, uint32_t max_len)
{
    *out_len = 0;
    if (first_arg_idx >= argc) {
        return -1;
    }

    for (int i = first_arg_idx; i < argc; ++i) {
        if (_parse_token_bytes(argv[i], out, out_len, max_len) != 0) {
            return -1;
        }
    }
    return 0;
}

static void _print_hexdump(const uint8_t *buf, uint32_t len, uint32_t bytes_per_line)
{
    if (!buf || len == 0) {
        printf("(no data)\n");
        return;
    }
    if (bytes_per_line == 0) {
        bytes_per_line = 16;
    }

    printf("length = %u bytes\n", (unsigned)len);
    for (uint32_t off = 0; off < len; off += bytes_per_line) {
        uint32_t line_len = (len - off > bytes_per_line) ? bytes_per_line : (len - off);

        printf("%04X: ", (unsigned)off);

        for (uint32_t i = 0; i < bytes_per_line; ++i) {
            if (i < line_len) {
                printf("%02X ", buf[off + i]);
            } else {
                printf("   ");
            }
            if (i == 7) {
                printf(" ");
            }
        }

        printf("\n");
    }
}

static inline int _arm_i2c_receive(uint8_t i2cIntf, uint8_t *buf, uint32_t len)
{
    i2c_transfer_addr_Hit_status = WISE_FAIL;
    i2c_transfer_end_status      = WISE_FAIL;

    wise_i2c_clear_fifo(i2cIntf);
    return wise_i2c_recv_nbyte(i2cIntf, buf, len);
}

static uint8_t _wait_transfer_and_get_count(uint8_t i2cIntf)
{
    uint8_t count = 0;

    i2c_transfer_addr_Hit_status = WISE_FAIL;
    i2c_transfer_end_status      = WISE_FAIL;

    while (i2c_slave_cmd_handler && (i2c_transfer_addr_Hit_status == WISE_FAIL)) {
        extern void _wise_uart_dispatch_task(void);
        _wise_uart_dispatch_task();
    }

    if (!i2c_slave_cmd_handler) {
        return 0;
    }

    printf("\nI2C Address Hit\n");

    for (unsigned wait_loop = 0; wait_loop < I2C_LISTEN_INNER_WAIT_MAX; ++wait_loop) {
        count = wise_i2c_get_data_count(i2cIntf);
        if (i2c_transfer_end_status == WISE_SUCCESS || count != 0) {
            break;
        }
        extern void _wise_uart_dispatch_task(void);
        _wise_uart_dispatch_task();
    }

    if (i2c_transfer_end_status != WISE_SUCCESS) {
        count = wise_i2c_get_data_count(i2cIntf);
    }

    return count;
}

/* ================== Callbacks =============== */
static void _i2c_event_cb(uint8_t i2c_channel, uint32_t i2c_event, void *context)
{
    (void)i2c_channel;
    (void)context;
    if (i2c_event == WISE_I2C_EVENT_TRANSFER_DONE) {
        i2c_transfer_end_status = WISE_SUCCESS;
    }
    if (i2c_event == WISE_I2C_EVENT_ADDRESS_HIT) {
        i2c_transfer_addr_Hit_status = WISE_SUCCESS;
    }
}
/* ================== GPIO ==================== */
static void _i2c_io_config(uint8_t i2c_channel)
{
    if (i2c_channel > 1) {
        printf("[I2C] invalid channel: %u\n", i2c_channel);
        return;
    }

    bool need_tcxo = false;
    bool need_swd  = false;

#if ES_DEVICE_PIO
    /* { func, pin } */
    const uint8_t i2c_pio_cfg[2][2][2] = {
        {   /* I2C0 */
            { MODE_PIO_FUNC_I2C0_SCL, 2 },
            { MODE_PIO_FUNC_I2C0_SDA, 3 },
        },
        {   /* I2C1 */
            { MODE_PIO_FUNC_I2C1_SCL, 7 },
            { MODE_PIO_FUNC_I2C1_SDA, 8 },
        }
    };

    printf("[I2C%u][PIO] configuring 2 pins\n", i2c_channel);

    /* First pass: check used pins */
    for (int i = 0; i < 2; i++) {
        uint8_t pin = i2c_pio_cfg[i2c_channel][i][1];

        if (pin == 10) need_tcxo = true;
        if (pin == 14 || pin == 15) need_swd = true;
    }

#else
    /* Non-PIO behavior (your original table) */
    WISE_GPIO_CFG_T i2c_io_cfg[2][2] = {
        {   /* I2C0 */
            {4, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
            {5, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}
        },
        {   /* I2C1 */
            {8, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE},
            {9, MODE_PERI_1, GPIO_DIR_INPUT, GPIO_INT_DISABLE, GPIO_INT_TYPE_NONE}
        }
    };

    printf("[I2C%u][GPIO] configuring 2 pins\n", i2c_channel);

    /* First pass: detect if pin10/14/15 are used */
    for (int i = 0; i < 2; i++) {
        uint8_t pin = i2c_io_cfg[i2c_channel][i].pin;

        if (pin == 10) need_tcxo = true;
        if (pin == 14 || pin == 15) need_swd = true;
    }
#endif

    /* Disable SWD / TCXO only if needed */
    if (need_swd) {
        wise_sys_swd_config(false);
        printf("  [CFG] SWD disabled (pin14/15 in use)\n");
    }

    if (need_tcxo) {
        wise_sys_tcxo_config(false);
        printf("  [CFG] TCXO disabled (pin10 in use)\n");
    }

    /* Second pass: apply config */
#if ES_DEVICE_PIO
    for (int i = 0; i < 2; i++) {
        uint8_t func = i2c_pio_cfg[i2c_channel][i][0];
        uint8_t pin  = i2c_pio_cfg[i2c_channel][i][1];

        wise_gpio_func_cfg(pin, func);

        printf("  Pin %2u -> %-12s (func=%u)\n",
               pin, gpio_pin_func_to_str(func), func);
    }
#else
    for (int i = 0; i < 2; i++) {
        wise_gpio_cfg(&i2c_io_cfg[i2c_channel][i]);

        printf("  Pin %2u -> mode=%d, dir=%d\n",
               i2c_io_cfg[i2c_channel][i].pin,
               i2c_io_cfg[i2c_channel][i].mode,
               i2c_io_cfg[i2c_channel][i].dir);
    }
#endif
}

/* ================== Shared helpers ========== */

typedef struct {
    uint8_t i2cIntf;
    uint8_t mode;         /* 0 = Transmitter / 1 = Receiver */
    uint8_t speed_mode;   /* 0/1/2 = 100k/400k/1M */
    uint8_t addressing;   /* 0 = 7b, 1 = 10b */
    uint16_t target_addr; /* HEX */
    uint8_t dmaEn;
} I2CInitArgs;

static int _parse_i2c_init_args(int argc, char **argv, I2CInitArgs *out)
{
    if (argc != 9 || strcmp(argv[2], "init") != 0) {
        return -1;
    }

    out->i2cIntf     = (uint8_t)strtoul(argv[3], NULL, 10);
    out->mode        = (uint8_t)strtoul(argv[4], NULL, 10);
    out->speed_mode  = (uint8_t)strtoul(argv[5], NULL, 10);
    out->addressing  = (uint8_t)strtoul(argv[6], NULL, 10);
    out->target_addr = (uint16_t)strtoul(argv[7], NULL, 16);
    out->dmaEn       = (uint8_t)strtoul(argv[8], NULL, 10);

    if (out->i2cIntf > 1 || out->mode > 1 || out->speed_mode > 2 || out->addressing > 1 || (out->addressing == 1 && out->target_addr > 0x3FF) ||
        (out->addressing == 0 && out->target_addr > 0x7F) || out->dmaEn > 1) {
        return -1;
    }
    return 0;
}

static void _print_cfg(const char *role, const I2CInitArgs *a, uint8_t adjusted_dir)
{
    printf("I2C %d %s config:\n", a->i2cIntf, role);
    printf("mode = %s\n", a->mode ? "Transmitter" : "Receiver");
    printf("speed = %s\n", a->speed_mode == 0 ? "100 kb/s" : (a->speed_mode == 1 ? "400 kb/s" : "1 Mb/s"));
    printf("addressing = %s\n", a->addressing ? "10-bit mode" : "7-bit mode");
    printf("target_address = 0x%x\n", a->target_addr);
    printf("dmaEn = 0x%d\n", a->dmaEn);
    (void)adjusted_dir;
}

static int _register_cb(uint8_t idx)
{
    if (wise_i2c_register_event_callback(idx, _i2c_event_cb, NULL) == WISE_SUCCESS) {
        printf("i2c %d callback registered\n", idx);
        i2c_transfer_end_status = WISE_FAIL;
        return 0;
    }
    printf("i2c callback register failed for channel %d\n", idx);
    return -1;
}

/* role = I2C_MASTER or I2C_SLAVE；SUCESS return 0  */
static int _do_i2c_init(uint8_t role, const I2CInitArgs *a)
{
    uint8_t adjusted_dir = a->mode;
    if (role == I2C_MASTER) {
        adjusted_dir = a->mode ? 0 : 1;
    }

    WISE_I2C_CONF_T cfg = {.i2c_idx        = a->i2cIntf,
                           .role           = role,
                           .i2cEn          = I2C_ENABLE,
                           .dmaEn          = a->dmaEn ? (DMA_TX_EN | DMA_RX_EN) : 0,
                           .speedMode      = a->speed_mode,
                           .addressing     = a->addressing,
                           .dir            = adjusted_dir,
                           .target_address = (uint8_t)a->target_addr};

    _print_cfg(role == I2C_MASTER ? "master" : "slave", a, adjusted_dir);

    wise_i2c_init();
    wise_i2c_open(a->i2cIntf);

    (void)_register_cb(a->i2cIntf);
    _i2c_io_config(a->i2cIntf);

    if (wise_i2c_config(a->i2cIntf, &cfg) == WISE_FAIL) {
        printf("I2C %s initialization failed\n", role == I2C_MASTER ? "master" : "slave");
    } else {
        if (role == I2C_MASTER) {
            g_cfg_shadow[a->i2cIntf] = cfg;
            g_cfg_valid[a->i2cIntf]  = true;
        }
    }
    return 0;
}

static int _wait_for_transfer_done(void)
{
    while (i2c_transfer_end_status == WISE_FAIL) {
        __NOP();
    }
    return (i2c_transfer_end_status == WISE_SUCCESS) ? 0 : -1;
}

static int _do_transfer_once(uint8_t i2cIntf, uint8_t *buf, uint32_t nbytes, bool is_send)
{
    i2c_transfer_end_status = WISE_FAIL;

    int32_t rc = is_send ? wise_i2c_send_nbyte(i2cIntf, buf, nbytes) : wise_i2c_recv_nbyte(i2cIntf, buf, nbytes);

    if (rc == WISE_FAIL) {
        printf("I2C %d %s failed\n", i2cIntf, is_send ? "transmission" : "reception");
        return -1;
    }

    printf("I2C %d %s start (count=%u)\n", i2cIntf, is_send ? "transmission" : "reception", (unsigned)nbytes);
    printf("wait to %s...\n", is_send ? "send" : "recv");

    if (_wait_for_transfer_done() != 0) {
        printf("I2C %d %s wait failed\n", i2cIntf, is_send ? "transmission" : "reception");
        return -1;
    }

    if (is_send) {
        printf("send data:\n");
        _print_hexdump(buf, nbytes, 16);
        printf("I2C %d transmission complete\n", i2cIntf);
    } else {
        printf("recv data:\n");
        _print_hexdump(buf, nbytes, 16);
        printf("I2C %d recvive complete\n", i2cIntf);
    }
    return 0;
}

static inline void _print_listen_banner(uint8_t i2cIntf, bool with_intro)
{
    printf("I2C %d reception start\n", i2cIntf);
    printf("Slave Listening ...\n");
    if (with_intro) {
        printf("[Info] Press Ctrl+G to leave slave listen mode.\n");
    }
}

static inline void _dump_rx_decimal(const uint8_t *buf, uint8_t len)
{
    printf("recv data : \n");
    for (uint8_t i = 0; i < len; i++) {
        printf("%u ", buf[i]);
    }
    printf("\n");
}

/* ================== Commands ================= */

static int _masterInitCmd(int argc, char **argv)
{
    I2CInitArgs a;
    if (_parse_i2c_init_args(argc, argv, &a) != 0) {
        printf("usage: i2c master init\n"
               "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
               "       mode [0 = Transmitter / 1 = Receiver]\n"
               "       speed_mode [0 = 100 kb/s / 1 = 400 kb/s / 2 = 1 Mb/s]\n"
               "       addressing [0 = 7-bit / 1 = 10-bit]\n"
               "       target_address (HEX)\n"
               "       dmaEn [0 = disable / 1 = enable]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    (void)_do_i2c_init(I2C_MASTER, &a);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _slaveInitCmd(int argc, char **argv)
{
    I2CInitArgs a;
    if (_parse_i2c_init_args(argc, argv, &a) != 0) {
        printf("usage: i2c slave init\n"
               "       i2cIntf [0 = I2C0 / 1 = I2C1]\n"
               "       mode [0 = Transmitter / 1 = Receiver]\n"
               "       speed_mode [0 = 100 kb/s / 1 = 400 kb/s / 2 = 1 Mb/s]\n"
               "       addressing [0 = 7-bit / 1 = 10-bit]\n"
               "       target_address (HEX)\n"
               "       dmaEn [0 = disable / 1 = enable]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    (void)_do_i2c_init(I2C_SLAVE, &a);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _sendCmd(int argc, char **argv)
{
    if (argc != 4) {
        printf("usage: i2c send [i2cIntf] [wanted n bytes (DEC or 0xHEX)]\n"
               "       i2cIntf [0 = I2C0 / 1 = I2C1]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t dataCnt;
    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &dataCnt)) {
        printf("usage: i2c send [i2cIntf] [wanted n bytes (DEC or 0xHEX)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (dataCnt > I2C_TEST_BUF_LEN) {
        dataCnt = I2C_TEST_BUF_LEN;
    }

    for (uint32_t i = 0; i < sizeof(testArray); i++) {
        testArray[i] = (uint8_t)i;
    }
    (void)_do_transfer_once(i2cIntf, testArray, dataCnt, /*is_send=*/true);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _recvCmd(int argc, char **argv)
{
    if (argc != 4) {
        printf("usage: i2c recv [i2cIntf] [wanted n bytes (DEC or 0xHEX)]\n"
               "       i2cIntf [0 = I2C0 / 1 = I2C1]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t dataCnt;
    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &dataCnt)) {
        printf("usage: i2c recv [i2cIntf] [wanted n bytes (DEC or 0xHEX)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (dataCnt > I2C_TEST_BUF_LEN) {
        dataCnt = I2C_TEST_BUF_LEN;
    }

    (void)_do_transfer_once(i2cIntf, recvArray, dataCnt, /*is_send=*/false);
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* ========== listen ========== */
static int _slaveListenCmd(int argc, char **argv)
{
    if (argc != 3) {
        printf("usage: i2c listen [i2cIntf]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    if (i2cIntf > 1) {
        printf("usage: i2c listen [i2cIntf]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (_arm_i2c_receive(i2cIntf, recvArray, I2C_TEST_BUF_LEN) == WISE_FAIL) {
        printf("I2C %d reception failed\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    printf("I2C %d reception start\n", i2cIntf);
    printf("Slave Listening ...\n");
    printf("[Info] Press Ctrl+G to leave slave listen mode.\n");

    i2c_slave_cmd_handler = 1;

    while (i2c_slave_cmd_handler) {
        uint8_t current_count = _wait_transfer_and_get_count(i2cIntf);
        if (!i2c_slave_cmd_handler) {
            break;
        }

        if (current_count == 0) {
            if (_arm_i2c_receive(i2cIntf, recvArray, I2C_TEST_BUF_LEN) == WISE_FAIL) {
                printf("I2C %d reception failed\n", i2cIntf);
                break;
            }
            printf("I2C %d reception start\n", i2cIntf);
            printf("Slave Listening ...\n");
            continue;
        }

        if (_wait_for_transfer_done() != 0) {
            printf("I2C %d %s wait failed\n", i2cIntf, "reception");
            return -1;
        }
        //        _dump_rx_decimal(recvArray, current_count);
        _print_hexdump(recvArray, current_count, 16);

        printf("I2C %d recvive complete\n", i2cIntf);

        if (_arm_i2c_receive(i2cIntf, recvArray, I2C_TEST_BUF_LEN) == WISE_FAIL) {
            printf("I2C %d reception failed\n", i2cIntf);
            break;
        }
        printf("I2C %d reception start\n", i2cIntf);
        printf("Slave Listening ...\n");
    }

    printf("Exit slave listen mode.\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _writeWithAddrCmd(int argc, char **argv)
{
    if (argc < 6) {
        printf("usage: i2c waddr [i2cIntf] [addr(DEC/0xHEX)] [count(DEC/0xHEX)] [data...]\n"
               " - data support DEC or 0xHEX\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t addr_u32, nbytes_u32;
    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &addr_u32) || !parse_u32_dec_hex(argv[4], &nbytes_u32)) {
        printf("usage: i2c waddr [i2cIntf] [addr] [count] [data...]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (!g_cfg_valid[i2cIntf]) {
        printf("I2C %d not initialized as master. Use 'i2c master init ...' first.\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (nbytes_u32 == 0 || nbytes_u32 > I2C_TEST_BUF_LEN) {
        printf("count must be 1..%u\n", (unsigned)I2C_TEST_BUF_LEN);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint32_t parsed_len = 0;
    if (_parse_bytes_list(5, argc, argv, testArray, &parsed_len, I2C_TEST_BUF_LEN) != 0) {
        printf("parse data bytes failed (accept DEC or 0xHEX; separated by space/comma)\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (parsed_len != nbytes_u32) {
        printf("data count mismatch: expect %u, got %u\n", (unsigned)nbytes_u32, (unsigned)parsed_len);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (g_cfg_shadow[i2cIntf].addressing == I2C_ADDR_10_BITS) {
        if (addr_u32 > 0x3FF) {
            printf("addr out of range for 10-bit\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    } else {
        if (addr_u32 > 0x7F) {
            printf("addr out of range for 7-bit\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    WISE_I2C_CONF_T cfg = g_cfg_shadow[i2cIntf];
    cfg.target_address  = (uint8_t)addr_u32;
    cfg.dir             = 1;

    if (wise_i2c_config(i2cIntf, &cfg) == WISE_FAIL) {
        printf("I2C %d config failed (waddr)\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    (void)_do_transfer_once(i2cIntf, testArray, (uint32_t)parsed_len, /*is_send=*/true);
    return SHLCMD_HIST | SHLCMD_DONE;
}

static int _readWithAddrCmd(int argc, char **argv)
{
    if (argc != 5) {
        printf("usage: i2c raddr [i2cIntf] [addr(DEC/0xHEX)] [count(DEC/0xHEX)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t addr_u32, nbytes_u32;
    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &addr_u32) || !parse_u32_dec_hex(argv[4], &nbytes_u32)) {
        printf("usage: i2c raddr [i2cIntf] [addr] [count]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (!g_cfg_valid[i2cIntf]) {
        printf("I2C %d not initialized as master. Use 'i2c master init ...' first.\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (nbytes_u32 == 0 || nbytes_u32 > I2C_TEST_BUF_LEN) {
        printf("count must be 1..%u\n", (unsigned)I2C_TEST_BUF_LEN);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (g_cfg_shadow[i2cIntf].addressing == I2C_ADDR_10_BITS) {
        if (addr_u32 > 0x3FF) {
            printf("addr out of range for 10-bit\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    } else {
        if (addr_u32 > 0x7F) {
            printf("addr out of range for 7-bit\n");
            return SHLCMD_HIST | SHLCMD_DONE;
        }
    }

    WISE_I2C_CONF_T cfg = g_cfg_shadow[i2cIntf];
    cfg.target_address  = (uint8_t)addr_u32;
    cfg.dir             = 0;

    if (wise_i2c_config(i2cIntf, &cfg) == WISE_FAIL) {
        printf("I2C %d config failed (raddr)\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    (void)_do_transfer_once(i2cIntf, recvArray, (uint32_t)nbytes_u32, /*is_send=*/false);
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* ========== Combined Demo: read memory/register ========== */
static int _memReadCmd(int argc, char **argv)
{
    if (argc != 7) {
        printf("usage: i2c rmem [i2cIntf] [addr(DEC/0xHEX)] [reg(DEC/0xHEX)] [reg_len(1..4)] [count(DEC/0xHEX)]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t addr_u32, reg_u32, reg_len_u32, count_u32;

    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &addr_u32) || !parse_u32_dec_hex(argv[4], &reg_u32) ||
        !parse_u32_dec_hex(argv[5], &reg_len_u32) || !parse_u32_dec_hex(argv[6], &count_u32)) {
        printf("usage: i2c rmem [i2cIntf] [addr] [reg] [reg_len] [count]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (!g_cfg_valid[i2cIntf]) {
        printf("I2C %d not initialized as master. Use 'i2c master init ...' first.\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (reg_len_u32 == 0 || reg_len_u32 > 4) {
        printf("reg_len must be 1..4\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (count_u32 == 0 || count_u32 > I2C_TEST_BUF_LEN) {
        printf("count must be 1..%u\n", (unsigned)I2C_TEST_BUF_LEN);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_ten = (g_cfg_shadow[i2cIntf].addressing == I2C_ADDR_10_BITS);
    if ((!is_ten && addr_u32 > 0x7F) || (is_ten && addr_u32 > 0x3FF)) {
        printf("addr out of range for %s-bit\n", is_ten ? "10" : "7");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t reg_hdr[4];
    _u32_to_be_bytes(reg_u32, reg_hdr, (uint8_t)reg_len_u32);

    uint8_t flags0 = is_ten ? WISE_I2C_M_TEN : 0;
    uint8_t flags1 = (is_ten ? WISE_I2C_M_TEN : 0) | WISE_I2C_M_RD;

    WISE_I2C_XFER_MSG_T seq[2] = {
        {.addr = (uint16_t)addr_u32, .flags = flags0, .buf = reg_hdr, .len = (uint16_t)reg_len_u32},
        {.addr = (uint16_t)addr_u32, .flags = flags1, .buf = recvArray, .len = (uint16_t)count_u32},
    };

    i2c_transfer_end_status = WISE_FAIL;
    int32_t rc              = wise_i2c_transfer(i2cIntf, seq, 2, /*interval_ms=*/5);
    if (rc == WISE_FAIL) {
        printf("I2C %d rmem transfer failed\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    printf("I2C %d rmem start (addr=0x%X, reg=0x%X, reg_len=%u, count=%u)\n", i2cIntf, (unsigned)addr_u32, (unsigned)reg_u32, (unsigned)reg_len_u32,
           (unsigned)count_u32);
    printf("wait to recv...\n");

    (void)_wait_for_transfer_done();
    if (i2c_transfer_end_status == WISE_FAIL) {
        printf("fail:\n");
    }

    printf("recv data:\n");
    _print_hexdump(recvArray, (uint32_t)count_u32, 16);
    printf("I2C %d rmem complete\n", i2cIntf);
    return SHLCMD_HIST | SHLCMD_DONE;
}

/* ========== Combined Demo: Write memory/register ========== */
static int _memWriteCmd(int argc, char **argv)
{
    if (argc < 7) {
        printf("usage: i2c wmem [i2cIntf] [addr(DEC/0xHEX)] [reg(DEC/0xHEX)] [reg_len(1..4)] [data...]\n"
               " - data support DEC or 0xHEX，total length <= 255\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t i2cIntf = (uint8_t)strtoul(argv[2], NULL, 10);
    uint32_t addr_u32, reg_u32, reg_len_u32;

    if (i2cIntf > 1 || !parse_u32_dec_hex(argv[3], &addr_u32) || !parse_u32_dec_hex(argv[4], &reg_u32) ||
        !parse_u32_dec_hex(argv[5], &reg_len_u32)) {
        printf("usage: i2c wmem [i2cIntf] [addr] [reg] [reg_len] [data...]\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    if (!g_cfg_valid[i2cIntf]) {
        printf("I2C %d not initialized as master. Use 'i2c master init ...' first.\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (reg_len_u32 == 0 || reg_len_u32 > 4) {
        printf("reg_len must be 1..4\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint32_t data_len = 0;
    if (_parse_bytes_list(6, argc, argv, testArray, &data_len, I2C_TEST_BUF_LEN) != 0) {
        printf("parse data bytes failed (accept DEC or 0xHEX; separated by space/comma)\n");
        return SHLCMD_HIST | SHLCMD_DONE;
    }
    if (data_len == 0 || (reg_len_u32 + data_len) > I2C_TEST_BUF_LEN) {
        printf("total length (reg_len + data) must be 1..%u\n", (unsigned)I2C_TEST_BUF_LEN);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    bool is_ten = (g_cfg_shadow[i2cIntf].addressing == I2C_ADDR_10_BITS);
    if ((!is_ten && addr_u32 > 0x7F) || (is_ten && addr_u32 > 0x3FF)) {
        printf("addr out of range for %s-bit\n", is_ten ? "10" : "7");
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    uint8_t txbuf[4 + I2C_TEST_BUF_LEN];
    _u32_to_be_bytes(reg_u32, txbuf, (uint8_t)reg_len_u32);
    memcpy(&txbuf[reg_len_u32], testArray, data_len);
    uint16_t total = (uint16_t)(reg_len_u32 + data_len);

    WISE_I2C_XFER_MSG_T m = {.addr = (uint16_t)addr_u32, .flags = is_ten ? WISE_I2C_M_TEN : 0, .buf = txbuf, .len = total};

    i2c_transfer_end_status = WISE_FAIL;
    int32_t rc              = wise_i2c_transfer(i2cIntf, &m, 1, /*timeout_ms=*/0);
    if (rc == WISE_FAIL) {
        printf("I2C %d wmem transfer failed\n", i2cIntf);
        return SHLCMD_HIST | SHLCMD_DONE;
    }

    printf("I2C %d wmem start (addr=0x%X, reg=0x%X, reg_len=%u, data_len=%u)\n", i2cIntf, (unsigned)addr_u32, (unsigned)reg_u32,
           (unsigned)reg_len_u32, (unsigned)data_len);
    printf("wait to send...\n");

    (void)_wait_for_transfer_done();

    printf("send data (reg+payload):\n");
    _print_hexdump(txbuf, total, 16);
    printf("I2C %d wmem complete\n", i2cIntf);
    return SHLCMD_HIST | SHLCMD_DONE;
}

#endif /* SHELL_I2C */
