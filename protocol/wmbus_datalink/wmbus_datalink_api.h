#ifndef __WMBUS_API_H__
#define __WMBUS_API_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file wmbus_api.h
 * @brief Wireless M-Bus link layer protocol API.
 *
 * @ingroup WISE_PROTOCOL
 *
 * This header defines the Wireless M-Bus link-layer interface, including:
 * - Role/mode configuration (Meter / Gateway; S/T/C/R/F/N modes)
 * - Security mode and encryption key provisioning
 * - Device identity configuration
 * - Payload transmission APIs
 * - Gateway admission control and whitelist management
 *
 * Typical usage:
 * 1. Call ::wmbus_link_init()
 * 2. Configure role/mode/security/device info
 * 3. (Optional) Register data callback via ::wmbus_link_register_data_cb()
 * 4. Call ::wmbus_link_start()
 * 5. Transmit payloads (meter/gateway APIs)
 * 6. Call ::wmbus_link_stop() when finished
 */

/**
 * @defgroup WMBUS_LINK_APIs WMBus Link APIs
 * @ingroup WISE_PROTOCOL
 * @brief Wireless M-Bus link layer protocol interface.
 *
 * The WMBus Link module provides link-layer control and payload transmission
 * services for Wireless M-Bus applications.
 *
 * @{
 */

/**
 * @typedef wmbus_link_data_cb_t
 * @brief Data indication callback type.
 *
 * The implementation invokes this callback when link-layer data is delivered
 * to the application.
 *
 * @param[in] owner_id Indicate the owner of the payload
 * @param[in] addr_p Pointer to a source address field (format is implementation-defined).
 * @param[in] len    Length of the address field in bytes.
 */
typedef void (*wmbus_link_data_cb_t)(uint32_t owner_id, const uint8_t *addr_p, uint8_t len);

/**
 * @enum WMBUS_LINK_role_t
 * @brief Link-layer operating role.
 */
typedef enum
{
    WMBUS_LINK_GW    = 0, /**< Gateway / collector role. */
    WMBUS_LINK_METER = 1  /**< Meter / end device role. */
} WMBUS_LINK_role_t;

/**
 * @enum WMBUS_LINK_security_mode_t
 * @brief Link-layer security mode selection.
 *
 * The actual security behavior depends on the Wireless M-Bus specification
 * and the stack implementation (e.g., Security Mode 0/5/7).
 */
typedef enum
{
    WMBUS_LINK_SM_0 = 0, /**< Security Mode 0 (no security). */
    WMBUS_LINK_SM_5 = 5, /**< Security Mode 5. */
    WMBUS_LINK_SM_7 = 7  /**< Security Mode 7. */
} WMBUS_LINK_security_mode_t;

/**
 * @enum WMBUS_LINK_mode_t
 * @brief Wireless M-Bus operating mode selection.
 *
 * Mode naming follows common Wireless M-Bus convention:
 * S1/S1m/S2/T1/T2/C1/C2/R2/F2/N.
 *
 * @note Supported modes depend on platform capability and build configuration.
 */
typedef enum
{
    WMBUS_LINK_MODE_S1  = 0,
    WMBUS_LINK_MODE_S1M = 1,
    WMBUS_LINK_MODE_S2  = 2,
    WMBUS_LINK_MODE_T1  = 3,
    WMBUS_LINK_MODE_T2  = 4,
    WMBUS_LINK_MODE_C1  = 5,
    WMBUS_LINK_MODE_C2  = 6,
    WMBUS_LINK_MODE_R2  = 7,
    WMBUS_LINK_MODE_F2  = 8,
    WMBUS_LINK_MODE_N   = 9
} WMBUS_LINK_mode_t;

typedef struct
{
    uint8_t major;
    uint8_t minor;
} WMBUS_LINK_VER_T;

/**
 * @struct WMBUS_LINK_device_info_t
 * @brief Device identity information used by the link layer.
 *
 * @note The @ref manufacturer field is a 3-byte code and is NOT a
 *       null-terminated C string.
 */
typedef struct
{
    uint32_t id;          /**< Device identifier (e.g., meter ID). */
    char manufacturer[3]; /**< 3-byte manufacturer code (no trailing '\0'). */
    uint8_t version;      /**< Device version. */
    uint8_t devType;      /**< Device type. */
} WMBUS_LINK_device_info_t;

