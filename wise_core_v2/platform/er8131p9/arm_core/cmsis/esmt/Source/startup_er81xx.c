/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/******************************************************************************
 * @file     startup_er81xx.c
 * @brief    CMSIS-Core(M) Device Startup File for Device ER81xx
 * @version  V2.0.3
 * @date     31. March 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cmsis/include/er8xxx.h"

#define __VECTOR_TABLE_ATTRIBUTE __attribute__((section(".vectors")))
#define TOTAL_INTERRUPTS (16 + EXT_IRQ_COUNT)

#ifdef __STACK_SIZE
#define STACK_SIZE __STACK_SIZE
#else
#define STACK_SIZE 0x00000800
#endif

#ifdef __HEAP_SIZE
#define HEAP_SIZE __HEAP_SIZE
#else
#define HEAP_SIZE 0x00000800
#endif

uint8_t stack_space[STACK_SIZE] __attribute__((section(".stack")));
uint8_t heap_space[HEAP_SIZE] __attribute__((section(".heap")));

/*----------------------------------------------------------------------------
 * Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void (*VECTOR_TABLE_Type)(void);

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __stack;
extern void __libc_init_array(void);
extern int main(void);

void dataInit();
void bssInit();

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) __attribute__((noreturn));
void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void EXTPMU_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PCRMU_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void NFC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void WDT_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PWM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PWM_SLOW_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UART0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPTMR5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void WUTMR_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void MAC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void AES_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SHA_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TRNG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FLCTL_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EFUSE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void ASARADC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __Vectors[TOTAL_INTERRUPTS];

const VECTOR_TABLE_Type __Vectors[TOTAL_INTERRUPTS] __VECTOR_TABLE_ATTRIBUTE = {
    (VECTOR_TABLE_Type)(&__stack), /*     Initial Stack Pointer */
    Reset_Handler,                 /*     Reset Handler         */
    NMI_Handler,                   /* -14 NMI Handler           */
    HardFault_Handler,             /* -13 Hard Fault Handler    */
    MemManage_Handler,             /* -12 MPU Fault Handler     */
    BusFault_Handler,              /* -11 Bus Fault Handler     */
    UsageFault_Handler,            /* -10 Usage Fault Handler   */
    0,                             /*     Reserved              */
    0,                             /*     Reserved              */
    0,                             /*     Reserved              */
    0,                             /*     Reserved              */
    SVC_Handler,                   /*  -5 SVC Handler           */
    DebugMon_Handler,              /*  -4 Debug Monitor Handler */
    0,                             /*     Reserved              */
    PendSV_Handler,                /*  -2 PendSV Handler        */
    SysTick_Handler,               /*  -1 SysTick Handler       */

    /* Interrupts */
    0,                   /*  16+ 0 Reserved           */
    EXTPMU_IRQHandler,   /*  16+ 1 EXTPMU             */
    PCRMU_IRQHandler,    /*  16+ 2 PCRMU              */
    NFC_IRQHandler,      /*  16+ 3 NFC                */
    RTC_IRQHandler,      /*  16+ 4 RTC                */
    WDT_IRQHandler,      /*  16+ 5 WDT                */
    GPIO_IRQHandler,     /*  16+ 6 GPIO               */
    SPI0_IRQHandler,     /*  16+ 7 SPI0               */
    PWM_IRQHandler,      /*  16+ 8 PWM                */
    PWM_SLOW_IRQHandler, /*  16+ 9 PWM_SLOW           */
    GPTMR0_IRQHandler,   /*  16+10 GPTMR0             */
    GPTMR1_IRQHandler,   /*  16+11 GPTMR1             */
    UART0_IRQHandler,    /*  16+12 UART0              */
    UART1_IRQHandler,    /*  16+13 UART1              */
    GPTMR2_IRQHandler,   /*  16+14 GPTMR2             */
    GPTMR3_IRQHandler,   /*  16+15 GPTMR3             */
    GPTMR4_IRQHandler,   /*  16+16 GPTMR4             */
    GPTMR5_IRQHandler,   /*  16+17 GPTMR5             */
    I2C0_IRQHandler,     /*  16+18 I2C0               */
    SPI1_IRQHandler,     /*  16+19 SPI1               */
    UART2_IRQHandler,    /*  16+20 UART2              */
    I2C1_IRQHandler,     /*  16+21 I2C1               */
    WUTMR_IRQHandler,    /*  16+22 WUTMR              */
    MAC_IRQHandler,      /*  16+23 MAC                */
    DMA_IRQHandler,      /*  16+24 DMA                */
    AES_IRQHandler,      /*  16+25 AES                */
    SHA_IRQHandler,      /*  16+26 SHA                */
    TRNG_IRQHandler,     /*  16+27 TRNG               */
    FLCTL_IRQHandler,    /*  16+28 FLCTL              */
    0,                   /*  16+29 Reserved           */
    EFUSE_IRQHandler,    /*  16+30 EFUSE              */
    0,                   /*  16+31 Reserved           */
    ASARADC_IRQHandler,  /*  16+32 ASARADC            */
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__)

#ifdef DMA_POOL
/* DMA pool symbols */
extern uint32_t __dma_data_lma__;
extern uint32_t __dma_data_start__;
extern uint32_t __dma_data_end__;
extern uint32_t __dma_bss_start__;
extern uint32_t __dma_bss_end__;
#endif

void dataInit()
{
    uint32_t *pSrc, *pDest;
    extern uint32_t __etext;
    extern uint32_t __data_start__;
    extern uint32_t __data_end__;
    pSrc  = &__etext;
    pDest = &__data_start__;

    for (; pDest < &__data_end__;) {
        *pDest++ = *pSrc++;
    }
}

void bssInit()
{
    uint32_t *pDest;
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;
    pDest = &__bss_start__;

    for (; pDest < &__bss_end__;) {
        *pDest++ = 0UL;
    }
}

void ramTextInit()
{
    uint32_t *pSrc, *pDest;
    extern uint32_t __RAM_TEXT_FROM__;
    extern uint32_t __RAM_TEXT_START__;
    extern uint32_t __RAM_TEXT_END__;
    pSrc  = &__RAM_TEXT_FROM__;
    pDest = &__RAM_TEXT_START__;

    for (; pDest < &__RAM_TEXT_END__;) {
        *pDest++ = *pSrc++;
    }
}
#ifdef DMA_POOL
void dma_data_init()
{
	uint32_t *src, *dst;

	src = &__dma_data_lma__;
	dst = &__dma_data_start__;
	while (dst < &__dma_data_end__) {
		*dst++ = *src++;
	}
}

void dma_bss_init()
{
	uint32_t *dst;

    dst = &__dma_bss_start__;
    while (dst < &__dma_bss_end__) {
        *dst++ = 0;
    }
}

#endif
#endif /* DMA_POOL*/
/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#ifndef __NO_SYSTEM_INIT
    SystemInit(); /* CMSIS System Initialization */
#endif

    dataInit();
    bssInit();
    ramTextInit();
#if defined(DMA_POOL) && defined(__GNUC__)
    dma_data_init();
    dma_bss_init();
#endif

    __libc_init_array();
    main();
    while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
    while (1)
        ;
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while (1)
        ;
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif
