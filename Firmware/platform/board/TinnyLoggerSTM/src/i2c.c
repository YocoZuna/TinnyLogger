#include "i2c.h"
#include "board_config.h"

static I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef board_init_i2c(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);

    __HAL_RCC_I2C1_CLK_ENABLE();
    I2C_InitTypeDef i2c_init_struct = {
        .Timing = 0x00100D14,
        .OwnAddress1 = 0,
        .AddressingMode = I2C_ADDRESSINGMODE_7BIT,
        .DualAddressMode = I2C_DUALADDRESS_DISABLE,
        .OwnAddress2 = 0,
        .GeneralCallMode = I2C_GENERALCALL_DISABLE,
        .NoStretchMode = I2C_NOSTRETCH_DISABLE
    };

    hi2c1.Instance = I2C_INSTANCE;
    hi2c1.Init = i2c_init_struct;
    return HAL_I2C_Init(&hi2c1);
}

HAL_StatusTypeDef board_i2c_tx(const uint8_t slaveAddr, uint8_t *data,size_t size,uint32_t timeout)
{
    return HAL_I2C_Master_Transmit(&hi2c1, slaveAddr, data, size, timeout);
}
HAL_StatusTypeDef board_i2c_rx(const uint8_t slaveAddr, uint8_t * data, size_t size,uint32_t timeout){

    return HAL_I2C_Master_Receive(&hi2c1, slaveAddr, data, size, timeout);
}
HAL_StatusTypeDef board_i2c_write_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    
    return HAL_I2C_Mem_Write(&hi2c1, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, size, timeout);
}
HAL_StatusTypeDef board_i2c_read_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    
    return HAL_I2C_Mem_Read(&hi2c1, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, size, timeout);
}
HAL_StatusTypeDef board_i2c_deinit()
{
    return HAL_I2C_DeInit(&hi2c1);
}


