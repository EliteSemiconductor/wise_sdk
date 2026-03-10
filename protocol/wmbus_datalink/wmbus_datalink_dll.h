#ifndef __WMBUS_DLL_H__
#define __WMBUS_DLL_H__

#include "wise_radio_wmbus_api.h"

extern uint8_t staticData_1[];
extern const uint16_t staticData_1_size;
extern uint8_t staticData_2[];
extern const uint16_t staticData_2_size;
extern uint8_t staticData_3[];
extern const uint16_t staticData_3_size;


#define WMBUS_DEV_ID 0x01020304  //BCD
#define WMBUS_VERSION 0x01

#define AFL_CI_SIZE 1
#define AFL_AFLL_SIZE 1
#define AFL_FCL_SIZE 2
#define AFL_MCL_SIZE 1
#define AFL_KI_SIZE 2
#define AFL_MCR_SIZE 4
#define AFL_MAC_SIZE 16
#define AFL_ML_SIZE 2
#define AFL_ALL_SIZE 29

#if 1 //javi
// Reference, it should be removed for replaced.
// !!!!!!!!! TI part  !!!!!!!!!!!!!!!!
//Enumeration for the message type specified in control field.
typedef enum {
  /*!
   * \b Collector: Send User Data (SND-UD).
   * Value: 0x53
   */
  E_APL_MESSAGE_TYPE_SND_UD = 0x53,
  /*!
   * \b Collector: Send User Data 2 (SND-UD2).
   * Value: 0x43
   */
  E_APL_MESSAGE_TYPE_SND_UD2 = 0x43,
  /*!
   * \b Collector: Request User Data Class 1 (REQ-UD1).
   * Value: 0x5A
   */
  E_APL_MESSAGE_TYPE_REQ_UD1 = 0x5A,
  /*!
   * \b Collector: Request User Data Class 2 (REQ-UD2).
   * Value: 0x5B
   */
  E_APL_MESSAGE_TYPE_REQ_UD2 = 0x5B,
  /*!
   * \b Collector: Confirm Installation Request (CNF-IR).
   * Value: 0x06
   */
  E_APL_MESSAGE_TYPE_CNF_IR = 0x06,
  /*!
   * \b Collector: Send Link Reset (SND-NKE).
   * Value: 0x40
   */
  E_APL_MESSAGE_TYPE_SND_NKE = 0x40,

  /*!
   * \b Meter: Send No-Reply (SND-NR).
   * Value: 0x44
   */
  E_APL_MESSAGE_TYPE_SND_NR = 0x44,
  /*!
   * \b Meter: Send Installation Request (SND-IR).
   * Value: 0x46
   */
  E_APL_MESSAGE_TYPE_SND_IR = 0x46,
  /*!
   * \b Meter: Response User Data (RSP-UD) - not supported to be created by application.
   * Value: 0x08
   */
  E_APL_MESSAGE_TYPE_RSP_UD = 0x08,
  /*!
   * \b Meter: Access Demand (ACC-DMD).
   * Value: 0x48
   */
  E_APL_MESSAGE_TYPE_ACC_DMD = 0x48,
  /*!
   * \b Meter: Access No-Reply (ACC-NR) - not supported to be created by application.
   * Value: 0x47
   */
  E_APL_MESSAGE_TYPE_ACC_NR = 0x47,

  /*!
   * \b Common: Acknowledgment (ACK) - not supported to be created by meter application.
   * Value: 0x00
   */
  E_APL_MESSAGE_TYPE_ACK = 0x00
} E_APL_MESSAGE_TYPE_t;
#endif

// !!!!!!!!! silabs part  !!!!!!!!!!!!!!!!
//Function code to use in the first block's C-field
typedef enum {
  WMBUS_FUNCTION_SND_NKE = 0x0, /**< SND-NKE */
  WMBUS_FUNCTION_SND_UD = 0x3,  /**< SND-UD/SND-UD2 */
  WMBUS_FUNCTION_SND_NR = 0x4,  /**< SND-NR */
  WMBUS_FUNCTION_SND_UD3 = 0x5,  /**< SND-UD3 */
  WMBUS_FUNCTION_SND_IR = 0x6,  /**< SND-IR */
  WMBUS_FUNCTION_ACC_NR = 0x7,  /**< ACC-NR */
  WMBUS_FUNCTION_ACC_DMD = 0x8, /**< ACC-DMD */
  WMBUS_FUNCTION_REQ_UD1 = 0xA, /**< REQ-UD1 */
  WMBUS_FUNCTION_REQ_UD2 = 0xB, /**< REQ-UD2 */
} WMBUS_function_code_t;


