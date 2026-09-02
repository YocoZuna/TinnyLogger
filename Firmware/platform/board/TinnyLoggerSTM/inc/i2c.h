#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_I2C_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_I2C_H

#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_rcc.h"
#include <stdint.h>
#include <stddef.h>

/* I2C SPI pins*/
#define SCL_PIN GPIO_PIN_9
#define SDA_PIN GPIO_PIN_10
#define I2C_GPIO_PORT GPIOB
#define I2C_INSTANCE I2C1

HAL_StatusTypeDef board_init_i2c();
HAL_StatusTypeDef board_i2c_tx(const uint8_t slaveAddr, uint8_t *data,size_t size,uint32_t timeout);
HAL_StatusTypeDef board_i2c_rx(const uint8_t slaveAddr, uint8_t * data, size_t size,uint32_t timeout);
HAL_StatusTypeDef board_i2c_write_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
HAL_StatusTypeDef board_i2c_read_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
HAL_StatusTypeDef board_i2c_deinit();

#endif // BOARD_TINNYLOGGER_TINNYLOGGERSTM_I2C_Hs