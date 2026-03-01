/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_gpio_api.h
 * @brief General-Purpose I/O (GPIO) driver APIs.
 *
 * @ingroup WISE_GPIO
 *
 * This header provides the interface for configuring and managing General
 * Purpose Input/Output (GPIO) pins, including mode selection, direction,
 * pull resistor, debounce, drive strength, interrupt, PWM and debug bus.
 */

#ifndef _WISE_GPIO_API_H_
#define _WISE_GPIO_API_H_

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_gpio.h"
#include "hal_intf_pmu.h"
#include "hal_intf_pio.h"
#include "types.h"

/**
 * @defgroup WISE_GPIO WISE GPIO APIs
 * @ingroup WISE_CORE_API
 * @brief GPIO configuration, control, and interrupt handling.
 * @{
 */

//#define MODE_PIO_FUNC_BASE                              (0x80)

/**
 * @typedef GPIO_USER_ISR_CB
 * @brief User callback type for GPIO interrupt service routines.
 *
 * @param cb_data Pointer to user-defined context passed to the callback.
 */
typedef void (*GPIO_USER_ISR_CB)(void *cb_data);

/**
 * @enum GPIO_MODE_T
 * @brief GPIO pin mode selection.
 */
typedef enum {
    MODE_GPIO   = 0, /**< General-purpose I/O mode. */
    MODE_PERI_1 = 1, /**< Peripheral function 1. */
    MODE_PWM    = 2, /**< PWM function. */
    MODE_PERI_2 = 3, /**< Peripheral function 2. */
    MODE_PIO    = 4, /**< PIO function (remapped I/O). */
    MODE_DEBUG  = 7, /**< Debug function. */
} GPIO_MODE_T;

/**
 * @enum GPIO_PIN_FUNC_T
 * @brief PIO function selection for a GPIO pin.
 *
 * These values are used in PIO mode to route peripheral signals
 * (UART/SPI/I2C/GPIO) to physical pins.
 */
typedef enum {
    MODE_PIO_FUNC_GPIO = 0,  /**< GPIO function. */
    MODE_PIO_FUNC_UART0_TX,  /**< UART0 TX. */
    MODE_PIO_FUNC_UART0_RX,  /**< UART0 RX. */
    MODE_PIO_FUNC_UART0_RTS, /**< UART0 RTS. */
    MODE_PIO_FUNC_UART0_CTS, /**< UART0 CTS. */
    MODE_PIO_FUNC_UART1_TX,  /**< UART1 TX. */
    MODE_PIO_FUNC_UART1_RX,  /**< UART1 RX. */
    MODE_PIO_FUNC_UART1_RTS, /**< UART1 RTS. */
    MODE_PIO_FUNC_UART1_CTS, /**< UART1 CTS. */
    MODE_PIO_FUNC_UART2_TX,  /**< UART2 TX. */
    MODE_PIO_FUNC_UART2_RX,  /**< UART2 RX. */
    MODE_PIO_FUNC_UART2_RTS, /**< UART2 RTS. */
    MODE_PIO_FUNC_UART2_CTS, /**< UART2 CTS. */
    MODE_PIO_FUNC_SPI0_CLK,  /**< SPI0 clock. */
    MODE_PIO_FUNC_SPI0_CS,   /**< SPI0 chip select. */
    MODE_PIO_FUNC_SPI0_MOSI, /**< SPI0 MOSI. */
    MODE_PIO_FUNC_SPI0_MISO, /**< SPI0 MISO. */
    MODE_PIO_FUNC_SPI0_HOLD, /**< SPI0 HOLD. */
    MODE_PIO_FUNC_SPI0_WP,   /**< SPI0 write-protect. */
    MODE_PIO_FUNC_SPI1_CLK,  /**< SPI1 clock. */
    MODE_PIO_FUNC_SPI1_CS,   /**< SPI1 chip select. */
    MODE_PIO_FUNC_SPI1_MOSI, /**< SPI1 MOSI. */
    MODE_PIO_FUNC_SPI1_MISO, /**< SPI1 MISO. */
    MODE_PIO_FUNC_SPI1_HOLD, /**< SPI1 HOLD. */
    MODE_PIO_FUNC_SPI1_WP,   /**< SPI1 write-protect. */
    MODE_PIO_FUNC_I2C0_SCL,  /**< I2C0 SCL. */
    MODE_PIO_FUNC_I2C0_SDA,  /**< I2C0 SDA. */
    MODE_PIO_FUNC_I2C1_SCL,  /**< I2C1 SCL. */
    MODE_PIO_FUNC_I2C1_SDA,  /**< I2C1 SDA. */
    MODE_PIO_FUNC_UNKOWN,    /**< Unknown/invalid PIO function. */
} GPIO_PIN_FUNC_T;