typedef enum {
  WMBUS_FUNCTION_ACK = 0x0, /**< ACK */
  WMBUS_FUNCTION_NACK = 0x1,  /**< NACK */
  WMBUS_FUNCTION_CNF_IR = 0x6,  /**< CNF-IR */
  WMBUS_FUNCTION_RSP_UD = 0x8,  /**< RSP-UD */
} WMBUS_secondary_function_code_t;


//Device type to be used in the address field(s)
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
  WMBUS_DEVICE_CUSTOMER_UNIT = 0x25,            /**< Customer unit (display device) */
  WMBUS_DEVICE_WASTE_WATER_METER = 0x28,        /**< Waste water meter */
  WMBUS_DEVICE_GARBAGE,                         /**< Garbage */
  WMBUS_DEVICE_CARBON_DIOXIDE,                  /**< Reserved for carbon dioxide */
  WMBUS_DEVICE_COMMUNICATION_CONTROLLER = 0x31, /**< Communication controller (Gateway) */
  WMBUS_DEVICE_UNIDIRECTIONAL_REPEATER,         /**< Unidirectional repeater */
  WMBUS_DEVICE_BIDIRECTIONAL_REPEATER,          /**< Bidirectional repeater */
  WMBUS_DEVICE_RADIO_CONVERTER_SYSTEM = 0x36,   /**< Radio converter (system side) */
  WMBUS_DEVICE_RADIO_CONVERTER_METER,           /**< Radio converter (meter side) */
} WMBUS_device_type_t;

// CI field codes
typedef enum {
  WMBUS_CI_DATA_TO_METER_NONE = 0x51,               /**< Data sent by readout device to the Meter without transport layer */
  WMBUS_CI_MBUS_DATA_TO_METER_SHORT = 0x5A,         /**< MBUS data sent by readout device to the Meter with short transport layer */
  WMBUS_CI_MBUS_DATA_TO_METER_LONG,                 /**< MBUS data sent by readout device to the Meter with long transport layer */
  WMBUS_CI_COSEM_DATA_TO_METER_LONG = 0x60,         /**< COSEM data sent by readout device to the Meter with long transport layer */
  WMBUS_CI_COSEM_DATA_TO_METER_SHORT,               /**< COSEM data sent by readout device to the Meter with short transport layer */
  WMBUS_CI_OBIS_DATA_TO_METER_LONG = 0x64,          /**< OBIS based data sent by readout device to the Meter with long transport layer */
  WMBUS_CI_OBIS_DATA_TO_METER_SHORT,                /**< OBIS based data sent by readout device to the Meter with short transport layer */
  WMBUS_CI_EN13757_3_DATA_TO_METER_NONE = 0x69,     /**< EN13757-3 application layer with Format frame and no transport layer */
  WMBUS_CI_EN13757_3_DATA_TO_METER_SHORT,           /**< EN13757-3 application layer with Format frame and short transport layer */
  WMBUS_CI_EN13757_3_DATA_TO_METER_LONG,            /**< EN13757-3 application layer with Format frame and long transport layer */
  WMBUS_CI_CLOCK_SYNC_ABSOLUTE,                     /**< Clock synchronisation (absolute) */
  WMBUS_CI_CLOCK_SYNC_RELATIVE,                     /**< Clock synchronisation (relative) */
  WMBUS_CI_APPLICATION_ERROR_SHORT,                 /**< Application error from Meter with short transport layer */
  WMBUS_CI_APPLICATION_ERROR_LONG,                  /**< Application error from Meter with long transport layer */
  WMBUS_CI_APPLICATION_ERROR_NONE,                  /**< Application error from Meter with no transport layer */
  WMBUS_CI_ALARM_NONE,                              /**< Alarm from Meter without transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_LONG,              /**< EN13757-3 application layer with long transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_CF_LONG,           /**< EN13757-3 application layer with compact frame and long transport layer */
  WMBUS_CI_ALARM_SHORT,                             /**< Alarm from Meter with short transport layer */
  WMBUS_CI_ALARM_LONG,                              /**< Alarm from Meter with long transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_NONE = 0x78,       /**< EN13757-3 application layer with no transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_CF_NONE,           /**< EN13757-3 application layer with compact frame and no transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_SHORT,             /**< EN13757-3 application layer with short transport layer */
  WMBUS_CI_EN13757_3_APPLICATION_CF_SHORT,          /**< EN13757-3 application layer with compact frame and short transport layer */
  WMBUS_CI_COSEM_APPLICATION_LONG,                  /**< COSEM application layer with long transport layer */
  WMBUS_CI_COSEM_APPLICATION_SHORT,                 /**< COSEM application layer with short transport layer */
  WMBUS_CI_OBIS_APPLICATION_LONG,                   /**< OBIS based application layer with long transport layer */
  WMBUS_CI_OBIS_APPLICATION_SHORT,                  /**< OBIS based application layer with short transport layer */
  WMBUS_CI_EN13757_3_TRANSPORT_TO_METER_LONG,       /**< EN13757-3 Transport layer (long) from readout device to the meter */
  WMBUS_CI_NETWORK_LAYER_DATA,                      /**< Network layer data */
  WMBUS_CI_FOR_FUTURE_USE,                          /**< For future use */
  WMBUS_CI_NETWORK_MANAGEMENT_APPLICATION,          /**< Network management application */
  WMBUS_CI_TRANSPORT_TO_METER_MBUS_COMPACT,         /**< Transport layer to Meter (M Bus-Compact frame expected) */
  WMBUS_CI_NETWORK_MANAGEMENT_DATA = 0x89,          /**< Reserved for Network management data (EN 13757-5) */
  WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_SHORT,    /**< EN13757-3 Transport layer (short) from the meter to the readout device */
  WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_LONG,     /**< EN13757-3 Transport layer (long) from the meter to the readout device */
  WMBUS_CI_EXTENDED_LINK_LAYER1,                    /**< Extended Link Layer I (2B) */
  WMBUS_CI_EXTENDED_LINK_LAYER2,                    /**< Extended Link Layer II (8B) */
  WMBUS_CI_EXTENDED_LINK_LAYER3,                    /**< Extended Link Layer III (10B) */
  WMBUS_CI_EXTENDED_LINK_LAYER4,                    /**< Extended Link Layer IV (16B) */
  WMBUS_CI_AUTHENTICATION_FRAGMENTATION = 0x90,     /* Indicates that an Authentication and Fragmentation Sublayer */
} WMBUS_ci_field_t;


