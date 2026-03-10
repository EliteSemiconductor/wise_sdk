/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "api/wise_uart_api.h"
#include "wise_file_system.h"
#include "loader_shell_cmds.h"

#if (MIDDLEWARE_WISE_FLASH_FILESYSTEM == 0) && (!defined APP_BOOT_ADDR)
#error "No APP boot address defined!!"
#endif

typedef void (*APP_ENTRY_T)(void);

APP_ENTRY_T appEntryProc;

void bootToAPP(uint32_t addr)
{
    uint32_t wait        = 20000;
    uint8_t remapMode    = 1;
    uint32_t resetVector = LOAD_LE_32(addr + 4);

    /*
    if((resetVector & 0xffff0000) == addr)
    {
        remapMode = 0;
    }
    */

    wise_uart_enable(STDIO_UART_PORT, WISE_UART_FLAG_NONE);
    wise_gpio_deinit();
    wise_pmu_module_clk_disable(0x1FFFFFFF);
    
    __disable_irq();
    while (wait--) {
        asm("nop");
    }

    if (remapMode) {
        wise_sys_remap(addr);
    } else {
        appEntryProc =
            (APP_ENTRY_T) *
            (uint32_t *)(addr + 4); /* code second word is reset address */
        __set_MSP(
            *(uint32_t *)
                addr); /* init app stack pointer(code first word is stack address) */
        appEntryProc();
    }
}

void normalBoot()
{
#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    PAR_INFO_T appPartition = {0};
#endif

    __disable_irq();

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    if (WISE_SUCCESS == wise_fs_check_par_valid(FS_SIG_FUTEMP)) {
        debug_print("New FW available, start FW updating...\n");
        if (WISE_SUCCESS == wise_fs_par_copy(FS_SIG_FUTEMP, FS_SIG_APP)) {
            wise_fs_par_invalidate(FS_SIG_FUTEMP);
            wise_fs_reload();
        }
        else {
        	wise_fs_par_invalidate(FS_SIG_FUTEMP);
        }
    }

    if (WISE_SUCCESS == wise_fs_check_par_valid(FS_SIG_APP)) {
        wise_fs_get_par_info(FS_SIG_APP, &appPartition);

        debug_print("Booting to APP@%08lx\n\n", appPartition.offset);
        bootToAPP(appPartition.offset);
    } else {
        debug_print("APP not found\n");
        __enable_irq();
    }
#else
    bootToAPP(APP_BOOT_ADDR);
#endif
}

int main(void)
{
    int consoleMode = E_LOADER_NORMAL_BOOT;

    if (WISE_SUCCESS != wise_init()) {
        debug_print("wise init fail!!\n");
        while (1)
            ;
    }

    debug_print("\nESMT Sphynx APP Loader V2.02\n");

#if MIDDLEWARE_WISE_FLASH_FILESYSTEM
    wise_fs_init();
#endif

    loader_shell_cmd_init();

    consoleMode = loader_shell_cmd_check();

    if (consoleMode == E_LOADER_NORMAL_BOOT) {
        debug_print("\nNormal boot\n");
        normalBoot();
    }

    while (1) {
        wise_main_proc();
    }
    return 0;
}
