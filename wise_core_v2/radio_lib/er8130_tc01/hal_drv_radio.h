/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __HAL_DRV_RADIO_H__
#define __HAL_DRV_RADIO_H__

#include "hal_intf_radio.h"
#include <stdint.h>

#define HAL_DRV_RADIO_VERSION                       1

/* ==== Define ============================================================== */
#define DATA_RATE_4P8K                              4800
#define DATA_RATE_12P5K                             12500
#define DATA_RATE_32P768K                           32768
#define DATA_RATE_50K                               50000
#define DATA_RATE_100K                              100000
#define DATA_RATE_125K                              125000
#define DATA_RATE_200K                              200000
#define DATA_RATE_250K                              250000
#define DATA_RATE_500K                              500000
#define DATA_RATE_1M                                1000000
#define DATA_RATE_2M                                2000000


/* -----ANA------------------------------------------------------------------ */
#define ANA_CLK                                     40000000
#define ANA_BOD_THRES                               ANA_BOD_1P90V

#define ANA_LO_250K                                 250000
#define ANA_LO_500K                                 500000
#define ANA_LO_750K                                 750000
#define ANA_LO_1M                                   1000000

#define ANA_MAX_TX_POWER_VALUE                      27

/* -----BBP------------------------------------------------------------------ */
#define DIG_TRAN(x)                                 (x * 8)

#define OSR16                                       16
#define OSR8                                        8

#define AGC_MULTI_ED_MODE                           ED_TYPE_THRES
#define AGC_NORM_ED_THRES_LEVEL                     7
#define AGC_MBUS_ED_THRES_LEVEL                     5
// AGC auto-adjustment parameters
#define AGC_PWR_RAW_POSITIVE_THRESHOLD              0x200    // Raw power positive value threshold
#define AGC_TARGET_PWR_MIN_RAW                      0x360    // Target min power raw value (-20dBm)
#define AGC_TARGET_PWR_MAX_RAW                      0x388    // Target max power raw value (-15dBm)
#define AGC_MAX_INDEX                               15       // Maximum AGC gain index
#define AGC_AUTO_ADJUST_MAX_ITERATIONS              10       // Maximum iterations for auto adjustment
#define AGC_SETTLE_DELAY_US                         100000   // AGC settling delay in microseconds

/* -----MAC------------------------------------------------------------------ */
#define VALIDATE_SADDR(addr)                        (addr != 0xFFFF)
#define VALIDATE_LADDR(l, h)                        ((l != 0xFFFFFFFF) && (h != 0xFFFFFFFF))


/* ==== ENUM ================================================================ */
enum phy_idx {
    PHY_IDX_0 = 0,
    PHY_IDX_1,
    PHY_IDX_MAX, // put it at the end
};

typedef enum {
    RADIO_INTERNAL = 0,
    RADIO_EXTERNAL,
} RADIO_INTF_TYPE_T;

enum modulation_idx {
    MOD_IDX_0P5 = 0,
    MOD_IDX_1   = 1,
    MOD_IDX_2   = 2,
    MOD_IDX_4   = 3,
    MOD_IDX_MAX, // put it at the end
};

enum mac_ctrl_mode {
    CTRL_MODE_MANUAL = 0,
    CTRL_MODE_AUTO,
};

enum modulation_type {
    MOD_TYPE_BPSK      = 0x0,
    MOD_TYPE_QPSK      = 0x1,
    MOD_TYPE_OQPSK     = 0x2,
    MOD_TYPE_FSK       = 0x3,
    MOD_TYPE_GFSK      = 0x4,
    MOD_TYPE_4FSK      = 0x5, 
    MOD_TYPE_BPSK_RAMP = 0x6, // sigfox
    MOD_TYPE_OOK       = 0x7,
    MOD_TYPE_MAX, // put it at the end
};

enum ana_gain_src_t { 
    ANA_GAIN_SRC_CSR = 0, 
    ANA_GAIN_SRC_BBP, 
};

enum ana_bod_lv_t {
    ANA_BOD_1P45V = 0,
    ANA_BOD_1P54V,
    ANA_BOD_1P65V,
    ANA_BOD_1P74V,
    ANA_BOD_1P90V,
    ANA_BOD_2P03V,
    ANA_BOD_2P20V,
    ANA_BOD_2P41V,
};

typedef enum {
    IQK_MODE_MANUAL = 0,
    IQK_MODE_AUTO,
} IQK_MODE;

enum es_pa_type {
    PA_TYPE_LOW = 0,
    PA_TYPE_HI  = 1,
};

enum iqk_tone {
    IQK_TONE_250KHZ  = 0,
    IQK_TONE_500KHZ  = 1,
    IQK_TONE_N250KHZ = 2,
    IQK_TONE_N500KHZ = 3,
};

enum if_freq {
    IF_FREQ_250KHZ = 0,
    IF_FREQ_500KHZ = 1,
};

enum bbp_ed_type_t { ED_TYPE_THRES = 0, ED_TYPE_RAMP_UP };

enum radio_phy_mode {
    PHY_MODE_TRANSPARENT = 0x0,
    PHY_MODE_802154      = 0x1,
    PHY_MODE_CCA         = 0x2,
    PHY_MODE_BLE         = 0x3,
    PHY_MODE_BLELR       = 0x4,
    PHY_MODE_MBUS        = 0x5,
};

enum rx_status {
    ST_RX_IDLE  = 0x0,
    ST_RX_PU    = 0x1,
    ST_RX_SETUP = 0x2,
    ST_RX_STR   = 0x3,
    ST_RX_LDCAP = 0x4,
    ST_RX_PLL   = 0x5,
    ST_RX_IQ    = 0x6, //IO calibration, exit when done
    ST_RX_RCV   = 0x7, //Normal RX
    ST_RX_EXIT  = 0x8,
    ST_RX_PD    = 0x9,
    ST_RX_FSSET = 0xA,
    ST_RX_ADCSTR = 0xB,
    ST_RX_ADCWARMUP = 0xC,
    ST_RX_IQHANG = 0xD, //IO hang
};

enum agc_mode {
	AGC_NORMAL_MODE = 0x01,
	AGC_DEBUG_MODE  = 0x03,
};

/* ==== General ============================================================= */
struct rx_status_t {
    uint32_t rx_ok;
    uint32_t rx_err;
    uint32_t rx_lose;
};

struct tx_status_t {
    uint32_t tx_ok;
    uint32_t tx_err;
    uint32_t tx_lose;
};

typedef struct {
    struct rx_status_t rx_status;
    uint32_t gfo_est;
    uint16_t ib_pwr;
    uint16_t agc_idx;
    uint16_t out_en;
} RADIO_MSR_DATA_T;

typedef struct {
    uint16_t rssi;
    uint16_t data_len;
    uint32_t channel;
} RADIO_FRAME_META_T;

typedef struct {
    uint32_t *rx_frame_buf;
    RADIO_FRAME_META_T rx_meta;
    RADIO_MSR_DATA_T rx_msr;
} RADIO_RX_INFO_T;

typedef struct tx_info_t {
    uint32_t *tx_buf;
    uint32_t tx_len;
    uint16_t ch;
    void *mode;
    struct tx_status_t tx_status;
} TX_INFO_T;

/* ==== MAC ================================================================= */

typedef struct {
    //uint32_t *evt_sts; //not used
    RADIO_RX_INFO_T radio_rx_info;
} MAC_INFO_T;

/* ==== BBP ================================================================= */

struct gt_gear_t {
    uint32_t agc02_mix_idx0;
    uint32_t agc03_mix_idx1;
    uint32_t agc04_mix_idx2;
    uint32_t agc05_mix_idx3;
    uint32_t agc06_lna_idx0;
    uint32_t agc07_lna_idx1;
    uint32_t agc08_lna_idx2;
    uint32_t agc09_lna_idx3;
    uint32_t agc10_pga_idx0;
    uint32_t agc11_pga_idx1;
    uint32_t agc12_pga_idx2;
    uint32_t agc13_pga_idx3;
    uint32_t agc14_flt_idx0;
    uint32_t agc15_flt_idx1;
    uint32_t agc16_flt_idx2;
    uint32_t agc17_flt_idx3;
};

