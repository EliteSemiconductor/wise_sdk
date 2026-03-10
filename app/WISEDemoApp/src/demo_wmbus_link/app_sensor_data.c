#include "app_sensor_data.h"
#include "wise_core.h"
#include "wise_sys_api.h"

typedef void (*SENSOR_INIT_PROC_T)();
typedef uint32_t (*SENSOR_UPDATE_PROC_T)();

typedef struct
{
    ST_SENSOR_DATA_T sensorInfo;
    SENSOR_INIT_PROC_T initProc;
    SENSOR_UPDATE_PROC_T updateProc;
} ST_SENSOR_INFO_T;

void _battery_vol_init();
uint32_t _battery_vol_update();

ST_SENSOR_INFO_T sensorList[] =
{
    {
        {0x46, 2, 0},           //0x46=voltage mv
        _battery_vol_init,
        _battery_vol_update
    }, 
};

uint32_t totalSensorNum = 0;

void app_sensor_data_init()
{
    int i;
    
    totalSensorNum = sizeof(sensorList) / sizeof(ST_SENSOR_INFO_T);

    for(i = 0; i < totalSensorNum; i++)
    {
        if(sensorList[i].initProc)
            (sensorList[i].initProc)();
    }
}

uint32_t app_sensor_num()
{
    return totalSensorNum;
}

void app_sensor_data_update(uint32_t index)
{
    if((index < totalSensorNum) && (sensorList[index].updateProc))
    {
        sensorList[index].sensorInfo.data = (sensorList[index].updateProc)();
    }
}

ST_SENSOR_DATA_T* app_sensor_data_get(uint32_t index)
{
    if(index < totalSensorNum)
        return &(sensorList[index].sensorInfo);
    return NULL;
}

void _battery_vol_init()
{
    static uint8_t inited = 0;

    if(inited)
        return;

    wise_asaradc_init();
    wise_asaradc_config(ASARADC_VREF_1P6V);

    inited = 1;
}

uint32_t _battery_vol_update()
{
    uint16_t sadcVal = 0;
    static uint16_t lastSadcVal = 0;

    if(WISE_SUCCESS == wise_asaradc_read_input(ASARADC_VIN_BATTERY, &sadcVal))
    {
        lastSadcVal = sadcVal;
        return sadcVal;
    }

    debug_print("battery adc read failed, keep last=%04x\n", lastSadcVal);
    
    return lastSadcVal;
}
