#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_I2C_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_I2C_H

#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_gpio.h"
#include <stdint.h>
#include <stddef.h>

/* I2C SPI pins*/
#define SCL_PIN GPIO_PIN_9
#define SDA_PIN GPIO_PIN_10

typedef struct
{
	I2C_HandleTypeDef hi2c;
	I2C_TypeDef instance;
} i2c_bus_t;

typedef struct
{
  uint32_t mode;
  uint32_t timing; 
  uint32_t own_address_low;    
  uint32_t addressing_mode;
  uint32_t dual_address_mode;  
  uint32_t own_address_high;
  uint32_t own_address_high_masks; 
  uint32_t general_call_mode;
  uint32_t no_stretch_mode;
} i2c_cfg_t;
#endif