//Application errors coded in the status field
typedef enum {
  WMBUS_STATUS_APPLICATION_BUSY = 0x1,    /**< Application busy */
  WMBUS_STATUS_APPLICATION_ERROR = 0x2,   /**< Any application error */
  WMBUS_STATUS_ABNORMAL_CONDITION = 0x3,  /**< Abnormal condition/alarm */
  WMBUS_STATUS_POWER_LOW = 1 << 2,        /**< Power low */
  WMBUS_STATUS_PERMANENT_ERROR = 1 << 3, /**< Permanent error */
  WMBUS_STATUS_TEMPORARY_ERROR = 1 << 4, /**< Temporary error */
} WMBUS_status_t;

//Accessibility settings in the config word
typedef enum {
  WMBUS_ACCESSIBILITY_NO_ACCESS = 0,        /**< No access (unidirectional) */
  WMBUS_ACCESSIBILITY_TEMPORARY_NO_ACCESS,  /**< Bidirectional, temporary no access */
  WMBUS_ACCESSIBILITY_LIMITED_ACCESS,       /**< Bidirectional, limited access (short time after TX) */
  WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS,     /**< Bidirectional, unlimited access */
} WMBUS_accessibility_t;

/*! Enumeration of accessibility types displayed in the configuration field.
    This enum is only used for bidirectional meter devices */
typedef enum{
  /* The last two bits of the configuration word will show that the
     telegram was send from an unidirectional device. (BIT15 = 0 Bit 14 = 0) */
  E_WMBUS_ACCESSIBILITY_UNIDIRECTIONAL = 0,
  /* The last two bits of the configuration word will show that the
     telegram was send from a bidirectional device. The meter supports
     bidirectional access in general, but there is no access window after this
     transmission  (BIT15 = 0 Bit 14 = 1) */
  E_WMBUS_ACCESSIBILITY_BIDIRECTIONAL_NO_ACCESS = 1,
  /* The last two bits of the configuration word will show that the
     telegram was send from a bidirectional device. The meter provides a short
     access window only immediately after this transmission   (BIT15 = 1 Bit 14 = 0) */
  E_WMBUS_ACCESSIBILITY_BIDIRECTIONAL_LIMITED_ACCESS = 2,
  /* The last two bits of the configuration word will show that the
     telegram was send from a bidirectional device. The meter provides unlimited
     access at least until next transmission  (BIT15 = 1 Bit 14 = 1) */
  E_WMBUS_ACCESSIBILITY_BIDIRECTIONAL_UNLIMITED_ACCESS = 3,
} E_WMBUS_ACCESSIBILITY_t;

