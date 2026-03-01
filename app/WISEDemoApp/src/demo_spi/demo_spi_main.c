/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_spi_main.c
 * @brief Example application demonstrating WISE_SPI master/slave transfer in regular mode.
 *
 * @ingroup WISE_EXAMPLE_APP_SPI
 *
 * This demo demonstrates basic SPI master/slave communication using WISE SPI
 * driver in REGULAR mode.
 *
 * Build-time role selection:
 * - Define @ref DEMO_SPI_ROLE to one of:
 *   - @ref DEMO_SPI_ROLE_MASTER
 *   - @ref DEMO_SPI_ROLE_SLAVE
 *
 * Hardware notes:
 * - Connect SPI signals between master and slave: CLK, CS, MOSI, MISO
 * - Additional pins HOLD/WP are configured if QUAD mode is selected.
 * - Pin mapping is defined by @ref spi_pio_cfg.
 *
 * Functional notes:
 * - Transfer is performed in non-blocking mode and waits for
 *   @ref WISE_SPI_EVENT_TRANSFER_DONE event.
 * - Master sends a command (SPI_MSG_FMT_CMD_EN) followed by payload data.
 * - Slave prepares an RX buffer and waits for the transfer completion event.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "demo_app_common.h"
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_spi_api.h"
#include "wise_sys_api.h"

/**
 * @defgroup WISE_EXAMPLE_APP_SPI SPI Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: SPI master/slave transfer demo.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_SPI  for SPI configuration and transfers
 * - @ref WISE_GPIO for pin function configuration (PIO mapping)
 * - @ref WISE_SYS  for SWD/TCXO pin mux handling (when special pins are used)
 *
 * The demo supports two build roles selected by @ref DEMO_SPI_ROLE:
 * - @ref DEMO_SPI_ROLE_MASTER
 * - @ref DEMO_SPI_ROLE_SLAVE
 * @{
 */

/* ============================================================
 * Build-time role select
 * ============================================================ */
#define DEMO_SPI_ROLE_MASTER 1 /**< Build as SPI master role. */
#define DEMO_SPI_ROLE_SLAVE  2 /**< Build as SPI slave role. */

#ifndef DEMO_SPI_ROLE
/** @brief Default SPI build role when DEMO_SPI_ROLE is not defined. */
#define DEMO_SPI_ROLE DEMO_SPI_ROLE_MASTER
#endif

/* ============================================================
 * Config
 * ============================================================ */
#define SPI_CHANNEL        0               /**< SPI channel index used by this demo. */
#define SPI_MODE_TYPE      SPI_MODE_REGULAR/**< SPI operating mode (driver enum). */
#define SPI_CLOCK_SEL      E_SPI_CLOCK_SEL_1M /**< SPI clock selection (driver enum). */
#define SPI_DATA_BIT_WIDTH 8               /**< SPI data bit width (bits per unit). */
#define TEST_DATA_SIZE     16              /**< Test payload length in bytes. */

/* ============================================================
 * SPI PIO mapping
 * ============================================================ */

/**
 * @brief SPI pin function mapping table.
 *
 * Layout:
 * - First dimension: SPI channel index (0..1)
 * - Second dimension: signal index in order:
 *   - CLK, CS, MOSI, MISO, HOLD, WP
 * - Third dimension: { function, pin }
 *
 * @note In REGULAR mode, only CLK/CS/MOSI/MISO are used.
 *       HOLD/WP are used when QUAD mode is selected.
 */
static const uint8_t spi_pio_cfg[2][6][2] = {
    {
        {MODE_PIO_FUNC_SPI0_CLK,  2},
        {MODE_PIO_FUNC_SPI0_CS,   3},
        {MODE_PIO_FUNC_SPI0_MOSI, 4},
        {MODE_PIO_FUNC_SPI0_MISO, 5},
        {MODE_PIO_FUNC_SPI0_HOLD, 6},
        {MODE_PIO_FUNC_SPI0_WP,   7},
    },
    {
        {MODE_PIO_FUNC_SPI1_CLK,  6},
        {MODE_PIO_FUNC_SPI1_CS,   7},
        {MODE_PIO_FUNC_SPI1_MOSI, 8},
        {MODE_PIO_FUNC_SPI1_MISO, 9},
        {MODE_PIO_FUNC_SPI1_HOLD, 10},
        {MODE_PIO_FUNC_SPI1_WP,   11},
    }
};

