#include "i2c.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_i2c.h"
#include <sys/types.h>



void i2c_init(I2C_TypeDef *I2C, LL_I2C_InitTypeDef *i2c_init_struct)
{
    LL_I2C_Disable(I2C);
    if (I2C == I2C1) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    LL_I2C_Init(I2C, i2c_init_struct);
    LL_I2C_Enable(I2C);
}

static int8_t i2c_abort(I2C_TypeDef *I2C, uint32_t timeout) {
    
    LL_I2C_GenerateStopCondition(I2C);
    uint32_t start_time = getTime();
    while (!LL_I2C_IsActiveFlag_STOP(I2C)) {
        if (time_expired(&start_time, timeout)) {
            return -3; // Timeout
        }
    }
    LL_I2C_ClearFlag_NACK(I2C);
    LL_I2C_ClearFlag_STOP(I2C);
    return 1;
}

int8_t i2c_tx(I2C_TypeDef *I2C,const uint8_t slaveAddr, const uint8_t *data,size_t len,uint32_t timeout){
    
    uint32_t start_time = getTime();
    
    while(LL_I2C_IsActiveFlag_BUSY(I2C)){
        if(time_expired(&start_time, timeout)) {
            return -3; 
        }
    };
    LL_I2C_HandleTransfer(I2C, slaveAddr<<1,LL_I2C_ADDRSLAVE_7BIT, len,LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    
    for(size_t i=0;i<len;i++){
        start_time = getTime();
        if(data==NULL && len>0) return -5; // Avoid sending 0x00 as it can be interpreted as a NACK in some cases
        
        while(!LL_I2C_IsActiveFlag_TXIS(I2C)){
            //Check if NACK was received
            if(LL_I2C_IsActiveFlag_NACK(I2C)){
                i2c_abort(I2C,timeout); // Abort the transfer
                return -1; // NACK received
            }
            if(time_expired(&start_time, timeout)) {
                i2c_abort(I2C,timeout); // Timeout
                return -3;
            }
        }
        LL_I2C_TransmitData8(I2C, data[i]);
    }
    start_time = getTime();
    while(!LL_I2C_IsActiveFlag_STOP(I2C)){
        if(time_expired(&start_time, timeout)) {
                i2c_abort(I2C,timeout); // Timeout
                return -3;
            }
    }
    LL_I2C_ClearFlag_STOP(I2C);

    return 1;
}

int8_t i2c_rx(I2C_TypeDef *I2C,const uint8_t slaveAddr, uint8_t * data, size_t len,uint32_t timeout){
    uint32_t start_time = getTime();
    while(LL_I2C_IsActiveFlag_BUSY(I2C)){
        if(time_expired(&start_time, timeout)) {
            return -3; 
        }
    }
    LL_I2C_HandleTransfer(I2C, slaveAddr<<1,LL_I2C_ADDRSLAVE_7BIT, len,LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    for(size_t i=0;i<len;i++){
        start_time = getTime();
        while(!LL_I2C_IsActiveFlag_RXNE(I2C)){
                //Check if NACK was received
                if(LL_I2C_IsActiveFlag_NACK(I2C)){
                    i2c_abort(I2C,timeout); // Abort the transfer
                    return -1; // NACK received
                }
                if(time_expired(&start_time, timeout)) {
                    i2c_abort(I2C,timeout); // Timeout
                    return -3;
                }
            }
        
        data[i] = LL_I2C_ReceiveData8(I2C);
    }
    
    start_time = getTime();
    while(!LL_I2C_IsActiveFlag_STOP(I2C)){
        if(time_expired(&start_time, timeout)) {
                i2c_abort(I2C,timeout); // Timeout
                return -3;
            }
    };
    LL_I2C_ClearFlag_STOP(I2C);

    return 1;
}

void i2c_deinit(I2C_TypeDef *I2C)
{
    if (I2C == I2C1) LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
    LL_I2C_Disable(I2C);
}

