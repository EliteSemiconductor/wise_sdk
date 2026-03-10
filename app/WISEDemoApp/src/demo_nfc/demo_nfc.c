/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_nfc_app.c
 * @brief Example application demonstrating WISE_NFC configuration and interrupt callbacks.
 *
 * @ingroup WISE_EXAMPLE_APP_NFC
 *
 * This example application demonstrates:
 * - Initializing the NFC module
 * - Configuring NFC work/lock mode and interrupt index
 * - Registering NFC interrupt callbacks
 * - Writing a tag content buffer into NFC blocks
 * - Providing a simple UART shell prompt (transport only)
 *
 * The shell prompt is configured as "NFC> " on the selected UART channel.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"
#include "wise_nfc_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"

/**
 * @defgroup WISE_EXAMPLE_APP_NFC NFC Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: NFC configuration, data write, and interrupt callback.
 *
 * This demo uses the following Core API modules:
 * - @ref WISE_NFC   for NFC configuration and tag data write
 * - @ref WISE_UART  for shell UART transport
 * - @ref WISE_CORE  for main processing loop
 *
 * It demonstrates configuring the NFC peripheral and receiving events via
 * registered interrupt callbacks.
 * @{
 */

#define DEMO_APP_PROMPT             "NFC> "

static void _demo_NFC_config(void);
static void _nfc_isr_callback(void *context, uint8_t idx);

/* ========================================================================== */
/* NFC Configuration                                                          */
/* ========================================================================== */

/**
 * @brief Configure NFC module and write initial tag content data.
 *
 * This function performs the following actions:
 * - Initialize NFC module via ::wise_nfc_init
 * - Register interrupt callbacks for NFC interrupt indices 0 and 1
 * - Apply NFC configuration via ::wise_nfc_config
 * - Write a demo tag content buffer via ::wise_nfc_block_write_data
 *
 * @note The demo writes a fixed 32-bit word buffer (tag_cnt) into NFC blocks.
 *       The buffer content and block range should be adjusted based on your
 *       NFC tag format and use case.
 */
static void _demo_NFC_config(void)
{
    WISE_NFC_CFG_T nfc_cfg = {
        .work_mode = 1,
        .lock_mode = 1,
        .int_idx = NFC_INT_IDX_1,
    };

    uint32_t tag_cnt[] = {
        0xC8743400,
        0x801CE3E1,
        0x0000009E,
        0x001210E1,
        0x65025419,
        0x2069486E,
        0x49202E2E,
        0x206D6120,
        0x79687053,
        0x7420786E,
        0xFE363063,
        0x00000000,
        0x00000000,
    };

    wise_nfc_init();
    wise_nfc_register_int_callback(NFC_INT_IDX_0, _nfc_isr_callback, NULL);
    wise_nfc_register_int_callback(NFC_INT_IDX_1, _nfc_isr_callback, NULL);
    wise_nfc_config(&nfc_cfg);
    wise_nfc_block_write_data(0, 14, tag_cnt);

    printf("NFC config done\r\n");
}

/**
 * @brief NFC interrupt callback.
 *
 * This callback is invoked when an NFC interrupt event occurs on the
 * registered interrupt index.
 *
 * @param[in] context User context pointer passed during registration.
 * @param[in] idx     NFC interrupt index that triggered the callback.
 */
static void _nfc_isr_callback(void *context, uint8_t idx)
{
    (void)context;
    printf("%s : NFC event triggered! idx = %d\r\n", __func__, idx);
}

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */

/**
 * @brief Main entry of the NFC example application.
 *
 * Initializes demo environment and shell, configures NFC, then enters the
 * main processing loop via ::wise_main_proc().
 */
void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    _demo_NFC_config();

    while (1) {
        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_NFC */
