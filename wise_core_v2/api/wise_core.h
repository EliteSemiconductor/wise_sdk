/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_core.h
 * @brief Core system API.
 *
 * @ingroup WISE_CORE
 *
 * This header provides the interface for core system management,
 * including SDK version query, interrupt control, critical sections,
 * and core initialization.
 */


#ifndef __WISE_CORE_H
#define __WISE_CORE_H

#include "types.h"
#include "esmt_chip_specific.h"
//#include "cmsis/include/er8xxx.h"
#include "util.h"
#include "wise_fifo.h"

/**
 * @defgroup WISE_CORE WISE Core APIs
 * @ingroup WISE_CORE_API
 * @brief Core subsystem APIs including initialization, IRQ control, 
 *        critical sections, and SDK version query.
 * @{
 */

/**
 * @typedef WISE_STATUS
 * @brief Status code type used by core APIs.
 *
 * Typical usage:
 * - ::WISE_SUCCESS indicates the operation completed successfully.
 * - ::WISE_FAIL or a negative value indicates an error.
 */
typedef int32_t WISE_STATUS;

/** @name Core status codes
 *  @brief Return codes used by core APIs.
 *  @{
 */
#define WISE_SUCCESS                0           /**< Operation completed successfully. */
#define WISE_FAIL                   (-1)        /**< Operation failed. */
/** @} */

/**
 * @brief Invalid index constant.
 *
 * Valid indices start from 0. This value is used to represent an invalid index.
 */
#define WISE_INVALID_INDEX          (-1)

/**
 * @enum CORE_IO_MODE_T
 * @brief Core I/O mode selection.
 *
 * Selects the behavior of core I/O operations (for example, blocking or
 * non-blocking access to underlying HW/SW resources).
 */
typedef enum {
    CORE_IO_BLOCKING = 0, /**< Blocking I/O mode. The call returns after the operation completes. */
    CORE_IO_NONBLOCKING,  /**< Non-blocking I/O mode. The call returns immediately. */
} CORE_IO_MODE_T;

/**
 * @typedef core_irq_state_t
 * @brief Type used to store IRQ state for critical sections.
 *
 * The value represents the processor interrupt state before entering
 * a critical section and is used to restore the state on exit.
 */
typedef uint32_t core_irq_state_t;

/**
 * @struct WISE_SDK_VERSION_T
 * @brief SDK version information.
 *
 * Contains SDK version numbers and a signature for build identification.
 */
typedef struct {
    uint16_t verMajor;      /**< Major version number. */
    uint16_t verMinor;      /**< Minor version number. */
    uint32_t signature;     /**< Build signature or identification code. */
} WISE_SDK_VERSION_T;

/**
 * @struct WISE_BUFFER_T
 * @brief Generic buffer descriptor.
 *
 * Describes a memory buffer by its length and start address. This type is
 * used by subsystems such as the radio core for TX/RX buffer management.
 */
typedef struct {
    uint32_t length;        /**< Buffer length in bytes. */
    uint32_t addr;          /**< Buffer start address. */
} WISE_BUFFER_T;

/**
 * @brief Get SDK version information.
 *
 * Fills a ::WISE_SDK_VERSION_T structure with the current SDK version and
 * signature. The caller must provide a valid pointer.
 *
 * @param[out] sdkVerInfo Pointer to a ::WISE_SDK_VERSION_T structure that
 *                         receives the version information.
 */
void wise_core_get_version(WISE_SDK_VERSION_T *sdkVerInfo);

/**
 * @brief Initialize the core system.
 *
 * Performs core-level initialization, such as low-level drivers and
 * system resources. This function should be called before using other
 * core-related APIs.
 *
 * @return ::WISE_SUCCESS on success, or ::WISE_FAIL (or a negative value)
 *         on failure.
 */
WISE_STATUS wise_core_init(void);

/**
 * @def CORE_DECLARE_IRQ_STATE
 * @brief Declare and initialize a local IRQ state variable.
 *
 * Declares a variable named @c irqState of type ::core_irq_state_t and
 * initializes it to 0. This macro is intended to be used together with
 * ::CORE_ENTER_CRITICAL and ::CORE_EXIT_CRITICAL.
 */
#define CORE_DECLARE_IRQ_STATE core_irq_state_t irqState = 0;

