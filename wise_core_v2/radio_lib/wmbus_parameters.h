#ifndef __WMBUS_PARAMETERS_H 
#define __WMBUS_PARAMETERS_H

#include <stdint.h>

#define REDUCE_WMBUS_PARAMS

// ---------------------- Mode S Parameters -----------------------
// Frequency Parameters (Hz)
#define WMBUS_S_M2O_FREQ_MIN                    868250000
#define WMBUS_S_M2O_FREQ_TYP                    868300000
#define WMBUS_S_M2O_FREQ_MAX                    868350000
#define WMBUS_S_M2O_FREQ_TOLERANCE_PPM          60
#define WMBUS_S_O2M_FREQ_MIN                    868278000
#define WMBUS_S_O2M_FREQ_TYP                    868300000
#define WMBUS_S_O2M_FREQ_MAX                    868322000
#define WMBUS_S_O2M_FREQ_TOLERANCE_PPM          25

// FSK Deviation (Hz)
#define WMBUS_S_FSK_DEV_MIN                     40000
#define WMBUS_S_FSK_DEV_TYP                     50000
#define WMBUS_S_FSK_DEV_MAX                     80000

// Chip Rate Transmit (cps)
#define WMBUS_S_CHIP_RATE                       32768

//Tolerance (%)
#define WMBUS_S_CHIP_RATE_TOLERANCE_PCT_PPM     15000

//Jitter (us)
#define WMBUS_S_JITTER_US                       3

// Data Rate (bps)
#define WMBUS_S_DATA_RATE                       (WMBUS_S_CHIP_RATE / 2)    //Manchester code

//Preamble (chips)
#define WMBUS_S_PREAMBLE_SYNC_WORD_SHORT_LEN     48
#define WMBUS_S_PREAMBLE_SYNC_WORD_LONG_LEN     576
#define WMBUS_S_POSTAMBLE_MIN                    2
#define WMBUS_S_POSTAMBLE_MAX                    8

#define WMBUS_S_SYNC_WORD                        0b000111011010010110   //0x7696
#define WMBUS_S_SYNC_WORD_LEN_BIT                18
#define WMBUS_S_PREAMBLE_01_SHORT_COUNT          15
#define WMBUS_S_PREAMBLE_01_LONG_COUNT           279


// ---------------------- Mode T Parameters -----------------------
// Frequency Parameters (Hz)
#define WMBUS_T_M2O_FREQ_MIN                    868900000
#define WMBUS_T_M2O_FREQ_TYP                    868950000
#define WMBUS_T_M2O_FREQ_MAX                    869000000
#define WMBUS_T_M2O_FREQ_TOLERANCE              60
#define WMBUS_T_O2M_FREQ_MIN                    868278000
#define WMBUS_T_O2M_FREQ_TYP                    868300000
#define WMBUS_T_O2M_FREQ_MAX                    868322000
#define WMBUS_T_O2M_FREQ_TOLERANCE              25

// FSK Deviation (Hz)
#define WMBUS_T_M2O_FSK_DEV_MIN                 40000
#define WMBUS_T_M2O_FSK_DEV_TYP                 50000
#define WMBUS_T_M2O_FSK_DEV_MAX                 80000
#define WMBUS_T_O2M_FSK_DEV_MIN                 40000
#define WMBUS_T_O2M_FSK_DEV_TYP                 50000
#define WMBUS_T_O2M_FSK_DEV_MAX                 80000


// Chip Rate Transmit (cps)
#define WMBUS_T_M2O_CHIP_RATE_MIN               90000
#define WMBUS_T_M2O_CHIP_RATE_TYP               100000
#define WMBUS_T_M2O_CHIP_RATE_MAX               110000
#define WMBUS_T_O2M_CHIP_RATE                   32768

//Tolerance (%)
#define WMBUS_T_M2O_CHIP_RATE_VARIATION_PCT_PPM 10000
#define WMBUS_T_O2M_CHIP_RATE_TOLERANCE_PPM     15000

// Data Rate (bps)
#define WMBUS_T_M2O_DATA_RATE                   ((WMBUS_S_CHIP_RATE * 2) / 3)    //3 out of 6 code
#define WMBUS_T_O2M_DATA_RATE                   (WMBUS_S_CHIP_RATE / 2)    //Manchester code

