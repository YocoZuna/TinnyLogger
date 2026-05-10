#include "AHT20.h"
#include <stdio.h>



static uint8_t AHT20_CalcCRC(const uint8_t* data, uint8_t len)
{
    uint8_t initalValue = 0xFFu;
    uint8_t polynomial = 0x31u;
    for (int i = 0; i < len; i++)
    {
        if ((initalValue ^ data[i]) & 0x80)
        {
            initalValue = (initalValue << 1) ^ polynomial;
        }
        else
        {
            initalValue <<= 1;
        }
    }
    return initalValue;
}

STATIC_TESTABLE AHT20_Status_t getStatusRegister(struct AHT20* handle,uint8_t* status)
{

    AHT20_Status_t ret;
    uint8_t cmd[] = {READ_STATUS_REGISTER_CMD};
    ret = handle->interface.write(AHT20_ADDRESS,cmd,sizeof(cmd));
    if (ret != AHT20_OK)
    {
        return ret;
    }
    ret = handle->interface.read(AHT20_ADDRESS,status,1);
    return ret;
}
STATIC_TESTABLE AHT20_Status_t calibrateSensor(struct AHT20* sensor_handle)
{
    AHT20_Status_t ret = 0;
    uint8_t cmd[]  = {INIT_CALIB_CMD,INIT_CALIB_HIGH,INIT_CALIB_LOW};
    ret = sensor_handle->interface.write(AHT20_ADDRESS,cmd,sizeof(cmd));
    if (ret != AHT20_OK)
        return ret;
    sensor_handle->interface.delay(10);
    // Make sure that calibration is done, we have to read calibration bit
    
    uint8_t status = 0;
    while(!AHT20_CHECK_CALIBRATION(status)){
        sensor_handle->interface.delay(1);
        ret = getStatusRegister(sensor_handle,&status);
        if (ret!=AHT20_OK)
            return ret;
    }
    return ret;
}



/*
    * Create a new AHT20 sensor instance
    * @param handle Pointer to the sensor handle
    * @param delay Delay function pointer
    * @param read Read function pointer
    * @param write Write function pointer
    * @return AHT20_Status_t Status of the operation
*/
AHT20_Status_t AHT20_Create(struct AHT20* handle,
                    SENSOR_delay_t delay,
                    SENSOR_read_t read,
                    SENSOR_write_t write)
{

    AHT20_Status_t status = AHT20_RegisterHooks(handle,delay,read,write);
    if (status != AHT20_OK) {
        return status;
    }
    handle->interface.delay = delay;
    handle->interface.read = read;
    handle->interface.write = write;
    handle->humidPercent = 0;
    handle->humidRaw = 0;
    handle->tempC = 0;
    handle->tempRaw = 0;

    return AHT20_OK;
}
AHT20_Status_t AHT20_RegisterHooks( struct AHT20* handle,
                                    SENSOR_delay_t delay,
                                    SENSOR_read_t read,
                                    SENSOR_write_t write)
{
    CHECK_HOOK(delay);
    CHECK_HOOK(read);
    CHECK_HOOK(write);

    handle->interface.delay = delay;
    handle->interface.read = read;
    handle->interface.write = write;
    return AHT20_OK;



}

AHT20_Status_t AHT20_Init(struct AHT20* sensor_handle)
{
    AHT20_Status_t ret;
    CHECK_HOOK(sensor_handle->interface.delay);
    CHECK_HOOK(sensor_handle->interface.read);
    CHECK_HOOK(sensor_handle->interface.write);
    sensor_handle->interface.delay(40);
    uint8_t status = 0;
    ret = getStatusRegister(sensor_handle,&status);
    ret  = calibrateSensor(sensor_handle);
    //printf("Elcuacabra");
    if (ret!=AHT20_OK)
    {
        // Sensor is not calibrated, we need to calibrate it    
        return ret;
    }
    
    if (!AHT20_CHECK_CALIBRATION(status))
    {
        ret  = calibrateSensor(sensor_handle);
        if (ret!=AHT20_OK)
        {
            return ret;
        }
        return ret;
    }
    
    return ret;
}

AHT20_Status_t AHT20_DeInit(struct AHT20* sensor_handle)
{
    if(!sensor_handle) 
        return AHT20_ERROR_NULL_PTR;

    sensor_handle->interface.delay = 0;
    sensor_handle->interface.read = 0;
    sensor_handle->interface.write = 0;
    sensor_handle->humidPercent = 0;
    sensor_handle->humidRaw = 0;
    sensor_handle->tempC = 0;
    sensor_handle->tempRaw = 0;

    return AHT20_OK;
}


void AHT20_ReadTempAndHum(struct AHT20* sensor_handle)
{
    uint8_t ret = 0;
    uint8_t cmd[] = {TRIGGER_MEASURMENT_CMD,TRIGGER_MEASURMENT_HIGH,TRIGGER_MEASURMENT_LOW};
    sensor_handle->interface.write(AHT20_ADDRESS,cmd,sizeof(cmd));
    sensor_handle->interface.delay(80);
    while(AHT20_BUSY_IN_MEASURMENT(getStatusRegister(sensor_handle,&ret))){
        sensor_handle->interface.delay(1);
    }
    uint8_t rawData[7];
    sensor_handle->interface.read(AHT20_ADDRESS,rawData,7);
    sensor_handle->humidRaw = ((uint32_t)rawData[1] << 12) |
                          ((uint32_t)rawData[2] << 4)  |
                          ((uint32_t)(rawData[3] >> 4));  
    sensor_handle->tempRaw = ((uint32_t)(rawData[3] & 0x0F) << 16) |
                         ((uint32_t)rawData[4] << 8) |
                         (uint32_t)rawData[5];
    

    /* TODO add CRC check*/
}
/// @brief 
/// @param sensor_handle 
void AHT20_CalcTemp(struct AHT20* sensor_handle)
{
    sensor_handle->tempC = ((float)sensor_handle->tempRaw/(1<<20))*200.0f-50.0f ;
}

/// @brief 
/// @param sensor_handle 
void AHT20_CalcTHumid(struct AHT20* sensor_handle)
{
    sensor_handle->humidPercent = ((float)sensor_handle->humidRaw / (1<<20)) * 100.0f;
}

/**
    @brief  This command is used to restart the sensor
    system without turning the power off and on again. After
    receiving this command, the sensor system begins to
    re-initialize and restore the default setting state, and the
    time required for soft reset does not exceed 20 ms.

    @param sensor_handle[in] 
*/
void AHT20_SoftReset(struct AHT20* sensor_handle)
{
    uint8_t cmd[] = {SOFT_RESET_CMD};
    sensor_handle->interface.write(AHT20_ADDRESS,cmd,sizeof(cmd));
    sensor_handle->interface.delay(20);
}

