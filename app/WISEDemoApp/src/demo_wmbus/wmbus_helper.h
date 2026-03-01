#ifndef __WMBUS_HELPER_H 
#define __WMBUS_HELPER_H

#include <stdint.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {
    WMBUS_FUNCTION_SND_NKE = 0x0, /**< SND-NKE */
    WMBUS_FUNCTION_SND_UD  = 0x3, /**< SND-UD/SND-UD2 */
    WMBUS_FUNCTION_SND_NR  = 0x4, /**< SND-NR */
    WMBUS_FUNCTION_SND_IR  = 0x6, /**< SND-IR */
    WMBUS_FUNCTION_ACC_NR  = 0x7, /**< ACC-NR */
    WMBUS_FUNCTION_ACC_DMD = 0x8, /**< ACC-DMD */
    WMBUS_FUNCTION_REQ_UD1 = 0xA, /**< REQ-UD1 */
    WMBUS_FUNCTION_REQ_UD2 = 0xB, /**< REQ-UD2 */
} WMBUS_functionCode_t;

typedef enum {
    WMBUS_ACCESSIBILITY_NO_ACCESS = 0,       /**< No access (unidirectional) */
    WMBUS_ACCESSIBILITY_TEMPORARY_NO_ACCESS, /**< Bidirectional, temporary no access */
    WMBUS_ACCESSIBILITY_LIMITED_ACCESS,      /**< Bidirectional, limited access (short time after TX) */
    WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS,    /**< Bidirectional, unlimited access */
} WMBUS_accessibility_t;

/**
 * @enum WMBUSframe_dibFunction_t
 * @brief Values for function field in DIB
 */
typedef enum {
    WMBUS_DIB_FUNCTION_INSTANTANEOUS, /**< Instantaneous value */
    WMBUS_DIB_FUNCTION_MINIMUM,       /**< Minimum value */
    WMBUS_DIB_FUNCTION_MAXIMUM,       /**< Maximum value */
    WMBUS_DIB_FUNCTION_ERROR          /**< Value during error state */
} WMBUSframe_dibFunction_t;

_Pragma("pack(1)") typedef struct {
    uint8_t lField; /**< Length field */
    union {
        uint8_t raw; /**< Control field raw mode */
        struct {
            WMBUS_functionCode_t functionCode : 4; /**< Control field function code */
            bool fcvDcf                       : 1; /**< Control field fcv/dcf */
            bool fcbAcd                       : 1; /**< Control field fcb/acd */
            bool primary                      : 1; /**< Control field, primary station bit */
        } detailed;                                /**< Control field detailed mode */
    } cField;                                      /**< Control field */
    union {
        uint8_t raw[8]; /**< Address field raw mode */
        struct {
            uint16_t manufacturer; /**< Address, manufacturer ID */
            uint32_t id;           /**< Address, device ID */
            uint8_t version;       /**< Address, version */
            uint8_t deviceType;    /**< Address, device type */
        } detailed;                /**< Address field detailed mode */
    } address;                     /**< Address field */
} WMBUS_dll_header_t;

/**
 * @union WMBUS_confWord_t
 * @brief Data link layer header (first block)
 */
typedef union {
    uint16_t raw; /**< Raw config word */
    struct {
        bool hopCounter                     : 1; /**< Hopcounter */
        bool repeatedAccess                 : 1; /**< Repeated access */
        uint8_t contentOfMessage            : 2; /**< Content of message */
        uint8_t numOfEncBlocks              : 4; /**< Number of encoded blocks */
        uint8_t mode                        : 5; /**< Mode (security) 5th bit is reserved as per 13757-4, but OMS uses it */
        bool synchronized                   : 1; /**< Synchronized or not */
        WMBUS_accessibility_t accessibility : 2; /**< Accessibility */
    } mode_0_5;                                  /**< Detailed config word for mode 0 and 5 */
    //add m2_3, m7 and m13 here
} WMBUS_confWord_t;

/**
 * @struct WMBUS_stl_header_t
 * @brief Short transport layer header
 */
typedef struct {
    uint8_t ciField;           /**< CI (control information) field */
    uint8_t accessNumber;      /**< Access number */
    uint8_t status;            /**< Status byte */
    WMBUS_confWord_t confWord; /**< Config word */
} WMBUS_stl_header_t;

