/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "wise.h"
#include "util.h"
#include "wise_core.h"
#include "wise_gpio_api.h"
#include "wise_tick_api.h"
#include "wise_uart_api.h"
#include "wise_sys_api.h"

#include "wise_shell_v2/src/shell.h"
#include "demo_app_common.h"
#include "wmbus_datalink_api.h"
#include "app_sensor_data.h"

#ifdef WMBUS_DEMO_LINK_METER
#define DEMO_APP_PROMPT                 "METER> "
#else
#define DEMO_APP_PROMPT                 "GATEWAY> "
#endif

#define METER_REPORT_INTERVAL           8000            //8000ms
#define METER_SENSOR_UPDATE_INTERVAL    10000           //10000ms
#define METER_DEV_ID                    0x40000001
#define GATEWAY_DEV_ID                  0x10000000

enum 
{
    WMBUS_DIB_FUNCTION_INSTANTANEOUS, /**< Instantaneous value */
    WMBUS_DIB_FUNCTION_MINIMUM,       /**< Minimum value */
    WMBUS_DIB_FUNCTION_MAXIMUM,       /**< Maximum value */
    WMBUS_DIB_FUNCTION_ERROR          /**< Value during error state */
};

#ifdef WMBUS_DEMO_LINK_METER
WMBUS_LINK_device_info_t dev_info = {METER_DEV_ID, "ESM", 0x01, 0x07};
#else
WMBUS_LINK_device_info_t dev_info = {GATEWAY_DEV_ID, "ESM", 0x01, 0x00};
#endif

