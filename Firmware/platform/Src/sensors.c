#include "sensors.h"
#include "i2c.h"
#include "itf_AHT20.h"
#include "time.h"

AHT20_Status_t itf_AHT20_delay(uint32_t ms)
{
    time_1ms_delay(ms);
    return AHT20_OK;
}

AHT20_Status_t itf_AHT20_read(uint8_t deviceAddres,uint8_t* data,uint8_t len)
{
    i2c_rx(I2C_PORT, deviceAddres, data, len);

    return AHT20_OK;
}

AHT20_Status_t itf_AHT20_write(uint8_t deviceAddres,uint8_t* data,uint8_t len)
{
    
    i2c_tx(I2C_PORT, deviceAddres, data,len);
    
    
       
    
    return AHT20_OK;
}