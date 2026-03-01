/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_i2c_main.c
 * @brief Example application demonstrating WISE_I2C master/slave communication.
 *
 * @ingroup WISE_EXAMPLE_APP_I2C
 *
 * This demo demonstrates basic I2C master/slave communication using WISE I2C
 * driver (Core API @ref WISE_I2C). The role is selected at build time via
 * @ref DEMO_I2C_ROLE:
 * - Master: sends a test pattern to a slave device.
 * - Slave : receives data and prints the received buffer after events.
 *
 * Hardware notes:
 * - Connect SCL/SDA between master and slave.
 * - External pull-up resistors are required on both lines (typically 4.7kΩ or 2.2kΩ).
 * - Pin mapping is defined by @ref i2c_pio_cfg.
 *
 * Functional notes:
 * - Default speed: 100kHz (I2C_SPEED_MODE_100K when @ref I2C_SPEED_MODE == 0)
 * - Addressing: 7-bit when @ref I2C_ADDRESSING == 0
 * - Non-blocking transfer completion is indicated by @ref WISE_I2C_EVENT_TRANSFER_DONE
 * - Default slave address: 0x50 (@ref I2C_SLAVE_ADDR)
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "demo_app_common.h"
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_i2c_api.h"
#include "wise_sys_api.h"

/**
 * @defgroup WISE_EXAMPLE_APP_I2C I2C Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: WISE I2C master/slave transfer demo.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_I2C  for I2C initialization/configuration and data transfer
 * - @ref WISE_GPIO for PIO pin function configuration
 * - @ref WISE_SYS  for disabling SWD/TCXO when shared pins are used
 *
 * The demo supports two build roles selected by @ref DEMO_I2C_ROLE:
 * - @ref DEMO_I2C_ROLE_MASTER
 * - @ref DEMO_I2C_ROLE_SLAVE
 * @{
 */

/* ============================================================
 * Build-time role select
 * ============================================================ */
#define DEMO_I2C_ROLE_MASTER 1 /**< Build as I2C master (TX). */
#define DEMO_I2C_ROLE_SLAVE  2 /**< Build as I2C slave (RX). */

#ifndef DEMO_I2C_ROLE
/** @brief Default role selection when DEMO_I2C_ROLE is not defined. */
#define DEMO_I2C_ROLE DEMO_I2C_ROLE_MASTER
#endif

/* ============================================================
 * Config
 * ============================================================ */
#define I2C_CHANNEL     0    /**< I2C controller index. */
#define I2C_SLAVE_ADDR  0x50 /**< Target address in 7-bit addressing mode. */
#define I2C_SPEED_MODE  0    /**< 0=100kHz, 1=400kHz, 2=1MHz (driver-specific encoding). */
#define I2C_ADDRESSING  0    /**< 0=7-bit, 1=10-bit. */
#define TEST_DATA_SIZE  16   /**< Transfer length for demo TX/RX. */
#define I2C_DMA_ENABLE  0    /**< Enable DMA for I2C transfers when non-zero. */

/* ============================================================
 * I2C PIO/GPIO mapping
 * ============================================================ */
/**
 * @brief I2C pin-mux configuration table.
 *
 * Layout:
 * - i2c_pio_cfg[i2c_channel][line][field]
 * - line : 0=SCL, 1=SDA
 * - field: 0=PIO function (MODE_PIO_FUNC_*), 1=GPIO pin index
 *
 * @note The mapping must match your board schematic / pinmux constraints.
 */
static const uint8_t i2c_pio_cfg[2][2][2] = { {
    /* I2C0 */
    { MODE_PIO_FUNC_I2C0_SCL, 2 },
    { MODE_PIO_FUNC_I2C0_SDA, 3 },
}, {
    /* I2C1 */
    { MODE_PIO_FUNC_I2C1_SCL, 7 },
    { MODE_PIO_FUNC_I2C1_SDA, 8 },
} };

/* ============================================================
 * Globals
 * ============================================================ */

/** @brief TX buffer used by master role. */
static uint8_t tx_buf[TEST_DATA_SIZE] __attribute__((aligned(4)));
/** @brief RX buffer used by slave role. */
static uint8_t rx_buf[TEST_DATA_SIZE] __attribute__((aligned(4)));

