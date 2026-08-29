#ifndef TAL_BOARD_SPI_H
#define TAL_BOARD_SPI_H

#ifdef TINNY_LOGGER
	#include "board/TinnyLogger/spi.h"
#endif

/* SPI Status types derived from HAL_StatusTypeDef */
typedef enum
{
  SPI_OK       = 0x00,
  SPI_ERROR    = 0x01,
  SPI_BUSY     = 0x02,
  SPI_TIMEOUT  = 0x03,
  SPI_INVALID_PARAM = 0x04,
} spi_status_t;

spi_status_t spi_init(spi_bus_t *bus, const spi_cfg_t *cfg);
spi_status_t spi_tx(spi_bus_t *bus, const uint8_t *data, size_t len, uint32_t timeout);
spi_status_t spi_rx(spi_bus_t *bus, uint8_t *data, size_t len, uint32_t timeout);
spi_status_t spi_tx_rx(spi_bus_t *bus,const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
spi_status_t spi_deinit(spi_bus_t *bus);

void spi_select(const spi_bus_t *bus);
void spi_deselect(const spi_bus_t *bus);

#endif /*TAL_BOARD_SPI_H*/


