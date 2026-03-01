/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */


#include "es_platform_components.h"

#ifdef WISE_WMBUS_SW_HELPER
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wise_core.h"
#include "radio_lib/wise_radio_api.h"
#include "api/wise_sys_api.h"
#include "cmd_wmbus.h"

#include "wise_radio_wmbus_api.h"
#include "wise_wmbus_crypto.h"
#include "internal_radio_ctrl.h"
#include "util_crc.h"
#include "wise_crypto_api.h"

#define WMBUS_TX_BUF_LEN                                512

#include "wise.h"
int8_t test_Rx_scheduleId = -1;
uint32_t RX_TO = 3000000;	//3sec
uint8_t trx_config_array[2] = {1, 1};	//crypt, reps
uint32_t txCount = 0, rxValidCount = 0, rxErrorCount = 0;

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


static const uint8_t cryptoKey[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

extern WISE_RADIO_BUFFER_T radioBuffer;
static uint8_t mbusRxFrame[WMBUS_MAX_PKT_LEN];
static uint8_t mbusTxBuffer[WMBUS_TX_BUF_LEN];

//address consts
static const char manuf[3] = {'E', 'S', 'M'}; //FLAG assigned for Silicon Labs
static const WMBUS_deviceType_t deviceType = WMBUS_DEVICE_WATER_METER;
static const uint8_t version = 0x02;
static uint32_t devId = 0x010101; //BCD
static WMBUS_accessibility_t wmbusAccessbility = WMBUS_ACCESSIBILITY_NO_ACCESS;
uint8_t accessNumber = 0U;
int8_t wmbusRadioIntf = 0;

extern uint16_t _add_idle_filler(uint8_t *buffer, uint16_t fillerLength);
void _wmbus_aes_test();
void radioWMbusEventCb(WISE_RADIO_EVT_T evt);
void radioWMbusPPtrxEventCb(WISE_RADIO_EVT_T evt);

void wmbus_testRxTimeoutProc(void* pData){
	wise_radio_wmbus_rx_stop(wmbusRadioIntf);

	debug_print("Rx timeout then try to Tx again !\n");
	_wmbus_pp_tx();
}

static int _ppCmd(int argc, char **argv){
	static uint8_t mode = 0, role = 0;
	static uint8_t wmbusStarted = 0;
	
	if (argc < 3) {
		goto invalid;
	}

	if (0 == strcmp(argv[2], "init")) {
		if (argc != 5) {
			goto invalid;
		}

		mode = strtoul(argv[3], NULL, 10);
		role = strtoul(argv[4], NULL, 10);

		if ((mode > 9) || (role > 1)) {
			goto invalid;
		}

		if (((mode != 3) && (mode != 4))) {
			debug_print("mode not supported yet.\n");
			goto finish;
		}

		if (wmbusStarted) {
			debug_print("WMbus already started mode=%d role=%d\n", mode, role);
			goto finish;
		}

		wise_radio_wmbus_init(wmbusRadioIntf);
		wise_radio_set_evt_callback(wmbusRadioIntf, radioWMbusPPtrxEventCb);
		wise_radio_set_buffer(wmbusRadioIntf, &radioBuffer);
		wise_radio_set_tx_io_mode(wmbusRadioIntf, CORE_IO_NONBLOCKING);
		wise_wmbus_crypto_init();
		wise_wmbus_crypto_set_key(cryptoKey);

		wise_radio_wmbus_set_mode(wmbusRadioIntf, role, mode);
		wise_radio_set_tx_pwr(wmbusRadioIntf, 127);

		if (role == 0) {
			devId = 0xa0000001;
		} else {
			devId = 0xb0000001;
		}

		wmbusStarted = 1;
	} 
	else if (0 == strcmp(argv[2], "rx")) 
	{
		if (argc < 4) 
		{
			goto invalid;
		}

		if (0 == strcmp(argv[3], "on")) 
		{
			debug_print("wmbus rx on\n");
			wise_radio_mbus_rx_start(wmbusRadioIntf, 0, RADIO_RX_ONE_SHOT);

			if(argc > 4){
				trx_config_array[0] = strtoul(argv[4], NULL, 10);	//crypt
			}
			//Rx on then keep or create one time out to Tx
			//test_Rx_scheduleId = wise_schlr_add_single(US_TO_SCHLR_UNIT(RX_TO), wmbus_testRxTimeoutProc, NULL );
		} 
		else if (0 == strcmp(argv[3], "off")) 
		{
			debug_print("wmbus rx off\n");
			wise_radio_wmbus_rx_stop(wmbusRadioIntf);
		} 
		else 
		{
			goto invalid;
		}
	} 
	else if (0 == strcmp(argv[2], "tx")) 
	{
		if (argc > 4){
			trx_config_array[0] = strtoul(argv[3], NULL, 10);	//crypt
			trx_config_array[1] = strtoul(argv[4], NULL, 10);	//reps
			if(trx_config_array[1] == 0){
				trx_config_array[1] = 1;
			}
		}
		_wmbus_pp_tx();

		debug_print("tx finished.\n");
	} 
	else 
	{
		goto invalid;
	}

finish:
	return SHLCMD_HIST | SHLCMD_DONE;

invalid:
	debug_print("Invalid parameters for ping-pong TX/RX switching\n");
	debug_print("Usage:\n");
	debug_print("	wmbus pp init [mode] [role]\n");
	debug_print("		[mode] 0=S1, 1=S1M, 2=S2, 3=T1, 4=T2, 5=C1, 6=C2, 7=R2, 9=F2, 9=N\n");
	debug_print("		[role]	0=collector, 1=meter\n");
	debug_print("	wmbus pp rx [on/off]\n");
	debug_print("	or wmbus pp rx [on/off] [crypt]\n");
	debug_print("		[crypt] 0=No crypt, 1=crypt\n");
	debug_print("	wmbus pp tx\n");
	debug_print("    or wmbus pp tx [crypt] [reps]\n");
	debug_print("		[crypt] 0=No crypt, 1=crypt\n");
	debug_print("		[reps] add 10 bytes for every reps\n");
	return SHLCMD_HIST | SHLCMD_DONE;
}

int WmbusCommandHandler(int argc, char **argv)
{
    static uint8_t mode = 0, role = 0;
    static uint8_t wmbusStarted = 0;
    
    if (argc <= 1) {
        goto invalid;
    }

    if (0 == strcmp(argv[1], "init")) {
        if (argc != 4) {
            goto invalid;
        }

        mode = strtoul(argv[2], NULL, 10);
        role = strtoul(argv[3], NULL, 10);

        if ((mode > 9) || (role > 1)) {
            goto invalid;
        }

        if (((mode != 3) && (mode != 4))) {
            debug_print("mode not supported yet.\n");
            goto finish;
        }

        if (wmbusStarted) {
            debug_print("WMbus already started mode=%d role=%d\n", mode, role);
            goto finish;
        }

        wise_radio_wmbus_init(wmbusRadioIntf);
        wise_radio_set_evt_callback(wmbusRadioIntf, radioWMbusEventCb);
        wise_radio_set_buffer(wmbusRadioIntf, &radioBuffer);
        wise_radio_set_tx_io_mode(wmbusRadioIntf, CORE_IO_BLOCKING);
        
        wise_wmbus_crypto_init();
        wise_wmbus_crypto_set_key(cryptoKey);

        wise_radio_wmbus_set_mode(wmbusRadioIntf, role, mode);
        wise_radio_set_tx_pwr(wmbusRadioIntf, 127);

        if (role == 0) {
            devId = 0xa0000001;
        } else {
            devId = 0xb0000001;
        }

        wmbusStarted = 1;
    } 
    else if (0 == strcmp(argv[1], "rx")) 
    {
        if (argc != 3) 
        {
            goto invalid;
        }

        if (0 == strcmp(argv[2], "on")) 
        {
            debug_print("wmbus rx on\n");
            wise_radio_mbus_rx_start(wmbusRadioIntf, 0, RADIO_RX_ONE_SHOT);
        } 
        else if (0 == strcmp(argv[2], "off")) 
        {
            debug_print("wmbus rx off\n");
            wise_radio_wmbus_rx_stop(wmbusRadioIntf);
        } 
        else 
        {
            goto invalid;
        }
    } 
    else if (0 == strcmp(argv[1], "tx")) 
    {
        uint32_t count = strtoul(argv[2], NULL, 10);
        uint32_t interval = strtoul(argv[3], NULL, 10);
        uint8_t reps = strtoul(argv[4], NULL, 10);

        if (argc != 5) 
        {
            goto invalid;
        }

        if (count == 0) 
        {
            count = 1;
        }

        if (interval == 0) 
        {
            interval = 1000;
        }

        debug_print("wmbus tx count:%lu, reps:%d\n", count, reps);
        while (count--) 
        {
            debug_print(".");
            _wmbus_meter_periodic_tx(&reps);
            wise_tick_delay_ms(interval);
        }

        debug_print("tx finished.\n");
    } 
    else if (0 == strcmp(argv[1], "pp")) 	//ping-pong TX/RX switching
    {
        _ppCmd(argc, argv);
    } 
    else if (0 == strcmp(argv[1], "aes")) 
    {
        _wmbus_aes_test();
    } 
    else 
    {
        goto invalid;
    }

finish:
    return SHLCMD_HIST | SHLCMD_DONE;

invalid:
    debug_print("Invalid parameters\n");
    debug_print("Usage:\n");
    debug_print("   wmbus init [mode] [role]\n");
    debug_print("       [mode]  0=S1, 1=S1M, 2=S2, 3=T1, 4=T2, 5=C1, 6=C2, 7=R2, 9=F2, 9=N\n");
    debug_print("       [role]  0=collector, 1=meter\n");
    debug_print("   wmbus rx [on/off]\n");
    debug_print("   wmbus tx [count] [interval_ms] [reps]\n");
    debug_print("   wmbus pp [action]\n");
    debug_print("   wmbus aes\n");
    return SHLCMD_HIST | SHLCMD_DONE;
}

void _wmbus_frame_m_field_2_chars(const uint16_t mfield, char *manuf)
{
    if (manuf != NULL) {
        manuf[0] = ((mfield >> 10) & 0x1F) + 64;
        manuf[1] = ((mfield >> 5) & 0x1F) + 64;
        manuf[2] = ((mfield >> 0) & 0x1F) + 64;
    }
}

uint16_t _wmbus_frame_chars_2_m_field(const char *manuf)
{
    uint16_t code = 0;

    if (manuf != NULL) {
        code  = (uint16_t)((manuf[0] - 64) << 10);
        code |= (uint16_t)((manuf[1] - 64) << 5);
        code |= (uint16_t)(manuf[2] - 64);
    }
    return code;
}

uint8_t _wmbus_frame_add_payload_i8(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int8_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 1); //DIF=1 is int8
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 1);
    return 3;
}

uint8_t _wmbus_frame_add_payload_i16(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int16_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 2); //DIF=2 is int16
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 2);
    return 4;
}

uint8_t _wmbus_frame_add_payload_i32(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int32_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 4); //DIF=4 is int32
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 4);

    return 6;
}

static void print_blocks(const uint8_t *buffer, uint16_t length)
{
    for (uint8_t i = 0; i < length; i++) {
        if (i % 16 == 0) {
            debug_print("[0x%02X ", buffer[i]);
        } else if (i % 16 == 8) {
            debug_print("| 0x%02X ", buffer[i]);
        } else if (i % 16 == 15 || i == length - 1) {
            debug_print("0x%02X]\n", buffer[i]);
        } else {
            debug_print("0x%02X ", buffer[i]);
        }
    }
}

void wmbusFrameParsing(uint8_t *pframe, uint16_t len)
{
    const WMBUS_dll_header_t *dllHeader = (WMBUS_dll_header_t *)pframe;
    const WMBUS_stl_header_t *stlHeader = (WMBUS_stl_header_t *)(pframe + sizeof(WMBUS_dll_header_t));
    char mField[3];

    _wmbus_frame_m_field_2_chars(dllHeader->address.detailed.manufacturer, mField);
    debug_print("Block-1:[L:%d,C:0x%02X,M:%c%c%c,ID:%08X,Version:0x%02X,devType:0x%02X]\n", dllHeader->lField, dllHeader->cField.raw, mField[0],
                mField[1], mField[2], (unsigned int)dllHeader->address.detailed.id, (unsigned int)dllHeader->address.detailed.version,
                (unsigned int)dllHeader->address.detailed.deviceType);

    if (stlHeader->ciField == WMBUS_CI_EN13757_3_APPLICATION_SHORT) {
        uint8_t *payload_start = pframe + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
        uint16_t payload_len   = dllHeader->lField - sizeof(WMBUS_dll_header_t) - sizeof(WMBUS_stl_header_t) + 1;

        debug_print("AppHeader:[CI:0x%02X,AccessNr:%d,Status:0x%02X,encMode:%d,Accessibility:%02X,encBlocks:%d,sync:%d]\n", stlHeader->ciField,
                    stlHeader->accessNumber, stlHeader->status, stlHeader->confWord.mode_0_5.mode, stlHeader->confWord.mode_0_5.accessibility,
                    stlHeader->confWord.mode_0_5.numOfEncBlocks, stlHeader->confWord.mode_0_5.synchronized);

        if (stlHeader->confWord.mode_0_5.mode == 5) {
            uint8_t iv[16];

            //with long transport layer header, the address from the header should be used
            memcpy(iv, &(dllHeader->address.raw), 8);
            memset(iv + 8, stlHeader->accessNumber, 8);

            wise_wmbus_crypto_decrypt(payload_start, payload_start, iv, payload_len);
        }

        print_blocks(payload_start, payload_len);
    } else {
        print_blocks(pframe + sizeof(WMBUS_dll_header_t), dllHeader->lField - sizeof(WMBUS_dll_header_t) + 1);
    }

    debug_print("\n");
}

