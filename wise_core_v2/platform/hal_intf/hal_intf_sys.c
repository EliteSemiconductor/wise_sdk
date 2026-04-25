/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_sys.h"
#include "esmt_chip_specific.h"
#include "drv/hal_drv_sys.h"
#include "drv/hal_drv_pmu.h"
#include "util_debug_log.h"

#ifdef CHIP_HAS_LFOSC

static uint8_t _ext32k_lp_workaround_en = 0;

void *hal_intf_sys_get_sclk_cfg(uint8_t osc_idx, uint8_t work_mode)
{
    const void *cfg = NULL;

    WISE_LOG_DBG("cfg_to_use = ");
    if (osc_idx == SYS_LFOSC_CLK_SRC_INTERNAL_32K) {
        switch (work_mode) {
        case SCLK_32K_SRC_RUN_32K:
            cfg = &sclk_src_32k_default;
            WISE_LOG_DBG("SCLK_32K_SRC_RUN_32K\n");
            break;
        case SCLK_32K_SRC_RUN_16K:
            cfg = &sclk_src_32k_run_16k;
            WISE_LOG_DBG("SCLK_32K_SRC_RUN_16K\n");
            break;
        case SCLK_32K_SRC_RUN_8K:
            cfg = &sclk_src_32k_run_8k;
            WISE_LOG_DBG("SCLK_32K_SRC_RUN_8K\n");
            break;
        default:
            cfg = &sclk_src_32k_default;
            WISE_LOG_ERR("osc 32k work_mode is bad = %d\n", work_mode);
            break;
        }
    }

    if (osc_idx == SYS_LFOSC_CLK_SRC_INTERNAL_16K) {
        switch (work_mode) {
        case SCLK_16K_PWR_NORMAL:
            cfg = &sclk_src_16k_run_default;
            WISE_LOG_DBG("SCLK_16K_PWR_NORMAL\n");
            break;
        case SCLK_16K_SRC_RUN_32K:
            cfg = &sclk_src_16k_run_32k;
            WISE_LOG_DBG("SCLK_16K_SRC_RUN_32K\n");
            break;
        case SCLK_16K_PWR_LOWPOWER:
            cfg = &sclk_src_16k_run_default_lowpwr;
            WISE_LOG_DBG("SCLK_16K_PWR_LOWPOWER\n");
            break;
        case SCLK_16K_PWR_LOWPOWER_0P6V:
            cfg = &sclk_src_16k_run_default_lowpwr0p6v;
            WISE_LOG_DBG("SCLK_16K_PWR_LOWPOWER_0P6V\n");
            break;
        default:
            cfg = &sclk_src_16k_run_default;
            WISE_LOG_ERR("osc 16k work_mode is bad = %d\n", work_mode);
            break;
        }
    }

    if (osc_idx == SYS_LFOSC_CLK_SRC_EXTERNAL_32K) {
#ifdef CHIP_LFOSC_SUPPORT_EXT_32K
        cfg = &sclk_src_ext32k;
        WISE_LOG_DBG("osc ext 32k\n");
#else
        cfg = NULL;
#endif
    }

    return (void *)cfg;
}

#endif //CHIP_HAS_LFOSC

void hal_intf_sys_switch_sclk_src(uint8_t sclk_idx)
{
    hal_drv_sys_switch_sclk_src(sclk_idx);
}