static const uint8_t cryptoKey[] = 
{
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

static const WMBUS_LINK_security_mode_t securityMode = WMBUS_LINK_SM_7;

/* ========================================================================== */
/* Main                                                                       */
/* ========================================================================== */
void app_wmbus_init();
void app_wmbus_log_info();

typedef void (*wmbus_link_data_cb_t)(uint32_t owner_id, const uint8_t *addr_p, uint8_t len);

void app_wmbus_meter_cfg();
void app_wmbus_meter_report_proc(void *pData);
void app_wmbus_meter_data_handler(uint32_t owner_id, const uint8_t *addr_p, uint8_t len);
void app_wmbus_meter_sensor_update();

void app_wmbus_gw_cfg();
void app_wmbus_gw_data_handler(uint32_t owner_id, const uint8_t *addr_p, uint8_t len);

void app_wmbus_init()
{    
    wise_wmbus_crypto_init();
    
    wmbus_link_init();
    wmbus_link_set_security_mode(securityMode);
    wmbus_link_set_encrypt_key(cryptoKey);

    app_sensor_data_init();
    app_wmbus_log_info();
}

void app_wmbus_log_info()
{
    WMBUS_LINK_VER_T libVersion = {0};
    
    wmbus_link_get_version_info(&libVersion);

    debug_print("\n================================\n");
    debug_print("   ESMT WMbus Link V%d.%02d\n", libVersion.major, libVersion.minor);
    debug_print("================================\n");
}


void main(void)
{
    demo_app_common_init();
    app_shell_init(DEMO_APP_PROMPT);

    app_wmbus_init();
    
#ifdef WMBUS_DEMO_LINK_METER
    app_wmbus_meter_cfg();
#else
    app_wmbus_gw_cfg();
#endif

    wmbus_link_start();

    while (1) 
    {
    	wise_main_proc();
    }
}

// WMBUS Meter application
void app_wmbus_meter_cfg()
{    
    wmbus_link_register_data_cb(app_wmbus_meter_data_handler);
    
    //set role and mode, currently only T mode is supported.
    wmbus_link_set_mode(WMBUS_LINK_MODE_T2);
    wmbus_link_set_role(WMBUS_LINK_METER);

    //basic information
    wmbus_link_set_device_info(&dev_info);
    
    wise_schlr_add_periodical(MS_TO_SCHLR_UNIT(METER_REPORT_INTERVAL), app_wmbus_meter_report_proc, NULL);
    wise_schlr_add_periodical(MS_TO_SCHLR_UNIT(METER_SENSOR_UPDATE_INTERVAL), app_wmbus_meter_sensor_update, NULL);
}

uint8_t _packPayload_I8(uint8_t *payload, uint8_t dibFunction, uint8_t vif, int8_t value)
{
    payload[0] = (uint8_t)((dibFunction << 4) + 1);   //DIF=1 is int8
    payload[1] = vif;
    payload[2] = value;
    
    return 3;
}

uint8_t _packPayload_I16(uint8_t* payload, uint8_t dibFunction, uint8_t vif, int16_t value)
{
    payload[0] = (uint8_t)((dibFunction << 4) + 2);   //DIF=2 is int16
    payload[1] = vif;
    memcpy(payload + 2, &value, 2);
    
    return 4;
}

uint8_t _packPayload_I32(uint8_t* payload, uint8_t dibFunction, uint8_t vif, int32_t value)
{
    payload[0] = (uint8_t)((dibFunction << 4) + 4);   //DIF=4 is int32
    payload[1] = vif;
    memcpy(payload + 2, &value, 4);

    return 6;
}

void app_wmbus_meter_report_proc(void *pData)
{
    uint8_t payload[6] = {0};
    uint8_t payloadLen = 0;
    ST_SENSOR_DATA_T* sensorData = app_sensor_data_get(0);

    if((sensorData) && (sensorData->data != 0))
    {
        uint32_t now = wise_get_tu();
        
        wise_log_time_info(now);
        debug_print("meter report vif=%02x value=%08x\n", sensorData->vif, sensorData->data);
    
        switch(sensorData->dif)
        {
            case 1:
                payloadLen = _packPayload_I8(payload, WMBUS_DIB_FUNCTION_INSTANTANEOUS, sensorData->vif, (int8_t)sensorData->data);
            break;

            case 2:
                payloadLen = _packPayload_I16(payload, WMBUS_DIB_FUNCTION_INSTANTANEOUS, sensorData->vif, (int16_t)sensorData->data);
            break;

            case 4:
                payloadLen = _packPayload_I32(payload, WMBUS_DIB_FUNCTION_INSTANTANEOUS, sensorData->vif, (int32_t)sensorData->data);
            break;

            default:
                debug_print("Invalid dif\n");
                return;
        }

        if(payloadLen)
        {
            wmbus_link_meter_write_payload(payload, payloadLen);
            wmbus_link_meter_start_session();
        }
    }
}

void app_wmbus_meter_sensor_update()
{
    uint32_t sensorNum = app_sensor_num();
    int i;

    for(i = 0; i < sensorNum; i++)
        app_sensor_data_update(i);
}

void app_wmbus_meter_data_handler(uint32_t owner_id, const uint8_t *addr_p, uint8_t len)
{
    debug_print("payload received, len=%d\n", len);
    dump_byte(addr_p, len);
}

// WMBUS Other application
void app_wmbus_gw_cfg()
{
    wmbus_link_register_data_cb(app_wmbus_gw_data_handler);

    //set role and mode, currently only T mode is supported.
    wmbus_link_set_mode(WMBUS_LINK_MODE_T2);
    wmbus_link_set_role(WMBUS_LINK_GW);

    //basic information
    wmbus_link_set_device_info(&dev_info);

    wmbus_link_gw_set_admission_ctrl_enabled(0);
}

void app_wmbus_gw_data_handler(uint32_t owner_id, const uint8_t *addr_p, uint8_t len)
{
    uint32_t now = wise_get_tu();
        
    wise_log_time_info(now);
    debug_print("Received meter report, dev=%02x\n", owner_id);

    if(((addr_p[0] >> 4) & 0xf) == WMBUS_DIB_FUNCTION_INSTANTANEOUS)
    {
        switch(addr_p[1])
        {
            case 0x46:
            {
                uint16_t voltage = LOAD_LE_16(&addr_p[2]);

                debug_print("   voltage: %d mv\n", voltage);
            }
            break;

            default:
                goto dump_raw;
        }
    }

    return;

dump_raw:
    debug_print("raw data:\n");
    dump_buffer(addr_p, len);
}

