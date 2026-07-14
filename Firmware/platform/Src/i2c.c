#include "i2c.h"

I2C_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c, I2C_TypeDef *I2C, I2C_InitTypeDef *i2c_init_struct)
{
    hi2c->Instance = I2C;
    hi2c->Init = *i2c_init_struct;
    return  (I2C_StatusTypeDef)HAL_I2C_Init(hi2c);
}
I2C_StatusTypeDef i2c_tx(I2C_HandleTypeDef *I2C,const uint8_t slaveAddr, uint8_t *data,size_t len,uint32_t timeout)
{
    return  (I2C_StatusTypeDef)HAL_I2C_Master_Transmit(I2C, slaveAddr, data, len, timeout);
}
I2C_StatusTypeDef i2c_rx(I2C_HandleTypeDef *I2C,const uint8_t slaveAddr, uint8_t * data, size_t len,uint32_t timeout){

    return  (I2C_StatusTypeDef)HAL_I2C_Master_Receive(I2C, slaveAddr, data, len, timeout);
}
I2C_StatusTypeDef i2c_write_mem(I2C_HandleTypeDef *I2C, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t len, uint32_t timeout){
    
    return  (I2C_StatusTypeDef)HAL_I2C_Mem_Write(I2C, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, len, timeout);
}
I2C_StatusTypeDef i2c_read_mem(I2C_HandleTypeDef *I2C, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t len, uint32_t timeout){
    
    return  (I2C_StatusTypeDef)HAL_I2C_Mem_Read(I2C, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, len, timeout);
}
I2C_StatusTypeDef i2c_deinit(I2C_HandleTypeDef *I2C)
{
    return  (I2C_StatusTypeDef)HAL_I2C_DeInit(I2C);
}


