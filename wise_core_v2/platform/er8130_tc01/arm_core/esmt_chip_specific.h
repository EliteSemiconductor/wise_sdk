/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __ESMT_CHIP_SPECIFIC_H
#define __ESMT_CHIP_SPECIFIC_H

#if defined CHIP_SOC_UNIQUE
#error "Platform conflict!!"
#endif

#define CHIP_SOC_UNIQUE "ER8130_TC01"
#define ESMT_SOC_CHIP_ID 0x9005

#include "cmsis/include/er8xxx.h"

#include "cmsis/include/regfiles/aes_reg_er8130.h"
#include "cmsis/include/regfiles/cache_reg_er8130.h"
#include "cmsis/include/regfiles/dma_reg_er8130.h"
#include "cmsis/include/regfiles/efuctl_reg_er8130.h"
#include "cmsis/include/regfiles/extpmu_reg_er8130.h"
#include "cmsis/include/regfiles/gptmr_reg_er8130.h"
#include "cmsis/include/regfiles/i2c_reg_er8130.h"
#include "cmsis/include/regfiles/ioctrl_reg_er8130.h"
#include "cmsis/include/regfiles/nfc_reg_er8130.h"
#include "cmsis/include/regfiles/pcrmu_reg_er8130.h"
#include "cmsis/include/regfiles/pio_reg_er8130.h"
#include "cmsis/include/regfiles/pwm_reg_er8130.h"
#include "cmsis/include/regfiles/pwmslow_reg_er8130.h"
#include "cmsis/include/regfiles/rtc_reg_er8130.h"
#include "cmsis/include/regfiles/sha_reg_er8130.h"
#include "cmsis/include/regfiles/spi_reg_er8130.h"
#include "cmsis/include/regfiles/sys_reg_er8130.h"
#include "cmsis/include/regfiles/trng_reg_er8130.h"
#include "cmsis/include/regfiles/uart_reg_er8130.h"
#include "cmsis/include/regfiles/wdt_reg_er8130.h"
#include "cmsis/include/regfiles/wutmr_reg_er8130.h"
#include "cmsis/include/regfiles/xipast_reg_er8130.h"
#include "regfiles/ana_reg_er8130.h"
#include "regfiles/bbp_reg_er8130.h"
#include "regfiles/mac_reg_er8130.h"

typedef enum {
    /* ----------------------  er8130 Specific Interrupt Numbers
       --------------------- */
    RESERVED0_IRQn = 0,  /*!<   0  Reserved interrupt  */
    RESERVED1_IRQn = 1,  /*!<   1  Reserved interrupt  */
    PCRMU_IRQn     = 2,  /*!<   2  PCRMU interrupt      */
    NFC_IRQn       = 3,  /*!<   3  NFC interrupt        */
    RTC_IRQn       = 4,  /*!<   4  RTC interrupt        */
    WDT_IRQn       = 5,  /*!<   5  WDT interrupt        */
    GPIO_IRQn      = 6,  /*!<   6  GPIO interrupt       */
    SPI0_IRQn      = 7,  /*!<   7  SPI0 interrupt       */
    PWM_IRQn       = 8,  /*!<   8  PWM interrupt        */
    PWM_SLOW_IRQn  = 9,  /*!<   9  PWM_SLOW interrupt   */
    GPTMR0_IRQn    = 10, /*!<  10  GPTMR0 interrupt    */
    GPTMR1_IRQn    = 11, /*!<  11  GPTMR1 interrupt    */
    UART0_IRQn     = 12, /*!<  12  UART0 interrupt     */
    UART1_IRQn     = 13, /*!<  13  UART1 interrupt     */
    GPTMR2_IRQn    = 14, /*!<  14  GPTMR2 interrupt    */
    GPTMR3_IRQn    = 15, /*!<  15  GPTMR3 interrupt    */
    GPTMR4_IRQn    = 16, /*!<  16  GPTMR4 interrupt    */
    GPTMR5_IRQn    = 17, /*!<  17  GPTMR5 interrupt    */
    I2C0_IRQn      = 18, /*!<  18  I2C0 interrupt      */
    SPI1_IRQn      = 19, /*!<  19  SPI1 interrupt      */
    UART2_IRQn     = 20, /*!<  20  UART2 interrupt     */
    I2C1_IRQn      = 21, /*!<  21  I2C1 interrupt      */
    WUTMR_IRQn     = 22, /*!<  22  WUTMR interrupt     */
    MAC_IRQn       = 23, /*!<  23  MAC interrupt       */
    DMA_IRQn       = 24, /*!<  24  DMA interrupt       */
    AES_IRQn       = 25, /*!<  25  AES interrupt       */
    SHA_IRQn       = 26, /*!<  26  SHA interrupt       */
    TRNG_IRQn      = 27, /*!<  27  TRNG interrupt      */
    FLCTL_IRQn     = 28, /*!<  28  FLCTL interrupt     */
    RESERVED2_IRQn = 29, /*!<  29  Reserved interrupt */
    EFUSE_IRQn     = 30, /*!<  30  EFUSE interrupt     */
    RESERVED3_IRQn = 31, /*!<  31  Reserved interrupt  */
    ASARADC_IRQn   = 32  /*!<  32  ASARADC interrupt  */
} ESMT_IRQn_Type;

