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
    WMBUS_LINK_MODE_S2  = 1,
    WMBUS_LINK_MODE_T1  = 2,
    WMBUS_LINK_MODE_T2  = 3,
    WMBUS_LINK_MODE_C1  = 4,
    WMBUS_LINK_MODE_C2  = 5,
    WMBUS_LINK_MODE_R2  = 6,
    WMBUS_LINK_MODE_F2  = 7,
    WMBUS_LINK_MODE_N   = 8
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
    uint8_t reserved[3];
} WMBUS_LINK_device_info_t;

/**
 * @brief Result of applying the GW admission policy to an unknown meter.
 */
typedef enum
{
    /** Learned into writable whitelist storage. */
    WMBUS_LINK_GW_ADMISSION_ACCEPTED_ADDED = 0,
    /** Rejected by fixed/admission-controlled whitelist. */
    WMBUS_LINK_GW_ADMISSION_REJECTED_UNKNOWN,
    /** Writable whitelist capacity is exhausted. */
    WMBUS_LINK_GW_ADMISSION_REJECTED_TABLE_FULL,
    /** No writable whitelist storage is configured. */
    WMBUS_LINK_GW_ADMISSION_REJECTED_NO_WRITABLE_STORAGE,
} WMBUS_LINK_gw_admission_result_t;

/**
 * @brief Gateway admission-result callback type.
 *
 * @note @p meter_info is valid only for the duration of the callback. Copy it
 *       if the application needs to retain the information.
 */
typedef void (*wmbus_link_gw_admission_cb_t)(
    WMBUS_LINK_gw_admission_result_t result,
    const WMBUS_LINK_device_info_t *meter_info);

/**
 * @brief Reason that an established GW connection was removed.
 */
typedef enum
{
    /** Connection table was full and the oldest connection was removed. */
    WMBUS_LINK_GW_CONNECTION_FAILURE_EVICTED = 0,
    /** Access-number mismatch exceeded the retry limit. */
    WMBUS_LINK_GW_CONNECTION_FAILURE_ACCESS_RETRY_LIMIT,
} WMBUS_LINK_gw_connection_failure_t;

/**
 * @brief GW connection state captured immediately before removal.
 */
typedef struct
{
    uint32_t device_id;                  /**< Meter whose connection was removed. */
    uint16_t queued_packet_count;        /**< Payloads still queued before removal. */
    uint8_t has_in_flight;               /**< Non-zero if an in-flight TX was aborted. */
    uint8_t access_mismatch_retry_count; /**< Access-number mismatch retry count. */
} WMBUS_LINK_gw_connection_failure_info_t;

/**
 * @brief GW connection-failure callback type.
 *
 * @note @p info is valid only for the duration of the callback. Copy it if the
 *       application needs to retain the information.
 */
typedef void (*wmbus_link_gw_connection_failure_cb_t)(
    WMBUS_LINK_gw_connection_failure_t reason,
    const WMBUS_LINK_gw_connection_failure_info_t *info);

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
 * @brief Register the GW whitelist admission-result callback.
 *
 * Reports whether an unknown meter was learned or rejected by the configured
 * GW whitelist/admission policy. Pass NULL to unregister.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_register_gw_admission_cb(wmbus_link_gw_admission_cb_t cb);

/**
 * @brief Register the GW connection-failure callback.
 *
 * Reports when an established connection is removed by eviction or after the
 * access-number mismatch retry limit is exceeded. Queued or in-flight payloads
 * reported by the callback will not be transmitted through that connection.
 * Pass NULL to unregister.
 *
 * @retval 0   Success.
 * @retval <0  Failure.
 */
