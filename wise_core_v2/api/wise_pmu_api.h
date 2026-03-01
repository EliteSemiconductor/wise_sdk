/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_pmu_api.h
 * @brief Power Management Unit (PMU) module clock control APIs.
 *
 * @ingroup WISE_PMU
 *
 * This header provides simple interfaces to enable/disable and query
 * module clocks via the PMU, using bitmask-based module identifiers.
 */

#ifndef _WISE_PMU_API_H_
#define _WISE_PMU_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_pmu.h"
#include "types.h"

/**
 * @defgroup WISE_PMU WISE PMU APIs
 * @ingroup WISE_CORE_API
 * @brief Module clock gating and status query.
 *
 * These APIs are used to control the clock of on-chip peripherals
 * through the Power Management Unit (PMU). Each module is represented
 * by a bitmask value, and multiple modules may be controlled at once
 * by OR-ing their masks together.
 * @{
 */
 

/**
 * @enum WISE_MODULE_T
 * @brief Bit field definition of each hardware module.
 *
 * Enumeration values map directly to PMU module bit masks defined in
 * the underlying HAL (DMA_MODULE, AES_MODULE, ...). They are intended
 * to be used in bitwise OR combinations as @p module_mask in the PMU
 * APIs, for example:
 *
 * @code
 * wise_pmu_module_clk_enable(E_WISE_MOD_UART0 | E_WISE_MOD_UART1);
 * @endcode
 */
typedef enum
{
    E_WISE_MOD_DMA      = DMA_MODULE,      /**< DMA controller module. */
    E_WISE_MOD_AES      = AES_MODULE,      /**< AES crypto module. */
    E_WISE_MOD_SHA      = SHA_MODULE,      /**< SHA hash module. */
    E_WISE_MOD_TRNG     = TRNG_MODULE,     /**< True Random Number Generator module. */
    E_WISE_MOD_UART0    = UART0_MODULE,    /**< UART0 module. */
    E_WISE_MOD_UART1    = UART1_MODULE,    /**< UART1 module. */
    E_WISE_MOD_UART2    = UART2_MODULE,    /**< UART2 module. */
    E_WISE_MOD_SPI0     = SPI0_MODULE,     /**< SPI0 module. */
    E_WISE_MOD_SPI1     = SPI1_MODULE,     /**< SPI1 module. */
    E_WISE_MOD_I2C0     = I2C0_MODULE,     /**< I2C0 module. */
    E_WISE_MOD_I2C1     = I2C1_MODULE,     /**< I2C1 module. */
    E_WISE_MOD_PWM      = PWM_MODULE,      /**< PWM module. */
    E_WISE_MOD_PWM_SLOW = PWM_SLOW_MODULE, /**< Slow PWM module. */
    E_WISE_MOD_GPTMR0   = GPTMR0_MODULE,   /**< General-purpose timer 0. */
    E_WISE_MOD_GPTMR1   = GPTMR1_MODULE,   /**< General-purpose timer 1. */
    E_WISE_MOD_GPTMR2   = GPTMR2_MODULE,   /**< General-purpose timer 2. */
    E_WISE_MOD_GPTMR3   = GPTMR3_MODULE,   /**< General-purpose timer 3. */
    E_WISE_MOD_GPTMR4   = GPTMR4_MODULE,   /**< General-purpose timer 4. */
    E_WISE_MOD_GPTMR5   = GPTMR5_MODULE,   /**< General-purpose timer 5. */
    E_WISE_MOD_GPIO     = GPIO_MODULE,     /**< GPIO module. */
    E_WISE_MOD_WUTMR    = WUTMR_MODULE,    /**< Wake-up timer module. */
    E_WISE_MOD_RTC      = RTC_MODULE,      /**< RTC (Real-Time Clock) module. */
    E_WISE_MOD_MAC      = MAC_MODULE,      /**< MAC (radio MAC) module. */
    E_WISE_MOD_BBP      = BBP_MODULE,      /**< Baseband processor module. */
    E_WISE_MOD_WDT      = WDT_MODULE,      /**< Watchdog timer module. */
    E_WISE_MOD_NFC      = NFC_MODULE,      /**< NFC module. */
    E_WISE_MOD_EFUSE    = EFUSE_MODULE,    /**< eFuse module. */
    E_WISE_MOD_ANA      = ANA_MODULE,      /**< Analog front-end module. */
} WISE_MODULE_T;

/**
 * @brief Enable clock for one or more modules.
 *
 * Any module whose bit is set in @p module_mask will have its clock enabled
 * via the PMU. It is valid to OR multiple ::WISE_MODULE_T values.
 *
 * @param[in] module_mask Bitmask of modules to enable (OR of E_WISE_MOD_*).
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed (invalid parameter or HW access error).
 */
int32_t wise_pmu_module_clk_enable(uint32_t module_mask);

/**
 * @brief Disable clock for one or more modules.
 *
 * Any module whose bit is set in @p module_mask will have its clock gated
 * (disabled) via the PMU. It is valid to OR multiple ::WISE_MODULE_T values.
 *
 * @param[in] module_mask Bitmask of modules to disable (OR of E_WISE_MOD_*).
 *
 * @retval 0   Operation completed successfully.
 * @retval <0  Operation failed (invalid parameter or HW access error).
 */
int32_t wise_pmu_module_clk_disable(uint32_t module_mask);

/**
 * @brief Check if clock is enabled for all modules in a mask.
 *
 * Tests whether the clock is currently enabled for the module(s) specified
 * in @p module_mask.
 *
 * @param[in] module_mask Bitmask of modules to check (OR of E_WISE_MOD_*).
 *
 * @retval true  All specified modules have clock enabled.
 * @retval false At least one specified module has clock disabled.
 */
bool wise_pmu_module_clk_is_enabled(uint32_t module_mask);

/** @} */ /* end of WISE_PMU group */

#endif /* _WISE_PMU_API_H_ */
