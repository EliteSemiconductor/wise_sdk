/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_core.h"
#include "api/wise_sys_api.h"
#include "api/wise_tick_api.h"
#include "hal_intf_dma.h"
#include "hal_intf_wutmr.h"
#include "hal_intf_radio.h"

static uint8_t inited = 0;
static WISE_LFOSC_SRC_T lfoscConfig;
static uint8_t _gain_ctrl_40m = 8;
static uint8_t _gain_ctrl_40m_s = 8;

void wise_sys_init(void)
{
    if (inited) {
        return;
    }

    //kevinyang, 20250728, reset all PMU modules when system startup
    hal_intf_pmu_module_sw_reset(0xffffffff);

    //todo: sys init prcedure
#ifdef CHIP_HAS_LFOSC
    memset(&lfoscConfig, 0, sizeof(WISE_LFOSC_SRC_T));
    hal_intf_pmu_lfosc_clk_disable();
#endif

    //set parameters for lower currents
    hal_intf_pmu_reset_module_clock();
    hal_intf_radio_set_dcdc_default_val();
    

    inited = 1;
}

void wise_sys_proc()
{
}

void wise_sys_reset(void)
{
    NVIC_SystemReset();
}

void wise_sys_chip_reset()
{
    hal_intf_pmu_set_sw_reset();
}

uint32_t wise_sys_get_chip_unique(void)
{
    uint8_t uniqueID[8] = {0x0};

    hal_intf_efuse_get_chip_unique(uniqueID);

    return WISE_SUCCESS;
}

void wise_sys_lock(void)
{
    hal_intf_sys_lock();
}

void wise_sys_remap(uint32_t remap_addr)
{
    hal_intf_sys_set_remap(remap_addr);
}

void wise_sys_set_cpu_pd(void)
{
    hal_intf_pmu_set_cpu_pd();
}

void wise_sys_enter_sleep_mode(void)
{
    hal_intf_sys_set_40m_gain_ctrl(_gain_ctrl_40m_s);
    
    // for phy0 radio
    hal_intf_radio_rx_stop(0);
    hal_intf_radio_set_pwr_mode(0, PWR_MODE_SLEEP);
    hal_intf_pmu_set_pwr_mode(PWR_MODE_SLEEP);
    // --------Sleep--------
    // System in Sleep mode
    // --------Wakeup-------
    hal_intf_radio_set_pwr_mode(0, PWR_MODE_ACTIVITY);
    hal_intf_pmu_set_pwr_mode(PWR_MODE_ACTIVITY);

    hal_intf_sys_set_40m_gain_ctrl(_gain_ctrl_40m);
}

void wise_sys_enter_shutdown_mode(SYS_SHUTDOWN_CFT_T shutdownCfg)
{
#ifdef CHIP_PMU_HAS_EXTERNAL
    hal_intf_extpmu_set_shutdown((uint8_t)shutdownCfg.wake_src, shutdownCfg.shutdown_ms, shutdownCfg.wake_io_idx);
#endif
}

uint32_t wise_sys_get_chip_id(void)
{
    return hal_intf_sys_get_chip_id();
}

int32_t wise_sys_lfosc_clk_src_config(WISE_LFOSC_SRC_T clk_cfg)
{
#ifdef CHIP_HAS_LFOSC
    uint32_t clockRate = 32768;

    if (clk_cfg.clk_src > SYS_LFOSC_CLK_SRC_EXTERNAL_32K) {
        return WISE_FAIL;
    }
    if ((clk_cfg.clk_src == SYS_LFOSC_CLK_SRC_INTERNAL_32K) && (clk_cfg.mode.mode_32k > LFOSC_32K_MODE_8K)) {
        return WISE_FAIL;
    }
    if ((clk_cfg.clk_src == SYS_LFOSC_CLK_SRC_INTERNAL_16K) && (clk_cfg.mode.mode_16k > LFOSC_16K_MODE_LP_VOLT_0P6)) {
        return WISE_FAIL;
    }

    if (clk_cfg.clk_src == SYS_LFOSC_CLK_DISABLE) {
        hal_intf_pmu_lfosc_clk_disable();
    } else if (clk_cfg.clk_src <= SYS_LFOSC_CLK_SRC_EXTERNAL_32K) {
        hal_intf_sys_switch_sclk_src(clk_cfg.clk_src);

        switch (clk_cfg.clk_src) {
        case SYS_LFOSC_CLK_SRC_INTERNAL_32K: {
            switch (clk_cfg.mode.mode_32k) {
            case LFOSC_32K_MODE_32K:
                clockRate = 32768;
                break;

            case LFOSC_32K_MODE_16K:
                clockRate = 16384;
                break;

            case LFOSC_32K_MODE_8K:
                clockRate = 8192;
                break;
            }
        } break;

        case SYS_LFOSC_CLK_SRC_INTERNAL_16K:
            clockRate = 16384;
            break;

        case SYS_LFOSC_CLK_SRC_EXTERNAL_32K:
            clockRate = 32768;
            break;
        }

        hal_intf_wutmr_set_clock_base(clockRate);
    }

    memcpy(&lfoscConfig, &clk_cfg, sizeof(WISE_LFOSC_SRC_T));
    lfoscConfig.calFinish = 0;

    return WISE_SUCCESS;
#else
    return WISE_FAIL;
#endif
}

void wise_sys_lfosc_clk_get_config(WISE_LFOSC_SRC_T *clk_cfg)
{
    if (clk_cfg) {
        memcpy(clk_cfg, &lfoscConfig, sizeof(WISE_LFOSC_SRC_T));
    }
}

int32_t wise_sys_lfosc_clk_calibration()
{
#ifdef CHIP_HAS_LFOSC
    const HAL_INTERNAL_SCLK_CFG_T *clkCfg = NULL;

    if (lfoscConfig.clk_src == SYS_LFOSC_CLK_DISABLE) {
        return WISE_FAIL;
    }

    clkCfg = hal_intf_sys_get_sclk_cfg(lfoscConfig.clk_src, lfoscConfig.mode.mode_select);
    if (!clkCfg) {
        return WISE_FAIL;
    }

    if (hal_intf_sys_exec_internal_sclk_calibration(clkCfg) != HAL_NO_ERR) {
        printf("internal sclk calibration fail!\n");
        return WISE_FAIL;
    }

    lfoscConfig.calFinish = 1;

    return WISE_SUCCESS;
#else
    return WISE_FAIL;
#endif
}

#if 0 //kevinyang, 20250709, replaced by wise_sys_swd_config and wise_sys_tcxo_config
void wise_sys_gpio_cfg(WISE_SYS_GPIO_CFG_T *gpio_cfg)
{
    if (!gpio_cfg) {
        return;
    }

    HAL_SYS_GPIO_CFG_T hal_cfg = {.swd_enable = gpio_cfg->swd_enable, .swd_pullup = gpio_cfg->swd_pullup, .tcxo_enable = gpio_cfg->tcxo_enable};

    hal_intf_sys_gpio_cfg(&hal_cfg);
}

void wise_sys_gpio_disable_swd_tcxo(void)
{
    WISE_SYS_GPIO_CFG_T cfg = {.swd_enable = false, .swd_pullup = false, .tcxo_enable = false};
    wise_sys_gpio_cfg(&cfg);
}
#endif

void wise_sys_swd_config(bool enable)
{
    hal_intf_sys_swd_cfg(enable);
}

void wise_sys_init_dma_channel(const uint8_t *dma_func_map)
{
#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    hal_intf_dma_init_channels(dma_func_map);
#else
    hal_intf_pmu_module_clk_enable(DMA_MODULE);
#endif
}
void wise_sys_dma_channel_export(void)
{
#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
    hal_intf_dma_func_map_export_printf();
#endif
}
int32_t wise_dma_memcpy_bytes(void *dst, const void *src, uint32_t byte_count)
{
    if (hal_intf_dma_channel_setup(HAL_DMA_USER, src, dst, byte_count, 2, 0) == HAL_ERR) {
        return WISE_FAIL;
    }
    hal_intf_dma_channel_trigger(HAL_DMA_USER);
    return WISE_SUCCESS;
}

void wise_sys_set_board_property(const WISE_SYS_BOARD_PROPERTY_T *property)
{
    if (property) {
        hal_intf_sys_tcxo_cfg(property->tcxo_output_en);
        hal_intf_sys_set_pa_type(property->pa_type);
        hal_intf_sys_set_board_match_type(property->matching_type);
        hal_intf_sys_set_40m_gain_ctrl(property->gain_ctrl_40m);
        hal_intf_sys_set_xtal_cfg(property->cap_xtal_i, property->cap_xtal_o);
        hal_intf_sys_set_sram_size(property->sram_retain);

        _gain_ctrl_40m = property->gain_ctrl_40m;
        _gain_ctrl_40m_s = property->gain_ctrl_40m_s;
        
        debug_print("gain_ctrl_40m=(%d,%d)\n", _gain_ctrl_40m, _gain_ctrl_40m_s);
    }
}

void wise_sys_tcxo_config(bool enable)
{
    hal_intf_sys_tcxo_cfg(enable);
}

void wise_sys_set_pa_type(uint8_t pa_type)
{
    hal_intf_sys_set_pa_type(pa_type);
}

uint8_t wise_sys_get_pa_type(void)
{
    return hal_intf_sys_get_pa_type();
}

void wise_sys_set_board_match_type(uint8_t mat_type)
{
    hal_intf_sys_set_board_match_type(mat_type);
}

uint8_t wise_sys_get_board_match_type(void)
{
    return hal_intf_sys_get_board_match_type();
}

void wise_sys_enable_bod(uint8_t bod_lv, uint8_t enable)
{
    if (enable) {
        hal_intf_pmu_module_clk_enable(MAC_MODULE |ANA_MODULE);
        hal_intf_radio_enable_bod(ENABLE, bod_lv);
        wise_tick_delay_ms(10);    //it needs to wait 10ms after radio enable bod
        hal_intf_pmu_enable_bod_reset(ENABLE);
    } else {
        hal_intf_pmu_enable_bod_reset(DISABLE);
        hal_intf_radio_enable_bod(DISABLE, 0);

        //check that the system is idle before disabling the clock
        while (!hal_intf_radio_is_state_idle()) {
                printf("Radio is prepared\n");
        }
        hal_intf_pmu_module_clk_disable(MAC_MODULE | ANA_MODULE);
    }
}

/* ================== ASARADC =============== */
#define ASARADC_CFG_DEF_FETCH_MODE              0
#define ASARADC_CFG_DEF_TIMER_MODE1             3
#define ASARADC_CFG_DEF_TIMER_MODE2             3
#define ASARADC_CFG_DEF_DAF_ENABLE              0
#define ASARADC_CFG_DEF_DAF_SHIFT_N             0


WISE_STATUS wise_asaradc_init(void)
{
    return WISE_SUCCESS;
}

WISE_STATUS wise_asaradc_config(WISE_ASARADC_VREF vref)
{                                
    hal_intf_asaradc_config((bool)vref, 
                            ASARADC_CFG_DEF_FETCH_MODE, 
                            ASARADC_CFG_DEF_TIMER_MODE1, 
                            ASARADC_CFG_DEF_TIMER_MODE2, 
                            ASARADC_CFG_DEF_DAF_ENABLE, 
                            ASARADC_CFG_DEF_DAF_SHIFT_N);
    
    return WISE_SUCCESS;
}