/*! Enumeration of fac delay types as specified by EN13757-4 (2013). */
typedef enum{
  /* FAC delay = 2 seconds. Applicable for mode S2,T2 and C2 */
  E_WMBUS_FAC_TX_DELAY_2 = 2,
  /* FAC delay = 3 seconds. Applicable for mode S2,T2 and C2 */
  E_WMBUS_FAC_TX_DELAY_3 = 3,
  /* FAC delay = 5 seconds. Applicable for mode S2,T2,C2 and N2 */
  E_WMBUS_FAC_TX_DELAY_5 = 5,
  /* FAC delay = 7 seconds. Applicable for mode N2 only */
  E_WMBUS_FAC_TX_DELAY_7 = 7,
  /* FAC delay = 13 seconds. Applicable for mode N2 only */
  E_WMBUS_FAC_TX_DELAY_13 = 13,
} E_WMBUS_FAC_TX_DELAY;

/*! Definition of the radio channels (as array index).
    Make sure the definitions to match those of the serial module.
*/
typedef enum E_RADIO_CHANNEL_INDEX
{
  /*! Channel none */
  E_RF_CFG_CHAN_NONE = 0,

  /* 868 MHZ channels */
  /*! Channel 868.3 MHz */
  E_RF_CFG_CHAN_868_30,
  /*! Channel 868.95 MHz */
  E_RF_CFG_CHAN_868_95,
  /*! Channel 868.52 MHz */
  E_RF_CFG_CHAN_869_52,

  /* 169 MHZ channels */
  /*! Channel 169.406250 MHz */
  E_RF_CFG_CHAN_169_1A,
  /*! Channel 169.418750 MHz */
  E_RF_CFG_CHAN_169_1B,
  /*! Channel 169.431250 MHz */
  E_RF_CFG_CHAN_169_2A,
  /*! Channel 169.443750 MHz */
  E_RF_CFG_CHAN_169_2B,
  /*! Channel 169.456250 MHz */
  E_RF_CFG_CHAN_169_3A,
  /*! Channel 169.468750 MHz */
  E_RF_CFG_CHAN_169_3B,
  /*! Channel 169.437500 MHz */
  E_RF_CFG_CHAN_169_0,
  /*! Channel 169.412500 MHz */
  E_RF_CFG_CHAN_169_1,
  /*! Channel 169.437500 MHz */
  E_RF_CFG_CHAN_169_2,
  /*! Channel 169.462500 MHz */
  E_RF_CFG_CHAN_169_3,
  /*! Channel max */
  /* maximum for arrays */
  E_RF_CFG_CHAN_MAX,
  /*! Channel unknown - error case, should never occur  */
  E_RF_CFG_CHAN_UNKNOWN
}E_RADIO_CHANNEL_INDEX_t;

/*================= Communication control field (CC-field) ===================*/
/*! Bidirectional bit of the CC-field */
#define ELL_CC_FIELD_BIDIRECTIONAL_BIT          0x80U
/*! Response delay bit of the CC-field */
#define ELL_CC_FIELD_DELAY_BIT                  0x40U
/*! Synchronous bit of the CC-field */
#define ELL_CC_FIELD_SYNCHRONOUS_BIT            0x20U
/*! Hop counter bit of the CC-field */
#define ELL_CC_FIELD_HOP_COUNTER_BIT            0x10U
/*! Priority bit of the CC-field */
#define ELL_CC_FIELD_PRIORITY_BIT               0x08U
/*! Accessibility bit of the CC-field */
#define ELL_CC_FIELD_ACCESSIBILITY_BIT          0x04U
/*! Repeated access bit of the CC-field */
#define ELL_CC_FIELD_REPEATED_ACCESS_BIT        0x02U

#pragma pack(push, 1)
/* Wireless M-Bus address (for DLL/IV) */
typedef struct {
    uint16_t  manuf;
    uint32_t  dev_id;
    uint8_t  version;
    uint8_t  dev_type;
} WMBUS_Address;