static uint16_t setupSTLHeader(void *buffer, uint8_t mode, uint8_t ciField, uint8_t accessNumber, WMBUS_accessibility_t accessibility,
                               bool synchronized, void *payload, uint16_t payloadLength)
{
    (void)ciField;
    WMBUS_stl_header_t *stlHeader = (WMBUS_stl_header_t *)buffer;

    memset(stlHeader, 0, sizeof(WMBUS_stl_header_t)); //clear the header

    stlHeader->ciField                         = WMBUS_CI_EN13757_3_APPLICATION_SHORT;
    stlHeader->accessNumber                    = accessNumber;
    stlHeader->confWord.mode_0_5.mode          = mode;
    stlHeader->confWord.mode_0_5.accessibility = accessibility;
    stlHeader->confWord.mode_0_5.synchronized  = synchronized;

    if (mode == 5) {
        uint8_t iv[16];
        uint16_t mf = _wmbus_frame_chars_2_m_field(manuf);

        //with long transport layer header, the address from the header should be used
        memcpy(iv, &mf, 2);
        memcpy(iv + 2, &devId, 4);
        memcpy(iv + 6, &version, 1);
        memcpy(iv + 7, &deviceType, 1);
        memset(iv + 8, accessNumber, 8);
        stlHeader->confWord.mode_0_5.numOfEncBlocks = wise_wmbus_crypto_encrypt(payload, payload, iv, payloadLength, 0);

        return 16 * stlHeader->confWord.mode_0_5.numOfEncBlocks; //payloadlength probably changed due to padding
    } else {
        return payloadLength;
    }
}