/**
 * @brief Enable all interrupts.
 *
 * Restores global interrupt enable state so that interrupt service routines
 * (ISRs) can be executed.
 */
void wise_core_enable_irq(void);

/**
 * @brief Disable all interrupts.
 *
 * Disables global interrupts to prevent any interrupt service routines
 * (ISRs) from being executed. This is typically used for very short
 * critical sections where nesting is not required.
 */
void wise_core_disable_irq(void);

/**
 * @brief Enter a critical section and save IRQ state.
 *
 * Disables interrupts and returns the previous interrupt state. The saved
 * state must later be passed to ::wise_core_exit_critical to restore the
 * original interrupt configuration.
 *
 * @return The saved interrupt state to be used with ::wise_core_exit_critical.
 */
core_irq_state_t wise_core_enter_critical(void);

/**
 * @brief Exit a critical section and restore IRQ state.
 *
 * Restores the interrupt state that was previously returned by
 * ::wise_core_enter_critical, re-enabling or keeping disabled the
 * interrupts as appropriate.
 *
 * @param[in] irqState The interrupt state previously returned by
 *                     ::wise_core_enter_critical.
 */
void wise_core_exit_critical(core_irq_state_t irqState);

/**
 * @def CORE_ENTER_CRITICAL
 * @brief Enter a critical section (macro form).
 *
 * Saves the current IRQ state into the local variable @c irqState and
 * disables interrupts by calling ::wise_core_enter_critical().
 *
 * @note The macro assumes that ::CORE_DECLARE_IRQ_STATE was used earlier
 *       in the same scope to declare @c irqState.
 */
#define CORE_ENTER_CRITICAL() irqState = wise_core_enter_critical()

/**
 * @def CORE_EXIT_CRITICAL
 * @brief Exit a critical section (macro form).
 *
 * Restores the previously saved IRQ state by calling
 * ::wise_core_exit_critical() with @c irqState.
 *
 * @note The macro assumes that ::CORE_DECLARE_IRQ_STATE was used earlier
 *       in the same scope to declare @c irqState.
 */
#define CORE_EXIT_CRITICAL() wise_core_exit_critical(irqState)

/**
 * @def CORE_IS_IN_ISR
 * @brief Check whether the CPU is currently executing in an ISR.
 *
 * Evaluates to non-zero if the current context is inside an interrupt
 * service routine. Otherwise, it evaluates to zero.
 *
 * @return Non-zero if running in an ISR, zero otherwise.
 */
#define CORE_IS_IN_ISR() ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0)

#ifdef CHIP_DMA_BUFFER_SECTION
/*==============================================================================
 * Linker Symbols
 *============================================================================*/

/** @brief Start address of DMA pool (defined in linker script) */
extern uint32_t __dma_pool_start__;

/** @brief End address of DMA pool (defined in linker script) */
extern uint32_t __dma_pool_end__;
/**
 * @def DMA_ATTR
 * @brief DMA buffer, zero-initialized (use for output buffers)
 *
 * @code
 * uint8_t rx_buf[256] DMA_ATTR;
 * @endcode
 */
#define DMA_ATTR    __attribute__((section(".dma_pool"), aligned(4)))

/**
 * @def DMA_DATA_ATTR
 * @brief DMA buffer with initialization support (use for keys, constants)
 *
 * @code
 * const uint8_t aes_key[16] DMA_DATA_ATTR = {0x2B, 0x7E, ...};
 * @endcode
 */
#define DMA_DATA_ATTR   __attribute__((section(".dma_data"), aligned(4)))

/**
 * @def STATIC_DMA
 * @brief Static DMA buffer, zero-initialized (for function scope)
 */
#define STATIC_DMA      static __attribute__((section(".dma_pool"), aligned(4)))

/**
 * @def STATIC_DMA_DATA
 * @brief Static DMA buffer with initialization (for function scope)
 *
 * @code
 * STATIC_DMA_DATA uint8_t key[16] = {0x2B, 0x7E, ...};
 * @endcode
 */
#define STATIC_DMA_DATA static __attribute__((section(".dma_data"), aligned(4)))

#else
#define DMA_ATTR
#define DMA_DATA_ATTR  
#define STATIC_DMA
#define STATIC_DMA_DATA 
#endif /* CHIP_DMA_BUFFER_SECTION */
/** @} */ /* end of WISE_CORE group */

#endif /* __WISE_CORE_H */