struct gt_pwr_t {
    uint32_t agc26_pwr_01_00;
    uint32_t agc27_pwr_03_02;
    uint32_t agc28_pwr_05_04;
    uint32_t agc29_pwr_07_06;
    uint32_t agc30_pwr_09_08;
    uint32_t agc31_pwr_11_10;
    uint32_t agc32_pwr_13_12;
    uint32_t agc33_pwr_15_14;
    uint32_t agc34_pwr_17_16;
    uint32_t agc35_pwr_19_18;
    uint32_t agc36_pwr_21_20;
    uint32_t agc37_pwr_23_22;
    uint32_t agc38_pwr_25_24;
    uint32_t agc39_pwr_27_26;
    uint32_t agc40_pwr_29_28;
    uint32_t agc41_pwr_31_30;
};

struct iqk_para_t {
    int16_t alpha_lmt;
    int16_t beta_lmt;
    uint8_t set_iqk_lmt;
};

struct gfo_para_t {
    uint8_t gfo_en;
    uint8_t gfo_tim_thr_r1m;
    uint8_t gfo_tim_thr_r2m;
    uint8_t gfo_tim_thr_l1m;
    uint8_t gfo_tim_thr_l2m;
};

struct dr_para_t {
    uint32_t init_gidx;
    int16_t ib_tgt;
    int16_t ed_thres[MOD_IDX_MAX];
    uint32_t agc_timeout_max;
    uint16_t ook_ed_thr;
    uint8_t if_freq;
    uint8_t iqk_tone_sel;
    uint8_t inband_pwr_win_size;
    uint8_t tot_pwr_win_size;
    int32_t pwr_ofs;
    uint32_t ed_ov_thr_prd;
};

struct bbp_mod_info_t {
    uint8_t ed_mode;
    uint8_t gain_loop;
    uint8_t bk_step;
    uint8_t bpsk_dig_gain;
    uint32_t bpsk_tr_thd;
    int32_t pwr_ofs;
    uint32_t ed_ov_thr_prd;
    uint8_t pe_pwr_thr_lr;
    uint8_t pe_pwr_raise_lr;
    uint8_t pe_pwr_thr;
    uint8_t pe_pwr_raise;
    struct dr_para_t *dr_para[E_HAL_DR_MAX];
};

typedef struct {
    struct iqk_para_t iqk_para;
    struct gfo_para_t gfo_para;
    struct bbp_mod_info_t mod_info;
} BBP_INFO_T;

/* ==== ANA ================================================================= */

typedef struct {
    //
} ANA_INFO_T;

struct ana_gain_t {
    uint8_t lna;
    uint8_t mix;
    uint8_t filt;
    uint8_t pga;
};

struct ana_filt_t {
    uint16_t filt_cal : 1;
    uint16_t filt_en  : 1;
    uint16_t freq_sel : 4;
    uint16_t cap_sel  : 6;
    // uint16_t reserve  : 4;
};

struct ana_para_t {
    uint32_t tx_fs_parm;
    uint8_t ctrl_mode;
};

typedef struct ana_iqk_backup_t {
    uint32_t rx_pwr_para;
    uint32_t rx_filter_para;
    uint32_t rx_mac_time_para;
} IQK_BACKUP;

typedef struct {
    // PA1G_CONFIG_0 (0x480)
    uint8_t pa1g_en;
    uint8_t pa1g_duty_cycle_en;
    uint8_t pa1g_vb_sel;
    uint8_t pa1g_pwr_lv;
    uint8_t pa1g_vb2_sel;

    // PA1G_CONFIG_1 (0x484)
    uint8_t pa1g_amux_sel;
    uint8_t pa1g_iref_sel;
    uint8_t pa1g_duty_sel;
    uint8_t pa1g_ramp_en;
    uint8_t pa1g_ramp_sel;

} PA_PARA;

typedef struct {
    // ANA_DCDC_CONFIG_1_ADDR (0x1204)
    uint8_t en_random12;
    uint8_t en_ipn;

    // ANCTL_DCDC_REG3 (0xE5C)
    uint8_t tar_idle;
    uint8_t tar_tx_1g;
    uint8_t tar_rx_1g;
    uint8_t tar_tx_2g;
    uint8_t tar_rx_2g;

    // ANCTL_PA1G_REG0 (0xE60)
    uint8_t ldo_sel_rx;
    uint8_t ldo_sel_tx;

    // ANCTL_FS_REG0 (0xE64)
    uint8_t iqbuf_rx;
    uint8_t iqbuf_tx;
    uint8_t fs_cp_tx;
    uint8_t fs_cp_rx;    

} ANCTL_DCDC_PARA;


/* ==== RADIO =============================================================== */

typedef union event_info_u {
    uint32_t event_status;
    struct {
        uint32_t rx_ok         : 1;
        uint32_t rx_err        : 1;
        uint32_t tx_ok         : 1;
        uint32_t tx_err        : 1;
        uint32_t rx_to         : 1;
        uint32_t tmr_evt       : 1;
        uint32_t syncw         : 1;
        uint32_t tx_fifo_full  : 1;
        uint32_t rx_fifo_empty : 1;
        uint32_t wakeup        : 1;
        uint32_t nfc           : 1;
        uint32_t tx_fifo_empty : 1;
        uint32_t rx_fifo_full  : 1;
        uint32_t rx_sync_word  : 1;
        uint32_t rx_cca        : 1;
    } event_b;
} EVENT_INFO_U;

typedef struct radio_adapter_t {
    int8_t phy_idx;

    uint8_t pa_type;
    uint8_t match_type;
    uint8_t osr;
    uint8_t mod_idx;   // modulation idx

    HAL_DATA_RATE_T dr_idx;
    RADIO_INTF_TYPE_T intf_type;
    RADIO_CFG_T radio_cfg;
    uint32_t int_mask;
    HAL_ISR_CALLBACK isr_callback;
    void *isr_cb_data;

    MAC_INFO_T *mac;
    BBP_INFO_T *bbp;
    ANA_INFO_T *ana;
} RADIO_ADAPT_T;

typedef struct rx_info_t {
    uint32_t *rx_buf;
    uint32_t ch_freq;
    struct rx_status_t rx_status;
    uint32_t gfo_est;
    uint16_t agc_idx;
    uint16_t out_en;
    uint16_t ib_pwr;
    uint16_t rssi;
    uint16_t rx_len;
} RX_INFO_T;

/* ==== Function include ==================================================== */

void hal_drv_radio_set_adapt(int8_t phy_idx);
void *hal_drv_radio_get_adapt(int8_t phy_idx);
void hal_drv_radio_set_isr_cb(int8_t phy_idx, HAL_ISR_CALLBACK cb_proc, void *cb_data);

//EVENT_INFO_U *hal_drv_radio_get_int_status(int8_t phy_idx);
//void hal_drv_radio_clear_status(int8_t phy_idx, EVENT_INFO_U *evt_info);
uint32_t hal_drv_radio_get_int_status(int8_t phy_idx);
void hal_drv_radio_clear_status(int8_t phy_idx, uint32_t status);
void hal_drv_radio_set_int_en_mask(int8_t phy_idx, uint32_t mask);
uint32_t hal_drv_radio_get_int_en_mask(int8_t phy_idx);
void hal_drv_radio_enable_interrupt(int8_t phy_idx);
void hal_drv_radio_disable_interrupt(int8_t phy_idx);

