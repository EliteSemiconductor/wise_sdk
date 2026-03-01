/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef _WISE_NFC_API_H_
#define _WISE_NFC_API_H_

/**
 * @file wise_nfc_api.h
 * @brief NFC (Near-Field Communication) driver APIs.
 *
 * @ingroup WISE_NFC
 *
 * This header provides the interface for configuring and controlling the NFC
 * block, including memory work/lock modes, data block access, DPE information,
 * interrupt callback registration, host lock control, and power source selection.
 */

#include "wise_core.h"
//#include "cmsis/include/er8xxx.h"
#include "types.h"
#include "esmt_chip_specific.h"
#include "hal_intf_pmu.h"
#include "hal_intf_nfc.h"

/**
 * @defgroup WISE_NFC WISE NFC APIs
 * @ingroup WISE_CORE_API
 * @brief NFC configuration, memory access, interrupt and power control.
 * @{
 */

/**
 * @enum NFC_WORK_MODE_E
 * @brief NFC memory working mode.
 *
 * The exact behavior of each mode is device-specific, but typically controls
 * whether NFC memory is fully accessible or in a limited / default state.
 */
typedef enum {
    NFC_WORK_MDOE_DEFAULT,  /**< Default work mode. */
    NFC_WORK_MDOE_FULL,     /**< Full-access work mode. */
} NFC_WORK_MODE_E;

/**
 * @enum NFC_MEM_LOCK_MODE_E
 * @brief NFC memory lock mode.
 *
 * Controls how the NFC memory lock mechanism behaves.
 */
typedef enum {
    NFC_MEM_LOCK_MODE_1,    /**< Memory lock mode 1. */
    NFC_MEM_LOCL_MODE_2,    /**< Memory lock mode 2. */
} NFC_MEM_LOCK_MODE_E;

/**
 * @enum NFC_INT_IDX_E
 * @brief NFC interrupt index selection.
 *
 * Used to select a specific NFC interrupt source/index.
 */
typedef enum {
    NFC_INT_IDX_0,          /**< NFC interrupt index 0. */
    NFC_INT_IDX_1,          /**< NFC interrupt index 1. */
} NFC_INT_IDX_E;

/**
 * @struct WISE_NFC_CFG_T
 * @brief NFC configuration structure.
 *
 * Used by ::wise_nfc_config() to set initial NFC work mode, memory lock mode
 * and interrupt index.
 */
typedef struct {
    NFC_WORK_MODE_E work_mode;      /**< NFC work mode. */
    NFC_MEM_LOCK_MODE_E lock_mode;  /**< NFC memory lock mode. */
    NFC_INT_IDX_E int_idx;          /**< NFC interrupt index. */
} WISE_NFC_CFG_T;

/* ==== API functions ====================================================== */

/**
 * @brief Initialize NFC subsystem.
 *
 * Must be called before using any other NFC APIs.
 */
void wise_nfc_init(void);

/**
 * @brief Deinitialize NFC subsystem.
 *
 * Releases NFC resources and returns the block to an uninitialized state.
 */
void wise_nfc_deinit(void);

/**
 * @brief Configure NFC parameters.
 *
 * Applies the configuration provided by @p cfg, including work mode, memory
 * lock mode and interrupt index.
 *
 * @param[in] cfg Pointer to a ::WISE_NFC_CFG_T structure.
 */
void wise_nfc_config(WISE_NFC_CFG_T *cfg);

/**
 * @brief Get current NFC memory work mode.
 *
 * @return Current work mode value, one of ::NFC_WORK_MODE_E.
 */
uint8_t wise_nfc_get_mem_work_mode(void);

/**
 * @brief Set NFC memory work mode.
 *
 * @param[in] mode Work mode selection, see ::NFC_WORK_MODE_E.
 */
void wise_nfc_set_mem_work_mode(NFC_WORK_MODE_E mode);

/**
 * @brief Get current NFC memory lock mode.
 *
 * @return Current lock mode value, one of ::NFC_MEM_LOCK_MODE_E.
 */
