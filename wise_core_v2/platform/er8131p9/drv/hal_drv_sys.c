/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "drv/hal_drv_sys.h"
#include "hdl/sys_er8130.h"
#include "ana_er8130.h"
#include "hal_intf_sys.h"
#include "drv/hal_drv_pmu.h"
#include "drv/hal_drv_extpmu.h"

#ifdef CHIP_TICK_TIMER_CHANNEL
#include "drv/hal_drv_gptmr.h"
#endif

#include "util.h"

static volatile uint32_t tickRound   = 0;
static volatile uint32_t prevRound   = 0;
static volatile uint32_t prevCounter = 0;

#define HAL_CLK_PER_US                  (__SYSTEM_CLOCK / 1000000)
#define HAL_US_TO_CLK(u)                ((u) * HAL_CLK_PER_US)
#define HAL_IS_IN_ISR()                 ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0)


#ifdef CHIP_HAS_LFOSC
const HAL_INTERNAL_SCLK_CFG_T sclk_src_ext32k = {.sclk_sel = SYS_LFOSC_CLK_SRC_EXTERNAL_32K };

const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_default = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_32K,
    .cfg      = {.sclk_32k_cfg = {.sclk_32k_option = SCLK_32K_SRC_RUN_32K,
                                  .tim_osc         = 0x3,
                                  .cap_ctrl        = 0x0,
                                  .ictrl           = 0x2,
                                  .ldo_vdd_lv      = 0x0,
                                  .osc_clk_div     = 0x7,
                                  .fref_clk_div    = 0x1,
                                  .cycles_target   = 9800}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_16k = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_32K,
    .cfg      = {.sclk_32k_cfg = {.sclk_32k_option = SCLK_32K_SRC_RUN_16K,
                                  .tim_osc         = 0x3,
                                  .cap_ctrl        = 0x0,
                                  .ictrl           = 0x1,
                                  .ldo_vdd_lv      = 0x0,
                                  .osc_clk_div     = 0x3,
                                  .fref_clk_div    = 0x1,
                                  .cycles_target   = 9780}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_8k = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_32K,
    .cfg      = {.sclk_32k_cfg = {.sclk_32k_option = SCLK_32K_SRC_RUN_8K,
                                  .tim_osc         = 0x3,
                                  .cap_ctrl        = 0x0,
                                  .ictrl           = 0x1,
                                  .ldo_vdd_lv      = 0x0,
                                  .osc_clk_div     = 0x1,
                                  .fref_clk_div    = 0x1,
                                  .cycles_target   = 9790}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_16K,
    .cfg      = {.sclk_16k_cfg = {.sclk_16k_option     = SCLK_16K_PWR_NORMAL,
                                  .ldo_cur             = 0x0,
                                  .ldo_sel             = 0x0,
                                  .ldo_tmp             = 0x0,
                                  .freq_ctrl_sel       = 3901,
                                  .sclk_settle         = 0x2,
                                  .target_count        = 73242,
                                  .step_coarse         = 20,
                                  .num_slow_sclk_cycle = 62,
                                  .init_ctrl_index     = 2048,
                                  .coarse_diff         = 211,
                                  .cal_cnt             = 100,
                                  .step_fine           = 0,
                                  .nostop              = 0}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_32k = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_16K,
    .cfg      = {.sclk_16k_cfg = {.sclk_16k_option     = SCLK_16K_SRC_RUN_32K,
                                  .ldo_cur             = 0x0,
                                  .ldo_sel             = 0x0,
                                  .ldo_tmp             = 0x0,
                                  .freq_ctrl_sel       = 3591,
                                  .sclk_settle         = 0x2,
                                  .target_count        = 36621,
                                  .step_coarse         = 35,
                                  .num_slow_sclk_cycle = 62,
                                  .init_ctrl_index     = 2048,
                                  .coarse_diff         = 511,
                                  .cal_cnt             = 100,
                                  .step_fine           = 0,
                                  .nostop              = 0}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_16K,
    .cfg      = {.sclk_16k_cfg = {.sclk_16k_option     = SCLK_16K_PWR_LOWPOWER,
                                  .ldo_cur             = 0x0,
                                  .ldo_sel             = 0x0,
                                  .ldo_tmp             = 0x0,
                                  .freq_ctrl_sel       = 3901,
                                  .sclk_settle         = 0x2,
                                  .target_count        = 73242,
                                  .step_coarse         = 20,
                                  .num_slow_sclk_cycle = 62,
                                  .init_ctrl_index     = 2048,
                                  .coarse_diff         = 211,
                                  .cal_cnt             = 100,
                                  .step_fine           = 0,
                                  .nostop              = 0}}};

const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr0p6v = {

    .sclk_sel = SYS_LFOSC_CLK_SRC_INTERNAL_16K,
    .cfg      = {.sclk_16k_cfg = {.sclk_16k_option     = SCLK_16K_PWR_LOWPOWER_0P6V,
                                  .ldo_cur             = 0x0,
                                  .ldo_sel             = 0x0,
                                  .ldo_tmp             = 0x0,
                                  .freq_ctrl_sel       = 3901,
                                  .sclk_settle         = 0x2,
                                  .target_count        = 73242,
                                  .step_coarse         = 20,
                                  .num_slow_sclk_cycle = 62,
                                  .init_ctrl_index     = 2048,
                                  .coarse_diff         = 211,
                                  .cal_cnt             = 100,
                                  .step_fine           = 0,
                                  .nostop              = 0}}};
#endif //CHIP_HAS_LFOSC
uint32_t _hal_enter_critical(void)
{
    uint32_t irqState = __get_PRIMASK();
    __disable_irq();
    return irqState;
}

void _hal_exit_critical(uint32_t irqState)
{
    if (irqState == 0U) {
        __enable_irq();
    }
}

uint32_t hal_drv_sys_get_chip_id(void)
{
    return sys_get_chip_id_er8130();
}

void hal_drv_sys_set_remap(uint32_t remap_addr)
{
    sys_set_remap_er8130(remap_addr);
}

void hal_drv_sys_lock(void)
{
    sys_lock_er8130();
}

void hal_drv_sys_unlock(void)
{
    sys_unlock_er8130();
}

#if 0 
void hal_drv_sys_gpio_cfg(bool swd_enable, bool swd_pullup, bool tcxo_enable)
{
    sys_gpio_cfg_er8130(swd_enable, swd_pullup, tcxo_enable);
}
#endif

void hal_drv_sys_swd_cfg(bool swd_enable)
{
    sys_cfg_swd_er8130(swd_enable);
}

void hal_drv_sys_tcxo_cfg(bool tcxo_enable)
{
    sys_cfg_tcxo_er8130(tcxo_enable);
}

uint8_t hal_drv_sys_get_pa_type(void)
{
    return SYS_GET_PA_TYPE();
}

void hal_drv_sys_set_pa_type(uint8_t pa_type)
{
    SYS_SET_PA_TYPE(pa_type);
}

uint8_t hal_drv_sys_get_board_match_type(void)
{
    return SYS_GET_BOARD_MATCH_TYPE();
}

void hal_drv_sys_set_board_match_type(uint8_t mat_type)
{
    SYS_SET_BOARD_MATCH_TYPE(mat_type);
}

void hal_drv_sys_set_40m_gain_ctrl(uint8_t gain_ctrl)
{
    //set val to sys module
    SYS_SET_BOARD_40M_GAIN(gain_ctrl);
    //set val to ana module
    ana_set_xtal_gain_ctrl_er81xx(gain_ctrl);
}

uint8_t hal_drv_sys_get_40m_gain_ctrl()
{
    return SYS_GET_BOARD_40M_GAIN();
}

uint32_t hal_drv_sys_get_xtal_cfg(void)
{
    return SYS_GET_XTAL_CFG();
}

void hal_drv_sys_set_xtal_cfg(uint8_t xtal_i, uint8_t xtal_o)
{
    SYS_SET_XTAL_CFG(xtal_i, xtal_o);
}

void hal_drv_sys_tick_delay_ms(uint32_t ms)
{
    hal_drv_sys_tick_delay_us(ms * 1000);
}

void hal_drv_sys_tick_delay_us(uint32_t us)
{
    uint32_t startClk = hal_drv_sys_tick_get_counter();

    while ((hal_drv_sys_tick_get_counter() - startClk) <= HAL_US_TO_CLK(us)) {
        __asm volatile("nop");
    }
}

int8_t hal_drv_sys_internal_16kosc_calibration(const DRV_SCLK16K_PARAM_T *p)
{
    if (!p) return HAL_ERR;

    hal_drv_pmu_module_clk_enable(ANA_MODULE);
    ana_control_16kosc_power(true);
    ana_disable_pmu_shutdown_control_for_32k_16k_osc();

    if (p->sclk_16k_option == SCLK_16K_SRC_RUN_32K) {
        ana_tune_16kosc_to_32k_mode(true);
    } else {
        ana_tune_16kosc_to_32k_mode(false);
    }

    ana_power_management_for_16kosc(p->sclk_16k_option);

    ana_ldo_paramaters_for_16kosc_calibration(
        p->ldo_cur,
        p->ldo_sel,
        p->ldo_tmp,
        p->freq_ctrl_sel
    );

    ana_reg0_paramaters_for_16kosc_calibration(
        p->sclk_settle,
        p->target_count,
        p->step_coarse,
		p->step_fine,
		p->nostop
    );

    ana_reg1_paramaters_for_16kosc_calibration(
        p->num_slow_sclk_cycle,
        p->coarse_diff
    );

    ana_reg2_paramaters_for_16kosc_calibration(
        p->init_ctrl_index,
        p->cal_cnt
    );


    if (ana_exec_anctl_osc16k_calibration() != 0) {
        printf("hal_drv_sys_internal_16kosc_calibration: 16k calibration failed\n");
        return HAL_ERR;
    }
    
    hal_drv_extpmu_select_clk_source(SYS_LFOSC_CLK_SRC_INTERNAL_16K);
    hal_drv_pmu_clk_src_sel(SYS_LFOSC_CLK_SRC_INTERNAL_16K);

    return HAL_NO_ERR;
}

int8_t hal_drv_sys_internal_32kosc_calibration(const DRV_SCLK32K_PARAM_T *p)
{
    return HAL_ERR;
}

void hal_drv_sys_switch_sclk_src(uint8_t sclk_idx)
{
    //set sleep source
    hal_drv_pmu_clk_src_sel(sclk_idx);
    //set shutdown source: 0=32K/1=16K
    hal_drv_extpmu_select_clk_source(sclk_idx);
}

#ifdef CHIP_TICK_TIMER_CHANNEL
void hal_drv_sys_tick_init()
{
    uint32_t tickTmrModule = (GPTMR0_MODULE << (CHIP_TICK_TIMER_CHANNEL));
    
    hal_drv_pmu_module_clk_enable(tickTmrModule);
    hal_drv_gptmr_config(CHIP_TICK_TIMER_CHANNEL,
                            TIMER_PERIODIC,
                            0,
                            0,
                            0xffffffff);
    hal_drv_gptmr_start(CHIP_TICK_TIMER_CHANNEL);
}

uint32_t hal_drv_sys_tick_get_counter()
{
    return (~(hal_drv_gptmr_get_cnt(CHIP_TICK_TIMER_CHANNEL)));
}
#else
void hal_drv_sys_tick_init()
{
    tickRound = 0;

    NVIC_EnableIRQ(SysTick_IRQn);

    SysTick->LOAD = 0xFFFFFF;                                                                        /* set reload register */
    SysTick->VAL  = 0;                                                                               /* Load the SysTick Counter Value */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
}

uint32_t hal_drv_sys_tick_get_counter()
{
    uint32_t now     = 0;
    uint32_t curTick = 0;
    uint8_t nowHi, prevHi;
    uint32_t irqState = 0;

    if (!HAL_IS_IN_ISR()) {
        irqState = _hal_enter_critical();
    }

    curTick = (SysTick->VAL);
    now     = ((uint32_t)tickRound << 24) | ((~curTick) & 0xffffff);
    nowHi   = tickRound & 0xff;
    prevHi  = (prevCounter >> 24) & 0xff;

    if (!HAL_IS_IN_ISR()) {
        _hal_exit_critical(irqState);
    }

    if (now < prevCounter) {
        if ((nowHi == 0x00) && (prevHi == 0xff)) {
            //debug_print("tick OVF\n");
        } else {
            return prevCounter;
        }
    }

    prevCounter = now;

    return now;
}

RAM_TEXT void SysTick_Handler(void)
{
    tickRound++;
}
#endif

/* ================== ASARADC =============== */
static bool f_asaradc_data_ready = 0;
void hal_drv_asaradc_start(void)
{
    f_asaradc_data_ready = 0;
    ana_asaradc_start_er8130();
}

bool hal_drv_asaradc_is_ready(void)
{
    //return ana_asaradc_is_ready_er8130();
    return f_asaradc_data_ready;
}

void hal_drv_asaradc_verf_config(bool vref, bool scaler_enable, uint8_t scaler_lv)
{
    ana_asaradc_vref_level(vref);
    ana_asaradc_verf_scaler_level(scaler_enable, scaler_lv);
}
void hal_drv_asaradc_basic_config(bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2)
{
    and_asaradc_set_fetch_mode_er8310(fetch_mode);
    ana_asaradc_set_timing_er8130(timer_mode1, timer_mode2);
}

void hal_drv_asaradc_enable_digital_average_filter(bool enable, uint8_t shift_n)
{
    ana_asaradc_daf_mode_er8130(enable, shift_n);
}

void hal_drv_asaradc_set_vin_sel(uint8_t vin_sel)
{
    ana_asaradc_set_vin_sel_er8130(vin_sel);
}

uint8_t hal_drv_asaradc_get_vin_sel(void)
{
    return ana_asaradc_get_vin_sel_er8130();
}

void hal_drv_asaradc_set_interrupt(bool enable)
{
    ana_asaradc_set_interrupt_er8130(enable);
    if (enable) {
        NVIC_EnableIRQ((IRQn_Type)ASARADC_IRQn);
    } else {
        NVIC_DisableIRQ((IRQn_Type)ASARADC_IRQn);
    }
}

bool hal_drv_asaradc_get_int_status(void)
{
    return ana_asaradc_get_int_status_er8130();
}

bool hal_drv_asaradc_get_int_masked_status(void)
{
    return ana_asaradc_get_int_masked_status_er8130();
}

void hal_drv_asaradc_clear_int_status(void)
{
    ana_asaradc_clear_int_status_er8130();
}

uint16_t hal_drv_asaradc_get_data(void)
{
    return ana_asaradc_get_data_er8130();
}

uint32_t hal_drv_asaradc_get_hr_data(void)
{
    return ana_asaradc_get_hr_data_er8130();
}

WEAK_ISR void ASARADC_IRQHandler(void)
{
    f_asaradc_data_ready = 1;
    hal_drv_asaradc_clear_int_status();
}
