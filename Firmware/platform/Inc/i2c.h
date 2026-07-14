#pragma once
#include "stm32l4xx_hal_i2c.h"
#include <stdint.h>
#include "time.h"

/* TODO this shoould be moved*/
#define SCL_PIN LL_GPIO_PIN_9
#define SDA_PIN LL_GPIO_PIN_10

/* I2C Status types derived from HAL_StatusTypeDef */
typedef enum
{
  I2C_OK       = 0x00,
  I2C_ERROR    = 0x01,
  I2C_BUSY     = 0x02,
  I2C_TIMEOUT  = 0x03
} I2C_StatusTypeDef;

I2C_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c, I2C_TypeDef *I2C, I2C_InitTypeDef *i2c_init_struct);
I2C_StatusTypeDef i2c_tx(I2C_HandleTypeDef *I2C,const uint8_t slaveAddr, uint8_t *data,size_t len,uint32_t timeout);
I2C_StatusTypeDef i2c_rx(I2C_HandleTypeDef *I2C,const uint8_t slaveAddr, uint8_t * data, size_t len,uint32_t timeout);
I2C_StatusTypeDef i2c_write_mem(I2C_HandleTypeDef *I2C, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t len, uint32_t timeout);
I2C_StatusTypeDef i2c_read_mem(I2C_HandleTypeDef *I2C, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t len, uint32_t timeout);
I2C_StatusTypeDef i2c_deinit(I2C_HandleTypeDef *I2C);

