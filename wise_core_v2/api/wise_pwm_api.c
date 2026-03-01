/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_pwm_api.h"

#define PWM_INITED                          (1 << 31)
#define PWM_CLK_ENABLED                     (1 << 30)

#define PWM_SET_INITED()                    (_pwmChannelState |= PWM_INITED)
#define PWM_CLEAR_INITED()                  (_pwmChannelState &= ~PWM_INITED)
#define PWM_IS_INITED()                     ((_pwmChannelState & PWM_INITED) ? 1 : 0)

#define PWM_SET_CLK_ENABLED()               (_pwmChannelState |= PWM_CLK_ENABLED)
#define PWM_CLEAR_CLK_DISABLED()            (_pwmChannelState &= ~PWM_CLK_ENABLED)
#define PWM_IS_CLK_ENABLED()                ((_pwmChannelState & PWM_CLK_ENABLED) ? 1 : 0)

#define PWM_CH_SET_CONF(ch)                 (_pwmChannelState |= (1 << (ch*2)))
#define PWM_CH_CLR_CONF(ch)                 (_pwmChannelState &= ~(1 << (ch*2)))
#define PWM_CH_IS_CONF(ch)                  ((_pwmChannelState & (1 << (ch*2))) ? 1 : 0)

#define PWM_CH_SET_START(ch)                (_pwmChannelState |= (1 << (ch*2 + 1)))
#define PWM_CH_CLR_START(ch)                (_pwmChannelState &= ~(1 << (ch*2 + 1)))
#define PWM_CH_IS_START(ch)                 ((_pwmChannelState & (1 << (ch*2 + 1))) ? 1 : 0)

static uint32_t _pwmChannelState = 0;

static WISE_STATUS _do_pwm_ch_configiture(uint8_t channel, WISE_PWM_CONF_T *pwm_conf);

/*---------------------------------------------------------------
  init / deinit
  ---------------------------------------------------------------*/
WISE_STATUS wise_pwm_init(void)
{
    _pwmChannelState = 0;
    PWM_SET_INITED();
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_pwm_deinit(void)
{
    
    return wise_pmu_module_clk_disable(PWM_MODULE);
}

/*---------------------------------------------------------------
  configure
  ---------------------------------------------------------------*/
static WISE_STATUS _do_pwm_ch_configiture(uint8_t channel, WISE_PWM_CONF_T *pwm_conf)
{
    HAL_PWM_CONF_T hal_pwm_conf = {0};

    if (pwm_conf->common.mode == PWM_MODE_PERIOD) 
    {
        hal_pwm_conf.period          = pwm_conf->u.period.period;
        hal_pwm_conf.active_period   = pwm_conf->u.period.active_period;
        hal_pwm_conf.center_align_en = pwm_conf->common.center_align_en;
        hal_pwm_conf.oneshot_en      = pwm_conf->common.oneshot_en;
        hal_pwm_conf.oneshot_num     = pwm_conf->common.oneshot_num;
        hal_intf_pwm_cfg_output_period(1u << channel, &hal_pwm_conf);
    } 
    else if (pwm_conf->common.mode == PWM_MODE_FREQUENCY) 
    {
        hal_pwm_conf.frequency_Hz    = pwm_conf->u.freq.frequency_Hz;
        hal_pwm_conf.duty_percent    = pwm_conf->u.freq.duty_percent;
        hal_pwm_conf.center_align_en = pwm_conf->common.center_align_en;
        hal_pwm_conf.oneshot_en      = pwm_conf->common.oneshot_en;
        hal_pwm_conf.oneshot_num     = pwm_conf->common.oneshot_num;
        hal_intf_pwm_cfg_output_frequency(1u << channel, &hal_pwm_conf);
    } 
    else 
    {
        return WISE_FAIL;
    }

    if (pwm_conf->common.oneshot_en == ENABLE) 
    {
        hal_intf_pwm_enable_oneshot_interrupt(1u << channel);
    } 
    else 
    {
        hal_intf_pwm_disable_oneshot_interrupt(1u << channel);
    }

    hal_intf_pwm_set_io_for_channel_pin(channel, &hal_pwm_conf);
    
    return WISE_SUCCESS;
}
  
WISE_STATUS wise_pwm_configure(uint8_t channel, uint8_t gpio_index, WISE_PWM_CONF_T *pwm_conf)
{
    if ((pwm_conf == NULL) || (!PWM_IS_INITED()))
    {
        return WISE_FAIL;
    }
    
    if(!PWM_IS_CLK_ENABLED())
    {
        wise_pmu_module_clk_enable(PWM_MODULE);
        PWM_SET_CLK_ENABLED();
    }
    
    wise_pwm_io_disable_pin(gpio_index);

    _do_pwm_ch_configiture(channel, pwm_conf);

    wise_gpio_set_pwm(channel, gpio_index, ENABLE);
    wise_gpio_set_mode(gpio_index, MODE_PWM);

    PWM_CH_SET_CONF(channel);
    
    return WISE_SUCCESS;
}

WISE_STATUS wise_pwm_reconf_channel(uint8_t channel, WISE_PWM_CONF_T* pwm_conf)
{
    if(PWM_CH_IS_START(channel))
        return _do_pwm_ch_configiture(channel, pwm_conf);
    else
        return WISE_FAIL;
}

/*---------------------------------------------------------------
  start / stop
  ---------------------------------------------------------------*/
void wise_pwm_start(uint32_t channel_mask)
{
    int i;

    for(i = 0; i < CHIP_PWM_CHANNEL_NUM; i++)
    {
        if((channel_mask >> i) & 0x1)
        {
            if(PWM_CH_IS_CONF(i))
                PWM_CH_SET_START(i);
            else
                channel_mask &= ~(1 << i);
        }
    }

    if(channel_mask)
        hal_intf_pwm_start(channel_mask);
}

void wise_pwm_stop(uint32_t channel_mask)
{
    int i;

    for(i = 0; i < CHIP_PWM_CHANNEL_NUM; i++)
    {
        if((channel_mask >> i) & 0x1)
        {
            if(PWM_CH_IS_START(i))
                PWM_CH_CLR_START(i);
            else
                channel_mask &= ~(1 << i);
        }
    }
    
    if(channel_mask)
        hal_intf_pwm_stop(channel_mask);
}

/*---------------------------------------------------------------
  Callback Registration
  ---------------------------------------------------------------*/
WISE_STATUS wise_pwm_register_callback(WISE_PWM_CB_EVENT_T event, CALLBACK_T cb, void *context)
{
    return hal_intf_pwm_register_callback(event, cb, context);
}

WISE_STATUS wise_pwm_unregister_callback(WISE_PWM_CB_EVENT_T event)
{
    return hal_intf_pwm_unregister_callback(event);
}