/*---------------------------------------------------------------------------------------------------------*/
/* MODULE constant definitions. */
/*---------------------------------------------------------------------------------------------------------*/
#define DMA_MODULE                              (uint32_t)(0x1ul << 0)       /*!< DMA     Module \hideinitializer */
#define AES_MODULE                              (uint32_t)(0x1ul << 1)       /*!< AES     Module \hideinitializer */
#define SHA_MODULE                              (uint32_t)(0x1ul << 2)       /*!< SHA     Module \hideinitializer */
#define TRNG_MODULE                             (uint32_t)(0x1ul << 3)      /*!< TRNG    Module \hideinitializer */
#define UART0_MODULE                            (uint32_t)(0x1ul << 4)     /*!< UART0   Module \hideinitializer */
#define UART1_MODULE                            (uint32_t)(0x1ul << 5)     /*!< UART1   Module \hideinitializer */
#define UART2_MODULE                            (uint32_t)(0x1ul << 6)     /*!< UART2   Module \hideinitializer */
#define SPI0_MODULE                             (uint32_t)(0x1ul << 7)      /*!< SPI0    Module \hideinitializer */
#define SPI1_MODULE                             (uint32_t)(0x1ul << 8)      /*!< SPI1    Module \hideinitializer */
#define I2C0_MODULE                             (uint32_t)(0x1ul << 9)      /*!< I2C0    Module \hideinitializer */
#define I2C1_MODULE                             (uint32_t)(0x1ul << 10)     /*!< I2C1    Module \hideinitializer */
#define PWM_MODULE                              (uint32_t)(0x1ul << 11)      /*!< PWM     Module \hideinitializer */
#define PWM_SLOW_MODULE                         (uint32_t)(0x1ul << 12) /*!< PWMSLOW Module \hideinitializer */
#define GPTMR0_MODULE                           (uint32_t)(0x1ul << 13)   /*!< GPTMR0  Module \hideinitializer */
#define GPTMR1_MODULE                           (uint32_t)(0x1ul << 14)   /*!< GPTMR1  Module \hideinitializer */
#define GPTMR2_MODULE                           (uint32_t)(0x1ul << 15)   /*!< GPTMR2  Module \hideinitializer */
#define GPTMR3_MODULE                           (uint32_t)(0x1ul << 16)   /*!< GPTMR3  Module \hideinitializer */
#define GPTMR4_MODULE                           (uint32_t)(0x1ul << 17)   /*!< GPTMR4  Module \hideinitializer */
#define GPTMR5_MODULE                           (uint32_t)(0x1ul << 18)   /*!< GPTMR5  Module \hideinitializer */
#define GPIO_MODULE                             (uint32_t)(0x1ul << 19)     /*!< GPIO    Module \hideinitializer */
#define WUTMR_MODULE                            (uint32_t)(0x1ul << 20)    /*!< WUTMR   Module \hideinitializer */
#define RTC_MODULE                              (uint32_t)(0x1ul << 21)      /*!< RTC     Module \hideinitializer */
#define MAC_MODULE                              (uint32_t)(0x1ul << 22)      /*!< MAC     Module \hideinitializer */
#define BBP_MODULE                              (uint32_t)(0x1ul << 23)      /*!< BBP     Module \hideinitializer */
#define WDT_MODULE                              (uint32_t)(0x1ul << 25)      /*!< WDT     Module \hideinitializer */
#define NFC_MODULE                              (uint32_t)(0x1ul << 26)      /*!< NFC     Module \hideinitializer */
#define EFUSE_MODULE                            (uint32_t)(0x1ul << 27)    /*!< EFUSE   Module \hideinitializer */
#define ANA_MODULE                              (uint32_t)(0x1ul << 28)      /*!< ANA     Module \hideinitializer */

// Clock rate definitions, moved from system_er81xx.h
#ifndef __HSI
#define __HSI                                   (40000000UL) /*!< PLL default output is 40MHz */
#endif

#ifndef __HXT
#define __HXT                                   (40000000UL) /*!< External Crystal Clock Frequency     */
#endif

