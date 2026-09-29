/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_sys_api.h
 * @brief System-level APIs for initialization, chip information, power control,
 *        DMA channel configuration, oscillator setup, and ASARADC access.
 *
 * @ingroup WISE_SYS
 *
 * This header exposes system-wide functionality such as reset control, chip ID
 * queries, sleep/shutdown management, DMA mapping, oscillator selection,
 * board-level RF properties, and ASARADC sensor read operations.
 */

#ifndef __WISE_SYS_API_H
#define __WISE_SYS_API_H

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "hal_intf_efuse.h"
#include "hal_intf_pmu.h"
#include "hal_intf_dma.h"
#include "radio_lib/hal_intf_radio.h"
#include "hal_intf_sys.h"
#include "hal_intf_extpmu.h"
#include "types.h"
#include <stdint.h>

/**
 * @defgroup WISE_SYS WISE System APIs
 * @ingroup WISE_CORE_API
 * @brief System initialization, power control, DMA configuration, LFOSC, and ADC.
 * @{
 */

#define CHIP_UNIQUE_LEN 8 /**< Length of the unique device ID (bytes). */

#define SYS_DMA_CHANNEL_NUM 6 /**< Total number of DMA channels. */

#ifndef ASARADC_MAX_POINTS
#define ASARADC_MAX_POINTS 6
#endif

#define RESET_INFO_SYS_REQ BIT0
#define RESET_INFO_WDOG_RST BIT1
#define RESET_INFO_LOCKUP_RST BIT2
#define RESET_INFO_BOD_RST BIT3
#define RESET_INFO_CHIP_RST BIT6
#define RESET_INFO_EXT_PMU_RST BIT7
#define WARM_RESET_RELEVANT_MASK                                                                                                                     \
    (RESET_INFO_SYS_REQ | RESET_INFO_WDOG_RST | RESET_INFO_LOCKUP_RST | RESET_INFO_BOD_RST | RESET_INFO_CHIP_RST | RESET_INFO_EXT_PMU_RST)

/**
 * @enum SYS_DMA_FUNC_MAP
 * @brief DMA function mapping values for each peripheral.
 *
 * These enumerations map a DMA channel to a specific peripheral direction.
 */
enum {
    SYS_DMA_FUNC_UNSED  = 0, /**< Unused DMA channel mapping. */
    SYS_DMA_FUNC_AES_IN = 3, /**< AES input channel. */
    SYS_DMA_FUNC_AES_OUT,    /**< AES output channel. */
    SYS_DMA_FUNC_AES_AUTH_IN,/**< AES authentication input channel. */
    SYS_DMA_FUNC_SHA,        /**< SHA hash engine input channel. */
    SYS_DMA_FUNC_SPI0_TX,    /**< SPI0 transmit channel. */
    SYS_DMA_FUNC_SPI0_RX,    /**< SPI0 receive channel. */
    SYS_DMA_FUNC_SPI1_TX,    /**< SPI1 transmit channel. */
    SYS_DMA_FUNC_SPI1_RX,    /**< SPI1 receive channel. */
    SYS_DMA_FUNC_UART0_TX,   /**< UART0 transmit channel. */
    SYS_DMA_FUNC_UART0_RX,   /**< UART0 receive channel. */
    SYS_DMA_FUNC_UART1_TX,   /**< UART1 transmit channel. */
    SYS_DMA_FUNC_UART1_RX,   /**< UART1 receive channel. */
    SYS_DMA_FUNC_UART2_TX,   /**< UART2 transmit channel. */
    SYS_DMA_FUNC_UART2_RX,   /**< UART2 receive channel. */
    SYS_DMA_FUNC_I2C0_TX,    /**< I2C0 transmit channel. */
    SYS_DMA_FUNC_I2C0_RX,    /**< I2C0 receive channel. */
    SYS_DMA_FUNC_I2C1_TX,    /**< I2C1 transmit channel. */
    SYS_DMA_FUNC_I2C1_RX,    /**< I2C1 receive channel. */
    SYS_DMA_FUNC_USER,       /**< User-defined DMA channel function. */
    SYS_DMA_FUNC_MAX         /**< Maximum number of DMA functions. */
};

/**
 * @enum LFOSC_32K_MODE_T
 * @brief 32K LFOSC operational modes.
 */
typedef enum {
    LFOSC_32K_MODE_32K = 0, /**< Standard 32 kHz mode. */
    LFOSC_32K_MODE_16K,     /**< 16 kHz derived mode. */
    LFOSC_32K_MODE_8K,      /**< 8 kHz derived mode. */
} LFOSC_32K_MODE_T;

