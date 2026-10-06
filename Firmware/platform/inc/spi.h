#ifndef PLATFORM_SPI_H
#define PLATFORM_SPI_H

#include <stdint.h>
#include <stddef.h>
#include "error.h"
#if defined(TINNY_LOGGER_STM)
	#include "board/board_spi.h"
#endif


error_status_t P_spi_init();
error_status_t P_spi_tx( const uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_spi_rx( uint8_t *data, size_t size, uint32_t timeout);
error_status_t P_spi_tx_rx(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
error_status_t P_spi_deinit();

void P_spi_select(const spi_slave_io_t *cs);
void P_spi_deselect(const spi_slave_io_t *cs);

#endif /*PLATFORM_P_SPI_H*/