/* ============================================================
 * Globals
 * ============================================================ */

/** @brief TX buffer used by master role. */
static uint8_t tx_buf[TEST_DATA_SIZE] __attribute__((aligned(4)));

/** @brief RX buffer used by slave role. */
static uint8_t rx_buf[TEST_DATA_SIZE] __attribute__((aligned(4)));

/**
 * @brief Transfer completion flag.
 *
 * Set to 1 when @ref WISE_SPI_EVENT_TRANSFER_DONE is received in callback.
 */
static volatile int8_t g_xfer_done = 0;

/* ============================================================
 * SPI callback
 * ============================================================ */

/**
 * @brief SPI event callback registered by ::wise_spi_register_event_callback().
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_event   SPI event ID.
 * @param[in] context     User context pointer passed during registration.
 */
static void demo_spi_callback(uint8_t spi_channel,
                              uint32_t spi_event,
                              void *context)
{
    (void)context;

    if (spi_event == WISE_SPI_EVENT_TRANSFER_DONE) {
        g_xfer_done = 1;
        printf("[SPI%u] TRANSFER_DONE\n", spi_channel);
    }
}

/* ============================================================
 * SPI IO config
 * ============================================================ */

/**
 * @brief Configure SPI pin mux (PIO) for a given channel and mode.
 *
 * This helper:
 * - Determines the required number of pins based on SPI mode:
 *   - QUAD : 6 pins (CLK/CS/MOSI/MISO/HOLD/WP)
 *   - 3WIRE: 3 pins (CLK/CS/MOSI) (driver-defined)
 *   - others: 4 pins (CLK/CS/MOSI/MISO)
 * - Disables SWD/TCXO if the selected pins overlap with special functions
 * - Calls ::wise_gpio_func_cfg() to apply the pin functions
 *
 * @param[in] spi_channel SPI channel index.
 * @param[in] spi_mode    SPI mode (driver enum).
 */
static void demo_spi_io_config(uint8_t spi_channel,
                               WISE_SPI_MODE_T spi_mode)
{
    int num_pins = (spi_mode == SPI_MODE_QUAD)  ? 6 :
                   (spi_mode == SPI_MODE_3WIRE) ? 3 : 4;

    bool need_tcxo = false;
    bool need_swd  = false;

    if (spi_channel > 1) {
        printf("[SPI] invalid channel: %u\n", spi_channel);
        return;
    }

    for (int i = 0; i < num_pins; i++) {
        uint8_t pin = spi_pio_cfg[spi_channel][i][1];
        if (pin == 10) {
            need_tcxo = true;
        }
        if (pin == 14 || pin == 15) {
            need_swd  = true;
        }
    }

    if (need_swd) {
        wise_sys_swd_config(false);
    }
    if (need_tcxo) {
        wise_sys_tcxo_config(false);
    }

    for (int i = 0; i < num_pins; i++) {
        wise_gpio_func_cfg(spi_pio_cfg[spi_channel][i][1],
                           spi_pio_cfg[spi_channel][i][0]);
    }
}

/* ============================================================
 * Slave listen
 * ============================================================ */

/**
 * @brief Configure slave RX transfer once and set slave ready.
 *
 * Prepares an RX transfer message in read-only mode and arms the slave for
 * one incoming transfer.
 *
 * @param[in] recv_count Number of data units to receive.
 */
static void demo_slave_listen_once(uint16_t recv_count)
{
    WISE_SPI_XFER_MSG_T msg = {0};

    memset(rx_buf, 0, sizeof(rx_buf));

    msg.trans_mode    = SPI_TM_READ_ONLY;
    msg.dummy_len     = 1;
    msg.rx_data_buff  = rx_buf;
    msg.rx_unit_count = recv_count;

    g_xfer_done = 0;

    wise_spi_reset(SPI_CHANNEL, WISE_SPI_RESET_RX);
    wise_spi_msg_xfer(SPI_CHANNEL, &msg);
    wise_spi_slave_set_ready(SPI_CHANNEL);
}

/**
 * @brief Start an SPI transfer and busy-wait until completion.
 *
 * This helper sends a transfer request by ::wise_spi_msg_xfer(), then waits
 * until @ref g_xfer_done is set by the SPI event callback.
 *
 * @param[in] ch  SPI channel index.
 * @param[in] msg Transfer message descriptor.
 */