/**
 * @enum LFOSC_16K_MODE_T
 * @brief 16K LFOSC operational modes.
 */
typedef enum {
    LFOSC_16K_MODE_TEMP_COMP = 0, /**< Temperature-compensated mode. */
    LFOSC_16K_MODE_32K,           /**< 32K derived mode. */
    LFOSC_16K_MODE_LOW_POWER,     /**< Low-power mode. */
    LFOSC_16K_MODE_LP_VOLT_0P6,   /**< 0.6 V low-power variant. */
} LFOSC_16K_MODE_T;

/**
 * @enum CACHE_SIZE_CFG_T
 * @brief Selectable XIP flash cache sizes.
 *
 * The cache is 2-way set associative with a 32-byte line. Each way is 4 KB, so
 * the size selection is really a way-enable selection.
 */
typedef enum {
    CACHE_SIZE_4K_BYTE = 0, /**< WAY0 only. */
    CACHE_SIZE_8K_BYTE = 1, /**< WAY0 and WAY1, the full cache. */
} CACHE_SIZE_CFG_T;

/**
 * @struct WISE_ASARADC_DATA_T
 * @brief ASARADC reading container (12-bit and high-resolution 27-bit data).
 */
typedef struct {
    uint16_t data_12bit; /**< 12-bit raw ADC data. */
    uint32_t data_27bit; /**< High-resolution 27-bit ADC data. */
} WISE_ASARADC_DATA_T;

/**
 * @enum WISE_ASARADC_VREF
 * @brief ASARADC voltage reference selection.
 */
typedef enum {
    ASARADC_VREF_1P6V = 0, /**< 1.6 V reference. */
    ASARADC_VREF_2P4V      /**< 2.4 V reference. */
} WISE_ASARADC_VREF;

/**
 * @enum WISE_SYS_ULPLDO_VREF_T
 * @brief ULPLDO reference voltage selection.
 */
typedef enum {
    WISE_SYS_ULPLDO_VREF_NORMAL = 0, /**< Default reference voltage. */
    WISE_SYS_ULPLDO_VREF_ULTRA_LOW,  /**< Ultra-low reference voltage. */
    WISE_SYS_ULPLDO_VREF_MAX,        /**< Maximum reference voltage selector (sentinel). */
} WISE_SYS_ULPLDO_VREF_T;

/**
 * @enum WISE_SYS_ULPLDO_ENMODE_T
 * @brief ULPLDO enable mode selection.
 */
typedef enum {
    WISE_SYS_ULPLDO_ENMODE_DISABLE = 0, /**< Disable ULPLDO enable mode override. */
    WISE_SYS_ULPLDO_ENMODE_ENABLE,      /**< Enable ULPLDO enable mode override. */
    WISE_SYS_ULPLDO_ENMODE_MAX,         /**< Maximum enable mode selector (sentinel). */
} WISE_SYS_ULPLDO_ENMODE_T;

/**
 * @enum SHUTDOWN_WAKE_SRC_T
 * @brief Wake-up source bitmask for shutdown mode.
 */
typedef enum {
    SHUTDOWN_WAKE_SRC_WUTMR = 0x01, /**< Wake up from wake-up timer (WUTMR). */
    SHUTDOWN_WAKE_SRC_NFC   = 0x02, /**< Wake up from NFC field detect. */
    SHUTDOWN_WAKE_SRC_GPIO  = 0x04, /**< Wake up from external GPIO pin. */
} SHUTDOWN_WAKE_SRC_T;

/**
 * @enum ASARADC_VIN_SEL_T
 * @brief ASARADC analog input source selection.
 */
typedef enum {
    ASARADC_VIN_EXTERNAL = 0,  /**< External analog input (ANAGPIO). */
    ASARADC_VIN_VREF09   = 11, /**< Internal 0.9 V reference (process drift monitor). */
    ASARADC_VIN_BATTERY  = 12, /**< Battery voltage (VOUT). */
    ASARADC_VIN_TEMP     = 14, /**< Temperature sensor (bandgap-related). */
} ASARADC_VIN_SEL_T;

/**
 * @struct WISE_SYS_BOARD_PROPERTY_T
 * @brief RF and XTAL board-level configuration properties.
 */
