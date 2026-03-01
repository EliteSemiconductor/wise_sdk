/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

/**
 * @file wise_radio_wmbus_api.h
 * @brief Wireless M-Bus (WMBus) radio PHY control APIs.
 *
 * @ingroup WISE_RADIO_WMBUS
 *
 * This header provides the interface for controlling the Wireless M-Bus
 * radio PHY on ESMT ER81xx SoC family devices.
 */

#ifndef __WSIE_RADIO_WMBUS_API_H_
#define __WSIE_RADIO_WMBUS_API_H_

#include <stdint.h>
#include "types.h"
#include "wmbus_parameters.h"
#include "wise_radio_api.h"

/**
 * @defgroup WISE_RADIO_WMBUS_APIs WISE Radio WMbus APIs
 * @ingroup WISE_CORE_API
 * @brief Wireless M-Bus radio PHY control (Core API) 
 *
 * This module provides APIs to initialize and configure the radio PHY for
 * Wireless M-Bus operation, control RX/TX, and manage interface lifecycle.
 * @{
 */

/**
 * @enum wmbus_role_t
 * @brief Wireless M-Bus device role selection.
 *
 */
typedef enum
{
    WMBUS_ROLE_OTHER = 0,           /**< Configure device role as "other". */
    WMBUS_ROLE_METER = 1            /**< Configure device role as "meter". */
} wmbus_role_t;

/**
 * @enum wmbus_mode_t
 * @brief Wireless M-Bus PHY mode selection.
 *
 * Notes:
 * - S1/S1m/S2 correspond to S-mode variants.
 * - T1/T2 correspond to T-mode variants.
 * - C1/C2 correspond to C-mode variants.
 * - Some modes are marked as not supported yet.
 */
typedef enum {
    WMBUS_MODE_S1  = 0, /**< S-mode, unidirectional. */
    WMBUS_MODE_S1M = 1, /**< S-mode (S1m), unidirectional. */
    WMBUS_MODE_S2  = 2, /**< S-mode, bidirectional. */
    WMBUS_MODE_T1  = 3, /**< T-mode, unidirectional. */
    WMBUS_MODE_T2  = 4, /**< T-mode, bidirectional. */
    WMBUS_MODE_C1  = 5, /**< C-mode, unidirectional. */
    WMBUS_MODE_C2  = 6, /**< C-mode, bidirectional. */
    WMBUS_MODE_R2  = 7, /**< R-mode, bidirectional (not supported yet). */
    WMBUS_MODE_F2  = 8, /**< F-mode, bidirectional (not supported yet). */
    WMBUS_MODE_N   = 9  /**< N-mode (not supported yet). */
} wmbus_mode_t;

/**
 * @enum wmbus_frame_type_t
 * @brief Wireless M-Bus frame format.
 */
typedef enum
{
    WMBUS_FRAME_TYPE_A = 0,  /**< Frame format A. */
    WMBUS_FRAME_TYPE_B,      /**< Frame format B. */
} wmbus_frame_type_t;

/* ------------------------------------------------------------------------- */
/*                               API Functions                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initialize a radio interface instance for Wireless M-Bus mode.
 *
 * Allocates and initializes resources associated with the specified radio
 * interface. This function must be invoked before using any other WMBus API
 * on the same interface.
 *
 * @param[in] intf_idx Radio interface index.
 *
 * @retval 0   Initialization completed successfully.
 * @retval <0  Initialization failed.
 */
int8_t wise_radio_wmbus_init(int8_t intf_idx);

/**
 * @brief Configure the radio PHY for a specific Wireless M-Bus role and mode.
 *
 * @param[in] intf_idx  Radio interface index.
 * @param[in] mbusRole  Wireless M-Bus role selection, see ::wmbus_role_t.
 * @param[in] mbusMode  Wireless M-Bus PHY mode selection, see ::wmbus_mode_t.
 *
 * @retval 0   Configuration completed successfully.
 * @retval <0  Configuration failed.
 */
int8_t wise_radio_wmbus_set_mode(int8_t intf_idx, wmbus_role_t mbusRole, wmbus_mode_t mbusMode);

/**
 * @brief Configure maximum radio frame length for Wireless M-Bus TX/RX.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] maxLen   Maximum frame length in bytes (default = 256).
 *
 * @retval 0   Configuration completed successfully.
 * @retval <0  Configuration failed.
 */
int8_t wise_radio_wmbus_set_max_frame_len(int8_t intf_idx, uint16_t maxLen);

/**
 * @brief Start reception on the configured Wireless M-Bus radio PHY.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] rx_mode  RX mode selection (continuous or one-shot),
 *                     see ::WISE_RADIO_RX_MODE_T.
 *
 * @retval 0   RX started successfully.
 * @retval <0  RX start failed.
 */
int8_t wise_radio_mbus_rx_start(int8_t intf_idx, WISE_RADIO_RX_MODE_T rx_mode);

/**
 * @brief Stop reception on the configured Wireless M-Bus radio PHY.
 *
 * @param[in] intf_idx Radio interface index.
 */
void wise_radio_wmbus_rx_stop(int8_t intf_idx);

/**
 * @brief Transmit a frame on the configured Wireless M-Bus radio PHY.
 *
 * Queues a frame for transmission.
 *
 * @param[in] intf_idx Radio interface index.
 * @param[in] pframe   Pointer to frame payload.
 * @param[in] length   Frame length in bytes.
 *
 * @retval 0   Transmission accepted.
 * @retval <0  Transmission rejected or configuration error.
 */
int8_t wise_radio_wmbus_tx_frame(int8_t intf_idx, uint8_t *pframe, uint16_t length);

/**
 * @brief Deinitialize a Wireless M-Bus radio interface instance.
 *
 * Releases resources and returns the interface to an uninitialized state.
 *
 * @param[in] intf_idx Radio interface index.
 */
void wise_radio_wmbus_deinit(int8_t intf_idx);

/** @} */ /* end of WISE_RADIO_WMBUS_APIs group */


#endif  /* __WSIE_RADIO_WMBUS_API_H_ */

