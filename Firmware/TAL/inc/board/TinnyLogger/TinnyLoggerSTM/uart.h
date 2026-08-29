#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_UART_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_UART_H


#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_gpio.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{
	UART_HandleTypeDef huart;
	USART_TypeDef instance;
} uart_bus_t;

typedef struct
{
	uint32_t baudrate;
	uint32_t word_length;
	uint32_t stop_bits;
	uint32_t parity;
	uint32_t mode;
	uint32_t hw_flow_ctl;
	uint32_t over_sampling;
	uint32_t one_bit_sampling;
	#if defined(USART_PRESC_PRESCALER)
    uint32_t clock_prescaler;
	#endif
}uart_cfg_t;

#endif