/**
 * @brief Initialize the WMBus link layer module.
 *
 * Initializes internal state and prepares the link layer for configuration.
 * This function should be called once before other WMBus Link APIs.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_init(void);

void wmbus_link_get_version_info(WMBUS_LINK_VER_T *verInfo);

/**
 * @brief Set the operating role (meter or gateway).
 *
 * @param[in] role Role to set.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_set_role(WMBUS_LINK_role_t role);

/**
 * @brief Set the Wireless M-Bus operating mode.
 *
 * @param[in] mode Mode to set.
 *
 * @retval 0   Success.
 * @retval <0  Failure (e.g., invalid parameter or unsupported configuration).
 */
int32_t wmbus_link_set_mode(WMBUS_LINK_mode_t mode);

/**
 * @brief Configure local device identity information.
 *
 * @param[in] devInfo_p Pointer to the device info structure.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 *
 * @note The implementation may copy the contents or keep the pointer.
 *       Unless specified otherwise, keep @p devInfo_p valid after this call.
 */
int32_t wmbus_link_set_device_info(WMBUS_LINK_device_info_t *devInfo_p);

/**
 * @brief Select link-layer security mode.
 *
 * @param[in] security_mode Security mode to set.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_set_security_mode(WMBUS_LINK_security_mode_t security_mode);

/**
 * @brief Set the encryption key used by the link layer.
 *
 * @param[in] newKey Pointer to key material.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 *
 * @note Key length is implementation-defined (commonly 16 bytes for AES-128).
 *       The key buffer must be accessible when this API is called. Whether the
 *       key is copied internally is implementation-defined.
 */
int32_t wmbus_link_set_encrypt_key(const uint8_t *newKey);

/**
 * @brief Register the data indication callback.
 *
 * @param[in] cb Callback function pointer. Pass NULL to unregister.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_register_data_cb(wmbus_link_data_cb_t cb);

/**
 * @brief Start link layer operation.
 *
 * Begins link processing based on the configured role/mode/security settings.
 * Call this after configuration APIs.
 */
void wmbus_link_start(void);

/**
 * @brief Stop link layer operation.
 *
 * Stops link processing and returns the module to an idle state.
 */
void wmbus_link_stop(void);

/* -------------------------------------------------------------------------- */
/* Meter APIs                                                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Meter: queue a payload for transmission.
 *
 * @param[in] payload Pointer to payload bytes.
 * @param[in] len     Payload length in bytes.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 *
 * @note Intended for meter role usage. If called in gateway role, the behavior
 *       depends on implementation.
 */
int32_t wmbus_link_meter_write_payload(uint8_t *payload, uint16_t len);

/**
 * @brief Meter: trigger FSM to start new session.
 *
 * @Application maybe trigger new session any time or periodically
 */
void wmbus_link_meter_start_session(void);

/* -------------------------------------------------------------------------- */
/* Gateway (GW) APIs                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Gateway: queue a payload to be transmitted to a specific target device.
 *
 * @param[in] targetID Target device ID.
 * @param[in] payload  Pointer to payload bytes.
 * @param[in] len      Payload length in bytes.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 *
 * @note Intended for gateway role usage.
 */
int32_t wmbus_link_gw_write_payload(uint32_t targetID, uint8_t *payload, uint16_t len);

/**
 * @brief Enable or disable gateway admission control.
 *
 * When admission control is enabled, the gateway may restrict device access
 * (e.g., only allow devices in the whitelist).
 *
 * @param[in] enable true to enable admission control; false to disable.
 */
void wmbus_link_gw_set_admission_ctrl_enabled(bool enable);

/**
 * @brief Add a device to the gateway whitelist.
 *
 * @param[in] devInfo_p Pointer to device info to be added.
 *
 * @note Duplicate handling (replace/ignore) is implementation-defined.
 */
void wmbus_link_gw_whitelist_add_device(WMBUS_LINK_device_info_t *devInfo_p);

/**
 * @brief Remove a device from the gateway whitelist by device ID.
 *
 * @param[in] device_id Target device ID to remove.
 *
 * @retval true  Device was found and removed.
 * @retval false Device was not found (no change).
 */
bool wmbus_link_gw_whitelist_del_device(uint32_t device_id);

/** @} */ /* end of WMBUS_LINK_APIs */

#ifdef __cplusplus
}
#endif

#endif /* __WMBUS_API_H__ */