//Data link layer header (first block)
typedef struct {
    uint8_t len_field;                             /**< Length field */
    union {
        uint8_t raw;                              /**< Control field raw mode */
        struct {
            //WMBUS_function_code_t function_code : 4;    /**< Control field function code */
            uint8_t function_code : 4;    /**< Control field function code */
            bool fcv_dfc : 1;                          /**< Control field fcv/dfc */
            bool fcb_acd : 1;                          /**< Control field fcb/acd */
            bool primary : 1;                         /**< Control field, primary station bit */
            uint8_t res : 1;                         /**< reservation - bit shall always be zero */
        } detailed;                               /**< Control field detailed mode */
    } c_field;                                   /**< Control field */

    WMBUS_Address address;
} WMBUS_dll_header_t;
#pragma pack(pop)

/* Communication Control Field (CC-field) */
typedef union {
    uint8_t raw;
    struct {
        uint8_t x_extended_delay : 1;  /* Bit 0, X-field  : Extended Delay subfield */
        uint8_t r_repeated_access: 1;  /* Bit 1, R-field  : Repeated Access subfield */
        uint8_t a_accessibility  : 1;  /* Bit 2, A-field  : Accessibility subfield */
        uint8_t p_priority       : 1;  /* Bit 3, P-field  : Priority subfield */
        uint8_t h_hop_count      : 1;  /* Bit 4, H-field  : Hop Count subfield */
        uint8_t s_synchronized   : 1;  /* Bit 5, S-field  : Synchronized subfield */
        uint8_t d_response_delay : 1;  /* Bit 6, D-field  : Response Delay subfield */
        uint8_t b_bidirectional  : 1;  /* Bit 7, B-field  : Bi-directional subfield */
    } detailed;
} WMBUS_CC_t;

//Extended Link Layer with CI = 8Ch
typedef struct {
    uint8_t ci_field;           /**< CI (control information) field */
    WMBUS_CC_t comm_control;      /**< Communication Control field */
    uint8_t access_number;      /**< Access number */
}WMBUS_ell_short_header_t;


//Data link layer header (first block)
#pragma pack(push, 1)
typedef union {
    uint16_t raw;                             /**< Raw config word */
    struct {
        bool hop_counter : 1;                     /**< Hopcounter */
        bool repeated_access : 1;                 /**< Repeated access */
        uint8_t content_message : 2;            /**< Content of message */
        uint8_t reserved : 4;              /**< Reserved */
        uint8_t mode : 5;                        /**< Mode (security) 5th bit is reserved as per 13757-4, but OMS uses it */
        bool synchronized : 1;                   /**< Synchronized or not */
        //WMBUS_accessibility_t accessibility : 2; /**< Accessibility */
        uint8_t accessibility : 2; /**< Bidirectional communication & Accessibility */
    } mode_0_0;  

    struct {
        bool hop_counter : 1;                     /**< Hopcounter */
        bool repeated_access : 1;                 /**< Repeated access */
        uint8_t content_message : 2;            /**< Content of message */
        uint8_t num_of_enc_blocks : 4;              /**< Number of encoded blocks */
        uint8_t mode : 5;                        /**< Mode (security) 5th bit is reserved as per 13757-4, but OMS uses it */
        bool synchronized : 1;                   /**< Synchronized or not */
        //WMBUS_accessibility_t accessibility : 2; /**< Accessibility */
        uint8_t accessibility : 2; /**<Bidirectional communication &  Accessibility */
    } mode_0_5;

    struct {
        uint16_t content_index      : 3;  /* bits 0..2 */
        uint16_t tpl_padding        : 1;  /* bit  3    */
        uint16_t enc_blocks_16B     : 4;  /* bits 4..7 */
        uint16_t sec_mode           : 5;  /* bits 8..12 */
        uint16_t counter_size     : 1;  /* bit  13   */
        uint16_t content_message : 2;  /* bits 14..15 */
    } mode_0_7;
} WMBUS_conf_word_t;
#pragma pack(pop)

/* ---------- mode 7 Config-extension ---------- */
typedef union {
    uint8_t raw;
    struct {
        uint8_t key_id        : 4; /* KKKK  (bits 0..3) */
        uint8_t kdf_selection : 2; /* DD    (bits 4..5) */
        uint8_t version_flag  : 1; /* V     (bit  6)  1 => will follow 1B Key Version */
        uint8_t reserved0     : 1; /* R     (bit  7) */
    }detailed;
} WMBUS_conf_ext_m7_t;