/** @brief Set to 1 when @ref WISE_I2C_EVENT_TRANSFER_DONE is received. */
static volatile int8_t g_xfer_done = 0;
/** @brief Set to 1 when @ref WISE_I2C_EVENT_ADDRESS_HIT is received (slave role). */
static volatile int8_t g_addr_hit  = 0;

/* ============================================================
 * I2C callback
 * ============================================================ */

/**
 * @brief I2C event callback for demo transfers.
 *
 * This callback is registered by ::wise_i2c_register_event_callback and sets
 * flags for:
 * - @ref WISE_I2C_EVENT_TRANSFER_DONE
 * - @ref WISE_I2C_EVENT_ADDRESS_HIT
 *
 * @param[in] i2c_channel I2C channel index that triggered the event.
 * @param[in] i2c_event   I2C event ID (driver-defined).
 * @param[in] context     User context pointer passed during registration.
 */
static void demo_i2c_callback(uint8_t i2c_channel, uint32_t i2c_event, void *context)
{
    (void)context;
    (void)i2c_channel;

    if (i2c_event == WISE_I2C_EVENT_TRANSFER_DONE) {
        g_xfer_done = 1;
    }
    if (i2c_event == WISE_I2C_EVENT_ADDRESS_HIT) {
        g_addr_hit = 1;
    }
}

/* ============================================================
 * I2C IO config
 * ============================================================ */

/**
 * @brief Configure pinmux for the selected I2C channel.
 *
 * This function:
 * - Checks whether selected pins conflict with SWD/TCXO pins
 * - Disables SWD/TCXO if required
 * - Configures GPIO function mapping using ::wise_gpio_func_cfg
 *
 * @param[in] i2c_channel I2C channel index (0 or 1).
 */
static void demo_i2c_io_config(uint8_t i2c_channel)
{
    bool need_tcxo = false;
    bool need_swd  = false;

    if (i2c_channel > 1) {
        printf("[I2C] invalid channel: %u\n", i2c_channel);
        return;
    }

    /* Check if special pins are used */
    for (int i = 0; i < 2; i++) {
        uint8_t pin = i2c_pio_cfg[i2c_channel][i][1];
        if (pin == 10) {
            need_tcxo = true;
        }
        if (pin == 14 || pin == 15) {
            need_swd = true;
        }
    }

    /* Disable SWD/TCXO if needed */
    if (need_swd) {
        wise_sys_swd_config(false);
    }
    if (need_tcxo) {
        wise_sys_tcxo_config(false);
    }

    /* Configure pins */
    for (int i = 0; i < 2; i++) {
        wise_gpio_func_cfg(i2c_pio_cfg[i2c_channel][i][1],
                           i2c_pio_cfg[i2c_channel][i][0]);
    }
}

/* ============================================================
 * Wait for transfer completion
 * ============================================================ */

/**
 * @brief Busy-wait until the transfer-done flag is set.
 *
 * The flag is set in ::demo_i2c_callback when @ref WISE_I2C_EVENT_TRANSFER_DONE occurs.
 *
 * @note This is a simple demo utility. Production code may prefer blocking APIs,
 *       task notification, or low-power wait (e.g., WFI) depending on design.
 */
static void i2c_xfer_and_wait(void)
{
    while (g_xfer_done == 0) {
        __NOP();
    }
}

/* ============================================================
 * Slave listen
 * ============================================================ */

/**
 * @brief Prepare slave to receive one transaction.
 *
 * Clears RX buffer, clears internal flags, clears FIFO, and arms a receive
 * transfer via ::wise_i2c_recv_nbyte.
 *
 * @param[in] recv_count Number of bytes expected to receive.
 */
static void demo_slave_listen_once(uint16_t recv_count)
{
    memset(rx_buf, 0, sizeof(rx_buf));

    g_xfer_done = 0;
    g_addr_hit  = 0;

    wise_i2c_clear_fifo(I2C_CHANNEL);
    wise_i2c_recv_nbyte(I2C_CHANNEL, rx_buf, recv_count);
}

/* ============================================================
 * Main
 * ============================================================ */

