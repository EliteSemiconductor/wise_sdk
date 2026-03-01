/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file demo_gpio_app.c
 * @brief Example application demonstrating WISE_GPIO_API usage.
 *
 * @ingroup WISE_EXAMPLE_APP_GPIO
 *
 * This example shows how to:
 * - Configure GPIO pins as output using both integrated configuration
 *   (::wise_gpio_cfg) and separate APIs (::wise_gpio_set_mode,
 *   ::wise_gpio_set_direction).
 * - Configure a GPIO pin as input and enable interrupt with a callback.
 * - Toggle output pins periodically using ::wise_gpio_write and
 *   ::wise_gpio_toggle.
 *
 * @note This file is an example application and is documented under the
 *       Example APP category (not Core API).
 */

#include <stdio.h>
#include "wise.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "util.h"
#include "demo_app_common.h"

#define DEMO_GPIO_PIN_1             5 /**< GPIO pin index used as output (pin 5). */
#define DEMO_GPIO_PIN_2             6 /**< GPIO pin index used as output (pin 6). */
#define DEMO_GPIO_PIN_3             7 /**< GPIO pin index used as input with interrupt (pin 7). */

/**
 * @defgroup WISE_EXAMPLE_APP_GPIO GPIO Example App
 * @ingroup WISE_EXAMPLE_APP
 * @brief Example application for demonstrating WISE_GPIO_API usage.
 *
 * This example uses the Core API module @ref WISE_GPIO to demonstrate:
 * - Output configuration and toggling
 * - Input configuration with interrupt callback
 * @{
 */

/**
 * @brief GPIO interrupt callback for DEMO_GPIO_PIN_3.
 *
 * This callback is registered via ::wise_gpio_register_callback and is
 * triggered based on the interrupt configuration set by ::wise_gpio_set_interrupt.
 *
 * @param[in] context User context pointer passed during callback registration.
 * @param[in] idx     GPIO pin index that triggered the interrupt.
 */
static void _gpio_int_callback(void *context, uint8_t idx);

static void _gpio_int_callback(void *context, uint8_t idx)
{
    (void)context;
    debug_print("gpio %d int\n");
}

/**
 * @brief Main entry of the GPIO example application.
 *
 * Initializes demo environment and demonstrates common WISE GPIO operations:
 * - Configure pin 5 as output using ::wise_gpio_cfg
 * - Configure pin 6 as output using ::wise_gpio_set_mode and ::wise_gpio_set_direction
 * - Configure pin 7 as input with interrupt and callback
 * - Periodically toggle pin outputs with delays
 */
void main(void)
{
    WISE_GPIO_CFG_T pinConfig;
    WISE_GPIO_INT_CFG_T gpioIntConfig;

    demo_app_common_init();

    /* wise_gpio_init() is already called in system global init. */

    /* Configure a GPIO pin as output using integrated configuration. */
    pinConfig.pin_idx = DEMO_GPIO_PIN_1;
    pinConfig.mode = MODE_GPIO;
    pinConfig.direction = GPIO_DIR_OUTPUT;
    pinConfig.int_en = GPIO_INT_DISABLE;
    pinConfig.int_type = GPIO_INT_TYPE_NONE;

    wise_gpio_cfg(&pinConfig);

    /* Configure a GPIO pin as output using separated APIs. */
    wise_gpio_set_mode(DEMO_GPIO_PIN_2, MODE_GPIO);
    wise_gpio_set_direction(DEMO_GPIO_PIN_2, GPIO_DIR_OUTPUT);

    /* Configure a GPIO pin as input mode and enable interrupt. */
    wise_gpio_set_mode(DEMO_GPIO_PIN_3, MODE_GPIO);
    wise_gpio_set_direction(DEMO_GPIO_PIN_3, GPIO_DIR_INPUT);
    wise_gpio_register_callback(DEMO_GPIO_PIN_3, _gpio_int_callback, NULL);
    gpioIntConfig.int_en = GPIO_INT_ENABLE;
    gpioIntConfig.int_type = GPIO_INT_TYPE_LEVEL_LOW;
    wise_gpio_set_interrupt(DEMO_GPIO_PIN_3, &gpioIntConfig);

    while (1)
    {
        debug_print("set io %d low\n", DEMO_GPIO_PIN_1);
        wise_gpio_write(DEMO_GPIO_PIN_1, GPIO_LOW);

        debug_print("toggle io %d\n", DEMO_GPIO_PIN_2);
        wise_gpio_toggle(DEMO_GPIO_PIN_2);

        wise_tick_delay_ms(2000);

        debug_print("set io %d high\n", DEMO_GPIO_PIN_1);
        wise_gpio_write(DEMO_GPIO_PIN_1, GPIO_HIGH);

        debug_print("toggle io %d\n", DEMO_GPIO_PIN_2);
        wise_gpio_toggle(DEMO_GPIO_PIN_2);

        wise_tick_delay_ms(2000);

        wise_main_proc();
    }
}

/** @} */ /* end of WISE_EXAMPLE_APP_GPIO */