static void setupDLLHeader(void *buffer, bool isFrameB, uint16_t frameLength, WMBUS_functionCode_t functionCode, bool primary, bool fcvDcf,
                           bool fcbAcd)
{
    WMBUS_dll_header_t *dllHeader = (WMBUS_dll_header_t *)buffer;

    memset(dllHeader, 0, sizeof(WMBUS_dll_header_t)); //clear the header

    dllHeader->cField.detailed.primary       = primary;
    dllHeader->cField.detailed.fcbAcd        = fcbAcd;
    dllHeader->cField.detailed.fcvDcf        = fcvDcf;
    dllHeader->cField.detailed.functionCode  = functionCode;
    dllHeader->address.detailed.manufacturer = _wmbus_frame_chars_2_m_field(manuf);
    dllHeader->address.detailed.id           = devId;
    dllHeader->address.detailed.version      = version;
    dllHeader->address.detailed.deviceType   = deviceType;

    frameLength -= 1; //l field shouldn't be counted

    if (isFrameB) {
        if (frameLength < 125) {
            dllHeader->lField = (uint8_t)(frameLength + 2); //2block, 1 crc
        } else {
            dllHeader->lField = (uint8_t)(frameLength + 4); //3block, 2 crc
        }
    } else {
        dllHeader->lField = (uint8_t)frameLength;
    }
}

void radioWMbusEventCb(WISE_RADIO_EVT_T evt)
{
    //debug_print("wmbus evt %08x\n", (unsigned int)evt);

    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR)) {
        uint32_t bufAddr    = 0;
        WISE_RX_META_T meta = {0};

        if (evt & WISE_RADIO_EVT_RX_FRAME) {
            if (WISE_SUCCESS == wise_radio_get_rx_frame_info(wmbusRadioIntf, &bufAddr, &meta)) {
                memcpy((void *)mbusRxFrame, (void *)bufAddr, meta.data_len);
            }
        }

        wise_radio_release_rx_frame(wmbusRadioIntf);

        if (meta.valid) {
            //debug_print("mbus rx frame len=%d rssi=%d freq=%lu time=%08x\n", meta.data_len, meta.rssi, meta.ch_frequency, meta.timestamp);
            //dump_buffer(mbusRxFrame, meta.data_len);

            wmbusFrameParsing(mbusRxFrame, meta.data_len);
        } else {
            debug_print("rx frame invalid\n");
        }

        wise_radio_mbus_rx_start(wmbusRadioIntf, 0, RADIO_RX_ONE_SHOT);
    }

    if ((evt & WISE_RADIO_EVT_TX_DONE) || (evt & WISE_RADIO_EVT_TX_ERR)) {
        debug_print("tx fin\n");
    }
}