static WISE_STATUS _asaradc_exec_once(uint32_t timeoutMs)
{
    WISE_STATUS result = WISE_SUCCESS;
    uint32_t startTime = 0;
    
    hal_intf_asaradc_set_interrupt(ENABLE);
    hal_intf_asaradc_start();

    startTime = wise_tick_get_counter();
    while(1)
    {
        if(hal_intf_asaradc_is_ready())
        {
            result = WISE_SUCCESS;
            break;
        }
        
        if(((uint32_t)(wise_tick_get_counter() - startTime)) > MS_TO_CLK(timeoutMs))
        {
            result = WISE_FAIL;
            break;
        }
    }
    
    hal_intf_asaradc_set_interrupt(DISABLE);
    
    return result;
}

WISE_STATUS wise_asaradc_read_input(ASARADC_VIN_SEL_T vin_sel, uint16_t* rawValue)
{
    WISE_STATUS result = WISE_SUCCESS;
    
    hal_intf_asaradc_set_vin_sel(vin_sel);
    
    result = _asaradc_exec_once(2000);
    if(result == WISE_SUCCESS)
        *rawValue = hal_intf_asaradc_get_data();
    else
        result = WISE_FAIL;
    
    return result;
}

WISE_STATUS wise_asaradc_read_input_hires(ASARADC_VIN_SEL_T vin_sel, uint32_t* rawValue)
{
    WISE_STATUS result = WISE_SUCCESS;
    
    hal_intf_asaradc_set_vin_sel(vin_sel);
    
    result = _asaradc_exec_once(2000);
    if(result == WISE_SUCCESS)
        *rawValue = hal_intf_asaradc_get_hr_data();
    else
        result = WISE_FAIL;
    
    return result;
}


//kevinyang, 20251114, keep these APIs as internal test
typedef struct {
    uint16_t code; /* ADC raw code (e.g., 0..4095) */
    int32_t mv;    /* measured voltage in millivolts */
} WISE_ASARADC_PT_T;

typedef struct {
    uint16_t x1, x2;          /* inclusive range [x1, x2] */
    int32_t slope_uV_per_LSB; /* uV/LSB */
    int32_t offset_uV;        /* uV: uV = slope*code + offset */
} WISE_ASARADC_SEG_T;

typedef struct {
    WISE_ASARADC_PT_T pts[ASARADC_MAX_POINTS]; /* sorted copy of user points   */
    uint8_t npts;
    WISE_ASARADC_SEG_T segs[ASARADC_MAX_POINTS - 1]; /* derived segments             */
    uint8_t nsegs;
    bool clamp; /* true: clamp; false: extrapolate */
} WISE_ASARADC_CALIB_T;

WISE_STATUS wise_asaradc_config_detail(WISE_ASARADC_VREF vref, 
                                bool fetch_mode, 
                                uint8_t timer_mode1, 
                                uint8_t timer_mode2, 
                                bool daf_enable,
                                uint8_t daf_shift_n, 
                                ASARADC_VIN_SEL_T vin_sel);
void wise_asaradc_start(void);
bool wise_asaradc_is_ready(void);
void wise_asaradc_enable_interrupt(bool enable);
bool wise_asaradc_check_interrupt(void);
void wise_asaradc_clear_interrupt(void);
bool wise_asaradc_read_data(WISE_ASARADC_DATA_T *result);
bool wise_asaradc_capture_once(ASARADC_VIN_SEL_T vin_sel, WISE_ASARADC_DATA_T *out_data);
int32_t wise_asaradc_code_to_mV(uint16_t code);
bool wise_asaradc_cal_table_set(const uint16_t *code, const int32_t *mv, uint8_t n, bool clamp);