typedef struct {
    uint8_t tcxo_output_en;                 /**< 0 = disable TCXO output, 1 = enable. */
    uint8_t pa_type;                        /**< 0 = 10 dB PA, 1 = 14 dB PA. */
    uint8_t matching_type;                  /**< 0 = 915 MHz, 1 = 868 MHz, 2 = 490 MHz. */
    uint8_t gain_ctrl_40m;                  /**< Gain control level (1–8). */
    uint8_t gain_ctrl_40m_s;                /**< Gain control level (1–8) for sleep mode. */
    uint8_t cap_xtal_i;                     /**< Internal XTAL capacitor setting (default = 64). */
    uint8_t cap_xtal_o;                     /**< External XTAL capacitor setting (default = 64). */
    uint8_t maincap_xtal_i;                 /**< 40M XTAL MAINCAP_I_EN (0/1). */
    uint8_t maincap_xtal_o;                 /**< 40M XTAL MAINCAP_O_EN (0/1). */
    uint8_t cap_lpxtal_i;                   /**< ext32k LPXTAL CAP_I_CTRL (0-63). All-zero CAP tuple = keep factory default. */
    uint8_t cap_lpxtal_o;                   /**< ext32k LPXTAL CAP_O_CTRL (0-63). */
    uint8_t maincap_lpxtal_i;               /**< ext32k LPXTAL MAINCAP_I_EN (0/1). */
    uint8_t maincap_lpxtal_o;               /**< ext32k LPXTAL MAINCAP_O_EN (0/1). */
    uint8_t gain_lpxtal;                    /**< ext32k LPXTAL GAIN_CTRL (0-63). 0 = keep whatever analog stack set. */
    uint8_t sram_retain;                    /**< SRAM retaintion mode, 0 = 32K, 1 = 64K */
    uint8_t ext32k_lp_workaround;           /**< 0 = disable, 1 = enable EXT32K low-power sleep-current workaround. */
    WISE_SYS_ULPLDO_VREF_T ulpldo_vref;     /**< ULPLDO reference selection. */
    WISE_SYS_ULPLDO_ENMODE_T ulpldo_enmode; /**< ULPLDO enable mode selection. */
} WISE_SYS_BOARD_PROPERTY_T;

/**
 * @struct SYS_SHUTDOWN_CFT_T
 * @brief Shutdown mode configuration parameters.
 */
typedef struct {
    SHUTDOWN_WAKE_SRC_T wake_src;    /**< Wake-up source bitmask (see @ref SHUTDOWN_WAKE_SRC_T). */
    uint32_t            shutdown_ms; /**< Timeout in milliseconds to exit shutdown if SHUTDOWN_WAKE_SRC_WUTMR is set. */
    uint8_t             wake_io_idx; /**< GPIO pin index to exit shutdown if SHUTDOWN_WAKE_SRC_GPIO is set. */
} SYS_SHUTDOWN_CFT_T;

/**
 * @struct WISE_LFOSC_SRC_T
 * @brief LFOSC clock source configuration.
 */
typedef struct {
    uint8_t clk_src; /**< Clock source selector (device-specific). */
    union {
        uint8_t          mode_select; /**< Raw mode selector byte. */
        LFOSC_32K_MODE_T mode_32k;    /**< 32K LFOSC mode options. */
        LFOSC_16K_MODE_T mode_16k;    /**< 16K LFOSC mode options. */
    } mode;            /**< Mode selection depends on @ref clk_src. */
    uint8_t calFinish; /**< Calibration status flag. */
} WISE_LFOSC_SRC_T;

/* ------------------------------------------------------------------------- */
/*                            System Core APIs                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Perform a full system reset.
 */
void wise_sys_reset(void);

/**
 * @brief Retrieve the device's unique ID.
 *
 * @param[out] uniqueID  Pointer to an 8-byte buffer that receives the ID.
 *
 * @retval >=0  Length of unique ID returned.
 * @retval <0   Error occurred.
 */
int32_t wise_sys_get_chip_unique(uint8_t uniqueID[8]);

/**
 * @brief Remap system memory to another address region.
 *
 * @param[in] remap_addr New base address for remapping.
 */
void wise_sys_remap(uint32_t remap_addr);

/**
 * @brief Lock system resources to prevent concurrent modification.
 */
void wise_sys_lock(void);

/**
 * @brief Configure SWD (debug) interface availability.
 *
 * @param[in] enable  true = enable SWD pins, false = disable.
 */
void wise_sys_swd_config(bool enable);

/**
 * @brief Reset the chip using hardware reset logic.
 */
void wise_sys_chip_reset();

/**
 * @brief Put CPU into power-down mode.
 */
void wise_sys_set_cpu_pd(void);

/**
 * @brief Enter MCU sleep mode.
 */
void wise_sys_enter_sleep_mode(void);

