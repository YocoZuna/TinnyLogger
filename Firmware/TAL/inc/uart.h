#ifndef TAL_BOARD_UART_H
#define TAL_BOARD_UART_H

#ifdef TINNY_LOGGER
	#include "board/TinnyLogger/uart.h"
#endif

/* UART Status types derived from HAL_StatusTypeDef */
typedef enum
{
  UART_OK       = 0x00,
  UART_ERROR    = 0x01,
  UART_BUSY     = 0x02,
  UART_TIMEOUT  = 0x03,
  UART_INVALID_PARAM = 0x04,
} uart_status_t;

uart_status_t uart_init(uart_bus_t *bus, const uart_cfg_t *cfg);
uart_status_t uart_tx(uart_bus_t *bus, const uint8_t *data, size_t len, uint32_t timeout);
uart_status_t uart_rx(uart_bus_t *bus, uint8_t *data, size_t len, uint32_t timeout);
uart_status_t uart_deinit(uart_bus_t *bus);

void uart_select(const uart_bus_t *bus);
void uart_deselect(const uart_bus_t *bus);

#endif /*TAL_BOARD_UART_H*/