/**
 * @enum GPIO_DRIV_STR_TYPE
 * @brief GPIO drive strength selection.
 */
typedef enum {
    DRIV_STR_4_5MA  = 0, /**< Drive strength of 4.5 mA. */
    DRIV_STR_9MA    = 1, /**< Drive strength of 9 mA. */
    DRIV_STR_13_5MA = 2, /**< Drive strength of 13.5 mA. */
    DRIV_STR_18MA   = 3, /**< Drive strength of 18 mA. */
} GPIO_DRIV_STR_TYPE;

/**
 * @enum GPIO_SMT_TYPE
 * @brief Schmitt trigger enable/disable.
 */
typedef enum {
    SMT_DISABLE = 0, /**< Schmitt trigger disabled. */
    SMT_ENABLE  = 1, /**< Schmitt trigger enabled. */
} GPIO_SMT_TYPE;

/**
 * @enum GPIO_PUSEL_TYPE
 * @brief GPIO pull resistor selection.
 */
typedef enum {
    PUSEL_DISABLE   = 0, /**< No pull-up or pull-down. */
    PUSEL_PULL_UP   = 1, /**< Enable pull-up resistor. */
    PUSEL_PULL_DOWN = 2, /**< Enable pull-down resistor. */
} GPIO_PUSEL_TYPE;

/**
 * @enum GPIO_DEBS_TIME
 * @brief GPIO debounce time configuration.
 */
typedef enum {
    DEBS_TIME_31_2MS = 0, /**< Debounce time of 31.2 ms. */
    DEBS_TIME_RESV   = 1, /**< Reserved. */
    DEBS_TIME_15_6MS = 2, /**< Debounce time of 15.6 ms. */
    DEBS_TIME_7_8MS  = 3, /**< Debounce time of 7.8 ms. */
    DEBS_TIME_OFF    = 4, /**< Debounce function disabled. */
} GPIO_DEBS_TIME;

/**
 * @enum GPIO_PWM_NUM
 * @brief PWM channel number.
 */
typedef enum {
    PWM_NUM_0   = 0, /**< PWM channel 0. */
    PWM_NUM_1   = 1, /**< PWM channel 1. */
    PWM_NUM_2   = 2, /**< PWM channel 2. */
    PWM_NUM_3   = 3, /**< PWM channel 3. */
    PWM_NUM_4   = 4, /**< PWM channel 4. */
    PWM_NUM_5   = 5, /**< PWM channel 5. */
    PWM_NUM_OFF = 6, /**< No PWM channel assigned. */
} GPIO_PWM_NUM;

/**
 * @enum GPIO_DIRECTION
 * @brief GPIO pin direction.
 */
typedef enum {
    GPIO_DIR_INPUT = 0, /**< Pin configured as input. */
    GPIO_DIR_OUTPUT,    /**< Pin configured as output. */
    GPIO_DIR_NONE,      /**< Direction not configured or invalid. */
} GPIO_DIRECTION;

/**
 * @enum GPIO_STATUS
 * @brief GPIO pin logic level.
 */
typedef enum {
    GPIO_LOW = 0, /**< Logic low level. */
    GPIO_HIGH,    /**< Logic high level. */
} GPIO_STATUS;

/**
 * @name GPIO interrupt enable/disable
 * @{
 */
enum {
    GPIO_INT_DISABLE = 0, /**< Disable interrupt. */
    GPIO_INT_ENABLE  = 1, /**< Enable interrupt. */
};
/** @} */

/**
 * @name GPIO interrupt types
 * @brief Level/edge and polarity configuration for GPIO interrupts.
 * @{
 */