void radioWMbusPPtrxEventCb(WISE_RADIO_EVT_T evt)
{
    //debug_print("wmbus evt %08x\n", (unsigned int)evt);

    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR)) {
        uint32_t bufAddr    = 0;
        WISE_RX_META_T meta = {0};

        if (evt & WISE_RADIO_EVT_RX_FRAME) {
            if (WISE_SUCCESS == wise_radio_get_rx_frame_info(wmbusRadioIntf, &bufAddr, &meta)) {
                memcpy((void *)mbusRxFrame, (void *)bufAddr, meta.data_len);
            }
        }

        wise_radio_release_rx_frame(wmbusRadioIntf);

        if (meta.valid) {
	        rxValidCount ++;
            debug_print("mbus rx frame len=%d rssi=%d freq=%lu time=%08x\n", meta.data_len, meta.rssi, meta.ch_frequency, meta.timestamp);
            dump_buffer(mbusRxFrame, meta.data_len);

            wmbusFrameParsing(mbusRxFrame, meta.data_len);
        } else {
	        rxErrorCount ++;
            debug_print("rx frame invalid\n");
        }
		debug_print("rxCount=%ld, rxValidCount=%ld, rxErrorCount=%ld\n", (rxValidCount + rxErrorCount), rxValidCount, rxErrorCount);
		debug_print("Rx done then trigger Tx immediately\n");
		if(test_Rx_scheduleId != -1){
			debug_print("so remove the last test_Rx_scheduleId=%d\n", test_Rx_scheduleId);
			wise_schlr_remove(test_Rx_scheduleId);
		}

		wise_tick_delay_ms(100);
		_wmbus_pp_tx(trx_config_array);
    }

    if ((evt & WISE_RADIO_EVT_TX_DONE) || (evt & WISE_RADIO_EVT_TX_ERR)) {
		txCount ++;

		debug_print("txCount=%d, tx done then Rx start and Rx timeout\n", txCount);
		wise_radio_wmbus_rx_stop(wmbusRadioIntf);
		wise_radio_mbus_rx_start(wmbusRadioIntf, 0, RADIO_RX_ONE_SHOT);

		if(test_Rx_scheduleId != -1){
			debug_print("Remove the last test_Rx_scheduleId=%d then schedule next Rx timeout \n", test_Rx_scheduleId);
			wise_schlr_remove(test_Rx_scheduleId);
		}

		/* Tx done then Rx  immediately and create one Rx time out*/
        test_Rx_scheduleId = wise_schlr_add_single(US_TO_SCHLR_UNIT(RX_TO), wmbus_testRxTimeoutProc, NULL );

        debug_print(".....\n");
    }
}

static WMBUS_accessibility_t wmbus_accessibility = WMBUS_ACCESSIBILITY_LIMITED_ACCESS;

uint16_t _wmbus_setup_tx_frame(uint8_t *buffer, uint8_t accessNumber, WMBUS_accessibility_t accessibility, uint8_t repetitions,
                               bool periodic, bool encrypt)
{
    uint8_t *payloadPtr = buffer + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    uint8_t *stlPtr     = buffer + sizeof(WMBUS_dll_header_t);
    uint8_t *dllPtr     = buffer;

    //setup the payload before the headers, so we'll know the length
    uint16_t frameLength = 0;
    if (encrypt) { //2B AES check
        frameLength += _add_idle_filler(payloadPtr + frameLength, 2);
    }

	for(uint8_t i = 0; i < repetitions; i++){
	    //VIF 0x13 is volume in m^3
	    frameLength += _wmbus_frame_add_payload_i32(payloadPtr + frameLength, WMBUS_DIB_FUNCTION_INSTANTANEOUS, 0x13, 0x1000+i);

		//VIF 0x3B is volume flow in m^3
		frameLength += _wmbus_frame_add_payload_i16(payloadPtr + frameLength, WMBUS_DIB_FUNCTION_INSTANTANEOUS, 0x3B, 0x10+i);
	}
	//debug_print("Raw data before encrypt length=%d\n", frameLength);
	//dump_buffer(payloadPtr, frameLength);

    //continue with STL header, since encryption could change the length
    frameLength =
        setupSTLHeader(stlPtr, encrypt ? 5 : 0, WMBUS_CI_EN13757_3_APPLICATION_SHORT, accessNumber, accessibility, periodic, payloadPtr, frameLength);

    //add the header lengths to frameLnegth
    frameLength += sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    //finally, set up the DLL header
    setupDLLHeader(dllPtr, false, frameLength, WMBUS_FUNCTION_SND_NR, true, false, false);

    return frameLength;
}

