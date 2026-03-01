/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_INTF_SYS_H
#define __HAL_INTF_SYS_H

#include "esmt_chip_specific.h"
#include "types.h"

#ifdef CHIP_HAS_LFOSC

typedef enum {
    SYS_LFOSC_CLK_DISABLE = 0,
    SYS_LFOSC_CLK_SRC_INTERNAL_32K = 1,
    SYS_LFOSC_CLK_SRC_INTERNAL_16K = 2,
    SYS_LFOSC_CLK_SRC_EXTERNAL_32K = 3,
} INTERNAL_SCLK_SEL_T;

typedef enum {
    SCLK_16K_PWR_NORMAL,
    SCLK_16K_SRC_RUN_32K,
    SCLK_16K_PWR_LOWPOWER,
    SCLK_16K_PWR_LOWPOWER_0P6V,
} INTERNAL_SCLK_16K_OPTION_T;

typedef enum {
    SCLK_32K_SRC_RUN_32K, //32.768kHz
    SCLK_32K_SRC_RUN_16K, //16.384kHz
    SCLK_32K_SRC_RUN_8K   //8.192kHz
} INTERNAL_SCLK_32K_OPTION_T;

typedef struct {
    INTERNAL_SCLK_16K_OPTION_T sclk_16k_option;
    uint8_t ldo_cur;
    uint8_t ldo_sel;
    uint8_t ldo_tmp;
    uint16_t freq_ctrl_sel;
    uint8_t sclk_settle;
    uint32_t target_count;
    uint8_t step_coarse;
    uint8_t num_slow_sclk_cycle;
    uint16_t init_ctrl_index;
    uint16_t coarse_diff;
    uint8_t cal_cnt;
    uint8_t step_fine;
    uint8_t nostop;
} INTERNAL_SCLK_16K_CFG_T;

typedef struct {
    INTERNAL_SCLK_32K_OPTION_T sclk_32k_option;
    uint8_t tim_osc;
    uint8_t cap_ctrl;
    uint8_t ictrl;
    uint8_t ldo_vdd_lv;
    uint16_t osc_clk_div;
    uint8_t fref_clk_div;
    uint32_t cycles_target;
} INTERNAL_SCLK_32K_CFG_T;

typedef struct {
    INTERNAL_SCLK_SEL_T sclk_sel;
    union {
        INTERNAL_SCLK_16K_CFG_T sclk_16k_cfg;
        INTERNAL_SCLK_32K_CFG_T sclk_32k_cfg;
    } cfg;
} HAL_INTERNAL_SCLK_CFG_T;

int8_t hal_intf_sys_exec_internal_sclk_calibration(const HAL_INTERNAL_SCLK_CFG_T *cfg);

extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_default;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_16k;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_32k_run_8k;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_default_lowpwr0p6v;
extern const HAL_INTERNAL_SCLK_CFG_T sclk_src_16k_run_32k;

#endif // __CHIP_HAS_LFOSC

uint32_t hal_intf_sys_get_chip_id(void);
int32_t hal_intf_sys_set_remap(uint32_t remap_addr);
int32_t hal_intf_sys_lock(void);
// void hal_intf_sys_set_delay(uint32_t us);

void hal_intf_sys_tick_init();
uint32_t hal_intf_sys_tick_get_counter();
void hal_intf_sys_tick_delay_ms(uint32_t ms);
void hal_intf_sys_tick_delay_us(uint32_t us);
void hal_intf_sys_set_pa_type(uint8_t pa_type);
uint8_t hal_intf_sys_get_pa_type(void);
void hal_intf_sys_set_board_match_type(uint8_t mat_type);
uint8_t hal_intf_sys_get_board_match_type(void);
void hal_intf_sys_set_40m_gain_ctrl(uint8_t ctrl_value);

void hal_intf_sys_swd_cfg(bool swd_enable);
void hal_intf_sys_tcxo_cfg(bool tcxo_enable);

void *hal_intf_sys_get_sclk_cfg(uint8_t osc_idx, uint8_t work_mode);
void hal_intf_sys_switch_sclk_src(uint8_t sclk_idx);

void hal_intf_sys_set_xtal_cfg(uint8_t xtal_i, uint8_t xtal_o);
void hal_intf_sys_set_sram_size(uint8_t sram_32_64);

/* ================== ASARADC =============== */
#ifdef CHIP_ADC_SUPPORT
void hal_intf_asaradc_start(void);
bool hal_intf_asaradc_is_ready(void);
void hal_intf_asaradc_config(bool vref, bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2, bool daf_enable, uint8_t daf_shift_n);
void hal_intf_asaradc_set_vin_sel(uint8_t vin_sel);
uint8_t hal_intf_asaradc_get_vin_sel(void);
void hal_intf_asaradc_set_interrupt(bool enable);
bool hal_intf_asaradc_get_int_status(void);
bool hal_intf_asaradc_get_int_masked_status(void);
void hal_intf_asaradc_clear_int_status(void);
uint16_t hal_intf_asaradc_get_data(void);
uint32_t hal_intf_asaradc_get_hr_data(void);
#else
static inline void hal_intf_asaradc_start(void)
{ /* no-op */
}

static inline bool hal_intf_asaradc_is_ready(void)
{
    return false;
}

static inline void hal_intf_asaradc_config(bool vref, bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2, bool daf_enable, uint8_t daf_shift_n)
{
    (void)(vref);
    (void)(fetch_mode);
    (void)(timer_mode1);
    (void)(timer_mode2);
    (void)(daf_enable);
    (void)(daf_shift_n);
}

static inline void hal_intf_asaradc_set_vin_sel(uint8_t vin_sel)
{
    (void)(vin_sel);
}

static inline uint8_t hal_intf_asaradc_get_vin_sel(void)
{
    return HAL_ERR;
}

static inline void hal_intf_asaradc_set_interrupt(bool enable)
{
    (void)(enable);
}

static inline bool hal_intf_asaradc_get_int_status(void)
{
    return HAL_ERR;
}

static inline bool hal_intf_asaradc_get_int_masked_status(void)
{
    return HAL_ERR;
}

static inline void hal_intf_asaradc_clear_int_status(void)
{ /* no-op */
}

static inline uint16_t hal_intf_asaradc_get_data(void)
{
    return HAL_ERR;
}

static inline uint32_t hal_intf_asaradc_get_hr_data(void)
{
    return HAL_ERR;
}

#endif //CHIP_ADC_SUPPORT
#endif
