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

#define CHIP_UNIQUE_LEN                 8   /**< Length of the unique device ID (bytes). */

#define SYS_DMA_CHANNEL_NUM             6   /**< Total number of DMA channels. */

#ifndef ASARADC_MAX_POINTS
#define ASARADC_MAX_POINTS 6
#endif

/**
 * @enum SYS_DMA_FUNC_MAP
 * @brief DMA function mapping values for each peripheral.
 *
 * These enumerations map a DMA channel to a specific peripheral direction.
 */
enum {
    SYS_DMA_FUNC_UNSED  = 0,
    SYS_DMA_FUNC_AES_IN = 3,
    SYS_DMA_FUNC_AES_OUT,
    SYS_DMA_FUNC_AES_AUTH_IN,
    SYS_DMA_FUNC_SHA,
    SYS_DMA_FUNC_SPI0_TX,
    SYS_DMA_FUNC_SPI0_RX,
    SYS_DMA_FUNC_SPI1_TX,
    SYS_DMA_FUNC_SPI1_RX,
    SYS_DMA_FUNC_UART0_TX,
    SYS_DMA_FUNC_UART0_RX,
    SYS_DMA_FUNC_UART1_TX,
    SYS_DMA_FUNC_UART1_RX,
    SYS_DMA_FUNC_UART2_TX,
    SYS_DMA_FUNC_UART2_RX,
    SYS_DMA_FUNC_I2C0_TX,
    SYS_DMA_FUNC_I2C0_RX,
    SYS_DMA_FUNC_I2C1_TX,
    SYS_DMA_FUNC_I2C1_RX,
    SYS_DMA_FUNC_USER,
    SYS_DMA_FUNC_MAX
};

/**
 * @enum LFOSC_32K_MODE_T
 * @brief 32K LFOSC operational modes.
 */
typedef enum {
    LFOSC_32K_MODE_32K = 0,
    LFOSC_32K_MODE_16K,
    LFOSC_32K_MODE_8K,
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
    ASARADC_VREF_1P6V = 0,  /**< 1.6 V reference. */
    ASARADC_VREF_2P4V       /**< 2.4 V reference. */
} WISE_ASARADC_VREF;

typedef enum
{
    SHUTDOWN_WAKE_SRC_WUTMR = 0x01,
    SHUTDOWN_WAKE_SRC_NFC = 0x02,
    SHUTDOWN_WAKE_SRC_GPIO = 0x04,
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
    uint8_t tcxo_output_en;     /**< 0 = disable TCXO output, 1 = enable. */
    uint8_t pa_type;            /**< 0 = 10 dB PA, 1 = 14 dB PA. */
    uint8_t matching_type;      /**< 0 = 915 MHz, 1 = 868 MHz, 2 = 490 MHz. */
    uint8_t gain_ctrl_40m;      /**< Gain control level (1–8). */
    uint8_t gain_ctrl_40m_s;    /**< Gain control level (1–8) for sleep mode. */
    uint8_t cap_xtal_i;         /**< Internal XTAL capacitor setting (default = 64). */
    uint8_t cap_xtal_o;         /**< External XTAL capacitor setting (default = 64). */
    uint8_t sram_retain;        /**< SRAM retaintion mode, 0 = 32K, 1 = 64K */
} WISE_SYS_BOARD_PROPERTY_T;

typedef struct
{
    SHUTDOWN_WAKE_SRC_T wake_src;
    uint32_t shutdown_ms; //period in ms to exit shutdown if SHUTDOWN_WAKE_SRC_WUTMR is set
    uint8_t wake_io_idx; //gpio pin to exit shutdown if SHUTDOWN_WAKE_SRC_GPIO is set
} SYS_SHUTDOWN_CFT_T;

/**
 * @struct WISE_LFOSC_SRC_T
 * @brief LFOSC clock source configuration.
 */
typedef struct {
    uint8_t clk_src;                /**< Clock source selector (device-specific). */
    union {
        uint8_t mode_select;
        LFOSC_32K_MODE_T mode_32k;
        LFOSC_16K_MODE_T mode_16k;
    } mode;                         /**< Mode selection depends on @ref clk_src. */
    uint8_t calFinish;              /**< Calibration status flag. */
} WISE_LFOSC_SRC_T;

/* ------------------------------------------------------------------------- */
/*                            System Core APIs                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize the system.
 *
 * Performs core system initialization including clocks, power, and subsystems.
 */
void wise_sys_init(void);

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
uint32_t wise_sys_get_chip_unique(void);

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
 * @param[in] wu_src      Wake-up source index.
 * @param[in] wu_time_ms  Wake-up time in milliseconds.
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
 */
uint8_t wise_sys_get_board_match_type(void);

void wise_sys_enable_bod(uint8_t bod_lv, uint8_t enable);


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

void wise_sys_enable_bod(uint8_t bod_lv, uint8_t enable);


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
WISE_STATUS wise_asaradc_read_input(ASARADC_VIN_SEL_T vin_sel, uint16_t* rawValue);

/**
 * @brief Read high-resolution (27-bit) ASARADC value.
 *
 * @param[in]  vin_sel  Analog input select.
 * @param[out] rawValue Pointer to variable that receives 27-bit data.
 *
 * @retval WISE_SUCCESS Read successful.
 * @retval WISE_FAIL    Read failed.
 */
WISE_STATUS wise_asaradc_read_input_hires(ASARADC_VIN_SEL_T vin_sel, uint32_t* rawValue);

/** @} */ /* end of WISE_SYS group */


#endif /* __WISE_SYS_API_H */