void _wmbus_meter_periodic_tx(void *pData)
{
    uint16_t frameLen = 0;
    uint8_t *reps_p = (uint8_t *)pData;

    memset(mbusTxBuffer, 0, WMBUS_TX_BUF_LEN);
    frameLen = _wmbus_setup_tx_frame(mbusTxBuffer, accessNumber, wmbusAccessbility, *reps_p, true, true);

    //debug_print("mbus tx frame len=%d\n", frameLen);
    //dump_buffer(mbusTxBuffer, frameLen);

    wise_radio_wmbus_tx_frame(wmbusRadioIntf, 0, mbusTxBuffer, frameLen);
    
    accessNumber++;
}

void _wmbus_pp_tx(void)
{
    uint16_t frameLen = 0;

    memset(mbusTxBuffer, 0, WMBUS_TX_BUF_LEN);

    debug_print("crypt=%d, reps=%d\n", trx_config_array[0], trx_config_array[1]);
    frameLen = _wmbus_setup_tx_frame(mbusTxBuffer, accessNumber, wmbusAccessbility, trx_config_array[1], true, trx_config_array[0]);

    //debug_print("mbus tx frame len=%d\n", frameLen);
    //dump_buffer(mbusTxBuffer, frameLen);

    wise_radio_wmbus_tx_frame(wmbusRadioIntf, 0, mbusTxBuffer, frameLen);
    
    accessNumber++;
}


uint8_t rawPktSilabs[] = {
    0xec, 0xe4, 0x38, 0xdc, 0xe2, 0x8c, 0x1a, 0xab, 0x69, 0x9a, 0xa6, 0x69, 0x1a, 0xab, 0xc9, 0x99, 0x56, 0x96, 0x72, 0xc6, 0x8d, 0x9a, 0xc6, 0x6a,
    0x99, 0xe9, 0xa4, 0x59, 0x63, 0x8e, 0x25, 0x3b, 0x99, 0xb1, 0x6c, 0x9a, 0x4e, 0xea, 0xc8, 0x19, 0x9b, 0xb2, 0x72, 0x56, 0x3a, 0x6c, 0x93, 0x71,
    0x99, 0x96, 0x99, 0xe5, 0x64, 0xc6, 0x93, 0x69, 0x1c, 0xa9, 0x3a, 0x96, 0x53, 0xbd, 0x02, 0x7d, 0x09, 0x48, 0xf0, 0xd2, 0x8a, 0x65, 0x53, 0x4a,
    0x0f, 0xbc, 0x02, 0x51, 0x8d, 0xa8, 0x51, 0xa9, 0xe5, 0x42, 0x55, 0x39, 0x3d, 0x78, 0x58, 0x55, 0x24, 0x2a, 0x5a, 0x08, 0x8f, 0x93, 0x92, 0x88,
    0x6c, 0x0f, 0x19, 0x5a, 0x4a, 0x54, 0x23, 0x7c, 0x1f, 0x8b, 0x08, 0x80, 0x08, 0x1a, 0x58, 0xc9, 0x5b, 0x42, 0x55, 0xa9, 0x42, 0x45, 0xce, 0x0f,
    0xa1, 0x05, 0x45, 0xf8, 0x1a, 0xd5, 0xf5, 0x9a, 0x55, 0xa2, 0x8c, 0x21, 0x4b, 0xd4, 0x34, 0x55, 0xf6, 0x20, 0x22, 0x98, 0xa5, 0xe3, 0x63, 0x89,
    0x14, 0xd2, 0xf1, 0x94, 0x43, 0x59, 0x30, 0x0f, 0x90, 0x49, 0xde, 0x1a, 0x23, 0x8a, 0xda, 0x11, 0x84, 0xc9, 0x4c, 0x96, 0x7a, 0x32, 0xad, 0x64,
    0x5d, 0xa3, 0x42, 0x4e, 0xac, 0x83, 0x86, 0xa0, 0x0e, 0x62, 0x10, 0x3a, 0x21, 0x42, 0x0f, 0xe4, 0xe2, 0xe4, 0x99, 0x60, 0x6a, 0xc1, 0x11, 0x62,
    0x79, 0x08, 0x0d, 0x55, 0xd8, 0x62, 0xe0, 0x0d, 0x81, 0x9a, 0x94, 0xf3, 0xa0, 0x76, 0x24, 0x4d, 0x78, 0x5f, 0xe0, 0x45, 0xc2, 0x25, 0xa2, 0xd2,
    0xa6, 0x1d, 0x9a, 0x36, 0x91, 0x65, 0x81, 0x6a, 0x02, 0x16, 0x25, 0x44, 0x6f, 0x41, 0x15, 0xfc, 0xda, 0xc5, 0x64, 0x05, 0x21, 0x4a, 0x8f, 0xa9,
    0x42, 0x16, 0xb7, 0x44, 0xa9, 0x8c, 0x5e, 0x50, 0x71, 0xa0, 0x13, 0x41, 0x12, 0x2e, 0x45, 0x9e,
};

