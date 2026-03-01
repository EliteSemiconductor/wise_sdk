/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_pwmslow_api.h"

/*---------------------------------------------------------------
  init / deinit
  ---------------------------------------------------------------*/
WISE_STATUS wise_pwmslow_init(void)
{
    return WISE_SUCCESS;
}

WISE_STATUS wise_pwmslow_deinit(void)
{
    return wise_pmu_module_clk_disable(PWM_SLOW_MODULE);
}
/*---------------------------------------------------------------
  configure
  ---------------------------------------------------------------*/
WISE_STATUS wise_pwmslow_configure(uint8_t gpio_index, WISE_PWMSLOW_CONF_T *pwmslow_conf)
{
    if (pwmslow_conf == NULL) {
        return WISE_INVALID_INDEX;
    }

    wise_pmu_module_clk_enable(PWM_SLOW_MODULE);
    wise_pwm_io_disable_pin(gpio_index);
    HAL_PWMSLOW_CONF_T hal_pwmslow_conf;
    memset(&hal_pwmslow_conf, 0, sizeof(hal_pwmslow_conf));

    if (pwmslow_conf->common.mode == PWMSLOW_MODE_PERIOD) {
        hal_pwmslow_conf.period          = pwmslow_conf->u.period.period;
        hal_pwmslow_conf.active_period   = pwmslow_conf->u.period.active_period;
        hal_pwmslow_conf.center_align_en = pwmslow_conf->common.center_align_en;
        hal_pwmslow_conf.oneshot_en      = pwmslow_conf->common.oneshot_en;
        hal_pwmslow_conf.oneshot_num     = pwmslow_conf->common.oneshot_num;
        hal_intf_pwmslow_cfg_output_period(&hal_pwmslow_conf);
    } else if (pwmslow_conf->common.mode == PWMSLOW_MODE_FREQUENCY) {
        hal_pwmslow_conf.frequency_Hz    = pwmslow_conf->u.freq.frequency_Hz;
        hal_pwmslow_conf.duty_percent    = pwmslow_conf->u.freq.duty_percent;
        hal_pwmslow_conf.center_align_en = pwmslow_conf->common.center_align_en;
        hal_pwmslow_conf.oneshot_en      = pwmslow_conf->common.oneshot_en;
        hal_pwmslow_conf.oneshot_num     = pwmslow_conf->common.oneshot_num;
        hal_intf_pwmslow_cfg_output_period(&hal_pwmslow_conf);
        hal_intf_pwmslow_cfg_output_frequency(&hal_pwmslow_conf);
    } else {
        return WISE_INVALID_INDEX;
    }

    if (pwmslow_conf->common.oneshot_en == ENABLE) {
        hal_intf_pwmslow_enable_oneshot_interrupt();
    } else {
        hal_intf_pwmslow_disable_oneshot_interrupt();
    }

    hal_intf_pwmslow_set_io_pin(&hal_pwmslow_conf);
    wise_gpio_set_pwmslow(gpio_index, ENABLE);
    wise_gpio_set_mode(gpio_index, MODE_PWM);
    return WISE_SUCCESS;
}
/*---------------------------------------------------------------
  start / stop
  ---------------------------------------------------------------*/
void wise_pwmslow_start(void)
{
    hal_intf_pwmslow_start();
}

void wise_pwmslow_stop(void)
{
    hal_intf_pwmslow_stop();
}
/*---------------------------------------------------------------
  Callback Registration
  ---------------------------------------------------------------*/
WISE_STATUS wise_pwmslow_register_callback(WISE_PWMSLOW_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_intf_pwmslow_register_callback(event, cb, context);
}

WISE_STATUS wise_pwmslow_unregister_callback(WISE_PWMSLOW_CB_EVENT_T event)
{
    return hal_intf_pwmslow_unregister_callback(event);
}