WISE_STATUS wise_asaradc_config_detail(WISE_ASARADC_VREF vref, bool fetch_mode, uint8_t timer_mode1, uint8_t timer_mode2, bool daf_enable,
                                uint8_t daf_shift_n, ASARADC_VIN_SEL_T vin_sel)
{
    hal_intf_asaradc_config((bool)vref, fetch_mode, timer_mode1, timer_mode2, daf_enable, daf_shift_n);
    hal_intf_asaradc_set_vin_sel(vin_sel);
    return WISE_SUCCESS;
}

void wise_asaradc_start(void)
{
    hal_intf_asaradc_start();
}

bool wise_asaradc_is_ready(void)
{
    return hal_intf_asaradc_is_ready();
}

void wise_asaradc_enable_interrupt(bool enable)
{
    hal_intf_asaradc_set_interrupt(enable);
    
    if (enable) {
        NVIC_EnableIRQ((IRQn_Type)ASARADC_IRQn);
    } else {
        NVIC_DisableIRQ((IRQn_Type)ASARADC_IRQn);
    }
}

bool wise_asaradc_check_interrupt(void)
{
    return hal_intf_asaradc_get_int_masked_status();
}

void wise_asaradc_clear_interrupt(void)
{
    hal_intf_asaradc_clear_int_status();
}

bool wise_asaradc_read_data(WISE_ASARADC_DATA_T *result)
{
    if (!hal_intf_asaradc_is_ready()) {
        return false;
    }

    result->data_12bit = hal_intf_asaradc_get_data();
    result->data_27bit = hal_intf_asaradc_get_hr_data();

    return true;
}

bool wise_asaradc_capture_once(ASARADC_VIN_SEL_T vin_sel, WISE_ASARADC_DATA_T *out_data)
{
    hal_intf_asaradc_set_vin_sel(vin_sel);
    wise_asaradc_enable_interrupt(ENABLE);
    hal_intf_asaradc_start();
    while (wise_asaradc_is_ready() == FALSE) {
        ;
    }
    wise_asaradc_enable_interrupt(DISABLE);
    out_data->data_12bit = hal_intf_asaradc_get_data();
    out_data->data_27bit = hal_intf_asaradc_get_hr_data();

    return true;
}

/* ----------------------- Utilities ---------------------------------------- */
static WISE_ASARADC_CALIB_T g_calib_ctx;

static inline void swap_pts(WISE_ASARADC_PT_T *a, WISE_ASARADC_PT_T *b)
{
    WISE_ASARADC_PT_T t = *a;

    *a = *b;
    *b = t;
}

static void sort_by_code(WISE_ASARADC_PT_T *p, uint8_t n)
{
    for (uint8_t i = 1; i < n; ++i) {
        uint8_t j = i;
        while (j > 0 && p[j - 1].code > p[j].code) {
            swap_pts(&p[j - 1], &p[j]);
            --j;
        }
    }
}

/* Find segment: return index where segs[idx] satisfies x1 <= code <= x2; 
   return 0xFF if not found */
static uint8_t find_segment(const WISE_ASARADC_CALIB_T *c, uint16_t code)
{
    for (uint8_t i = 0; i < c->nsegs; ++i) {
        if (code >= c->segs[i].x1 && code <= c->segs[i].x2) {
            return i;
        }
    }
    return 0xFF;
}

/**
 * @brief Initialize calibration from user points (code,mV). 
 * @param ctx    Calibration context (output).
 * @param in     User adc points array 
 * @param n      Number of points (>=2, <=ASARADC_MAX_POINTS).
 * @param clamp  true: clamp out-of-range to end points; false: extrapolate.
 */
