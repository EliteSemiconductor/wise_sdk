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

/* IQK Manual Mode Configuration Constants */
#define IQK_SPP_POWER_THRESHOLD                     0xA0      // SPP Power threshold for valid calibration
#define IQK_MANUAL_MAX_RETRY_ATTEMPTS               50        // Maximum calibration retry attempts
#define IQK_MANUAL_CALIBRATION_DELAY_US             20000     // Delay between trigger and status check (20ms)
#define IQK_RESULT_INVALID_VALUE                    0x0       // Invalid calibration result indicator

/* -----MAC------------------------------------------------------------------ */
#define VALIDATE_SADDR(addr)                        (addr != 0xFFFF)
#define VALIDATE_LADDR(l, h)                        ((l != 0xFFFFFFFF) && (h != 0xFFFFFFFF))
/* CRC Hardware Default Values */
#define CRC_DEFAULT_BIT_ENDIAN_IN                   1      // LSB first 
#define CRC_DEFAULT_BIT_ENDIAN_OUT                  1      // LSB first 
#define CRC_DEFAULT_BYTE_ENDIAN                     0      // Byte order: LSB first
#define CRC_DEFAULT_WIDTH                           16     // 16-bit CRC
#define CRC_DEFAULT_POLYNOMIAL                      0x1021 // CCITT polynomial
#define CRC_DEFAULT_SEED                            0      // No initial seed
#define CRC_DEFAULT_INVERSE                         0      // No inversion
#define CRC_DEFAULT_INCLUDE_PHR                     0      // Exclude PHR from CRC
#define CRC_DEFAULT_ENDIAN_IN_FCS                   0      // Standard FCS endianness

/* RX-specific CRC defaults (Designer-recommended) */
#define CRC_RX_ALTCHK_DEFAULT                       1      // Enable alternative check
#define CRC_RX_ENDIAN_IN_FCS_DEFAULT                1      // Enable FCS endian inversion

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

enum crc_cfg_target {
    CRC_CFG_TARGET_TX   = 0x0,
    CRC_CFG_TARGET_RX   = 0x1,
    CRC_CFG_TARGET_BOTH = 0x2,
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
    PA_TYPE_MAX = 2,
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
    uint8_t sw12_f;
    uint8_t en_random12;
    uint8_t vsel;
    uint8_t en_ipn;

    // ANCTL_DCDC_REG3 (0xE5C)
    uint8_t tar_idle;
    uint8_t tar_tx_1g;
    uint8_t tar_rx_1g;
} ANCTL_DCDC_PARA;

typedef struct {
    // BBP_RXIQCAL_CFG register fields
    uint8_t co_step;    // RG_BB_RXIQ_CO_STEP (4 bits)
    uint8_t co_dft;     // RG_BB_RXIQ_CO_DFT  (2 bits)
    uint8_t fi_dft0;    // RG_BB_RXIQ_FI_DFT0 (2 bits)
    uint8_t fi_dft1;    // RG_BB_RXIQ_FI_DFT1 (2 bits)
    uint8_t fi_dft2;    // RG_BB_RXIQ_FI_DFT2 (2 bits)
    uint8_t fi_th1;     // RG_BB_RXIQ_FI_TH1  (8 bits)
    uint8_t fi_th2;     // RG_BB_RXIQ_FI_TH2  (4 bits)
} BBP_RXIQ_CAL_PARA;


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
void hal_drv_radio_do_iqk(int8_t phy_idx, uint32_t ch_freq);
void hal_drv_radio_trig_tpm_calib(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_drv_radio_get_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_drv_radio_set_tpm_cal_val(TPM_CAL_PARA *tpm_cal_val);
void hal_drv_radio_set_dcdc_default_val(void);
HAL_STATUS hal_drv_radio_set_rx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
HAL_STATUS hal_drv_radio_set_tx_config(int8_t phy_idx, RADIO_CFG_T *radio_cfg);
void hal_drv_radio_rx_iqk(int8_t phy_idx);
void hal_drv_radio_set_ulpldo(uint8_t enable);
uint8_t hal_drv_radio_get_tx_ramp_tpm(void);
void hal_drv_radio_set_tx_ramp_tpm(uint8_t enable);
void hal_drv_radio_enable_bod(uint8_t enable, uint8_t bod_lv);
uint8_t hal_drv_radio_is_state_idle(void);
void hal_drv_radio_enable_efuse_write_data(uint8_t enable);
void hal_drv_radio_enable_prbs9(uint8_t enable);

#ifdef ES_DEVICE_TRX_RADIO
void hal_drv_radio_get_rx_fifo(uint8_t *rx_buf, uint32_t rx_len);
void hal_drv_radio_reset_rx_fifo(void);

#endif


#endif /* __HAL_DRV_SPI_H */