void hal_drv_radio_get_rx_info(int8_t phy_idx, void *rx_meta);
void hal_drv_radio_set_pwr_mode(int8_t phy_idx, uint8_t mode);
void hal_drv_radio_set_tx_pwr(int8_t phy_idx, uint8_t lv);
const TX_PWR_MAPPING* hal_drv_radio_get_tx_pwr_table(uint8_t* levelNum);
uint8_t hal_drv_radio_get_phy_mode(int8_t phy_idx);
void hal_drv_radio_set_phy_mode(int8_t phy_idx, uint8_t phy_mode);

HAL_STATUS hal_drv_radio_wait_rssi_rdy(int8_t phy_idx);
void hal_drv_radio_print_rx_info(int8_t phy_idx);
void hal_drv_radio_set_ctrl_mode(int8_t phy_idx, uint8_t ctrl_mode);
void hal_drv_radio_rx_start(int8_t phy_idx, uint32_t rx_max_len);
void hal_drv_radio_set_rx_restart(int8_t phy_idx, uint8_t enable);
uint8_t hal_drv_radio_get_rx_busy(int8_t phy_idx);
uint8_t hal_drv_radio_rx_stop(int8_t phy_idx);
void hal_drv_radio_enable_singletone(int8_t phy_idx, uint32_t pwr_lv, uint8_t enable);
void hal_drv_radio_tx_start(int8_t phy_idx, uint8_t *tx_buf, uint32_t tx_len);
void hal_drv_radio_tx_stop(int8_t phy_idx);
HAL_STATUS hal_drv_radio_set_phy(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_drv_radio_set_panid(int8_t phy_idx, uint16_t pan_id);
void hal_drv_radio_set_short_addr(int8_t phy_idx, uint16_t addr);
void hal_drv_radio_set_long_addr(int8_t phy_idx, const uint8_t *laddr);
void hal_drv_radio_set_ch(int8_t phy_idx, uint32_t ch);
void hal_drv_radio_set_data_rate(int8_t phy_idx, uint8_t phy_mode, uint32_t data_rate);
void hal_drv_radio_set_osr(int8_t phy_idx, uint32_t mod_type, HAL_DATA_RATE_T dr_idx);
void hal_drv_radio_enable_rx_timeout(int8_t phy_idx, uint32_t rx_to_ms);
void hal_drv_radio_disable_rx_timeout(int8_t phy_idx);
void hal_drv_radio_set_bbp_rate(int8_t phy_idx, uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx);
void hal_drv_radio_set_ed_thres(uint16_t thres);
uint16_t hal_drv_radio_rx_calib(int8_t phy_idx, uint8_t phy_mode, uint8_t times);
void hal_drv_radio_enable_cdr_est(uint8_t enable);
void hal_drv_radio_trig_tpm_calib(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_drv_radio_get_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_drv_radio_set_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_drv_radio_set_dcdc_default_val(void);
HAL_STATUS hal_drv_radio_set_rx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
HAL_STATUS hal_drv_radio_set_tx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_drv_radio_rx_iqk(int8_t phy_idx);
void hal_drv_radio_enable_bod(uint8_t enable, uint8_t bod_lv);
uint8_t hal_drv_radio_is_state_idle(void);
void hal_drv_radio_enable_efuse_write_data(uint8_t enable);
void hal_drv_radio_enable_prbs9(uint8_t enable);

#ifdef ES_DEVICE_TRX_RADIO
void hal_drv_radio_get_rx_fifo(uint8_t *rx_buf, uint32_t rx_len);
void hal_drv_radio_reset_rx_fifo(void);

#endif


#if defined(ES_DEVICE_RADIO)
/*############################radio PHY 0 only ###############################*/

/* ========== MARCO define: ANA ============================================= */
#define ANA_GET_ANA_INFO()                                              ana_get_ana_info_er81xx()
#define ANA_SET_MOD_TYPE(phy_idx, mod_type)                             ana_set_mod_type_er81xx(mod_type)
#define ANA_SET_RX_DATA_RATE(phy_idx, phy_mode, data_rate)              ana_set_rx_data_rate_er81xx(phy_mode, data_rate)
#define ANA_GET_RX_DATA_RATE(phy_idx)                                   ana_get_data_rate_er81xx()
#define ANA_SET_FREQ_DEVIA(phy_idx, freq_devia)                         ana_set_freq_devia_er81xx(freq_devia)
#define ANA_SET_BASIC_CFG(phy_idx)                                      ana_set_basic_cfg_er81xx()
#define ANA_SET_CTRL_MODE(phy_idx, ctrl_mode)                           ana_set_ctrl_mode_er81xx(ctrl_mode)
#define ANA_SET_ADC_CFG(phy_idx, phy_mode, data_rate)                   ana_set_adc_cfg_er81xx(phy_mode, data_rate)
#define ANA_SET_DCDC_CFG(phy_idx, pa_type, mat_type)                    ana_set_dcdc_cfg_er81xx(pa_type, mat_type)
#define ANA_SET_XTAL_CFG(phy_idx)                                       ana_set_xtal_cfg_er81xx()
#define ANA_DO_FILTER_CALIBRATION(phy_idx)                              /* Not supported */
#define ANA_SET_MODULE_PD_RST(phy_idx)                                  ana_set_module_pd_rst_er81xx()
#define ANA_SET_CHARGE_PUMP(phy_idx, phy_mode, data_rate)               ana_set_charge_pump_er81xx(phy_mode, data_rate)
#define ANA_SET_TPM_CAL(phy_idx, mod_type, data_rate)                   ana_set_tpm_cal_er81xx(mod_type, data_rate)
#define ANA_SET_TPM_DMI(phy_idx, phy_mode, mod_type, data_rate)         ana_set_tpm_dmi_er81xx(phy_mode, mod_type, data_rate)
#define ANA_SET_PA_CFG(phy_idx, pa_type, mat_type, mod_type, data_rate) ana_set_pa_cfg_er81xx(pa_type, mat_type, mod_type, data_rate)
#define ANA_SET_PA_PWR_TBL(phy_idx, mod_type)                           ana_set_pa_pwr_tbl_er81xx(mod_type)
#define ANA_SET_SYNTHESIZE(phy_idx, mod_type)                           ana_set_synthesize_er81xx(mod_type)
#define ANA_SET_TX_PWR_LV(phy_idx, mod_type, lv)                        ana_set_tx_pwr_lv_er81xx(mod_type, lv)
#define ANA_SET_TX_RAMP_BACKUP(phy_idx)                                 ana_set_tx_ramp_backup_er81xx()
#define ANA_SET_OSR(phy_idx, osr)                                       /* Not supported */
#define ANA_SET_RX_ATOP_CFG(phy_idx)                                    ana_set_rx_atop_cfg_er81xx()
#define ANA_SET_GAIN_SRC(phy_idx, gain_src)                             ana_set_gain_src_er81xx(gain_src)
#define ANA_SET_RX_GAIN(phy_idx)                                        ana_set_rx_gain_er81xx()
#define ANA_SET_RX_FILT(phy_idx, phy_mode, data_rate, mod_idx)          ana_set_rx_filt_er81xx(phy_mode, data_rate, mod_idx)
#define ANA_SET_USER_CFG(phy_idx)                                       ana_set_user_cfg_er81xx()
#define ANA_SET_CH_FREQ(phy_idx, ch)                                    ana_set_ch_freq_er81xx(ch)
#define ANA_ENABLE_SINGLETONE(phy_idx, mod_type, pwr_lv, enable)        ana_enable_singletone_er81xx(mod_type, pwr_lv, enable)
#define ANA_SET_IQK_CFG(phy_idx, mode)                                  ana_set_iqk_cfg_er81xx(mode)
#define ANA_GET_RX_STATUS(phy_idx)                                      ana_get_rx_status_er81xx()
#define ANA_CLEAN_IQK_CFG(phy_idx, mode)                                ana_clean_iqk_cfg_er81xx(mode)
#define ANA_GET_CH_FREQ(phy_idx, rx_info)                               ana_get_ch_freq_er81xx(rx_info)
#define ANA_GET_TX_RAMP_BACKUP(phy_idx)                                 ana_get_tx_ramp_backup_er81xx()
#define ANA_SET_PWR_MODE(phy_idx, mode)                                 ana_set_pwr_mode_er81xx(mode)
#define ANA_SET_TX_RAMP_CFG(phy_idx, mod_type)                          ana_set_tx_ramp_cfg_er81xx(mod_type)
#define ANA_GET_TPM_DIN_VAL(phy_idx)                                    ana_get_tpm_din_val_er81xx()
#define ANA_SET_TPM_DIN_VAL(phy_idx, val)                               ana_set_tpm_din_val_er81xx(val)
#define ANA_TOGGLE_TX_EN(phy_idx)                                       ana_toggle_tx_en_er81xx()
#define ANA_IS_TPM_CAL_DONE()                                           ana_is_tpm_cal_done_er81xx()
#define ANA_GET_TPM_DIN_HB()                                            ana_get_tpm_din_hb_er81xx()
#define ANA_SET_TPM_DIN_HB(val)                                         ana_set_tpm_din_hb_er81xx(val)
#define ANA_GET_KVC()                                                   ana_get_kvc_er81xx()
#define ANA_SET_KVC(val)                                                ana_set_kvc_er81xx(val)
#define ANA_SET_DCDC_DEFAUL_VAL()                                       ana_set_dcdc_default_val_er81xx()
#define ANA_SET_OSC_CAL_ENABLE(us)                                      ana_set_osc_cal_enable_er81xx(us)
#define ANA_SET_OSC_CAL_FINISH();                                       ana_set_osc_cal_finish_er81xx()
#define ANA_SET_OSC_BACKUP()                                            ana_set_osc_backup_er81xx()
#define ANA_SET_RX_IQK_PRECFG(phy_idx, iqk_backup)                      ana_set_rx_iqk_precfg_er81xx(phy_idx, iqk_backup)
#define ANA_RESTORE_RX_IQK_PRECFG(phy_idx, iqk_backup)                  ana_restore_rx_iqk_precfg_er81xx(phy_idx, iqk_backup)
#define ANA_GET_MAC_STATUS()                                            ana_get_mac_status_er81xx();

/* ========== MARCO define: BBP ============================================= */
#define BBP_GET_BBP_INFO()                                              bbp_get_bbp_info_er81xx()
#define BBP_SET_BASIC_CFG(phy_idx, bbp, mod_type, dr_idx)               bbp_set_basic_cfg_er81xx(bbp, mod_type, dr_idx)
#define BBP_SET_TX_FILT(phy_idx, mod_type, dr_idx)                      bbp_set_tx_filt_er81xx(mod_type, dr_idx)
#define BBP_SET_OSR(phy_idx, osr)                                       bbp_set_osr_er81xx(osr)
#define BBP_SET_RX_FILT(phy_idx, phy_mode, dr_idx, mod_idx)             bbp_set_rx_filt_er81xx(phy_mode, dr_idx, mod_idx)
#define BBP_SET_AGC(phy_idx, bbp, phy_mode, mod_type, dr_idx)           bbp_set_agc_er81xx(bbp, phy_mode, mod_type, dr_idx, mod_idx)
#define BBP_SET_BPSK_GAIN_THD(phy_idx, bbp)                             bbp_set_bpsk_gain_thd_er81xx(bbp)
#define BBP_SET_POP_CLIP(phy_idx)                                       bbp_set_pop_clip_er81xx()
#define BBP_SET_WIN_SIZE(phy_idx, bbp, phy_mode, dr_idx)                bbp_set_win_size_er81xx(bbp, phy_mode, dr_idx)
#define BBP_SET_OOK_CFG(phy_idx, bbp, dr_idx)                           /* Not supported */
#define BBP_SET_USER_CFG(phy_idx)                                       bbp_set_user_cfg_er81xx()
#define BBP_SET_RX_SYNCWORD(phy_idx, syncword, sw_len)                  bbp_set_rx_syncword_er81xx(syncword, sw_len)
#define BBP_SET_RX_PN9(phy_id, enable)                                  bbp_set_rx_pn9_er81xx(enable)
#define BBP_SET_RX_FEC(phy_id, enable)                                  bbp_set_rx_fec_er81xx(enable)
#define BBP_SET_IQK_CFG(phy_idx, bbp, dr_idx)                           bbp_set_iqk_cfg_er81xx(bbp, dr_idx)
#define BBP_SET_IQK_LMT(phy_idx, bbp)                                   bbp_set_iqk_lmt_er81xx(bbp)
#define BBP_CLEAN_IQK_CFG(phy_idx)                                      bbp_clean_iqk_cfg_er81xx()
#define BBP_GET_IQK_VAL(phy_idx)                                        bbp_get_iqk_val_er81xx()
#define BBP_SET_IQK_VAL(phy_idx, val)                                   bbp_set_iqk_val_er81xx(val)
#define BBP_GET_RX_INFO(phy_idx, rx_info)                               bbp_get_rx_info_er81xx(rx_info)
#define BBP_CHK_RSSI_RDY(phy_idx)                                       bbp_chk_rssi_rdy_er81xx()
#define BBP_SET_DDC_IF_PHI(phy_mode, data_rate)                         bbp_set_ddc_if_phi_er81xx(phy_mode, data_rate)
#define BBP_SET_BT_RATE(phy_idx, phy_mode, mod_type, dr, mod)           bbp_set_bt_rate_er81xx(phy_mode, mod_type, dr, mod)
#define BBP_SET_AGC_MODE(mode, gain_idx)                                bbp_set_agc_mode_er81xx(mode, gain_idx)
#define BBP_GET_AGC_IDX()                                               bbp_get_agc_idx_er81xx()
#define BBP_SET_AGC_IDX(idx)                                            bbp_set_agc_idx_er81xx(idx)
#define BBP_GET_BACKUP_AGC_IDX()                                        bbp_get_backup_agc_idx_er81xx()
#define BBP_GET_AGC_IBPWR()                                             bbp_get_inband_pwr_er81xx()
#define BBP_SET_ED_THRES(ed_thres)                                      bbp_set_ed_threshold_er81xx(ed_thres)
#define BBP_ENABLE_CDR_EST(enable)                                      bbp_enable_cdr_est_er81xx(enable)
#define BBP_GET_RSSI()                                                  bbp_get_rssi_er81xx()
#define BBP_SET_WMBUS_RX_EXTRA_CFG(wmbus_mode, dr)                      bbp_set_wmbus_rx_extra_cfg_er81xx(wmbus_mode, dr)
#define BBP_SETUP(phy_idx, phy_mode, dr, mod, wmbus)                    bbp_setup_er81xx(phy_mode, dr, mod, wmbus)
#define BBP_SET_AGC_GAIN_UPDATE_LOOP(update_times)                      bbp_set_agc_gian_update_loop_er81xx(update_times)

/* ========== MARCO define: MAC ============================================= */
#define MAC_GET_MAC_INFO()                                              mac_get_mac_info_er81xx()
#define MAC_SET_MOD_TYPE(phy_idx, mod_type)                             mac_set_mod_type_er81xx(mod_type)
#define MAC_SET_TX_DATA_RATE(phy_idx, data_rate)                        mac_set_tx_data_rate_er81xx(data_rate)
#define MAC_SET_TX_PHY_FORMAT_CFG(phy_idx, mod_type, tx_hw_opt)         mac_set_tx_phy_format_cfg_er81xx(mod_type, tx_hw_opt)
#define MAC_SET_RX_MANCHESTER(phy_idx, enable)                          mac_set_rx_manchester_er81xx(enable)
#define MAC_SET_TX_PREAMBLE(phy_idx, pream, pream_len)                  mac_set_tx_preamble_er81xx(pream, pream_len)
#define MAC_SET_TX_SYNCWORD(phy_idx, mod_type, sync_word, sw_len)       mac_set_tx_sync_word_er81xx(mod_type, sync_word, sw_len)
#define MAC_SET_TX_WMBUS_SYNCWORD(phy_idx, wmbus_mode, dr)              mac_set_tx_wmbus_sync_word_er81xx(wmbus_mode, dr)
#define MAC_SET_TX_WHITENING(phy_idx, mod_type, enable)                 mac_set_tx_whitening_er81xx(mod_type, enable)
#define MAC_GET_OSR(phy_idx, mod_type, dr_idx)                          mac_get_osr_er81xx(mod_type, dr_idx)
#define MAC_SET_RX_802154_KEEP_FMT(phy_idx, enable)                     mac_set_rx_802154_keepfmt_er81xx(enable)
#define MAC_SET_RX_MAX_LEN(phy_idx, rx_max_len)                         mac_set_rx_maxlen_er81xx(rx_max_len)
#define MAC_RX_START(phy_idx, phy_mode)                                 mac_rx_start_er81xx(phy_mode)
#define MAC_RX_STOP(phy_idx, phy_mode)                                  mac_rx_stop_er81xx(phy_mode)
#define MAC_SET_RX_RESTART(phy_idx, enable)                             mac_set_rx_restart_er81xx(enable)
#define MAC_GET_RX_BSY(phy_idx)                                         mac_get_rx_fsm_busy_er81xx()
#define MAC_ENABLE_INTERRUPT(phy_idx, mask)                             mac_enable_interrupt_er81xx(mask)
#define MAC_DISABLE_INTERRUPT(phy_idx)                                  mac_disable_interrupt_er81xx()
#define MAC_GET_RX_VAL_LEN(phy_idx, rx_info)                            mac_get_rx_val_len_er81xx(rx_info)
#define MAC_TX_START(phy_idx, tx_buf, tx_len)                           mac_tx_start_er81xx(tx_buf, tx_len)
#define MAC_TX_STOP(phy_idx)                                            mac_tx_stop_er81xx()
#define MAC_EVENT_GET_STATUS(phy_idx, event_info)                       mac_event_get_status_er81xx(event_info)
#define MAC_EVENT_CLEAR_STATUS(phy_idx, event_status)                   mac_event_clear_status_er81xx(event_status)
#define MAC_GET_IQK_CH_FREQ(phy_idx, mat_type)                          mac_get_iqk_ch_er81xx(mat_type)
#define MAC_SET_PANID(phy_idx, panid)                                   mac_set_panid_er81xx(panid)
#define MAC_SET_SHORT_ADDR(phy_idx, s_addr)                             mac_set_short_addr_er81xx(s_addr)
#define MAC_SET_LONG_ADDR(phy_idx, l_addr)                              mac_set_long_addr_er81xx(l_addr)
#define MAC_ENABLE_RX_TIMEOUT(phy_idx, to_base, to_num)                 mac_enable_rx_timeout_er81xx(to_base, to_num)
#define MAC_DISABLE_RX_TIMEOUT(phy_idx)                                 mac_disable_rx_timeout_er81xx()
#define MAC_SET_RX_PAYLOAD_ORDER(phy_idx, bit_rev, byte_rev)            mac_set_rx_payload_order_er81xx(bit_rev, byte_rev)
#define MAC_SET_TX_PAYLOAD_ORDER(phy_idx, bit_rev, byte_rev)            mac_set_tx_payload_order_er81xx(bit_rev, byte_rev)
#define MAC_SET_CRC_INVERSE(phy_idx, enable)                            mac_set_crc_inverse_er81xx(enable)
#define MAC_SET_CRC_BIT_ENDIAN(phy_idx, din, dout)                      mac_set_crc_bit_endian_er81xx(din, dout)
#define MAC_SET_CRC_BYTE_ENDIAN(phy_idx, endian)                        mac_set_crc_byte_endian_er81xx(endian)
#define MAC_SET_CRC_CONFIG(phy_idx, bit, poly, init_val)                mac_set_crc_config_er81xx(bit, poly, init_val)
#define MAC_ENABLE_CRC_CHECK(phy_idx, enable)                           mac_enable_crc_check_er81xx(enable)
#define MAC_ENABLE_PHR_CHECK(phy_idx, enable)                           mac_enable_phr_check_er81xx(enable)
#define MAC_SET_RX_PHR_CONFIG(phy_idx, len, offset, inv_en, bits)       mac_set_rx_phr_config_er81xx(len, offset, inv_en, bits)
#define MAC_SET_TX_PHR_CONFIG(phy_idx, len, offset, inv_en)             mac_set_tx_phr_config_er81xx(len, offset, inv_en)
/* ========== MARCO define: SYS ============================================= */
#define SYS_DELAY(phy_idx, us)                                          hal_drv_sys_tick_delay_us(us)

/* ========== MARCO define: PMU ============================================= */
#define MODULE_SW_RESET(phy_idx, module)                                hal_drv_pmu_module_sw_reset(module)

#elif defined(ES_DEVICE_RADIO) && defined(ES_DEVICE_TRX_RADIO)
/*####################### radio PHY 0 + PHY 1s ###############################*/

/* ========== MARCO define: ANA ============================================= */
#define ANA_SET_MOD_TYPE(phy_idx, mod_type)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_MOD_TYPE(mod_type);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_MOD_TYPE(mod_type);                                    \
        }                                                                      \
    }