//Jitter (us)
#define WMBUS_T_JITTER_US                       3

//Preamble (chips)
#define WMBUS_T_PREAMBLE_SYNC_WORD_MTR_FORMAT_A_LEN         48
#define WMBUS_T_SYNC_WORD_MTR_FORMAT_A                      0b0000111101   //0x3D
#define WMBUS_T_SYNC_WORD_MTR_FORMAT_A_LEN_BIT              10
#define WMBUS_T_PREAMBLE_01_MTR_FORMAT_A_COUNT              19

#define WMBUS_T_PREAMBLE_SYNC_WORD_GW_FORMAT_A_LEN         48
#define WMBUS_T_SYNC_WORD_GW_FORMAT_A                      0b000111011010010110   //0x7696
#define WMBUS_T_SYNC_WORD_GW_FORMAT_A_LEN_BIT              18
#define WMBUS_T_PREAMBLE_01_GW_FORMAT_A_COUNT              15

#define WMBUS_T_POSTAMBLE_MIN                   2
#define WMBUS_T_POSTAMBLE_MAX                   8


// ---------------------- Mode C Parameters -----------------------
// Frequency Parameters (Hz)
#define WMBUS_C_M2O_FREQ_MIN                    868928000
#define WMBUS_C_M2O_FREQ_TYP                    868950000
#define WMBUS_C_M2O_FREQ_MAX                    869720000
#define WMBUS_C_M2O_FREQ_TOLERANCE              25
#define WMBUS_C_O2M_FREQ_MIN                    869503000
#define WMBUS_C_O2M_FREQ_TYP                    869525000
#define WMBUS_C_O2M_FREQ_MAX                    869547000
#define WMBUS_C_O2M_FREQ_TOLERANCE              25

// FSK Deviation (Hz)
#define WMBUS_C_M2O_FSK_DEV_MIN                 33750
#define WMBUS_C_M2O_FSK_DEV_TYP                 45000
#define WMBUS_C_M2O_FSK_DEV_MAX                 56250
#define WMBUS_C_O2M_GFSK_DEV_MIN                18750
#define WMBUS_C_O2M_GFSK_DEV_TYP                25000
#define WMBUS_C_O2M_GFSK_DEV_MAX                31250

// Chip Rate Transmit (cps)
#define WMBUS_C_M2O_CHIP_RATE                   100000
#define WMBUS_C_O2M_CHIP_RATE                   50000

//Tolerance (ppm)
#define WMBUS_C_CHIP_RATE_TOLERANCE_PCT_PPM     100

// Data Rate (bps)
#define WMBUS_C_M2O_DATA_RATE                   (WMBUS_C_M2O_CHIP_RATE)    //NRZ
#define WMBUS_C_O2M_DATA_RATE                   (WMBUS_C_O2M_CHIP_RATE)    //NRZ

//Preamble and sync (chips)
#define WMBUS_C_PREAMBLE_LEN_MIN                32
#define WMBUS_C_PREAMBLE_LEN_MAX                32
#define WMBUS_C_SYNC_MIN                        32
#define WMBUS_C_SYNC_MAX                        32

#define WMBUS_C_SYNC_WORD_FORMAT_A              0b01010100001111010101010011001101  //0x543D54CD
#define WMBUS_C_SYNC_WORD_FORMAT_B              0b01010100001111010101010000111101  //0x543D543D
#define WMBUS_C_SYNC_WORD_LEN_BIT               32
#define WMBUS_C_PREAMBLE_01_COUNT               16

// ---------------------- Mode R Parameters -----------------------
// Frequency Parameters (Hz)
#define WMBUS_R_M2O_FREQ_TYP                    868330000
#define WMBUS_R_M2O_FREQ_TOLERANCE              20
#define WMBUS_R_O2M_FREQ_TYP                    868030000
#define WMBUS_R_O2M_FREQ_TOLERANCE              20

