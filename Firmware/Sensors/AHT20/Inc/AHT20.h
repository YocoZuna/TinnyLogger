#ifndef AHT20_H
#define AHT20_H

#ifdef __cplusplus
extern "C" {

#include "AHT20_typedefs.h"
#include "itf_AHT20.h"




struct AHT20{
    itf_AHT20_t interface;
    uint32_t tempRaw;
    float tempC;
    uint32_t humidRaw;
    float humidPercent;
};

#define CHECK_HOOK(ptr) do{                     \
                   if(ptr==0){                  \
                    return AHT20_ERROR_NULL_PTR; \
                    }                          \
                                                \
                }while(0)                       



AHT20_Status_t AHT20_Create(struct AHT20* handle,
                    SENSOR_delay_t delay,
                    SENSOR_read_t read,
                    SENSOR_write_t write);
AHT20_Status_t AHT20_RegisterHooks( struct AHT20* handle,
                                    SENSOR_delay_t delay,
                                    SENSOR_read_t read,
                                    SENSOR_write_t write);
AHT20_Status_t AHT20_Init(struct AHT20* sensor_handle);
AHT20_Status_t AHT20_DeInit(struct AHT20* sensor_handle);
void AHT20_ReadTempAndHum(struct AHT20* sensor_handle);
void AHT20_CalcTemp(struct AHT20* sensor_handle);
void AHT20_CalcTHumid(struct AHT20* sensor_handle);
void AHT20_SoftReset(struct AHT20* sensor_handle);
#ifdef UNIT_TESTS
#define STATIC_TESTABLE __attribute__((weak))
STATIC_TESTABLE AHT20_Status_t getStatusRegister(struct AHT20* handle,uint8_t* status);
STATIC_TESTABLE AHT20_Status_t calibrateSensor(struct AHT20* sensor_handle);
#else
#define STATIC_TESTABLE static 

#endif

}
#else

#include "AHT20_typedefs.h"
#include "itf_AHT20.h"


struct AHT20{
    itf_AHT20_t interface;
    uint32_t tempRaw;
    float tempC;
    uint32_t humidRaw;
    float humidPercent;
};

#define CHECK_HOOK(ptr) do{                     \
                   if(ptr==0){                  \
                    return AHT20_ERROR_NULL_PTR; \
                    }                          \
                                                \
                }while(0)                       



AHT20_Status_t AHT20_Create(struct AHT20* handle,
                    SENSOR_delay_t delay,
                    SENSOR_read_t read,
                    SENSOR_write_t write);
AHT20_Status_t AHT20_RegisterHooks( struct AHT20* handle,
                                    SENSOR_delay_t delay,
                                    SENSOR_read_t read,
                                    SENSOR_write_t write);
AHT20_Status_t AHT20_Init(struct AHT20* sensor_handle);
AHT20_Status_t AHT20_DeInit(struct AHT20* sensor_handle);
void AHT20_ReadTempAndHum(struct AHT20* sensor_handle);
void AHT20_CalcTemp(struct AHT20* sensor_handle);
void AHT20_CalcTHumid(struct AHT20* sensor_handle);
void AHT20_SoftReset(struct AHT20* sensor_handle);

#ifdef UNIT_TESTS
#define STATIC_TESTABLE __attribute__((weak))
STATIC_TESTABLE AHT20_Status_t getStatusRegister(struct AHT20* handle,uint8_t* status);
STATIC_TESTABLE AHT20_Status_t calibrateSensor(struct AHT20* sensor_handle);
#else
#define STATIC_TESTABLE static 
#endif

#endif
#endif // AHT20_H