//Short transport layer header
#pragma pack(push, 1)
typedef struct {
    uint8_t ci_field;           /**< CI (control information) field */
    uint8_t access_number;      /**< Access number */
    uint8_t status;            /**< Status byte */
    WMBUS_conf_word_t conf_word; /**< Config word */

    /* mode 7 extension */
    WMBUS_conf_ext_m7_t conf_ext;  /* DD/KKKK/V */
} WMBUS_stl_header_t;
#pragma pack(pop)

//Long transport layer header
#pragma pack(push, 1)
typedef struct {
    uint8_t ci_field;                  /**< CI (control information) field */
    struct {
        uint32_t id;                  /**< Address, device ID */
        uint16_t manufacturer;        /**< Address, manufacturer ID */
        uint8_t version;              /**< Address, version */
        uint8_t device_type;           /**< Address, device type */
    } ltl_address;                     /**< Detailed address */
    uint8_t access_number;             /**< Access number */
    uint8_t status;                   /**< Status byte */
    WMBUS_conf_word_t conf_word;        /**< Config word */
    /* mode 7 extension */
    WMBUS_conf_ext_m7_t conf_ext;  /* DD/KKKK/V */
/* The following two items are optional and OMS say only mode 10 to use them */
//uint8_t  key_version;         /* if has_key_version = true */
//uint32_t message_counter;
} WMBUS_ltl_header_t;
#pragma pack(pop)


/* =======================================================================
 *  AFL(Authentication & Fragmentation Layer)
 * =======================================================================*/

/* ---------- MCL(Message Control Field) ---------- */
typedef union {
    uint8_t raw;
    struct {
        uint8_t at : 4; /* bits 0..3 : Authentication-Type (Table 6) */
        uint8_t kimp : 1; /* bit 4   : Key Information in Message present */
        uint8_t mcmp : 1; /* bit 5   : Message counter in Message present */
        uint8_t mlmp : 1; /* bit 6   : Message length in Message present */
        uint8_t res7 : 1; /* bit 7   : Reserved (0) */
    }detailed;
} WMBUS_mcl_field_t;

/* ---------- KI(Key Information Field) ---------- */
typedef union {
    uint16_t raw;
    struct {
        uint16_t key_id : 4; /* bits 0..3 */
        uint16_t kdf_select : 2; /* bits 4..5 */
        uint16_t reserved : 2; /* bits 6..7 = 0 */
        uint16_t key_version : 8; /* bits 8..15 */
    }detailed;
} WMBUS_ki_field_t;

/* ---------- FCL(Fragmentation Control Field)
  FID is LSB-first in low 8 bits,  other presence/flag
*/
typedef union {
    uint16_t raw;
    struct {
        uint16_t fid   : 8; /* bits 0..7 : Fragment-ID(0 => unfragment) */
        uint16_t res8  : 1; /* bit 8 : Reserved (0) */
        uint16_t kip   : 1; /* bit 9 : Key Information present */
        uint16_t macp  : 1; /* bit 10 : MAC present */
        uint16_t mcrp  : 1; /* bit 11 : Message counter present */
        uint16_t mlp   : 1; /* bit 12 : Message length present */
        uint16_t mclp  : 1; /* bit 13 : MCL present */
        uint16_t mf    : 1; /* bit 14 : More Fragments (1=> more fragment) */
        uint16_t res15 : 1;  /* bit 15 : Reserved (0) */
    }detailed;
} WMBUS_fcl_field_t;

/* ---------- AFL Header(not include data payload, with MAC) ---------- 
    CI (1) | AFLL (1) | FCL (2) | [MCL] | [KI] | [MCR(4)] | [ML(2)] | [MAC(n)]
*/
#pragma pack(push, 1)
typedef struct {
    /* permanent */
    uint8_t           ci;    /* 1B : AFL indication */
    uint8_t           afll;  /* 1B : AFL length(excluding the field AFL.AFLL itself.) */
    WMBUS_fcl_field_t fcl;   /* 2B : FCL(LSB-first) */

    /* option(LSB-first) */
    WMBUS_mcl_field_t mcl;     /* 1B */
    WMBUS_ki_field_t  ki;      /* 2B */
    uint32_t          mcr;     /* 4B Message counter */
    /* MAC (by selection of Authentication tag length, if 0 : No authentication) */
    uint8_t  mac_len;          /* 0/2/4/8/12/16 */
    uint8_t  mac[16];          /* max 16 bytes */
    uint16_t          ml;      /* 2B Message length */
} WMBUS_afl_header_t;
#pragma pack(pop)