uint8_t rawPktESMT[] = {
    0xec, 0xe4, 0x38, 0xdc, 0xe2, 0x8c, 0x1a, 0xab, 0x69, 0x9a, 0xa6, 0x69, 0x1a, 0xab, 0xc9, 0x99, 0x56, 0x96, 0x72, 0xa6, 0x69, 0x9a, 0xc6, 0x6a,
    0x9c, 0xe9, 0xd0, 0xb4, 0xd3, 0xc8, 0x19, 0xdb, 0x2c, 0x0b, 0x37, 0x66, 0xa9, 0xc9, 0x4d, 0x93, 0x3c, 0x8d, 0x19, 0x3b, 0x4e, 0x53, 0x6c, 0x8e,
    0x65, 0x1a, 0xb3, 0x65, 0x6a, 0x8d, 0x66, 0xa9, 0x00, 0x90, 0xb3, 0xd5, 0x9b, 0x6a, 0xba, 0xba, 0x99, 0x1c, 0x10, 0x35, 0xdd, 0x2a, 0x06, 0x2c,
    0x5a, 0x9d, 0xe9, 0x2c, 0x90, 0xaa, 0x14, 0x29, 0xad, 0xac, 0x98, 0x2a, 0x25, 0x1c, 0x09, 0x9d, 0x96, 0x8d, 0xd8, 0x4e, 0x21, 0xbd, 0x89, 0xc1,
    0x4c, 0xa8, 0xa8, 0x8a, 0xa0, 0x6a, 0x31, 0xaa, 0xa6, 0x53, 0x0d, 0xa9, 0xb6, 0xe6, 0x27, 0x8d, 0xca, 0x89, 0xe4, 0x72, 0x29, 0x5d, 0xa5, 0xa9,
    0xaa, 0xf9, 0x4c, 0xcd, 0x6c, 0x55, 0xa1, 0x38, 0xa9, 0x45, 0x4d, 0xb0, 0xab, 0xac, 0xdb, 0x8c, 0x81, 0x0a, 0xca, 0x4d, 0xba, 0x48, 0x84, 0xb2,
    0x6a, 0x04, 0x8b, 0xae, 0x23, 0xec, 0x2c, 0x8a, 0x84, 0x0c, 0x68, 0x24, 0xb5, 0xdb, 0x99, 0x8c, 0xc8, 0x2a, 0xb6, 0xb9, 0x94, 0xa8, 0xb2, 0x0a,
    0xc6, 0x3e, 0xe4, 0x6f, 0x81, 0x28, 0xd4, 0x7d, 0x50, 0x28, 0xcb, 0x61, 0x22, 0x89, 0x4e, 0x54, 0xbf, 0xa8, 0x92, 0x80, 0xa6, 0xea, 0x26, 0xb5,
    0xca, 0x1d, 0x99, 0x20, 0x5e, 0xbb, 0xc3, 0xb2, 0x8e, 0x56, 0xa4, 0x8f, 0x34, 0xa9, 0x6a, 0x1d, 0x94, 0x1a, 0x4c, 0x2f, 0x8e, 0xbd, 0xf8, 0x6a,
    0x12, 0x88, 0x49, 0x2c, 0x6c, 0xbe, 0xd4, 0xda, 0xab, 0xea, 0xa0, 0x2a, 0xd3, 0xac, 0x9a, 0x22, 0x2a, 0xdd, 0x6a, 0xae, 0x01, 0xb9, 0x94, 0xaa,
    0x44, 0xa9, 0xb4, 0x89, 0x92, 0x99, 0x00, 0x2b, 0xab, 0x2d, 0x5b, 0xaa, 0xa6, 0xac, 0xe2, 0x5b,
};