// FSK Deviation (Hz)
#define WMBUS_R_M2O_FSK_DEV_MIN                 4800
#define WMBUS_R_M2O_FSK_DEV_TYP                 6000
#define WMBUS_R_M2O_FSK_DEV_MAX                 7200
#define WMBUS_R_O2M_FSK_DEV_MIN                 4800
#define WMBUS_R_O2M_FSK_DEV_TYP                 6000
#define WMBUS_R_O2M_FSK_DEV_MAX                 7200

// Chip Rate Transmit (cps)
#define WMBUS_R_M2O_CHIP_RATE                   4800
#define WMBUS_R_O2M_CHIP_RATE                   4800

//Tolerance (ppm)
#define WMBUS_R_CHIP_RATE_TOLERANCE_PCT_PPM     1500

// Data Rate (bps)
#define WMBUS_R_M2O_DATA_RATE                   (WMBUS_S_CHIP_RATE / 2)    //Manchester code
#define WMBUS_R_O2M_DATA_RATE                   (WMBUS_S_CHIP_RATE / 2)    //Manchester code

//Preamble (chips)
#define WMBUS_R_PREAMBLE_SYNC_WORD_FORMAT_A_LEN          96
#define WMBUS_R_SYNC_WORD_FORMAT_A                       0b000111011010010110   //0x7696
#define WMBUS_R_SYNC_WORD_FORMAT_A_LEN_BIT               18
#define WMBUS_R_PREAMBLE_FORMAT_A_01_COUNT               39

//Not found Mode R with the preamble and sync word of  Format B

#define WMBUS_R_POSTAMBLE_MIN                   2
#define WMBUS_R_POSTAMBLE_MAX                   8


// ---------------------- Mode N Parameters -----------------------
// Frequency definitions (Hz)
#define WMBUS_N_FREQ_169MHZ_A_MIN               169400000
#define WMBUS_N_FREQ_169MHZ_A_TYP               169437500
#define WMBUS_N_FREQ_169MHZ_A_MAX               169475000
#define WMBUS_N_FREQ_169MHZ_B_MIN               169475000
#define WMBUS_N_FREQ_169MHZ_B_TYP               169481250
#define WMBUS_N_FREQ_169MHZ_B_MAX               169487500
#define WMBUS_N_FREQ_169MHZ_C_MIN               169487500
#define WMBUS_N_FREQ_169MHZ_C_TYP               169537500
#define WMBUS_N_FREQ_169MHZ_C_MAX               169587500
#define WMBUS_N_FREQ_169MHZ_D_MIN               169587500
#define WMBUS_N_FREQ_169MHZ_D_TYP               169700000
#define WMBUS_N_FREQ_169MHZ_D_MAX               169812500

// Bitrate (bps)
#define WMBUS_N_BITRATE_2K4                     2400
#define WMBUS_N_BITRATE_4K8                     4800
#define WMBUS_N_BITRATE_6K4                     6400
#define WMBUS_N_BITRATE_19K2                    19200

// Frequency tolerance (Hz)
#define WMBUS_N_FREQ_TOLERANCE                  2000

// Bit/symbol rate tolerance (ppm)
#define WMBUS_N_SYMBOL_RATE_TOLERANCE_PPM       100

// Channel spacing (Hz)
#define WMBUS_N_CHANNEL_SPACING_12K5            12500
#define WMBUS_N_CHANNEL_SPACING_25K             25000
#define WMBUS_N_CHANNEL_SPACING_50K             50000

// Transmitted power (mW)
#define WMBUS_N_TX_PWR_500_MW                   500
#define WMBUS_N_TX_PWR_10_MW                    10

//Transmitted duty cycle (%)
#define WMBUS_N_TX_DUTY_CYCLE_10_PCT_PPM        100000  //10%
#define WMBUS_N_TX_DUTY_CYCLE_01_PCT_PPM        1000  //0.1%
#define WMBUS_N_TX_DUTY_CYCLE_0001_PCT_PPM      10  //0.001%


// GFSK Deviation (Hz)
#define WMBUS_N_GFSK_DEV_2K4_MIN                1680
#define WMBUS_N_GFSK_DEV_2K4_TYP                2400
#define WMBUS_N_GFSK_DEV_2K4_MAX                3120

#define WMBUS_N_GFSK_DEV_4K8_MIN                1680
#define WMBUS_N_GFSK_DEV_4K8_TYP                2400
#define WMBUS_N_GFSK_DEV_4K8_MAX                3120