_Pragma("pack()")
typedef enum {
    WMBUS_CI_DATA_TO_METER_NONE       = 0x51,      /**< Data sent by readout device to the Meter without transport layer */
    WMBUS_CI_MBUS_DATA_TO_METER_SHORT = 0x5A,      /**< MBUS data sent by readout device to the Meter with short transport layer */
    WMBUS_CI_MBUS_DATA_TO_METER_LONG,              /**< MBUS data sent by readout device to the Meter with long transport layer */
    WMBUS_CI_COSEM_DATA_TO_METER_LONG = 0x60,      /**< COSEM data sent by readout device to the Meter with long transport layer */
    WMBUS_CI_COSEM_DATA_TO_METER_SHORT,            /**< COSEM data sent by readout device to the Meter with short transport layer */
    WMBUS_CI_OBIS_DATA_TO_METER_LONG = 0x64,       /**< OBIS based data sent by readout device to the Meter with long transport layer */
    WMBUS_CI_OBIS_DATA_TO_METER_SHORT,             /**< OBIS based data sent by readout device to the Meter with short transport layer */
    WMBUS_CI_EN13757_3_DATA_TO_METER_NONE = 0x69,  /**< EN13757-3 application layer with Format frame and no transport layer */
    WMBUS_CI_EN13757_3_DATA_TO_METER_SHORT,        /**< EN13757-3 application layer with Format frame and short transport layer */
    WMBUS_CI_EN13757_3_DATA_TO_METER_LONG,         /**< EN13757-3 application layer with Format frame and long transport layer */
    WMBUS_CI_CLOCK_SYNC_ABSOLUTE,                  /**< Clock synchronisation (absolute) */
    WMBUS_CI_CLOCK_SYNC_RELATIVE,                  /**< Clock synchronisation (relative) */
    WMBUS_CI_APPLICATION_ERROR_SHORT,              /**< Application error from Meter with short transport layer */
    WMBUS_CI_APPLICATION_ERROR_LONG,               /**< Application error from Meter with long transport layer */
    WMBUS_CI_APPLICATION_ERROR_NONE,               /**< Application error from Meter with no transport layer */
    WMBUS_CI_ALARM_NONE,                           /**< Alarm from Meter without transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_LONG,           /**< EN13757-3 application layer with long transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_CF_LONG,        /**< EN13757-3 application layer with compact frame and long transport layer */
    WMBUS_CI_ALARM_SHORT,                          /**< Alarm from Meter with short transport layer */
    WMBUS_CI_ALARM_LONG,                           /**< Alarm from Meter with long transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_NONE = 0x78,    /**< EN13757-3 application layer with no transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_CF_NONE,        /**< EN13757-3 application layer with compact frame and no transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_SHORT,          /**< EN13757-3 application layer with short transport layer */
    WMBUS_CI_EN13757_3_APPLICATION_CF_SHORT,       /**< EN13757-3 application layer with compact frame and short transport layer */
    WMBUS_CI_COSEM_APPLICATION_LONG,               /**< COSEM application layer with long transport layer */
    WMBUS_CI_COSEM_APPLICATION_SHORT,              /**< COSEM application layer with short transport layer */
    WMBUS_CI_OBIS_APPLICATION_LONG,                /**< OBIS based application layer with long transport layer */
    WMBUS_CI_OBIS_APPLICATION_SHORT,               /**< OBIS based application layer with short transport layer */
    WMBUS_CI_EN13757_3_TRANSPORT_TO_METER_LONG,    /**< EN13757-3 Transport layer (long) from readout device to the meter */
    WMBUS_CI_NETWORK_LAYER_DATA,                   /**< Network layer data */
    WMBUS_CI_FOR_FUTURE_USE,                       /**< For future use */
    WMBUS_CI_NETWORK_MANAGEMENT_APPLICATION,       /**< Network management application */
    WMBUS_CI_TRANSPORT_TO_METER_MBUS_COMPACT,      /**< Transport layer to Meter (M Bus-Compact frame expected) */
    WMBUS_CI_NETWORK_MANAGEMENT_DATA = 0x89,       /**< Reserved for Network management data (EN 13757-5) */
    WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_SHORT, /**< EN13757-3 Transport layer (short) from the meter to the readout device */
    WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_LONG,  /**< EN13757-3 Transport layer (long) from the meter to the readout device */
    WMBUS_CI_EXTENDED_LINK_LAYER1,                 /**< Extended Link Layer I (2B) */
    WMBUS_CI_EXTENDED_LINK_LAYER2,                 /**< Extended Link Layer II (8B) */
    WMBUS_CI_EXTENDED_LINK_LAYER3,                 /**< Extended Link Layer III (10B) */
    WMBUS_CI_EXTENDED_LINK_LAYER4,                 /**< Extended Link Layer IV (16B) */
} WBMUS_ciField_t;