/* ---------- AT: Authentication tag length ---------- */
static inline uint8_t wmbus_at_to_taglen(uint8_t at)
{
    /* 
       0 => No authentication
       3 => AES-CMAC-128, 2 bytes
       4 => AES-CMAC-128, 4 bytes
       5 => AES-CMAC-128, 8 bytes
       6 => AES-CMAC-128, 12 bytes
       7 => AES-CMAC-128, 16 bytes
       8 => AES-GMAC-128, 12 bytes (support by other mode)
       other case as 0(No MAC)
    */
    switch (at & 0x0F) {
        case 3:  return 2;
        case 4:  return 4;
        case 5:  return 8;
        case 6:  return 12;
        case 7:  return 16;
        case 8:  return 12;
        default: return 0;
    }
}

typedef struct {
    uint8_t  ci;                 /* CI */
    WMBUS_Address addr;          /* Long transport layer */
    uint8_t  acc;                /* Access number */
    uint8_t  sts;                /* Status */
    WMBUS_conf_word_t conf_word; /* 2 bytes */

    uint8_t  security_mode;      /* 0 = plain text; 5 = mode5; 7 = mode7 */

    /* Support mode 7 */
    WMBUS_conf_ext_m7_t conf_ext;    /* DD/KKKK/V */
    bool     has_key_version;        /* conf_ext.version_flag == 1 to be true */
    uint8_t  key_version;            /* if has_key_version==true */
    bool     has_message_counter;    /* Z==1 to be true */
    uint32_t message_counter;        /* LSB-first */
    uint8_t  content_index;          /* III (0..7) */
} WMBUS_TPL_Params;

typedef struct {
    uint8_t wmbus_role;
    
    /* Flag that a installation request must be sent. */
    //The flag should be 1 for factory default value
    uint8_t wmbus_is_installed;
    
    //stores the current mbus Mode.
    wmbus_mode_t wmbus_mode;
    
    /// Constant for accessibility setting used in the app
    WMBUS_accessibility_t wmbus_accessibility;
    /* Note: It must be set to WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS in GW role */
    
    // control function code
    uint8_t wmbus_function_code;
    
    // control information field
    uint8_t wmbus_ci_field;
    
    /// Access number for the WMBus protocol  generated randomly
    uint8_t wmbus_access_number;
    uint8_t wmbus_access_number_tpl;
    
    char wmbus_manuf[3]; //FLAG assigned for ESMT
    
    // Identification number
    uint32_t wmbus_device_id;  //BCD, 4 bytes
    
    // Version
    uint8_t wmbus_version;
    
    // Device type
    WMBUS_device_type_t wmbus_device_type;

    //uint8_t wmbus_encrypt;    //[0/1]enable/disable that be replaced by security mode
    /* 0 = plain text; 5 = mode5; 7 = mode7 */
    uint8_t wmbus_security_mode;;
    
    /* The parameters of C-filed */
    uint8_t wmbus_primary;
    uint8_t wmbus_FCB;
    uint8_t wmbus_FCV;
    uint8_t wmbus_ACD;
    uint8_t wmbus_DFC;

    WMBUS_ell_short_header_t ell;
    WMBUS_afl_header_t  afl;
    WMBUS_TPL_Params  tpl;
} WMBUS_System_Params;

//Values for function field in DIB
typedef enum {
  WMBUS_DIB_FUNCTION_INSTANTANEOUS, /**< Instantaneous value */
  WMBUS_DIB_FUNCTION_MINIMUM,       /**< Minimum value */
  WMBUS_DIB_FUNCTION_MAXIMUM,       /**< Maximum value */
  WMBUS_DIB_FUNCTION_ERROR          /**< Value during error state */
} WMBUSframe_dibFunction_t;

typedef struct wmbus_readout_data{
  uint8_t vif;
  uint32_t data;
  struct wmbus_readout_data* next;
} wmbus_readout_data_t;

void dump_byte(uint8_t* p_dump, int length);
void dump_raw(const char *tag, uint8_t *p_dump, int length);

void wmbus_link_syncTimeoutProc(void* pData);
void wmbus_link_syncTxProc(void* pData);
void wmbus_link_syncRxProc(void* pData);
void wmbus_link_syncRxTimeoutProc(void* pData);

uint8_t wmbus_link_mode_is_unidirection(wmbus_mode_t mode);

