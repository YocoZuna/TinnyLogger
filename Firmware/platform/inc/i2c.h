#ifndef PLATFORM_I2C_H
#define PLATFORM_I2C_H

#include <stdint.h>
#include <stddef.h>
#include "error.h"
#if defined(TINNY_LOGGER_STM)
	#include "board/i2c.h"
#endif

error_status_t P_i2c_init(void);
error_status_t P_i2c_tx(const uint8_t slaveAddr, uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_i2c_rx(const uint8_t slaveAddr, uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_i2c_write_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_i2c_read_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_i2c_deinit(void);


#endif /*PLATFORM_I2C_H*/