#define ANA_SET_RX_DATA_RATE(phy_idx, phy_mode, data_rate)                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_RX_DATA_RATE(phy_mode, data_rate);                               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_RX_DATA_RATE(data_rate);                               \
        }                                                                      \
    }

#define ANA_GET_RX_DATA_RATE(phy_idx)                                          \
    ((phy_idx) == PHY_IDX_0   ? SOC_ANA_GET_RX_DATA_RATE()                       \
     : TRX_ANA_GET_RX_DATA_RATE())

#define ANA_SET_FREQ_DEVIA(phy_idx, freq_devia)                               \
        {                                                                          \
            if (phy_idx == PHY_IDX_0) {                                            \
                SOC_ANA_SET_FREQ_DEVIA(freq_devia);                               \
            }                                                                      \
            if (phy_idx == PHY_IDX_1) {                                            \
                TRX_ANA_SET_FREQ_DEVIA(freq_devia);                               \
            }                                                                      \
        }


#define ANA_SET_BASIC_CFG(phy_idx)                                             \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_BASIC_CFG();                                           \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_BASIC_CFG();                                           \
        }                                                                      \
    }

#define ANA_SET_CTRL_MODE(phy_idx, ctrl_mode)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_CTRL_MODE(ctrl_mode);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_CTRL_MODE(ctrl_mode);                                  \
        }                                                                      \
    }