int asaradc_calib_init(WISE_ASARADC_CALIB_T *ctx, const WISE_ASARADC_PT_T *in, uint8_t n, bool clamp)
{
    if (!ctx || !in || n < 2 || n > ASARADC_MAX_POINTS) {
        return -1; /* invalid args */
    }

    /* copy & sort */
    for (uint8_t i = 0; i < n; ++i) {
        ctx->pts[i] = in[i];
    }
    sort_by_code(ctx->pts, n);

    /* build segments */
    for (uint8_t i = 0; i < n - 1; ++i) {
        if (ctx->pts[i + 1].code == ctx->pts[i].code) {
            return -2; /* duplicate code detected */
        }

        int32_t dy_mV = (int32_t)ctx->pts[i + 1].mv - (int32_t)ctx->pts[i].mv;
        int32_t dx    = (int32_t)ctx->pts[i + 1].code - (int32_t)ctx->pts[i].code;
        if (dx == 0) {
            return -3; /* zero-length segment */
        }

        /* Input voltage(uV) = slope * code + offset */
        int32_t slope_uV  = (dy_mV * 1000) / dx; /* uV/LSB */
        int32_t offset_uV = (int32_t)ctx->pts[i].mv * 1000 - slope_uV * (int32_t)ctx->pts[i].code;

        ctx->segs[i].x1               = ctx->pts[i].code;
        ctx->segs[i].x2               = ctx->pts[i + 1].code;
        ctx->segs[i].slope_uV_per_LSB = slope_uV;
        ctx->segs[i].offset_uV        = offset_uV;
    }

    ctx->npts  = n;
    ctx->nsegs = n - 1;
    ctx->clamp = clamp;
    return 0;
}

/**
 * @brief Build the global calibration table (user provides code[] / mv[])
 * @param code   Array of raw ADC codes, length n
 * @param mv     Array of voltages in mV, length n
 * @param n      Number of points (>=2 && <=ASARADC_MAX_POINTS)
 * @param clamp  true: clamp to endpoints; false: allow extrapolation
 * @return true on success; false on failure (invalid args / duplicate code / zero-length segment)
 */
bool wise_asaradc_cal_table_set(const uint16_t *code, const int32_t *mv, uint8_t n, bool clamp)
{
    if (!code || !mv || n < 2 || n > ASARADC_MAX_POINTS) {
        return false;
    }

    WISE_ASARADC_PT_T tmp[ASARADC_MAX_POINTS];
    for (uint8_t i = 0; i < n; ++i) {
        tmp[i].code = (uint16_t)code[i];
        tmp[i].mv   = (int32_t)mv[i];
    }

    int rc = asaradc_calib_init(&g_calib_ctx, tmp, (uint8_t)n, clamp);
    return (rc == 0);
}

/**
 * @brief Convert raw code to mV 
 * @param ctx   Initialized calibration context.
 * @param code  ADC raw code.
 */
static int32_t asaradc_code_to_mV(const WISE_ASARADC_CALIB_T *ctx, uint16_t code)
{
    if (!ctx || ctx->nsegs == 0) {
        return 0;
    }

    uint8_t idx                   = find_segment(ctx, code);
    const WISE_ASARADC_SEG_T *seg = NULL;

    if (idx == 0xFF) {
        if (ctx->clamp) {
            /* Clamp to endpoints (return endpoint mV directly) */
            if (code < ctx->pts[0].code) {
                int32_t uV = (int32_t)ctx->pts[0].mv * 1000;
                return (uV + 500) / 1000;
            } else {
                int32_t uV = (int32_t)ctx->pts[ctx->npts - 1].mv * 1000;
                return (uV + 500) / 1000;
            }
        } else {
            /* Extrapolate: use the first or last segment coefficients */
            seg = (code < ctx->segs[0].x1) ? &ctx->segs[0] : &ctx->segs[ctx->nsegs - 1];
        }
    } else {
        seg = &ctx->segs[idx];
    }

    int32_t uV = seg->slope_uV_per_LSB * (int32_t)code + seg->offset_uV;
    /* Round to nearest mV */
    return (uV >= 0) ? (uV + 500) / 1000 : (uV - 500) / 1000;
}

int32_t wise_asaradc_code_to_mV(uint16_t code)
{
    return asaradc_code_to_mV(&g_calib_ctx, code);
}