int8_t hal_intf_sys_exec_internal_sclk_calibration(const HAL_INTERNAL_SCLK_CFG_T *cfg)
{
    switch (cfg->sclk_sel) {

        case SYS_LFOSC_CLK_SRC_INTERNAL_16K: {
            DRV_SCLK16K_PARAM_T p = {
                .sclk_16k_option      = cfg->cfg.sclk_16k_cfg.sclk_16k_option,
                .ldo_cur              = cfg->cfg.sclk_16k_cfg.ldo_cur,
                .ldo_sel              = cfg->cfg.sclk_16k_cfg.ldo_sel,
                .ldo_tmp              = cfg->cfg.sclk_16k_cfg.ldo_tmp,
                .freq_ctrl_sel        = cfg->cfg.sclk_16k_cfg.freq_ctrl_sel,
                .sclk_settle          = cfg->cfg.sclk_16k_cfg.sclk_settle,
                .target_count         = cfg->cfg.sclk_16k_cfg.target_count,
                .step_coarse          = cfg->cfg.sclk_16k_cfg.step_coarse,
                .num_slow_sclk_cycle  = cfg->cfg.sclk_16k_cfg.num_slow_sclk_cycle,
                .init_ctrl_index      = cfg->cfg.sclk_16k_cfg.init_ctrl_index,
                .coarse_diff          = cfg->cfg.sclk_16k_cfg.coarse_diff,
                .cal_cnt              = cfg->cfg.sclk_16k_cfg.cal_cnt,
                .step_fine            = cfg->cfg.sclk_16k_cfg.step_fine,
                .nostop               = cfg->cfg.sclk_16k_cfg.nostop,
            };

            return hal_drv_sys_internal_16kosc_calibration(&p);
        }
        case SYS_LFOSC_CLK_SRC_INTERNAL_32K: {
            DRV_SCLK32K_PARAM_T p = {
                .sclk_32k_option = cfg->cfg.sclk_32k_cfg.sclk_32k_option,
                .tim_osc         = cfg->cfg.sclk_32k_cfg.tim_osc,
                .cap_ctrl        = cfg->cfg.sclk_32k_cfg.cap_ctrl,
                .ictrl           = cfg->cfg.sclk_32k_cfg.ictrl,
                .ldo_vdd_lv      = cfg->cfg.sclk_32k_cfg.ldo_vdd_lv,
                .osc_clk_div     = cfg->cfg.sclk_32k_cfg.osc_clk_div,
                .fref_clk_div    = cfg->cfg.sclk_32k_cfg.fref_clk_div,
                .cycles_target   = cfg->cfg.sclk_32k_cfg.cycles_target,
            };

            return hal_drv_sys_internal_32kosc_calibration(&p);
        }
        case SYS_LFOSC_CLK_SRC_EXTERNAL_32K:
            hal_intf_sys_switch_sclk_src(cfg->sclk_sel);
            if (_ext32k_lp_workaround_en) {
                hal_intf_sys_ext32k_workaround();
            }
        	return HAL_NO_ERR;
        default:
        	return HAL_ERR;
    }
    return HAL_ERR;
}

int32_t hal_intf_sys_set_remap(uint32_t remap_addr)
{
    hal_drv_sys_set_remap(remap_addr);
    return HAL_ERR; // why return fail?
}

int32_t hal_intf_sys_lock()
{
    hal_drv_sys_lock();
    return HAL_NO_ERR;
}

void hal_intf_sys_tick_init()
{
    hal_drv_sys_tick_init();
}

uint32_t hal_intf_sys_tick_get_counter()
{
    return hal_drv_sys_tick_get_counter();
}

void hal_intf_sys_tick_delay_ms(uint32_t ms)
{
    hal_drv_sys_tick_delay_ms(ms);
}

void hal_intf_sys_tick_delay_us(uint32_t us)
{
    hal_drv_sys_tick_delay_us(us);
}

uint32_t hal_intf_sys_get_chip_id(void)
{
    return hal_drv_sys_get_chip_id();
}

void hal_intf_sys_swd_cfg(bool swd_enable)
{
#ifdef CHIP_GPIO_BIND_ALT
    hal_drv_sys_swd_cfg(swd_enable);
#endif
}

void hal_intf_sys_tcxo_cfg(bool tcxo_enable)
{
#ifdef CHIP_GPIO_BIND_ALT
    hal_drv_sys_tcxo_cfg(tcxo_enable);
#endif
}

