/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_core.h"
#include "wise_sys_api.h"
#include "wise_tick_api.h"
#include "wise_wutmr_api.h"
#include "wise_flash_api.h"
#include "git_version.h"

// SDK major version: increased when major architecture modification
#define WISE_SDK_VERSION_MAJOR                          4

// SDK minor version: increased for each official release
#define WISE_SDK_VERSION_MINOR                          5

// SDK short SHA of git revision
#define WISE_SDK_VERSION_BUILD                          GIT_COMMIT_HASH

void debugHardfault(uint32_t *sp);
void _core_xip_mode_config();
extern void _flash_probe();
extern int32_t _flash_get_xip_cfg(uint8_t* mode, uint8_t* clk);

void wise_core_get_version(WISE_SDK_VERSION_T *sdkVerInfo)
{
    if (sdkVerInfo) {
        sdkVerInfo->verMajor = WISE_SDK_VERSION_MAJOR;
        sdkVerInfo->verMinor = WISE_SDK_VERSION_MINOR;
        sdkVerInfo->signature = WISE_SDK_VERSION_BUILD;
    }
}

void wise_core_disable_irq(void)
{
    __disable_irq();
}

void wise_core_enable_irq(void)
{
    __enable_irq();
}

core_irq_state_t wise_core_enter_critical(void)
{
    core_irq_state_t irqState = __get_PRIMASK();
    __disable_irq();
    return irqState;
}

void wise_core_exit_critical(core_irq_state_t irqState)
{
    if (irqState == 0U) {
        __enable_irq();
    }
}

WISE_STATUS wise_core_init(void)
{
    WISE_STATUS status = WISE_SUCCESS;

    if(ESMT_SOC_CHIP_ID != wise_sys_get_chip_id())
        return WISE_FAIL;
    
    wise_sys_init();
    wise_sys_lock(); // for soc remapping

#ifdef CHIP_XIP_SUPPORT_RUNTIME_CONFIG
    _core_xip_mode_config();
#endif

    return status;
}

__attribute__((used)) void debugHardfault(uint32_t *sp)
{
    uint32_t cfsr  = SCB->CFSR;
    uint32_t hfsr  = SCB->HFSR;
    uint32_t mmfar = SCB->MMFAR;
    uint32_t bfar  = SCB->BFAR;

    uint32_t r0  = sp[0];
    uint32_t r1  = sp[1];
    uint32_t r2  = sp[2];
    uint32_t r3  = sp[3];
    uint32_t r12 = sp[4];
    uint32_t lr  = sp[5];
    uint32_t pc  = sp[6];
    uint32_t psr = sp[7];

    printf("HardFault:\n");
    printf("SCB->CFSR   0x%08lx\n", cfsr);
    printf("SCB->HFSR   0x%08lx\n", hfsr);
    printf("SCB->MMFAR  0x%08lx\n", mmfar);
    printf("SCB->BFAR   0x%08lx\n", bfar);
    printf("\n");

    printf("SP          0x%08lx\n", (uint32_t)sp);
    printf("R0          0x%08lx\n", r0);
    printf("R1          0x%08lx\n", r1);
    printf("R2          0x%08lx\n", r2);
    printf("R3          0x%08lx\n", r3);
    printf("R12         0x%08lx\n", r12);
    printf("LR          0x%08lx\n", lr);
    printf("PC          0x%08lx\n", pc);
    printf("PSR         0x%08lx\n", psr);

    printf("\n");
    while (1)
        ;
}

void HardFault_Handler()
{
    // debug_print("HardFault!!\n");

    __asm volatile("tst lr, #4                                    \n"
                   "ite eq                                        \n"
                   "mrseq r0, msp                                 \n"
                   "mrsne r0, psp                                 \n"
                   "ldr r1, debugHardfault_address                \n"
                   "bx r1                                         \n"
                   "debugHardfault_address: .word debugHardfault  \n");

    while (1);
}

void _core_xip_mode_config()
{
    WISE_MSBI_INFO_T msbi;
    uint8_t xipMode, xipClk;
    
    _flash_probe();
    
    if(WISE_SUCCESS == wise_flash_shadow_read_msbi_info(&msbi))
    {
        //bootrom has already configured it according to msbi
        return;
    }
    
    if(WISE_SUCCESS == _flash_get_xip_cfg(&xipMode, &xipClk))
    {
        hal_intf_pmu_set_xip_clk(xipClk);
        hal_intf_xip_set_spi_mode(xipMode);
    }
}