uint8_t wise_nfc_get_mem_lock_mode(void);

/**
 * @brief Set NFC memory lock mode.
 *
 * @param[in] mode Lock mode selection, see ::NFC_MEM_LOCK_MODE_E.
 */
void wise_nfc_set_mem_lock_mode(NFC_MEM_LOCK_MODE_E mode);

/**
 * @brief Check if NFC memory is locked.
 *
 * @retval 0 Memory is not locked.
 * @retval 1 Memory is locked.
 */
uint8_t wise_nfc_is_mem_locked(void);

/**
 * @brief Read data from NFC memory block.
 *
 * Reads @p block_len words starting from @p block_idx into @p rx_data_buff.
 * Word size is implementation-specific (usually 32 bits).
 *
 * @param[in]  block_idx     Block index to read from.
 * @param[in]  block_len     Number of blocks/words to read.
 * @param[out] rx_data_buff  Pointer to receive buffer for read data.
 */
void wise_nfc_block_read_data(uint8_t block_idx,
                              uint8_t block_len,
                              uint32_t *rx_data_buff);

/**
 * @brief Write data to NFC memory block.
 *
 * Writes @p block_len words from @p tx_data_buff to NFC memory starting at
 * @p block_idx.
 *
 * @param[in] block_idx    Block index to write to.
 * @param[in] block_len    Number of blocks/words to write.
 * @param[in] tx_data_buff Pointer to transmit data buffer.
 */
void wise_nfc_block_write_data(uint8_t block_idx,
                               uint8_t block_len,
                               uint32_t *tx_data_buff);

/**
 * @brief Get DPE (Digital Power Engine) status information.
 *
 * @return DPE status value (device-specific bitfield).
 */
uint8_t wise_nfc_get_dpe_status_info(void);

/**
 * @brief Get DPE (Digital Power Engine) control information.
 *
 * @return DPE control register value (device-specific bitfield).
 */
uint8_t wise_nfc_get_dpe_ctrl_info(void);

/**
 * @brief Register an NFC interrupt callback.
 *
 * The callback type @c CALLBACK_T is defined in the underlying HAL.
 *
 * @param[in] int_idx NFC interrupt index, see ::NFC_INT_IDX_E.
 * @param[in] cb      Callback function pointer.
 * @param[in] context User context pointer passed to the callback.
 */
void wise_nfc_register_int_callback(NFC_INT_IDX_E int_idx,
                                    CALLBACK_T cb,
                                    void *context);

/**
 * @brief Unregister previously registered NFC interrupt callback.
 *
 * @param[in] int_idx NFC interrupt index, see ::NFC_INT_IDX_E.
 */
void wise_nfc_unregister_int_callback(NFC_INT_IDX_E int_idx);

/**
 * @brief Set host lock status.
 *
 * Controls whether NFC-related host access is locked or unlocked.
 *
 * @param[in] enable Non-zero to enable host lock, zero to disable.
 */
void wise_nfc_set_host_locked(uint8_t enable);

/**
 * @brief Get current NFC interrupt index.
 *
 * @return Current interrupt index, one of ::NFC_INT_IDX_E.
 */
uint8_t wise_nfc_get_interrupt_idx(void);

/**
 * @brief Set NFC interrupt index.
 *
 * @param[in] int_idx Interrupt index to be used, see ::NFC_INT_IDX_E.
 */
void wise_nfc_set_interrupt_idx(NFC_INT_IDX_E int_idx);

/**
 * @brief Switch NFC power source.
 *
 * Selects the power source index used by the NFC block. The mapping from
 * index value to actual source is device-specific.
 *
 * @param[in] src Power source index.
 */
void wise_nfc_switch_pwr_src(uint8_t src);

/**
 * @brief Get NFC power source index.
 *
 * @return Current power source index.
 */
uint8_t wise_nfc_get_pwr_src_idx(void);

void wise_nfc_set_wakeup_config(uint8_t pwr_mode);


/** @} */ /* end of WISE_NFC group */

#endif /* _WISE_PMU_API_H_ */