// 4GFSK Deviation (Hz)
#define WMBUS_N_GFSK_DEV_6K4_MIN                2240
#define WMBUS_N_GFSK_DEV_6K4_TYP1               3200
#define WMBUS_N_GFSK_DEV_6K4_TYP2               1060
#define WMBUS_N_GFSK_DEV_6K4_MAX                4160

#define WMBUS_N_GFSK_DEV_19K2_MIN               5040
#define WMBUS_N_GFSK_DEV_19K2_TYP1              7200
#define WMBUS_N_GFSK_DEV_19K2_TYP2              2400
#define WMBUS_N_GFSK_DEV_19K2_MAX               9360

//Preamble and sync (chips)
#define WMBUS_N_PREAMBLE_LEN_MIN                16
#define WMBUS_N_PREAMBLE_LEN_MAX                16
#define WMBUS_N_SYNC_MIN                        16
#define WMBUS_N_SYNC_MAX                        16

#define WMBUS_N_SYNC_WORD_FORMAT_A              0b1111011010001101  //0xF68D
#define WMBUS_N_SYNC_WORD_FORMAT_B              0b1111011001110010  //0xF672
#define WMBUS_N_SYNC_WORD_LEN_BIT               16
#define WMBUS_N_PREAMBLE_01_COUNT               8

#define WMBUS_N_SYNC_WORD_4GFSK_FORMAT_A        0b11111111011111011101010111110111  //0xFF7DD5F7
#define WMBUS_N_SYNC_WORD_4GFSK_FORMAT_B        0b11111111011111010111111101011101  //0xFF7D7F5D
#define WMBUS_N_SYNC_WORD_4GFSK_LEN_BIT         32
#define WMBUS_N_PREAMBLE_0111_4GFSK_COUNT       8

#define WMBUS_N_CHANNEL_COUNT                   13


// ---------------------- Mode F Parameters -----------------------
// Frequency Parameters (Hz)
#define WMBUS_F_M2O_FREQ_MIN                    433813000
#define WMBUS_F_M2O_FREQ_TYP                    433820000
#define WMBUS_F_M2O_FREQ_MAX                    433827000
#define WMBUS_F_M2O_FREQ_TOLERANCE              16
#define WMBUS_F_O2M_FREQ_MIN                    433813000
#define WMBUS_F_O2M_FREQ_TYP                    433820000
#define WMBUS_F_O2M_FREQ_MAX                    433827000
#define WMBUS_F_O2M_FREQ_TOLERANCE              16


// FSK Deviation (Hz)
#define WMBUS_F_M2O_FSK_DEV_MIN                 4800
#define WMBUS_F_M2O_FSK_DEV_TYP                 5500
#define WMBUS_F_M2O_FSK_DEV_MAX                 7000
#define WMBUS_F_O2M_FSK_DEV_MIN                 4800
#define WMBUS_F_O2M_FSK_DEV_TYP                 5500
#define WMBUS_F_O2M_FSK_DEV_MAX                 7000

// Chip Rate Transmit (cps)
#define WMBUS_F_M2O_CHIP_RATE                   2400
#define WMBUS_F_O2M_CHIP_RATE                   2400

//Tolerance (ppm)
#define WMBUS_F_CHIP_RATE_TOLERANCE_PCT_PPM     100

// Data Rate (bps)
#define WMBUS_F_M2O_DATA_RATE                   (WMBUS_F_M2O_CHIP_RATE)    //NRZ code
#define WMBUS_F_O2M_DATA_RATE                   (WMBUS_F_O2M_CHIP_RATE)    //NRZ code

//Preamble (chips)
#define WMBUS_F_PREAMBLE_SYNC_WORD_LEN          94
#define WMBUS_F_SYNC_WORD_FORMAT_A              0b1111011010001101   //0xF68D
#define WMBUS_F_SYNC_WORD_FORMAT_B              0b1111011001110010   //0xF672
#define WMBUS_F_SYNC_WORD_LEN_BIT               16
#define WMBUS_F_PREAMBLE_01_COUNT               39

#define WMBUS_F_POSTAMBLE_MIN                   0
#define WMBUS_F_POSTAMBLE_MAX                   0