enum {
    GPIO_INT_TYPE_LEVEL_LOW    = 0, /**< Level-low triggered interrupt. */
    GPIO_INT_TYPE_LEVEL_HIGH   = 1, /**< Level-high triggered interrupt. */
    GPIO_INT_TYPE_EDGE_FALLING = 2, /**< Edge-falling triggered interrupt. */
    GPIO_INT_TYPE_EDGE_RISING  = 3, /**< Edge-rising triggered interrupt. */
    GPIO_INT_TYPE_EDGE_BOTH    = 4, /**< Edge-both triggered interrupt. */
    GPIO_INT_TYPE_NONE         = 5, /**< No interrupt. */
};
/** @} */

/**
 * @struct WISE_GPIO_CFG_T
 * @brief GPIO configuration structure.
 *
 * Describes basic configuration for a single GPIO pin.
 */
typedef struct {
    uint8_t pin_idx;   /**< GPIO pin index. */
    uint8_t mode;      /**< Pin mode, see ::GPIO_MODE_T. */
    uint8_t direction; /**< Direction, see ::GPIO_DIRECTION. */
    uint8_t int_en;    /**< Interrupt enable flag (GPIO_INT_ENABLE / GPIO_INT_DISABLE). */
    uint8_t int_type;  /**< Interrupt type, see GPIO_INT_TYPE_* definitions. */
} WISE_GPIO_CFG_T;

/**
 * @struct WISE_GPIO_INT_CFG_T
 * @brief GPIO interrupt configuration.
 */
typedef struct {
    uint8_t int_en;   /**< Interrupt enable flag. */
    uint8_t int_type; /**< Interrupt type, see GPIO_INT_TYPE_* definitions. */
} WISE_GPIO_INT_CFG_T;

/**
 * @brief Initialize the GPIO subsystem.
 *
 * Must be called before using any other GPIO APIs.
 */
void wise_gpio_init(void);

/**
 * @brief Deinitialize the GPIO subsystem.
 *
 * Releases any resources allocated for GPIO and returns the subsystem
 * to an uninitialized state.
 */
void wise_gpio_deinit(void);

/**
 * @brief Configure a GPIO pin.
 *
 * Applies the configuration contained in @p gpio_cfg to the specified pin.
 *
 * @param[in] gpio_cfg Pointer to a ::WISE_GPIO_CFG_T structure.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Configuration failed (invalid parameters or HW error).
 */
int32_t wise_gpio_cfg(WISE_GPIO_CFG_T *gpio_cfg);

/**
 * @brief Set pin mode.
 *
 * @param[in] pin_idx GPIO pin index.
 * @param[in] mode    Pin mode, see ::GPIO_MODE_T.
 */
void wise_gpio_set_mode(uint8_t pin_idx, uint8_t mode);

/**
 * @brief Configure PIO function for a pin.
 *
 * Selects a peripheral function (UART/SPI/I2C/GPIO) for the given pin.
 *
 * @param[in] pinIdx  GPIO pin index.
 * @param[in] pinFunc Function selection, see ::GPIO_PIN_FUNC_T.
 *
 * @retval 0   Configuration successful.
 * @retval <0  Configuration failed.
 */
int32_t wise_gpio_func_cfg(uint8_t pinIdx, uint8_t pinFunc);

/**
 * @brief Set GPIO direction.
 *
 * Configures a pin as input or output. 
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx   GPIO pin index.
 * @param[in] direction Direction, see ::GPIO_DIRECTION.
 */
void wise_gpio_set_direction(uint8_t pin_idx, GPIO_DIRECTION direction);

/**
 * @brief Get GPIO direction.
 *
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx GPIO pin index.
 *
 * @return Current direction value (one of ::GPIO_DIRECTION).
 */
uint8_t wise_gpio_get_direction(uint8_t pin_idx);

/**
 * @brief Configure interrupt for a GPIO pin.
 *
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx GPIO pin index.
 * @param[in] int_cfg Pointer to interrupt configuration.
 */
void wise_gpio_set_interrupt(uint8_t pin_idx, WISE_GPIO_INT_CFG_T *int_cfg);

void wise_gpio_clear_int_status(uint8_t pin_idx);

/**
 * @brief Register a callback for a GPIO interrupt.
 *
 * The callback type @c CALLBACK_T is defined in the underlying HAL interface.
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] gpio_idx GPIO pin index.
 * @param[in] cb       Callback function.
 * @param[in] context  User context pointer passed to the callback.
 *
 * @retval 0   Callback registered successfully.
 * @retval <0  Registration failed.
 */