/**
 * @enum WMBUS_deviceType_t
 * @brief Device type to be used in the address field(s)
 */
typedef enum {
    WMBUS_DEVICE_OTHER = 0x00,                    /**< Other */
    WMBUS_DEVICE_OIL_METER,                       /**< Oil meter */
    WMBUS_DEVICE_ELECTRICITY_METER,               /**< Electricity meter */
    WMBUS_DEVICE_GAS_METER,                       /**< Gas meter */
    WMBUS_DEVICE_HEAT_METER,                      /**< Heat meter */
    WMBUS_DEVICE_STEAM_METER,                     /**< Steam meter */
    WMBUS_DEVICE_WARM_WATER_METER,                /**< Warm water meter (30-90C) */
    WMBUS_DEVICE_WATER_METER,                     /**< Water meter */
    WMBUS_DEVICE_HEAT_COST_ALLOCATOR,             /**< Heat cost allocator */
    WMBUS_DEVICE_COMPRESSED_AIR,                  /**< Compressed air */
    WMBUS_DEVICE_COOLING_METER_OUTLET,            /**< Cooling meter (volume measured at return temperature: outlet) */
    WMBUS_DEVICE_COOLING_METER_INLET,             /**< Cooling meter (volume measured at flow temperature: inlet) */
    WMBUS_DEVICE_HEAT_METER_INLET,                /**< Heat meter (volume measured at flow temperature: inlet) */
    WMBUS_DEVICE_COMBINED_HEAT_COOLING_METER,     /**< Combined heat/cooling meter */
    WMBUS_DEVICE_BUS_SYSTEM_COMPONENT,            /**< Bus/system component */
    WMBUS_DEVICE_CALORIFIC_VALUE = 0x14,          /**< Calorific value */
    WMBUS_DEVICE_HOT_WATER_METER,                 /**< Hot water meter (>90C) */
    WMBUS_DEVICE_COLD_WATER_METER,                /**< Cold water meter */
    WMBUS_DEVICE_DUAL_WATER_METER,                /**< Dual register (hot/cold) water meter */
    WMBUS_DEVICE_PRESSURE_METER,                  /**< Pressure meter */
    WMBUS_DEVICE_AD_CONVERTER,                    /**< A/D converter */
    WMBUS_DEVICE_SMOKE_DETECTOR,                  /**< Smoke detector */
    WMBUS_DEVICE_ROOM_SENSOR,                     /**< Room sensor (e.g. temperature or humidity) */
    WMBUS_DEVICE_GAS_DETECTOR,                    /**< Gas detector */
    WMBUS_DEVICE_BREAKER = 0x20,                  /**< Breaker (electricity) */
    WMBUS_DEVICE_VALVE,                           /**< Valve (gas or water) */
    WMBUS_DEVICE_CUSTOMER_UNIT     = 0x25,        /**< Customer unit (display device) */
    WMBUS_DEVICE_WASTE_WATER_METER = 0x28,        /**< Waste water meter */
    WMBUS_DEVICE_GARBAGE,                         /**< Garbage */
    WMBUS_DEVICE_CARBON_DIOXIDE,                  /**< Reserved for carbon dioxide */
    WMBUS_DEVICE_COMMUNICATION_CONTROLLER = 0x31, /**< Communication controller (Gateway) */
    WMBUS_DEVICE_UNIDIRECTIONAL_REPEATER,         /**< Unidirectional repeater */
    WMBUS_DEVICE_BIDIRECTIONAL_REPEATER,          /**< Bidirectional repeater */
    WMBUS_DEVICE_RADIO_CONVERTER_SYSTEM = 0x36,   /**< Radio converter (system side) */
    WMBUS_DEVICE_RADIO_CONVERTER_METER,           /**< Radio converter (meter side) */
} WMBUS_deviceType_t;

uint16_t wmbus_setup_tx_frame(uint8_t *buffer, uint8_t accessNumber, WMBUS_accessibility_t accessibility, bool periodic, bool encrypt);
uint16_t wmbus_setup_null_frame(uint8_t *buffer, uint8_t accessNumber, WMBUS_accessibility_t accessibility, bool periodic, bool encrypt);

void wmbus_dump_frame_info(uint8_t *pframe, uint16_t len);

#endif