#define MBUS_MODE_FLAG_O2M                      (1 << 31)
#define MBUS_MODE_INDEX(mode)                   (mode & 0xff)

/*
 * Maximum packed-frame lengths for a 256-byte DLL frame:
 *
 * Frame A:
 *   - First block:       10 data bytes + 2 CRC bytes
 *   - Following blocks: up to 16 data bytes + 2 CRC bytes each
 *   - Remaining bytes:  256 - 10 = 246 = 15 * 16 + 6
 *   - Length:            12 + 15 * 18 + (6 + 2) = 290 bytes
 *
 * 3-of-6:
 *   - Each 8-bit byte becomes 12 encoded bits
 *   - Length: ceil(290 * 12 / 8) = ceil(290 * 3 / 2) = 435 bytes
 *
 * Manchester:
 *   - Each input bit becomes two encoded bits
 *   - Length: 290 * 2 = 580 bytes
 */
#define WMBUS_PARAM_NA                          0
#define WMBUS_MAX_DLL_FRAME_LEN                 256
#define WMBUS_MAX_FRAME_A_LEN                   290
#define WMBUS_MAX_3OF6_FRAME_LEN                435
#define WMBUS_MAX_MANCHESTER_FRAME_LEN          580
#define WMBUS_MAX_PKT_LEN                       WMBUS_MAX_DLL_FRAME_LEN
#define WMBUS_MAX_PACKED_FRAME_LEN              WMBUS_MAX_MANCHESTER_FRAME_LEN

/* Customer data is queued before DLL construction and RF packing. */
#define WMBUS_CONNECTION_DATA_BUFFER_SIZE       WMBUS_MAX_DLL_FRAME_LEN

/* Per-meter GW pre-encryption buffers contain complete packed RF frames. */
#define WMBUS_GW_PREENC_PACKED_BUFFER_SIZE       WMBUS_MAX_PACKED_FRAME_LEN

typedef enum {
    WMBUS_SUBBAND_A = 0,
    WMBUS_SUBBAND_B,
    WMBUS_SUBBAND_C,
    WMBUS_SUBBAND_D,
    WMBUS_SUBBAND_NUM    
} wmbus_mode_n_subband_t;

//Note:
//Beacuse the WMBUS_F_PREAMBLE_SYNC_WORD_LEN is 94 bits, so the last byte  is not completed byte
//Therefore, the output will be 0x55 ... 55 57 D9 C8
//the last four bytes: 01010101  01010111  11011001  11001000

enum
{
    WMBUS_MOD_FSK = 0,
    WMBUS_MOD_GFSK,
};

typedef struct
{
    uint8_t codec;
    uint32_t freq_typ_hz;
    uint32_t fsk_deviation_typ_hz;
    uint8_t data_rate;
    uint8_t modulation;
    uint32_t syncword;
    uint8_t syncword_len;
    uint8_t preamble;
    uint8_t preamble_len;
} wmbus_phy_param_t;

//Mode S,T,C,R
typedef struct {
    uint32_t mode;
    
    wmbus_phy_param_t m2o;
    wmbus_phy_param_t o2m;
} wmbus_mode_param_t;

typedef struct {
    wmbus_mode_n_subband_t subband;
    uint8_t index;
    uint32_t base_frequency_hz;
    uint32_t channel_spacing_hz;
    uint32_t gfsk_bps;
    uint32_t four_gfsk_bps;
    uint32_t freq_tolerance_hz;
    uint8_t channel_range;
    uint32_t tx_power_mw;
    uint32_t tx_duty_cycle;
} wmbus_mode_n_channel_index_info_t;

typedef struct {
    uint32_t data_rate_bps;
    uint32_t deviation_hz_min;
    uint32_t deviation_hz_typ;
    uint32_t deviation_hz_max;
    uint8_t gfsk_relative_bw_0_5_0;
    uint32_t bit_symbol_tolerance_ppm;
    uint8_t preamble_length_bits;
    uint8_t sync_length_bits;
    uint8_t postamble_length_bits;
} wmbus_mode_n_modulation_t;

extern const wmbus_mode_param_t radioWmbusCfg[];

#endif
