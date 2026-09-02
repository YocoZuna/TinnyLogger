#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_SPI_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_SPI_H


#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_rcc.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
} spi_bus_t;

HAL_StatusTypeDef board_init_spi(void);
HAL_StatusTypeDef board_spi_tx(const uint8_t *data, size_t len, uint32_t timeout);
HAL_StatusTypeDef board_spi_rx(uint8_t *data, size_t len, uint32_t timeout);
HAL_StatusTypeDef board_spi_tx_rx(const uint8_t *tx_buffer, uint8_t *rx_buffer, size_t size, uint32_t timeout);
HAL_StatusTypeDef board_spi_deinit(void);

void board_spi_select(const spi_bus_t *cs);
void board_spi_deselect(const spi_bus_t *cs);
#endif