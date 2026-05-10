#ifndef ITF_AHT20_H
#define ITF_AHT20_H

#include <stdint.h>
#include "AHT20_typedefs.h"
typedef struct itf_AHT20 itf_AHT20_t;

typedef AHT20_Status_t(*SENSOR_delay_t)(uint32_t ms);
typedef AHT20_Status_t(*SENSOR_read_t)(uint8_t deviceAddres,uint8_t* data,uint8_t len);
typedef AHT20_Status_t(*SENSOR_write_t)(uint8_t deviceAddres,uint8_t* data,uint8_t len);

/// @brief 
struct itf_AHT20
{
    SENSOR_delay_t delay;
    SENSOR_read_t  read;
    SENSOR_write_t write;

};


AHT20_Status_t __attribute__((weak)) itf_AHT20_delay(uint32_t ms);
AHT20_Status_t __attribute__((weak)) itf_AHT20_read(uint8_t deviceAddres,uint8_t* data,uint8_t len);
AHT20_Status_t __attribute__((weak)) itf_AHT20_write(uint8_t deviceAddres,uint8_t* data,uint8_t len);
#endif