int32_t wmbus_link_register_gw_connection_failure_cb(wmbus_link_gw_connection_failure_cb_t cb);

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
 * @note Intended for gateway role usage. The payload is accepted into the
 *       target meter queue and, when possible, pre-encrypted and pre-packed
 *       immediately. Success means ownership was accepted by the link layer;
 *       RF transmission may occur later as part of the meter session.
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
 * @brief Gateway: configure a fixed whitelist table.
 *
 * The application owns the device info table and provides a writable RAM
 * buffer for link-layer per-meter runtime state. The state layout is internal
 * to the link layer. Unknown meters are rejected and reported by the GW
 * admission-result callback when registered.
 *
 * Calling this API enables gateway admission control automatically.
 *
 * @param[in]     info_p            Device info table provided by the application.
 * @param[in]     count             Number of entries in the device info table.
 * @param[in,out] state_buffer_p    Writable RAM buffer provided by the application.
 * @param[in]     state_buffer_size State buffer size in bytes.
 *
 * @retval true  Fixed whitelist was accepted.
 * @retval false Invalid parameters or insufficient state buffer size.
 *
 * @note When count is non-zero, both info_p and state_buffer_p must be non-NULL.
 * @note state_buffer_p must be 4-byte aligned.
 */
bool wmbus_link_gw_whitelist_load_fixed(const WMBUS_LINK_device_info_t *info_p,
                                        uint16_t count,
                                        void *state_buffer_p,
                                        uint32_t state_buffer_size);

/**
 * @brief Gateway: configure a fixed whitelist with a security mode per entry.
 *
 * @param[in]     info_p            Device info table provided by the application.
 * @param[in]     security_mode_p   Per-entry security modes. NULL selects Mode 7 for all entries.
 * @param[in]     count             Number of entries in the device info table.
 * @param[in,out] state_buffer_p    Writable RAM buffer provided by the application.
 * @param[in]     state_buffer_size State buffer size in bytes.
 *
 * @retval true  Fixed whitelist was accepted.
 * @retval false Invalid parameters, mode, or state buffer size.
 */
bool wmbus_link_gw_whitelist_load_fixed_with_security(
    const WMBUS_LINK_device_info_t *info_p,
    const WMBUS_LINK_security_mode_t *security_mode_p,
    uint16_t count,
    void *state_buffer_p,
    uint32_t state_buffer_size);

/**
 * @brief Query fixed whitelist state buffer size.
 *
 * @param[in] count Number of fixed whitelist entries.
 *
 * @return Required state buffer size in bytes.
 */
uint32_t wmbus_link_gw_whitelist_get_state_buffer_size(uint16_t count);

/**
 * @brief Gateway: configure a dynamic learning whitelist buffer.
 *
 * The application provides one writable RAM buffer. The link layer lays out
 * device info entries and state entries inside this buffer. Unknown meters are
 * learned and accepted while capacity is available.
 *
 * Calling this API disables gateway admission control automatically so dynamic
 * learning can admit unknown meters.
 * Dynamically learned entries use Security Mode 7.
 *
 * @param[in,out] buffer_p    Writable RAM buffer provided by the application.
 * @param[in]     buffer_size Buffer size in bytes.
 * @param[in]     max_count   Maximum number of meters to learn/store.
 *
 * @retval true  Dynamic whitelist buffer was accepted.
 * @retval false Invalid parameters or insufficient buffer size.
 *
 * @note buffer_p must be 4-byte aligned.
 */
bool wmbus_link_gw_whitelist_load_dynamic_buffer(void *buffer_p,
                                                 uint32_t buffer_size,
                                                 uint16_t max_count);

/**
 * @brief Query dynamic whitelist buffer size.
 *
 * The returned size covers both learned device info entries and internal
 * per-meter runtime state, including alignment padding.
 *
 * @param[in] max_count Maximum number of meters to learn/store.
 *
 * @return Required dynamic buffer size in bytes.
 */
uint32_t wmbus_link_gw_whitelist_get_dynamic_buffer_size(uint16_t max_count);

/** @} */ /* end of WMBUS_LINK_APIs */

#ifdef __cplusplus
}
#endif

#endif /* __WMBUS_API_H__ */