/**
 * @brief Enter MCU shutdown mode.
 *
 * Configures wake-up sources and puts the system into deep shutdown mode.
 *
 * @param[in] shutdownCfg Shutdown mode configuration parameters.
 */
void wise_sys_enter_shutdown_mode(SYS_SHUTDOWN_CFT_T shutdownCfg);

/**
 * @brief Get chip ID value from hardware.
 *
 * @return Chip ID (device-specific format).
 */
uint32_t wise_sys_get_chip_id(void);

/**
 * @brief System background processing routine.
 *
 * Called periodically from main loop.
 */
void wise_sys_proc();

/* ------------------------------------------------------------------------- */
/*                               DMA Mapping                                 */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize DMA channels based on a function mapping table.
 *
 * @param[in] dma_func_map Pointer to an array of function IDs for each DMA channel.
 */
void wise_sys_init_dma_channel(const uint8_t *dma_func_map);

/**
 * @brief Export the current DMA channel configuration for debugging.
 */
void wise_sys_dma_channel_export(void);

/**
 * @brief Perform DMA-based memory copy.
 *
 * @param[out] dst        Destination pointer.
 * @param[in]  src        Source pointer.
 * @param[in]  byte_count Number of bytes to copy.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wise_dma_memcpy_bytes(void *dst, const void *src, uint32_t byte_count);

/* ------------------------------------------------------------------------- */
/*                         Board RF Property Config                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Apply RF board-matching and oscillator properties.
 *
 * @param[in] property Pointer to a ::WISE_SYS_BOARD_PROPERTY_T structure.
 */
void wise_sys_set_board_property(const WISE_SYS_BOARD_PROPERTY_T *property);

/**
 * @brief Enable or disable TCXO configuration.
 *
 * @param[in] enable  true = enable TCXO, false = disable.
 */
void wise_sys_tcxo_config(bool enable);

/**
 * @brief Set PA (power amplifier) type.
 *
 * @param[in] pa_type 0 = 10 dB, 1 = 14 dB.
 */
void wise_sys_set_pa_type(uint8_t pa_type);

/**
 * @brief Get current PA type.
 *
 * @return PA type value.
 */
uint8_t wise_sys_get_pa_type(void);

/**
 * @brief Set RF matching network type.
 *
 * @param[in] mat_type 0 = 915, 1 = 868, 2 = 490 MHz.
 */
void wise_sys_set_board_match_type(uint8_t mat_type);

/**
 * @brief Get RF matching network type.
 *
 * @return Board RF matching type (0 = 915 MHz, 1 = 868 MHz, 2 = 490 MHz).
 */
uint8_t wise_sys_get_board_match_type(void);

/**
 * @brief Dynamically set the 40M XTAL capacitor configuration and apply it to the analog block.
 *
 * @param[in] cap_i     XO_40M_CAP_I_CTRL trim (0-127).
 * @param[in] cap_o     XO_40M_CAP_O_CTRL trim (0-127).
 * @param[in] maincap_i XO_40M_MAINCAP_I_EN (0/1).
 * @param[in] maincap_o XO_40M_MAINCAP_O_EN (0/1).
 */
void wise_sys_set_xtal_cap(uint8_t cap_i, uint8_t cap_o, uint8_t maincap_i, uint8_t maincap_o);

/**
 * @brief Get the current 40M XTAL capacitor configuration.
 *
 * Any of the output pointers may be NULL.
 *
 * @param[out] cap_i     Receives XO_40M_CAP_I_CTRL trim.
 * @param[out] cap_o     Receives XO_40M_CAP_O_CTRL trim.
 * @param[out] maincap_i Receives XO_40M_MAINCAP_I_EN.
 * @param[out] maincap_o Receives XO_40M_MAINCAP_O_EN.
 */
void wise_sys_get_xtal_cap(uint8_t *cap_i, uint8_t *cap_o, uint8_t *maincap_i, uint8_t *maincap_o);

/**
 * @brief Dynamically set the 40M XTAL gain control and apply it to the analog block.
 *
 * @param[in] gain XO_40M_GAIN_CTRL level (1-8).
 */
void wise_sys_set_40m_gain_ctrl(uint8_t gain);

/**
 * @brief Get the current 40M XTAL gain control level.
 *
 * @return XO_40M_GAIN_CTRL level.
 */
uint8_t wise_sys_get_40m_gain_ctrl(void);

/**
 * @brief Configure deglitch function of BOD
 *
 * @param[in] dg_en         enable/disable deglitch function (0:disable, 1:enable)
 * @param[in] dg_period     configure deglitch period to (N+2) clock counter, N: 0-7 
 */
