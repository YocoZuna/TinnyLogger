#pragma once
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"
#include <stddef.h>
#include <stdint.h>
#include "time.h"

#define I2C_PORT I2C1
#define SCL_PIN LL_GPIO_PIN_9
#define SDA_PIN LL_GPIO_PIN_10

void i2c_init(I2C_TypeDef *I2C, LL_I2C_InitTypeDef *i2c_init_struct);
int8_t i2c_tx(I2C_TypeDef *I2C,const uint8_t slaveAddr, const uint8_t *data,size_t len,uint32_t timeout);
int8_t i2c_rx(I2C_TypeDef *I2C,const uint8_t slaveAddr, uint8_t * data, size_t len,uint32_t timeout);
void i2c_deinit(I2C_TypeDef *I2C);

