#ifndef __APP_SENSOR_DATA_H 
#define __APP_SENSOR_DATA_H

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t vif;
    uint8_t dif;
    uint32_t data;
} ST_SENSOR_DATA_T;

void app_sensor_data_init();
uint32_t app_sensor_num();
void app_sensor_data_update(uint32_t index);
ST_SENSOR_DATA_T* app_sensor_data_get(uint32_t index);

#endif