#define ANA_SET_ADC_CFG(phy_idx, phy_mode, data_rate)                                       \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_ADC_CFG(phy_mode, data_rate);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_ADC_CFG(phy_mode, data_rate);                                     \
        }                                                                      \
    }

#define ANA_SET_DCDC_CFG(phy_idx, pa_type, mat_type)                                     \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_DCDC_CFG(pa_type, mat_type);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_DCDC_CFG(pa_type, mat_type);                                     \
        }                                                                      \
    }

#define ANA_SET_XTAL_CFG(phy_idx)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_XTAL_CFG();                                            \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_XTAL_CFG();                                            \
        }                                                                      \
    }

#define ANA_DO_FILTER_CALIBRATION(phy_idx)                                     \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_DO_FILTER_CALIBRATION();                                   \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_DO_FILTER_CALIBRATION();                                   \
        }                                                                      \
    }

#define ANA_SET_MODULE_PD_RST(phy_idx)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_MODULE_PD_RST();                                       \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_MODULE_PD_RST();                                       \
        }                                                                      \
    }

#define ANA_SET_CHARGE_PUMP(phy_idx, phy_mode, data_rate)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_CHARGE_PUMP(phy_mode, data_rate);                                       \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_CHARGE_PUMP(phy_mode, data_rate);                                       \
        }                                                                      \
    }


