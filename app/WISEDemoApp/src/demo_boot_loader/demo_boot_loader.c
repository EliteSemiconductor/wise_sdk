/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_loader_shell_app.c
 * @brief Example loader application with UART shell, kermit update, and boot-to-app.
 *
 * @ingroup WISE_EXAMPLE_APP_LOADER
 *
 * This example application demonstrates:
 * - Building a simple shell backend over UART using WISE_UART_API
 * - Adding shell commands (boot / kermit)
 * - Updating an application image into flash via kermit protocol
 * - Booting to an application image by remapping system memory
 *
 * The application is intended to run as a small loader/monitor program.
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_sys_api.h"
#include "wise_uart_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"
#include "wise_kermit.h"

/* Demo banner name (was a -D define; now provided in the demo source). */
#define DEMO_APP_NAME "Boot Loader"

#ifndef STDIO_UART_PORT
#error "STDIO UART port is not defined."
#endif

/**
 * @defgroup WISE_EXAMPLE_APP_LOADER Loader Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application: shell-based loader with kermit update and boot.
 *
 * This example uses the following Core APIs:
 * - @ref WISE_UART (UART shell I/O)
 * - @ref WISE_SYS  (system remap / boot helper)
 * - @ref WISE_GPIO (deinit before remap)
 * - @ref WISE_PMU  (clock gating before remap)
 *
 * It also uses:
 * - wise_shell_v2 for command registration
 * - wise_kermit for receiving the application image
 * @{
 */

#define APP_BOOT_ADDR               0x6000 /**< Boot address of the target application image. */
#define DEMO_APP_PROMPT             "LOADER> "
#define LOADER_UART_BUF_SIZE        256

uint8_t loaderUartBuf[LOADER_UART_BUF_SIZE];

static void _loader_boot_to_app(uint32_t addr);
static void _loader_print_banner();
static void _loader_platform_init();
static void _loader_peripheral_init();
static int _loader_kermit_update();


/* ========================================================================== */
/* Boot Helper                                                                */
/* ========================================================================== */

/**
 * @brief Boot to an application image by remapping system memory.
 *
 * This routine performs a minimal "handoff" sequence:
 * - Disable STDIO UART (stop shell/printf traffic)
 * - Deinitialize GPIO and gate peripheral clocks
 * - Disable interrupts
 * - Insert a short delay (NOP loop) to allow HW to settle
 * - Remap system memory to @p addr
 *
 * @param[in] addr Boot address of the application image.
 *
 * @note The actual behavior after remap depends on the platform boot flow.
 *       Some systems will jump to reset vector automatically; others may
 *       require additional steps (e.g., setting MSP/PC and branching).
 */
static void _loader_boot_to_app(uint32_t addr)
{
    uint32_t wait = 20000;

    wise_uart_enable(STDIO_UART_PORT, WISE_UART_FLAG_NONE);
    wise_gpio_deinit();
    wise_pmu_module_clk_disable(0x1FFFFFFF);

    __disable_irq();
    while (wait--) 
    {
        asm("nop");
    }

    wise_sys_remap(addr);
}

static void _loader_print_banner()
{
    printf("\n");
    printf("============================================\n");
    printf("   ESMT WISE Demo Application: %s\n", DEMO_APP_NAME);
    printf("   Built@ %s %s\n", __DATE__, __TIME__);
    printf("============================================\n");
    fflush(stdout);
}

static void _loader_platform_init()
{
    if (WISE_SUCCESS != wise_core_init()) 
    {
        while (1);
    }

    wise_gpio_init();
    wise_tick_init();
    wise_flash_init();
}

static void _loader_peripheral_init()
{
    WISE_BUFFER_T rxBuffer = {LOADER_UART_BUF_SIZE, (uint32_t)loaderUartBuf};
    WISE_UART_CFG_T uartConfig =
    {
        ES_UART0_BAUDRATE,
        E_UART_DATA_8_BIT,
        E_UART_PARITY_NONE,
        E_UART_STOP_1_BIT,
        0,
    };
    
    wise_uart_init();
    wise_uart_config(STDIO_UART_PORT, &uartConfig);
    wise_uart_set_buffer(STDIO_UART_PORT, &rxBuffer, NULL);
    wise_uart_enable(STDIO_UART_PORT, WISE_UART_FLAG_RX | WISE_UART_FLAG_TX);
    wise_uart_enable_interrupt(STDIO_UART_PORT);
        
    wise_gpio_func_cfg(ES_UART0_TX_PIN, MODE_PIO_FUNC_UART0_TX);
    wise_gpio_func_cfg(ES_UART0_RX_PIN, MODE_PIO_FUNC_UART0_RX);

    retarget_set_port(STDIO_UART_PORT);
};

int _loader_wait_user_input()
{
    uint32_t checkCount = 4;
    uint32_t hitCounter = 0;
    
    debug_print("Press 'cccc' to enter console...\n");
    while (checkCount--) 
    {
        uint32_t startTick = wise_tick_get_counter();
        
        while (1) 
        {
            uint8_t inputC;
            uint32_t curTick = wise_tick_get_counter();
            
            if((curTick - startTick) >= MS_TO_CLK(1000))
            {
                wise_uart_write_char(STDIO_UART_PORT, '.');
                break;
            }

            if(WISE_SUCCESS == wise_uart_read_char(STDIO_UART_PORT, &inputC))
	        {
	            wise_uart_write_char(STDIO_UART_PORT, inputC);
	            if(inputC == 'c')
	            {
	                hitCounter++;
                    if (hitCounter >= 4) 
                    {
                        return 1;
                    }
	            }
	        }
        }
    }
    
    return 0;
}

static int _loader_kermit_update()
{
    uint32_t rxDataLen = 0;
    uint16_t rxDataCrc = 0;

    if (WISE_SUCCESS == wise_kermit_init(E_KERMIT_TARGET_FLASH, APP_BOOT_ADDR, STDIO_UART_PORT)) 
    {
        printf("\nStart kermit receiver\n");

        if (WISE_SUCCESS == wise_kermit_load(&rxDataLen, &rxDataCrc)) 
        {
            printf("APP updated size=%08lx crc=%04x\n", (unsigned long)rxDataLen, rxDataCrc);
        } 
        else 
        {
            printf("aborted\n");
            return WISE_FAIL;
        }
    }

    return WISE_SUCCESS;
}


/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */
void main(void)
{
    _loader_platform_init();
    _loader_peripheral_init();
    _loader_print_banner();

    if(_loader_wait_user_input())
    {
        while(1)
        {
            if(WISE_SUCCESS == _loader_kermit_update())
                break;
        }
    }
    
    printf("booting to APP @%08lx\r\n", (unsigned long)APP_BOOT_ADDR);
    _loader_boot_to_app(APP_BOOT_ADDR);
    
    while (1) 
    {
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_LOADER */
