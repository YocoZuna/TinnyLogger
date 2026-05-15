#pragma once
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"
#include <stddef.h>
#include <stdint.h>




void i2c_init(I2C_TypeDef *I2C, LL_I2C_InitTypeDef *i2c_init_struct);
void i2c_tx_byte(I2C_TypeDef *I2C,const uint8_t slaveAddr,const uint8_t memAddr, const uint8_t data);
void i2c_rx_byte(I2C_TypeDef *I2C,const uint8_t slaveAddr, const uint8_t memAddr, const uint8_t data);
void i2c_deinit(I2C_TypeDef *I2C);