#ifndef __LXT
#define __LXT                                   (32768UL) /*!< External Crystal Clock Frequency 32.768KHz */
#endif

#define __HIRC                                  (40000000UL) /*!< Internal 40M RC Oscillator Frequency */
#define __LIRC                                  (32768UL)    /*!< Internal 32.768K RC Oscillator Frequency */

// frequency        */
#define __SYSTEM_CLOCK                          (1UL * __HXT)
#define __SYSTEM_CLOCK_DIVIDER                  (1 << PCRMU->MCU_CLK_FAC_REG)

// Features supported in chip
#define CHIP_HAS_ADC
#define CHIP_HAS_CRYPTO
#define CHIP_HAS_DMA
#define CHIP_HAS_FLASH
#define CHIP_HAS_GPIO
#define CHIP_HAS_I2C
#define CHIP_HAS_NFC
#define CHIP_HAS_PMU
#define CHIP_HAS_PWM
#define CHIP_HAS_RADIO
#define CHIP_HAS_SPI
#define CHIP_HAS_TIMER
#define CHIP_HAS_TRNG
#define CHIP_HAS_UART

// features for ADC
#define CHIP_ADC_SUPPORT
#define CHIP_ADC_CHANNEL_NUM                    0 //?

// features for crypto
#define CHIP_CRYPTO_HAS_AES
#define CHIP_CRYPTO_HAS_SHA
#define CHIP_CRYPTO_HAS_CRC
#define CHIP_CRYPTO_AES_MAX_KEY_WORDS           8

// features for low-frequency osc
#define CHIP_HAS_LFOSC

// features for DMA
#define CHIP_DMA_CHANNEL_NUM                    8
#define CHIP_DMA_SUPPORT_PERIPHERAL

// features for FLASH
#define CHIP_FLASH_MAP_ADDR                     0x00000000
#define CHIP_FLASH_INTERNAL_PAGE_SIZE           256
#define CHIP_FLASH_INTERNAL_PAGE_MASK           0xff

// features for GPIO
#define CHIP_GPIO_NUM                           16
#define CHIP_GPIO_BIND_ALT // GPIO14/GPIO15 for SWD, GPIO10 for TCXO
#define CHIP_GPIO_HAS_PIO  // GPIO can be used as PIO, SPI, I2C, UART

// features for I2C
#define CHIP_I2C_CHANNEL_NUM                    2
#define CHIP_I2C_MAX_XFER                       255
// features for NFC

// features for PMU
#define CHIP_PMU_HAS_EXT_PMU

// features for RADIO
#define CHIP_RADIO_HAS_SUB_GHZ
#define CHIP_RADIO_HAS_IEEE802154
#define CHIP_RADIO_HAS_W_MBUS
#define CHIP_RADIO_HAS_WOR
#define CHIP_RADIO_HAS_SYNCWORD_INTERRUPT
#define CHIP_RADIO_NEED_RX_CAL
#define CHIP_RADIO_NEED_TX_TPM_CAL
#define CHIP_RADIO_FLEXIBLE_FMT_VERSION         1

#define CHIP_SUPPORT_CUSTOM_BOARD //kevinyang, 20250701, temporarily

// features for SPI
#define CHIP_SPI_CHANNEL_NUM                    2

// features for TIMER
#define CHIP_TIMER_CHANNEL_NUM                  6
#define CHIP_TIMER_COUNTER_DOWN
#define CHIP_TICK_TIMER_CHANNEL                 5

// features for TRNG

// features for UART
#define CHIP_UART_CHANNEL_NUM                   3

// features for pwm
#define CHIP_PWM_CHANNEL_NUM                    6

#if ER81XX_MATCHING_FREQ_490MHZ
//490MHz need a workaround
#define WORKAROUND_490MHZ
#endif

#define WORKAROUND_WMBUS
#ifdef WMBUS_SCENARIO_SPEC_TESTING
#define WMBUS_SCENARIO_REDUCE_FALSE_ALARM       0
#else
#define WMBUS_SCENARIO_REDUCE_FALSE_ALARM       1
#endif

//feaures for nfc
#define NFC_INT_NUM                             2
#define MAX_NFC_BLOCK_IDX                       256

//features for PMU
#define CHIP_PMU_HAS_CPU_PD
#define CHIP_PMU_HAS_EXTERNAL

// #define RAM_TEXT
#define RAM_TEXT                                __attribute__((section(".ram_text")))

#ifdef __USER_CUSTOM_ISR //only used for internal test mode
#define WEAK_ISR                                __attribute__((weak))
#else
#define WEAK_ISR
#endif

#endif