void wise_sys_config_bod_deglitch(uint8_t dg_en, uint8_t dg_period);

/**
 * @brief Enable/disable BOD reset function
 *
 * @param[in] bod_lv        configure trigger level of BOD threshold
 * @param[in] bod_en        enable/disable BOD reset function (0:disable, 1:enable)
 */
void wise_sys_enable_bod(uint8_t bod_lv, uint8_t bod_en);

/* ------------------------------------------------------------------------- */
/*                     LFOSC (Low Frequency Oscillator)                      */
/* ------------------------------------------------------------------------- */

/**
 * @brief Configure LFOSC clock source.
 *
 * @param[in] clk_cfg Structure specifying LFOSC mode and source.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wise_sys_lfosc_clk_src_config(WISE_LFOSC_SRC_T clk_cfg);

/**
 * @brief Get current LFOSC configuration.
 *
 * @param[out] clk_cfg Pointer to structure that receives current LFOSC config.
 */
void wise_sys_lfosc_clk_get_config(WISE_LFOSC_SRC_T *clk_cfg);

/**
 * @brief Perform LFOSC calibration procedure.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wise_sys_lfosc_clk_calibration();

/* ------------------------------------------------------------------------- */
/*                              ASARADC APIs                                 */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize ASARADC subsystem.
 *
 * @retval WISE_SUCCESS Initialization successful.
 * @retval WISE_FAIL    Initialization failed.
 */
WISE_STATUS wise_asaradc_init(void);

/**
 * @brief Configure ASARADC voltage reference.
 *
 * @param[in] vref Reference setting, see ::WISE_ASARADC_VREF.
 *
 * @retval WISE_SUCCESS Configuration successful.
 * @retval WISE_FAIL    Configuration failed.
 */
WISE_STATUS wise_asaradc_config(WISE_ASARADC_VREF vref);

/**
 * @brief Read 12-bit ASARADC value from a selected input.
 *
 * @param[in]  vin_sel  Analog input select, see ::ASARADC_VIN_SEL_T.
 * @param[out] rawValue Pointer to variable that receives the 12-bit result.
 *
 * @retval WISE_SUCCESS Read successful.
 * @retval WISE_FAIL    Read failed.
 */
WISE_STATUS wise_asaradc_read_input(ASARADC_VIN_SEL_T vin_sel, uint16_t *rawValue);

/**
 * @brief Read high-resolution (27-bit) ASARADC value.
 *
 * @param[in]  vin_sel  Analog input select.
 * @param[out] rawValue Pointer to variable that receives 27-bit data.
 *
 * @retval WISE_SUCCESS Read successful.
 * @retval WISE_FAIL    Read failed.
 */
WISE_STATUS wise_asaradc_read_input_hires(ASARADC_VIN_SEL_T vin_sel, uint32_t *rawValue);

/* ------------------------------------------------------------------------- */
/*                           Warm Reset Info                                 */
/* ------------------------------------------------------------------------- */

/**
 * @brief Get warm reset information.
 *
 * @return Warm reset info register value.
 */
uint32_t wise_sys_get_warm_reset_info(void);

/**
 * @brief Clear warm reset information flags.
 */
void wise_sys_clear_warm_reset_info(void);

/* ------------------------------------------------------------------------- */
/*                           system cache                                    */
/* ------------------------------------------------------------------------- */

/**
 * @brief Configure and enable the XIP flash cache.
 *
 * Selects how many ways are active (see ::CACHE_SIZE_CFG_T), enables the
 * hit/miss counters, and leaves the cache enabled on success.
 *
 * @note The cache contents are invalidated as part of this call, so the first
 *       accesses afterwards run cold.
 * @note Intended for the system initialisation phase. It is called once from
 *       wise_core_init(); calling it at run time discards a warm cache.
 *
 * @param[in] cache_size Cache size to apply.
 *
 * @retval WISE_SUCCESS Configuration applied.
 * @retval WISE_FAIL    Invalid size, or the invalidate handshake timed out.
 *                      The cache is left enabled either way.
 */
WISE_STATUS wise_sys_cache_config(CACHE_SIZE_CFG_T cache_size);

/**
 * @brief Discard the entire cache contents.
 *
 * The enable state in force before the call is restored afterwards.
 *
 * @retval WISE_SUCCESS Cache invalidated.
 * @retval WISE_FAIL    The invalidate handshake timed out.
 */
WISE_STATUS wise_sys_cache_invalidate(void);

/** @} */ /* end of WISE_SYS group */


#endif /* __WISE_SYS_API_H */
