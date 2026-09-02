#ifndef PLATFORM_UART_H
#define PLATFORM_UART_H

#include <stdint.h>
#include <stddef.h>
#include "error.h"
#if defined(TINNY_LOGGER_STM)
	#include "board/uart.h"
#endif


error_status_t P_uart_init();
error_status_t P_uart_tx(const uint8_t *tx_buffer,size_t size, uint32_t timeout);
error_status_t P_uart_rx(uint8_t *rx_buffer,size_t size,uint32_t timeout);
error_status_t P_uart_deinit();

#endif /*PLATFORM_UART_H*/