void hal_intf_sys_set_pa_type(uint8_t pa_type)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    hal_drv_sys_set_pa_type(pa_type);
#endif
}

uint8_t hal_intf_sys_get_pa_type(void)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    return hal_drv_sys_get_pa_type();
#endif
}

void hal_intf_sys_set_board_match_type(uint8_t mat_type)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    hal_drv_sys_set_board_match_type(mat_type);
#endif
}

uint8_t hal_intf_sys_get_board_match_type(void)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    return hal_drv_sys_get_board_match_type();
#endif
}

void hal_intf_sys_set_40m_gain_ctrl(uint8_t ctrl_value)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    hal_drv_sys_set_40m_gain_ctrl(ctrl_value);
#endif
}

void hal_intf_sys_set_xtal_cfg(uint8_t xtal_i, uint8_t xtal_o)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    hal_drv_sys_set_xtal_cfg(xtal_i, xtal_o);
#endif
}

void hal_intf_sys_set_lpxtal_cfg(uint8_t cap_i, uint8_t cap_o, uint8_t maincap_i, uint8_t maincap_o, uint8_t gain)
{
#ifdef CHIP_SUPPORT_CUSTOM_BOARD
    hal_drv_sys_set_lpxtal_cfg(cap_i, cap_o, maincap_i, maincap_o, gain);
#else
    (void)cap_i; (void)cap_o; (void)maincap_i; (void)maincap_o; (void)gain;
#endif
}

void hal_intf_sys_set_sram_size(uint8_t sram_32_64)
{
    hal_intf_pmu_set_sram_pd_mode(sram_32_64);
}

void hal_intf_sys_ext32k_workaround(void)
{
#ifdef CHIP_HAS_EXT32K_LP_WORKAROUND
    hal_drv_sys_ext32k_workaround();
#endif
}

void hal_intf_sys_set_ext32k_lp_workaround(uint8_t enable)
{
#ifdef CHIP_HAS_LFOSC
    _ext32k_lp_workaround_en = enable ? 1 : 0;
#else
    (void)enable;
#endif
}

/* ================== ASARADC =============== */
#ifdef CHIP_ADC_SUPPORT

void hal_intf_asaradc_start(void)
{
    hal_drv_asaradc_start();
}

bool hal_intf_asaradc_is_ready(void)
{
    return hal_drv_asaradc_is_ready();
}

void hal_intf_asaradc_config(bool vref, bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2, bool daf_enable, uint8_t daf_shift_n)
{
    hal_drv_asaradc_verf_config(vref, DISABLE, 0);
    hal_drv_asaradc_basic_config(fetch_mode, timer_mode1, timer_mode2);
    hal_drv_asaradc_enable_digital_average_filter(daf_enable, daf_shift_n);
}

void hal_intf_asaradc_set_vin_sel(uint8_t vin_sel)
{
    hal_drv_asaradc_set_vin_sel(vin_sel);
}

uint8_t hal_intf_asaradc_get_vin_sel(void)
{
    return hal_drv_asaradc_get_vin_sel();
}

void hal_intf_asaradc_set_interrupt(bool enable)
{
    hal_drv_asaradc_set_interrupt(enable);
}

bool hal_intf_asaradc_get_int_status(void)
{
    return hal_drv_asaradc_get_int_status();
}

bool hal_intf_asaradc_get_int_masked_status(void)
{
    return hal_drv_asaradc_get_int_masked_status();
}

void hal_intf_asaradc_clear_int_status(void)
{
    hal_drv_asaradc_clear_int_status();
}

uint16_t hal_intf_asaradc_get_data(void)
{
    return hal_drv_asaradc_get_data();
}

uint32_t hal_intf_asaradc_get_hr_data(void)
{
    return hal_drv_asaradc_get_hr_data();
}
#endif //#ifdef CHIP_ADC_SUPPORT
