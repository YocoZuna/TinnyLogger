#pragma once

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"

void uart_init(USART_TypeDef *USART, LL_USART_InitTypeDef *uart_init_struct);
void uart_tx_byte(USART_TypeDef *USART, const uint8_t data);
void uart_rx_byte(USART_TypeDef *USART, uint8_t *data);
void uart_deinit(USART_TypeDef *USART);