void _wmbus_aes_test()
{
#define TEST_DATA_LEN 32
#define TEST_COUNT 5

    static const uint8_t cryptoKey[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    uint8_t iv[16]                   = {0};
    uint8_t *inputBuf                = NULL;
    uint8_t *outputBuf               = NULL;
    uint8_t *tmpBuf                  = NULL;
    int i;
    WISE_AES_DATA_T aesData       = {0};
    WISE_AES_KEY_CONFIG_T key_cfg = {.key_size  = AES_KEY_128,
                                     .key_src   = AES_KEY_SRC_FROM_USER,
                                     .key_num   = 0,
                                     .key_bytes = (uint32_t *)cryptoKey,
                                     .swap_mode = AES_SWAP_NONE,
                                     .iv_or_cnt = (uint32_t *)iv};

    static const uint8_t debugPins[] = {3, 6, 7, 9};
    inputBuf                         = malloc(TEST_DATA_LEN);
    outputBuf                        = malloc(TEST_DATA_LEN);
    tmpBuf                           = malloc(TEST_DATA_LEN);

    for (i = 0; i < TEST_DATA_LEN; i++) {
        inputBuf[i] = i;
    }
    memset(outputBuf, 0, TEST_DATA_LEN);
    memset(tmpBuf, 0, TEST_DATA_LEN);

    wise_debug_io_init(debugPins, 4);

    wise_wmbus_crypto_init();
    wise_wmbus_crypto_set_key(cryptoKey);

    wise_crypto_init();

    if (wise_aes_key_config(&key_cfg) != WISE_SUCCESS) {
        printf("set aes key fail\n");
        return;
    }
    for (i = 0; i < TEST_COUNT; i++) {
        wise_debug_io_low(0);
        wise_debug_io_low(1);
        wise_debug_io_low(2);
        wise_debug_io_low(3);

        for (int i = 0; i < 10000; i++) {
            ;
        }
        memset(outputBuf, 0, TEST_DATA_LEN);
        memset(tmpBuf, 0, TEST_DATA_LEN);

        wise_debug_io_low(0);
        wise_debug_io_high(0);
        wise_wmbus_crypto_encrypt(inputBuf, outputBuf, iv, TEST_DATA_LEN, 0);
        wise_debug_io_low(0);

        wise_debug_io_low(1);
        wise_debug_io_high(1);
        wise_wmbus_crypto_decrypt(outputBuf, tmpBuf, iv, TEST_DATA_LEN);
        wise_debug_io_low(1);

        memset(outputBuf, 0, TEST_DATA_LEN);
        memset(tmpBuf, 0, TEST_DATA_LEN);

        aesData.input  = inputBuf;
        aesData.output = outputBuf;
        aesData.length = TEST_DATA_LEN;
        aesData.iv_or_cnt = iv;

        WISE_AES_CONFIG_CTX_T cbc_ctx = {
            .aes_mode      = AES_MODE_CBC,
            .aes_direction = AES_ENCRYPT,
            .data_cfg      = aesData,
        };

        wise_debug_io_low(2);
        wise_debug_io_high(2);
        wise_aes_exec(&cbc_ctx);
        //wise_aes_cbc_encrypt(&key_cfg, &aesData);
        wise_debug_io_low(2);

        aesData.input         = outputBuf;
        aesData.output        = tmpBuf;
        aesData.length        = TEST_DATA_LEN;
        cbc_ctx.aes_direction = AES_DECRYPT;
        cbc_ctx.data_cfg      = aesData;
        wise_debug_io_low(3);
        wise_debug_io_high(3);
        wise_aes_exec(&cbc_ctx);
        //wise_aes_cbc_decrypt(&key_cfg, &aesData);
        wise_debug_io_low(3);
    }

    free(inputBuf);
    free(outputBuf);
    free(tmpBuf);
}


#endif