int32_t wise_gpio_register_callback(uint8_t gpio_idx, CALLBACK_T cb, void *context);

/**
 * @brief Unregister a previously registered GPIO callback.
 *
 * @param[in] gpio_idx GPIO pin index.
 *
 * @retval 0   Callback unregistered successfully.
 * @retval <0  Operation failed.
 */
int32_t wise_gpio_unregister_callback(uint8_t gpio_idx);

/**
 * @brief Read pin value.
 *
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx GPIO pin index.
 *
 * @return Pin status ::GPIO_LOW or ::GPIO_HIGH.
 */
GPIO_STATUS wise_gpio_read(uint8_t pin_idx);

/**
 * @brief Write pin value.
 *
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx GPIO pin index.
 * @param[in] status  Desired pin level (::GPIO_LOW or ::GPIO_HIGH).
 */
void wise_gpio_write(uint8_t pin_idx, GPIO_STATUS status);

/**
 * @brief Toggle pin value.
 *
 * Flips the current output state of the specified GPIO pin.
 * The specific pin should be configured to MODE_GPIO mode first.
 *
 * @param[in] pin_idx GPIO pin index.
 */
void wise_gpio_toggle(uint8_t pin_idx);

/**
 * @brief Configure pull-up/pull-down for a GPIO pin.
 *
 * @param[in] pin_idx  GPIO pin index.
 * @param[in] pull_sel Pull selection, see ::GPIO_PUSEL_TYPE.
 */
void wise_gpio_set_pull_sel(uint8_t pin_idx, uint8_t pull_sel);

/**
 * @brief Set debounce time for a GPIO pin.
 *
 * @param[in] pin_idx   GPIO pin index.
 * @param[in] debs_time Debounce setting, see ::GPIO_DEBS_TIME.
 */
void wise_gpio_set_debs_time(uint8_t pin_idx, uint8_t debs_time);

/**
 * @brief Set drive strength for a GPIO pin.
 *
 * @param[in] pin_idx  GPIO pin index.
 * @param[in] driv_str Drive strength, see ::GPIO_DRIV_STR_TYPE.
 */
void wise_gpio_set_driv_str(uint8_t pin_idx, uint8_t driv_str);

/**
 * @brief Enable or disable Schmitt trigger on a GPIO pin.
 *
 * @param[in] pin_idx GPIO pin index.
 * @param[in] enable  Non-zero to enable, zero to disable.
 */
void wise_gpio_set_schmitt_trig(uint8_t pin_idx, uint8_t enable);

/**
 * @brief Assign or unassign a PWM channel to a GPIO pin.
 *
 * @param[in] pwm_num PWM channel, see ::GPIO_PWM_NUM.
 * @param[in] pin_idx GPIO pin index.
 * @param[in] enable  Non-zero to enable PWM on this pin, zero to disable.
 */
void wise_gpio_set_pwm(uint8_t pwm_num, uint8_t pin_idx, uint8_t enable);

/**
 * @brief Configure PWM slow mode on a GPIO pin.
 *
 * @param[in] pin_idx GPIO pin index.
 * @param[in] enable  Non-zero to enable slow mode, zero to disable.
 */
void wise_gpio_set_pwmslow(uint8_t pin_idx, uint8_t enable);

/**
 * @brief Disable PWM I/O on a GPIO pin.
 *
 * Restores the pin from PWM function back to normal GPIO operation.
 *
 * @param[in] gpio_index GPIO pin index.
 */
void wise_pwm_io_disable_pin(uint8_t gpio_index);

/**
 * @brief Configure debug bus output on a GPIO pin.
 *
 * Routes a selected internal debug signal to a given GPIO pin.
 *
 * @param[in] pin_idx        GPIO pin index.
 * @param[in] dbg_module_idx Debug module index.
 * @param[in] dbg_single_idx Debug signal index within the module.
 * @param[in] dbg_func_idx   Debug function selection.
 */
void wise_gpio_set_debug_bus(GPIO_DBG_CFG_INFO* gpio_dbg_cfg);

/** @} */ /* end of WISE_GPIO group */

#endif /* _WISE_GPIO_API_H_ */
