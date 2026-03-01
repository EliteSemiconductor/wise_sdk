#ifndef __WISE_RADIO_802154_API_H 
#define __WISE_RADIO_802154_API_H

#include "types.h"
#include "wise_core.h"
#include "wise_radio_api.h"

/**
 * @def WISE_802154_MAX_ADDRESSES
 * @brief Maximum number of addresses supported for IEEE 802.15.4 filtering.
 *
 * This macro defines the maximum number of addresses that can be specified
 * for destination filtering in IEEE 802.15.4 communication.
 */
#define WISE_802154_MAX_ADDRESSES               (3U)

/**
 * @struct WISE_RADIO_IEEE802154_AddrConfig_t
 * @brief Structure for IEEE 802.15.4 address configuration.
 *
 * This structure contains configuration for destination filtering in IEEE
 * 802.15.4 communication. It includes PAN IDs, short addresses, and long
 * addresses for filtering.
 */
typedef struct {
    /**
     * @brief PAN IDs for destination filtering.
     *
     * PAN IDs for destination filtering. All must be specified.
     * To disable a PAN ID, set it to the broadcast value, 0xFFFF.
     */
    uint16_t pan_id[WISE_802154_MAX_ADDRESSES];

    /**
     * @brief Short network addresses for destination filtering.
     *
     * A short network addresses for destination filtering. All must be
     * specified. To disable a short address, set it to the broadcast value,
     * 0xFFFF.
     */
    uint16_t short_addr[WISE_802154_MAX_ADDRESSES];

    /**
     * @brief A 64-bit address for destination filtering. All must be specified.
     *
     * This field is parsed in over-the-air (OTA) byte order. To disable a long
     * address, set it to the reserved value of 0x00 00 00 00 00 00 00 00.
     */
    uint8_t long_addr[WISE_802154_MAX_ADDRESSES][8];
} WISE_RADIO_IEEE802154_AddrConfig_t;


/**
 * @fn int8_t wise_radio_IEEE802154_init(int8_t intfIdx)
 * @brief Initializes the IEEE 802.15.4 radio interface.
 *
 * This function initializes the IEEE 802.15.4 radio interface specified by the
 * index.
 *
 * @param[in] intfIdx The index of the radio interface to initialize.
 * @return 0 on success, or a negative error code on failure.
 */
int8_t wise_radio_ieee802154_init(int8_t intf_idx);

/**
 * @fn void wise_radio_IEEE802154_deinit(int8_t intfIdx)
 * @brief Deinitializes the IEEE 802.15.4 radio interface.
 *
 * This function deinitializes the IEEE 802.15.4 radio interface specified by
 * the index, releasing any resources allocated during initialization.
 *
 * @param[in] intfIdx The index of the radio interface to deinitialize.
 */
void wise_radio_ieee802154_deinit(int8_t intf_idx);

/**
 * @fn void wise_radio_IEEE802154_SetPanId(int8_t intfIdx, uint16_t panId)
 * @brief Sets the PAN ID for the IEEE 802.15.4 radio interface.
 *
 * This function sets the PAN ID for the specified IEEE 802.15.4 radio
 * interface.
 *
 * @param[in] intfIdx The index of the radio interface to configure.
 * @param[in] panId The PAN ID to set. Set to 0xFFFF to disable.
 */
void wise_radio_ieee802154_set_pan_id(int8_t intf_idx, uint16_t pan_id);

/**
 * @fn void wise_radio_IEEE802154_SetShortAddress(int8_t intfIdx, uint16_t
 * shortAddr)
 * @brief Sets the short address for the IEEE 802.15.4 radio interface.
 *
 * This function sets the short address for the specified IEEE 802.15.4 radio
 * interface.
 *
 * @param[in] intfIdx The index of the radio interface to configure.
 * @param[in] shortAddr The short address to set. Set to 0xFFFF to disable.
 */
void wise_radio_ieee802154_set_short_address(int8_t intf_idx, uint16_t short_addr);

/**
 * @fn void wise_radio_IEEE802154_SetLongAddress(int8_t intfIdx, const uint8_t
 * *longAddr)
 * @brief Sets the long address for the IEEE 802.15.4 radio interface.
 *
 * This function sets the 64-bit long address for the specified IEEE 802.15.4
 * radio interface.
 *
 * @param[in] intfIdx The index of the radio interface to configure.
 * @param[in] longAddr Pointer to the 64-bit long address. Set to 0x00 00 00 00
 * 00 00 00 00 to disable.
 */
void wise_radio_ieee802154_set_long_address(int8_t intf_idx, const uint8_t *long_addr);

#endif