uint8_t wmbus_link_get_role(void);
//void wmbus_link_set_role(WMBUS_LINK_role_t role);
wmbus_mode_t wmbus_link_get_mode(void);
//void wmbus_link_set_mode(wmbus_mode_t mode);
uint8_t wmbus_link_get_is_installed(void);
void wmbus_link_set_is_installed(uint8_t req);
WMBUS_function_code_t wmbus_link_get_func_code(void);
void wmbus_link_set_func_code(WMBUS_function_code_t func_code);
uint8_t wmbus_link_get_security_mode(void);
//void wmbus_link_set_security_mode(WMBUS_LINK_security_mode_t security_mode);
uint8_t* wmbus_link_get_encrypt_key(void);
//void wmbus_link_set_encrypt_key(uint8_t* newKey);

char* wmbus_link_get_manfu(void);
void wmbus_link_set_manfu(char* manuf);
uint32_t wmbus_link_get_device_ID(void);
void wmbus_link_set_device_ID(uint32_t device_id);
uint8_t wmbus_link_get_version(void);
void wmbus_link_set_version(uint8_t version);
WMBUS_device_type_t wmbus_link_get_device_type(void);
void wmbus_link_set_device_type(WMBUS_device_type_t type);

WMBUS_ci_field_t wmbus_link_get_ci_field(void);
void wmbus_link_set_ci_field(WMBUS_ci_field_t wmbus_ci_field);
WMBUS_ci_field_t wmbus_link_get_tpl_ci_field(void);
void wmbus_link_set_tpl_ci_field(WMBUS_ci_field_t wmbus_ci_field);
WMBUS_accessibility_t wmbus_link_get_accessibility(void);
void wmbus_link_set_accessibility(WMBUS_accessibility_t accessibility);

uint8_t wmbus_link_get_access_number(void);
void wmbus_link_set_access_number(uint8_t access_number);
void wmbus_link_plus_one_access_number(void);
uint8_t wmbus_link_get_access_number_tpl(void);
void wmbus_link_set_access_number_tpl(uint8_t access_number_tpl);
void wmbus_link_plus_one_access_number_tpl(void);
uint8_t wmbus_link_check_access_number(void);

void wmbus_link_set_primary(uint8_t primary);
uint8_t wmbus_link_get_FCB(void);
void wmbus_link_set_FCB(uint8_t FCB);
void wmbus_link_toggle_FCB(void);
uint8_t wmbus_link_get_FCV(void);
void wmbus_link_set_FCV(uint8_t FCV);
void wmbus_link_check_FCV(WMBUS_function_code_t func_code);
void wmbus_link_set_ACD(uint8_t ACD);

void wmbus_link_trigger_fsm_event(uint8_t fsm_event);

void wmbus_link_tx_packet_directly(void);
void wmbus_link_tx_test_packet(uint8_t test_case);
void wmbus_link_api_dump_last_tx_pkt(void);

uint8_t wmbus_link_get_flag_GW_request(void);
void wmbus_link_set_flag_GW_request(uint8_t _data);


#ifdef WMBUS_GW_PREENCRYPTION
void GW_prepare_encrypt_tx_buffer(void);
#endif

void wmbus_link_clear_flow_control_setting(void);
void wmbus_link_flow_control(void);

uint8_t wmbus_link_api_parsing_packet(void);
void wmbus_link_api_gen_packet(uint8_t *data_ptr, uint16_t data_len);
void wmbus_link_api_notify_data2APP(void);

void wmbus_link_set_state2connection(uint32_t id);
void wmbus_link_get_state_from_connection(uint8_t *rx_buffer);

bool wmbus_link_gw_is_admission_ctrl_enabled(void);
//void wmbus_link_gw_set_admission_ctrl_enabled(bool enable);

uint8_t wmbus_link_is_from_MTR_primary(WMBUS_function_code_t func_code);


uint16_t wmbus_link_add_idle_filler(uint8_t *buffer, uint16_t fillerLength);

void wmbus_link_dump_connection_table(void);
void wmbus_link_dump_debug_message(void);

void wmbus_link_read_payload(void);

//void wmbus_link_set_device_info(WMBUS_LINK_device_info_t *devInfo_p);
void wmbus_link_show_device_info(void);

void wmbus_link_show_state(void);

void wmbus_link_dump_timing_check(void);

#define DEBUG_BY_GPIO 0
void wmbus_link_gpio_toggle(uint8_t pin_idx);

#endif 