/**
 * @brief Main entry of the I2C demo application.
 *
 * Common flow:
 * 1. Initialize demo environment
 * 2. Configure I2C pinmux
 * 3. Initialize and open I2C
 * 4. Register event callback
 * 5. Configure master/slave role and execute a single transfer
 *
 * @return This demo does not return under normal operation.
 */
int main(void)
{
    WISE_STATUS st;

    printf("\n=== I2C Demo (%s) ===\n",
#if (DEMO_I2C_ROLE == DEMO_I2C_ROLE_MASTER)
           "MASTER"
#else
           "SLAVE"
#endif
    );

    demo_app_common_init();

    demo_i2c_io_config(I2C_CHANNEL);

    st = wise_i2c_init();
    if (st != WISE_SUCCESS) {
        printf("[ERR] wise_i2c_init failed\n");
        return 0;
    }

    st = wise_i2c_open(I2C_CHANNEL);
    if (st != WISE_SUCCESS) {
        printf("[ERR] wise_i2c_open failed\n");
        return 0;
    }

    wise_i2c_register_event_callback(I2C_CHANNEL, demo_i2c_callback, NULL);

    WISE_I2C_CONF_T cfg = {0};
    cfg.i2c_idx        = I2C_CHANNEL;
    cfg.i2cEn          = I2C_ENABLE;
    cfg.dmaEn          = I2C_DMA_ENABLE;
    cfg.speedMode      = I2C_SPEED_MODE;
    cfg.addressing     = I2C_ADDRESSING;
    cfg.target_address = I2C_SLAVE_ADDR;

#if (DEMO_I2C_ROLE == DEMO_I2C_ROLE_MASTER)

    cfg.role = I2C_MASTER;
    cfg.dir  = 1; /* Transmitter */

    st = wise_i2c_config(I2C_CHANNEL, &cfg);
    if (st != WISE_SUCCESS) {
        printf("[ERR] master config failed\n");
        return 0;
    }

    printf("Master initialized (addr=0x%02X, speed=%s)\n",
           I2C_SLAVE_ADDR,
           I2C_SPEED_MODE == 0 ? "100kHz" : (I2C_SPEED_MODE == 1 ? "400kHz" : "1MHz"));

    /* Prepare test data */
    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        tx_buf[i] = (uint8_t)(i + 1);
    }

    g_xfer_done = 0;

    st = wise_i2c_send_nbyte(I2C_CHANNEL, tx_buf, TEST_DATA_SIZE);
    if (st != WISE_SUCCESS) {
        printf("[ERR] transmission failed\n");
        return 0;
    }

    printf("Master sending data...\n");
    i2c_xfer_and_wait();

    printf("[I2C%u] TRANSFER_DONE\n", I2C_CHANNEL);

    printf("Master TX: ");
    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        printf("%02X ", tx_buf[i]);
    }
    printf("\n");
    printf("Transmission complete\n");

#else /* SLAVE */

    cfg.role = I2C_SLAVE;
    cfg.dir  = 0; /* Receiver */

    st = wise_i2c_config(I2C_CHANNEL, &cfg);
    if (st != WISE_SUCCESS) {
        printf("[ERR] slave config failed\n");
        return 0;
    }

    printf("Slave initialized (addr=0x%02X, speed=%s)\n",
           I2C_SLAVE_ADDR,
           I2C_SPEED_MODE == 0 ? "100kHz" : (I2C_SPEED_MODE == 1 ? "400kHz" : "1MHz"));

    demo_slave_listen_once(TEST_DATA_SIZE);

    printf("Slave listening...\n");

    /* Wait for address hit */
    while (!g_addr_hit) {
        __NOP();
    }
    printf("[I2C%u] ADDRESS_HIT\n", I2C_CHANNEL);

    /* Wait for transfer done */
    while (!g_xfer_done) {
        __NOP();
    }
    printf("[I2C%u] TRANSFER_DONE\n", I2C_CHANNEL);

    printf("Slave RX: ");
    for (uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        printf("%02X ", rx_buf[i]);
    }
    printf("\n");
    printf("Reception complete\n");

#endif

    while (1) {
        __WFI();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_I2C */
