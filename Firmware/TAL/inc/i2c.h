#ifndef TAL_BOARD_I2C_H
#define TAL_BOARD_I2C_H

#ifdef TINNY_LOGGER
	#include "board/TinnyLogger/i2c.h"
#endif

/* I2C Status types derived from HAL_StatusTypeDef */
typedef enum
{
  I2C_OK       = 0x00,
  I2C_ERROR    = 0x01,
  I2C_BUSY     = 0x02,
  I2C_TIMEOUT  = 0x03,
  I2C_INVALID_PARAM = 0x04
} i2c_status_t;

i2c_status_t i2c_init(i2c_bus_t *bus, i2c_cfg_t *cfg);
i2c_status_t i2c_tx(i2c_bus_t *bus,const uint8_t slaveAddr, uint8_t *data,size_t size,uint32_t timeout);
i2c_status_t i2c_rx(i2c_bus_t *bus,const uint8_t slaveAddr, uint8_t * data, size_t size,uint32_t timeout);
i2c_status_t i2c_write_mem(i2c_bus_t *bus, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
i2c_status_t i2c_read_mem(i2c_bus_t *bus, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
i2c_status_t i2c_deinit(i2c_bus_t *bus);


#endif /*TAL_BOARD_I2C_H*/


