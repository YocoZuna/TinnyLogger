#include "i2c.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_i2c.h"



void i2c_init(I2C_TypeDef *I2C, LL_I2C_InitTypeDef *i2c_init_struct)
{
    LL_I2C_Disable(I2C);
    if (I2C == I2C1) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    else if (I2C == I2C2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
    
    LL_I2C_Init(I2C, i2c_init_struct);
    LL_I2C_EnableClockStretching(I2C); // Eanbleing clock stretching for slave devices that need more time to process data
    LL_I2C_Enable(I2C);
}

void i2c_tx_byte(I2C_TypeDef *I2C,const uint8_t slaveAddr,const uint8_t memAddr, const uint8_t data){
    
    while(!LL_I2C_IsActiveFlag_BUSY(I2C));
    LL_I2C_HandleTransfer(I2C, slaveAddr,LL_I2C_ADDRSLAVE_7BIT, sizeof(data),LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    while(!LL_I2C_IsActiveFlag_TXIS(I2C));
    LL_I2C_TransmitData8(I2C, data);
    while(!LL_I2C_IsActiveFlag_STOP(I2C));
    LL_I2C_ClearFlag_STOP(I2C);
}
void i2c_rx_byte(I2C_TypeDef *I2C,const uint8_t slaveAddr, const uint8_t memAddr, uint8_t * const data){
    while(!LL_I2C_IsActiveFlag_BUSY(I2C));
    LL_I2C_HandleTransfer(I2C, slaveAddr,LL_I2C_ADDRSLAVE_7BIT, sizeof(data),LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    while(!LL_I2C_IsActiveFlag_RXNE(I2C));
    *data = LL_I2C_ReceiveData8(I2C);
    while(!LL_I2C_IsActiveFlag_STOP(I2C));
    LL_I2C_ClearFlag_STOP(I2C);
}

void i2c_deinit(I2C_TypeDef *I2C)
{
    if (I2C == I2C1) LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
    else if (I2C == I2C2) LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C2);
    LL_I2C_Disable(I2C);
}