#define ANA_SET_TPM_DMI(phy_idx, phy_mode, mod_type, data_rate)                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TPM_DMI(phy_mode, mod_type, data_rate);                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TPM_DMI(phy_mode, mod_type, data_rate);                          \
        }                                                                      \
    }

#define ANA_SET_TPM_CAL(phy_idx, mod_type, data_rate)                          \
        {                                                                          \
            if (phy_idx == PHY_IDX_0) {                                            \
                SOC_ANA_SET_TPM_CAL(mod_type, data_rate);                          \
            }                                                                      \
            if (phy_idx == PHY_IDX_1) {                                            \
                TRX_ANA_SET_TPM_CAL(mod_type, data_rate);                          \
            }                                                                      \
        }

#define ANA_SET_PA_CFG(phy_idx, mat_type, pa_type, mod_type, data_rate)                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_PA_CFG(pa_type, mat_type, mod_type, data_rate);                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_PA_CFG(pa_type, mat_type, mod_type, data_rate);                  \
        }                                                                      \
    }

#define ANA_SET_PA_PWR_TBL(phy_idx, mod_type)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_PA_PWR_TBL(mod_type);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_PA_PWR_TBL(mod_type);                                  \
        }                                                                      \
    }

#define ANA_SET_SYNTHESIZE(phy_idx, mod_type)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_SYNTHESIZE(mod_type);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_SYNTHESIZE(mod_type);                                  \
        }                                                                      \
    }

#define ANA_SET_TX_PWR_LV(phy_idx, mod_type, lv)                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TX_PWR_LV(mod_type, lv);                               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TX_PWR_LV(mod_type, lv);                               \
        }                                                                      \
    }

#define ANA_SET_TX_RAMP_CFG(phy_idx, mod_type)                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TX_RAMP_CFG(mod_type);                               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TX_RAMP_CFG(mod_type);                               \
        }                                                                      \
    }

#define ANA_SET_TX_RAMP_BACKUP(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TX_RAMP_BACKUP();                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TX_RAMP_BACKUP();                                      \
        }                                                                      \
    }

#define ANA_SET_TX_WRKD(phy_idx, enable)                                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TX_WRKD(enable);                                             \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            /*not support */                                                   \
        }                                                                      \
    }

#define ANA_SET_TX_TPM_WRKD(phy_idx)                                           \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TX_TPM_WRKD();                                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            /*not support */                                                   \
        }                                                                      \
    }

#define ANA_SET_OSR(phy_idx, osr)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_OSR(osr);                                              \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_OSR(osr);                                              \
        }                                                                      \
    }

#define ANA_SET_RX_ATOP_CFG(phy_idx)                                           \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_RX_ATOP_CFG();                                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_RX_ATOP_CFG();                                         \
        }                                                                      \
    }

#define ANA_SET_GAIN_SRC(phy_idx, gain_src)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_GAIN_SRC(gain_src);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_GAIN_SRC(gain_src);                                    \
        }                                                                      \
    }

#define ANA_SET_RX_GAIN(phy_idx)                                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_RX_GAIN();                                             \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_RX_GAIN();                                             \
        }                                                                      \
    }

#define ANA_SET_RX_FILT(phy_idx, phy_mode, data_rate, mod_idx)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_RX_FILT(phy_mode, data_rate, mod_idx);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_RX_FILT(data_rate, mod_idx);                                    \
        }                                                                      \
    }

#define ANA_SET_USER_CFG(phy_idx)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_USER_CFG();                                            \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_USER_CFG();                                            \
        }                                                                      \
    }

#define ANA_SET_CH_FREQ(phy_idx, ch)                                           \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_CH_FREQ(ch);                                           \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_CH_FREQ(ch);                                           \
        }                                                                      \
    }

#define ANA_ENABLE_SINGLETONE(phy_idx, mod_type, pwr_lv, enable)               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_ENABLE_SINGLETONE(mod_type, pwr_lv, enable);               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_ENABLE_SINGLETONE(mod_type, pwr_lv, enable);               \
        }                                                                      \
    }

#define ANA_SET_IQK_CFG(phy_idx, mode)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_IQK_CFG(mode);                                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_IQK_CFG(mode);                                         \
        }                                                                      \
    }

#define ANA_GET_RX_STATUS(phy_idx)                                            \
    ((phy_idx) == PHY_IDX_0   ? SOC_ANA_GET_RX_STATUS()                       \
     : (phy_idx) == PHY_IDX_1 ? TRX_ANA_GET_RX_STATUS()                       \
     : 1)

#define ANA_CLEAN_IQK_CFG(phy_idx, mode)                                       \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_CLEAN_IQK_CFG(mode);                                       \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_CLEAN_IQK_CFG(mode);                                       \
        }                                                                      \
    }

#define ANA_GET_CH_FREQ(phy_idx, rx_info)                                      \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_GET_CH_FREQ(rx_info);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_GET_CH_FREQ(rx_info);                                      \
        }                                                                      \
    }

#define ANA_GET_TX_RAMP_BACKUP(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_GET_TX_RAMP_BACKUP();                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_GET_TX_RAMP_BACKUP();                                      \
        }                                                                      \
    }

#define ANA_SET_PWR_MODE(phy_idx, mode)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_PWR_MODE(mode);                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_PWR_MODE(mode);                                        \
        }                                                                      \
    }

#define ANA_GET_TPM_DIN_VAL(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_GET_TPM_DIN_VAL();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_GET_TPM_DIN_VAL();                                        \
        }                                                                      \
    }
    
#define ANA_SET_TPM_DIN_VAL(phy_idx, val)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TPM_DIN_VAL(val);                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TPM_DIN_VAL(val);                                        \
        }                                                                      \
    }

#define ANA_TOGGLE_TX_EN(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_TOGGLE_TX_EN();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_TOGGLE_TX_EN();                                        \
        }                                                                      \
    }

#define ANA_IS_TPM_CAL_DONE(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_IS_TPM_CAL_DONE();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_IS_TPM_CAL_DONE();                                        \
        }                                                                      \
    }

#define ANA_GET_TPM_DIN_HB(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_GET_TPM_DIN_HB();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_GET_TPM_DIN_HB();                                        \
        }                                                                      \
    }

#define ANA_SET_TPM_DIN_HB(phy_idx, val)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_TPM_DIN_HB();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_TPM_DIN_HB(val);                                        \
        }                                                                      \
    }

#define ANA_GET_KVC(phy_idx)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_GET_KVC();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_GET_KVC();                                        \
        }                                                                      \
    }

#define ANA_SET_KVC(phy_idx, val)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_ANA_SET_KVC();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_ANA_SET_KVC();                                        \
        }                                                                      \
    }

/* ========== MARCO define: BBP ============================================= */
#define BBP_SET_BASIC_CFG(phy_idx, bbp, mod_type, dr_idx)                      \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_BASIC_CFG(bbp, mod_type, dr_idx);                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_BASIC_CFG(bbp, mod_type, dr_idx);                      \
        }                                                                      \
    }

#define BBP_SET_TX_FILT(phy_idx, mod_type, dr_idx)                             \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_TX_FILT(mod_type, dr_idx);                             \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_TX_FILT(mod_type, dr_idx);                             \
        }                                                                      \
    }

#define BBP_SET_OSR(phy_idx, osr)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_OSR(osr);                                              \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_OSR(osr);                                              \
        }                                                                      \
    }

