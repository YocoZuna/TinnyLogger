#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_SPI_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_SPI_H


#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_gpio.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{
	SPI_HandleTypeDef hspi;
	SPI_TypeDef instance;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
} spi_bus_t;

typedef struct
{
	uint32_t mode;
	uint32_t direction;
	uint32_t data_size;
	uint32_t clock_polarity;
	uint32_t clock_phase;
	uint32_t nss;
	uint32_t baudrate_prescaler;
	uint32_t first_bit;
} spi_cfg_t;

#endif