static void spi_xfer_and_wait(uint8_t ch,
                              WISE_SPI_XFER_MSG_T *msg)
{
    g_xfer_done = WISE_FAIL;
    wise_spi_msg_xfer(ch, msg);
    while (g_xfer_done == WISE_FAIL) {
        __NOP();
    }
}

/* ============================================================
 * Main
 * ============================================================ */

/**
 * @brief Main entry of the SPI demo application.
 *
 * Common flow:
 * 1. Initialize demo environment
 * 2. Configure SPI pin mux
 * 3. Initialize SPI module and register event callback
 * 4. Configure SPI and open as master or slave
 * 5. Master: send command + payload
 * 6. Slave : arm RX buffer and wait for completion
 *
 * @return This demo does not return under normal operation.
 */
int main(void)
{
    WISE_STATUS st;

    demo_app_common_init();

    printf("\n=== SPI Demo (%s) ===\n",
#if (DEMO_SPI_ROLE == DEMO_SPI_ROLE_MASTER)
           "MASTER"
#else
           "SLAVE"
#endif
    );

    demo_spi_io_config(SPI_CHANNEL, SPI_MODE_TYPE);

    st = wise_spi_init();
    if (st != WISE_SUCCESS) {
        printf("[ERR] wise_spi_init failed\n");
        return 0;
    }

    wise_spi_register_event_callback(SPI_CHANNEL,
                                     demo_spi_callback,
                                     NULL);

    WISE_SPI_CONF_T cfg = {0};
    cfg.clock_mode     = CLOCK_MODE0;
    cfg.spi_mode       = SPI_MODE_TYPE;
    cfg.role           = (DEMO_SPI_ROLE == DEMO_SPI_ROLE_MASTER)
                           ? E_SPI_ROLE_MASTER
                           : E_SPI_ROLE_SLAVE;
    cfg.data_bit_width = SPI_DATA_BIT_WIDTH;
    cfg.addr_len       = 2;
    cfg.clock_sel      = SPI_CLOCK_SEL;
    cfg.bit_order      = SPI_MSB_FIRST;
    cfg.data_merge     = 1;
    cfg.addr_fmt       = 0;
    cfg.block_mode     = E_SPI_NONBLOCK_MODE;
    cfg.dma_enable     = 0;

#if (DEMO_SPI_ROLE == DEMO_SPI_ROLE_MASTER)

    st = wise_spi_master_open(SPI_CHANNEL, &cfg);
    if (st != WISE_SUCCESS) {
        printf("[ERR] master_open failed\n");
        return 0;
    }

    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        tx_buf[i] = (uint8_t)(i + 1);
    }

    WISE_SPI_XFER_MSG_T msg = {0};
    msg.msg_fmt      |= SPI_MSG_FMT_CMD_EN;
    msg.trans_mode    = SPI_TM_DMY_WRITE;
    msg.tx_data_buff  = tx_buf;
    msg.tx_unit_count = TEST_DATA_SIZE;

    if (cfg.spi_mode == SPI_MODE_QUAD) {
        msg.cmd       = WRITE_DATA_QUAD_IO;
        msg.dummy_len = 4;
    } else if (cfg.spi_mode == SPI_MODE_DUAL) {
        msg.cmd       = WRITE_DATA_DUAL_IO;
        msg.dummy_len = 1;
    } else {
        msg.cmd       = WRITE_DATA_SINGLE_IO;
        msg.dummy_len = 1;
    }

    spi_xfer_and_wait(SPI_CHANNEL, &msg);

    printf("Master TX: ");
    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        printf("%02X ", tx_buf[i]);
    }
    printf("\n");

#else /* SLAVE */

    st = wise_spi_slave_open(SPI_CHANNEL, &cfg);
    if (st != WISE_SUCCESS) {
        printf("[ERR] slave_open failed\n");
        return 0;
    }

    printf("Slave listening...\n");
    demo_slave_listen_once(TEST_DATA_SIZE);

    while (!g_xfer_done) {
        __NOP();
    }

    printf("Slave RX: ");
    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        printf("%02X ", rx_buf[i]);
    }
    printf("\n");

#endif

    while (1) {
        __WFI();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_SPI */