#define BBP_SET_RX_FILT(phy_idx, phy_mode, dr_idx, mod_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_RX_FILT(phy_mode, dr_idx, mod_idx);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_RX_FILT(phy_mode, dr_idx, mod_idx);                                  \
        }                                                                      \
    }

#define BBP_SET_AGC(phy_idx, bbp, phy_mode, mod_type, dr_idx, mod_idx)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_AGC(bbp, phy_mode, mod_type, dr_idx, mod_idx);                   \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_AGC(bbp, mod_type, dr_idx, mod_idx);                   \
        }                                                                      \
    }

#define BBP_SET_BPSK_GAIN_THD(phy_idx, bbp)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_BPSK_GAIN_THD(bbp);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_BPSK_GAIN_THD(bbp);                                    \
        }                                                                      \
    }

#define BBP_SET_POP_CLIP(phy_idx)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_POP_CLIP();                                            \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_POP_CLIP();                                            \
        }                                                                      \
    }

#define BBP_SET_WIN_SIZE(phy_idx, bbp, phy_mode, dr_idx)                                 \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_WIN_SIZE(bbp, phy_mode, dr_idx);                                 \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_WIN_SIZE(bbp, dr_idx);                                 \
        }                                                                      \
    }



#define BBP_SET_OOK_CFG(phy_idx, bbp, dr_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_OOK_CFG(bbp, dr_idx);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_OOK_CFG(bbp, dr_idx);                                  \
        }                                                                      \
    }

#define BBP_SET_USER_CFG(phy_idx)                                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_USER_CFG();                                            \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_USER_CFG();                                            \
        }                                                                      \
    }

#define BBP_SET_RX_SYNCWORD(phy_idx, syncword, sw_len)                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_RX_SYNCWORD(syncword, sw_len);                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_RX_SYNCWORD(syncword, sw_len);                         \
        }                                                                      \
    }

#define BBP_SET_RX_PN9(phy_id, enable)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_RX_PN9(enable);                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_RX_PN9(enable);                                        \
        }                                                                      \
    }

#define BBP_SET_RX_FEC(phy_id, enable)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_RX_FEC(enable);                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_RX_FEC(enable);                                        \
        }                                                                      \
    }

#define BBP_SET_IQK_CFG(phy_idx, bbp, dr_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_IQK_CFG(bbp, dr_idx);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_IQK_CFG(bbp, dr_idx);                                  \
        }                                                                      \
    }

#define BBP_SET_IQK_LMT(phy_idx, bbp)                                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_IQK_LMT(bbp);                                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_IQK_LMT(bbp);                                          \
        }                                                                      \
    }

#define BBP_CLEAN_IQK_CFG(phy_idx)                                             \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_CLEAN_IQK_CFG();                                           \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_CLEAN_IQK_CFG();                                           \
        }                                                                      \
    }

#define BBP_GET_IQK_VAL(phy_idx)                                               \
    ((phy_idx) == PHY_IDX_0   ? SOC_BBP_GET_IQK_VAL()                          \
     : (phy_idx) == PHY_IDX_1 ? TRX_BBP_GET_IQK_VAL()                          \
     : 0)

#define BBP_SET_IQK_VAL(phy_idx, val)                                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_IQK_VAL(val);                                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_IQK_VAL(val);                                          \
        }                                                                      \
    }

#define BBP_GET_RX_INFO(phy_idx, rx_info)                                      \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_GET_RX_INFO(rx_info);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_GET_RX_INFO(rx_info);                                      \
        }                                                                      \
    }

#define BBP_CHK_RSSI_RDY(phy_idx)                                              \
    ((phy_idx) == PHY_IDX_0   ? SOC_BBP_CHK_RSSI_RDY()                         \
     : (phy_idx) == PHY_IDX_1 ? TRX_BBP_CHK_RSSI_RDY()                         \
     : 0)

#define BBP_SETUP_MBUS(dr_idx)                                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SETUP_MBUS(dr_idx);                                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SETUP_MBUS(dr_idx);                                          \
        }                                                                      \
    }

#define BBP_SET_DDC_IF_PHI(phy_mode, data_rate)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_DDC_IF_PHI(phy_mode, data_rate);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_DDC_IF_PHI(data_rate);                                      \
        }                                                                      \
    }

#define BBP_SET_BT_RATE(phy_idx, phy_mode, mode_type, data_rate, mod_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_BT_RATE(phy_mode, mode_type, data_rate, mod_idx);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_BT_RATE(phy_mode, mode_type, data_rate, mod_idx);                                      \
        }                                                                      \
    }

#define BBP_SETUP(phy_idx, modulation_type, phy_mode, data_rate, mod_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SETUP(phy_mode, modulation_type, data_rate);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SETUP(phy_mode, modulation_type, data_rate);                                      \
        }                                                                      \
    }

#define BBP_SET_AGC_MODE(mode, gain_idx)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_AGC_MODE(mode, gain_idx);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_AGC_MODE(mode, gain_idx);                                      \
        }                                                                      \
    }

#define BBP_GET_AGC_IDX()                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_GET_AGC_IDX();                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_GET_AGC_IDX(;                                      \
        }                                                                      \
    }

#define BBP_GET_AGC_IBPWR()                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_GET_AGC_IBPWR();                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_GET_AGC_IBPWR(;                                      \
        }                                                                      \
    }

#define BBP_SET_ED_THRES(ed_thres)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_SET_ED_THRES(ed_thres);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_SET_ED_THRES(ed_thres);                                      \
        }                                                                      \
    }

#define BBP_ENABLE_CDR_EST(enable)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_BBP_ENABLE_CDR_EST(enable);                                      \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_BBP_ENABLE_CDR_EST(enable);                                      \
        }                                                                      \
    }
/* ========== MARCO define: MAC ============================================= */
#define MAC_SET_MOD_TYPE(phy_idx, mod_type)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_MOD_TYPE(mod_type);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_MOD_TYPE(mod_type);                                    \
        }                                                                      \
    }

#define MAC_SET_TX_DATA_RATE(phy_idx, data_rate)                               \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_DATA_RATE(data_rate);                               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_DATA_RATE(data_rate);                               \
        }                                                                      \
    }

#define MAC_SET_TX_PHY_FORMAT_CFG(phy_idx, mod_type, tx_hw_opt)                \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_PHY_FORMAT_CFG(mod_type, tx_hw_opt);                \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_PHY_FORMAT_CFG(mod_type, tx_hw_opt);                \
        }                                                                      \
    }

#define MAC_SET_RX_MANCHESTER(phy_idx, enable)                \
        {                                                                          \
            if (phy_idx == PHY_IDX_0) {                                            \
                SOC_MAC_SET_RX_MANCHESTER(enable);                \
            }                                                                      \
            if (phy_idx == PHY_IDX_1) {                                            \
                TRX_MAC_SET_RX_MANCHESTER(enable);                \
            }                                                                      \
        }

#define MAC_SET_TX_PREAMBLE(phy_idx, pream, pream_len)                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_PREAMBLE(pream, pream_len);                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_PREAMBLE(pream, pream_len);                         \
        }                                                                      \
    }

#define MAC_SET_TX_SYNCWORD(phy_idx, mod_type, sync_word, sync_word_len)       \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_SYNCWORD(mod_type, sync_word, sync_word_len);       \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_SYNCWORD(mod_type, sync_word, sync_word_len);       \
        }                                                                      \
    }

#define MAC_SET_TX_WHITENING(phy_idx, mod_type, enable)                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_WHITENING(mod_type, enable);                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_WHITENING(mod_type, enable);                        \
        }                                                                      \
    }

#define MAC_GET_OSR(phy_idx, mod_type, dr_idx)                                 \
    ((phy_idx) == PHY_IDX_0   ? SOC_MAC_GET_OSR(mod_type, dr_idx)              \
     : (phy_idx) == PHY_IDX_1 ? TRX_MAC_GET_OSR(mod_type, dr_idx)              \
     : 0)

#define MAC_SET_RX_802154_KEEP_FMT(phy_idx, enable)                            \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_RX_802154_KEEP_FMT(enable);                            \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_RX_802154_KEEP_FMT(enable);                            \
        }                                                                      \
    }

#define MAC_SET_RX_MAX_LEN(phy_idx, rx_max_len)                                \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_RX_MAX_LEN(rx_max_len);                                \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_RX_MAX_LEN(rx_max_len);                                \
        }                                                                      \
    }

#define MAC_RX_START(phy_idx, phy_mode)                                        \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_RX_START(phy_mode);                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_RX_START(phy_mode);                                        \
        }                                                                      \
    }

#define MAC_RX_STOP(phy_idx, phy_mode)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_RX_STOP(phy_mode);                                         \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_RX_STOP(phy_mode);                                         \
        }                                                                      \
    }

#define MAC_SET_RX_RESTART(phy_idx, enable)                                    \
        {                                                                      \
            if (phy_idx == PHY_IDX_0) {                                        \
                SOC_MAC_SET_RX_RESTART(enable);                                \
            }                                                                  \
            if (phy_idx == PHY_IDX_1) {                                        \
                TRX_MAC_SET_RX_RESTART(enable);                                \
            }                                                                  \
        }

#define MAC_ENABLE_INTERRUPT(phy_idx)                                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_ENABLE_INTERRUPT();                                        \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_ENABLE_INTERRUPT();                                        \
        }                                                                      \
    }

#define MAC_DISABLE_INTERRUPT(phy_idx)                                         \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_DISABLE_INTERRUPT();                                       \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_DISABLE_INTERRUPT();                                       \
        }                                                                      \
    }

#define MAC_GET_RX_VAL_LEN(phy_idx, rx_info)                                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_GET_RX_VAL_LEN(rx_info);                                   \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_GET_RX_VAL_LEN(rx_info);                                   \
        }                                                                      \
    }

#define MAC_TX_START(phy_idx, tx_buf, tx_len)                                  \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_TX_START(tx_buf, tx_len);                                  \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_TX_START(tx_buf, tx_len);                                  \
        }                                                                      \
    }

#define MAC_TX_STOP(phy_idx)                                                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_TX_STOP();                                                 \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_TX_STOP();                                                 \
        }                                                                      \
    }

#define MAC_EVENT_GET_STATUS(phy_idx, event_info)                              \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_EVENT_GET_STATUS(event_info);                              \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_EVENT_GET_STATUS(event_info);                              \
        }                                                                      \
    }

#define MAC_EVENT_CLEAR_STATUS(phy_idx, event_status)                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_EVENT_CLEAR_STATUS(event_status);                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_EVENT_CLEAR_STATUS(event_status);                          \
        }                                                                      \
    }

#define MAC_GET_IQK_CH_FREQ(phy_idx, mat_type)                                           \
    ((phy_idx) == PHY_IDX_0   ? SOC_MAC_GET_IQK_CH_FREQ(mat_type)                      \
     : (phy_idx) == PHY_IDX_1 ? TRX_MAC_GET_IQK_CH_FREQ(mat_type)                      \
     : 0)

#define MAC_SET_PANID(phy_idx, panid)                                          \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_PANID(panid);                                          \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_PANID(panid);                                          \
        }                                                                      \
    }

#define MAC_SET_SHORT_ADDR(phy_idx, s_addr)                                    \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_SHORT_ADDR(s_addr);                                    \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_SHORT_ADDR(s_addr);                                    \
        }                                                                      \
    }

#define MAC_SET_LONG_ADDR(phy_idx, l_addr)                                     \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_LONG_ADDR(l_addr);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_LONG_ADDR(l_addr);                                     \
        }                                                                      \
    }

#define MAC_ENABLE_RX_TIMEOUT(phy_idx, to_base, to_num)                                     \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_ENABLE_RX_TIMEOUT(to_base, to_num);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_ENABLE_RX_TIMEOUT(to_base, to_num);                                     \
        }                                                                      \
    }

#define MAC_DISABLE_RX_TIMEOUT(phy_idx)                                                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_DISABLE_RX_TIMEOUT();                                                 \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_DISABLE_RX_TIMEOUT();                                                 \
        }                                                                      \
    }

#define MAC_SET_RX_PAYLOAD_ORDER(phy_idx, bit_rev, byte_rev)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_RX_PAYLOAD_ORDER(bit_rev, byte_rev);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_RX_PAYLOAD_ORDER(bit_rev, byte_rev);                                     \
        }                                                                      \
    }

#define MAC_SET_TX_PAYLOAD_ORDER(phy_idx, bit_rev, byte_rev)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_PAYLOAD_ORDER(bit_rev, byte_rev);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_PAYLOAD_ORDER(bit_rev, byte_rev);                                     \
        }                                                                      \
    }

#define MAC_SET_CRC_INVERSE(phy_idx, enable)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_CRC_INVERSE(enable);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_CRC_INVERSE(enable);                                     \
        }                                                                      \
    }

#define MAC_SET_CRC_BIT_ENDIAN(phy_idx, din, dout)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_CRC_BIT_ENDIAN(din, dout);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_CRC_BIT_ENDIAN(din, dout);                                     \
        }                                                                      \
    }

#define MAC_SET_CRC_BYTE_ENDIAN(phy_idx, endian)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_CRC_BYTE_ENDIAN(endian);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_CRC_BYTE_ENDIAN(endian);                                     \
        }                                                                      \
    }

#define MAC_SET_CRC_CONFIG(phy_idx, bit, poly, init_val)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_CRC_CONFIG(bit, poly, init_val);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_CRC_CONFIG(bit, poly, init_val);                                     \
        }                                                                      \
    }

#define MAC_ENABLE_CRC_CHECK(phy_idx, enable)                   \
        {                                                                          \
            if (phy_idx == PHY_IDX_0) {                                            \
                SOC_MAC_ENABLE_CRC_CHECK(enable);                                     \
            }                                                                      \
            if (phy_idx == PHY_IDX_1) {                                            \
                TRX_MAC_ENABLE_CRC_CHECK(enable);                                     \
            }                                                                      \
        }

#define MAC_ENABLE_PHR_CHECK(phy_idx, enable)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_ENABLE_PHR_CHECK(enable);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_ENABLE_PHR_CHECK(enable);                                     \
        }                                                                      \
    }

#define MAC_SET_RX_PHR_CONFIG(phy_idx, len, offset, inv_en, bits)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_RX_PHR_CONFIG(len, offset, inv_en, bits);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_RX_PHR_CONFIG(phy_idx, len, offset, inv_en, bits);                                     \
        }                                                                      \
    }

#define MAC_SET_TX_PHR_CONFIG(phy_idx, len, offset, inv_en)                   \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            SOC_MAC_SET_TX_PHR_CONFIG(len, offset, inv_en);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            TRX_MAC_SET_TX_PHR_CONFIG(phy_idx, len, offset, inv_en);                                     \
        }                                                                      \
    }
/* ========== MARCO define: SYS ============================================= */
#define SYS_DELAY(phy_idx, us)                                                 \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            hal_drv_sys_tick_delay_us(us);                                     \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            host_intf_sys_delay(us);                                           \
        }                                                                      \
    }
/* ========== MARCO define: PMU ============================================= */
#define MODULE_SW_RESET(phy_idx, module)                                       \
    {                                                                          \
        if (phy_idx == PHY_IDX_0) {                                            \
            hal_drv_pmu_module_sw_reset(module);                               \
        }                                                                      \
        if (phy_idx == PHY_IDX_1) {                                            \
            hal_drv_trx_pmu_module_sw_reset(module);                           \
        }                                                                      \
    }

#endif

#endif /* __HAL_DRV_SPI_H */
