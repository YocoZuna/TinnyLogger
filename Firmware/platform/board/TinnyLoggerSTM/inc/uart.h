#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_UART_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_UART_H


#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_dma.h"
#include "stm32l4xx_hal_rcc.h"
#include <stdint.h>
#include <stddef.h>

HAL_StatusTypeDef board_init_uart();
HAL_StatusTypeDef board_uart_tx(const uint8_t *data, size_t len, uint32_t timeout);
HAL_StatusTypeDef board_uart_rx(uint8_t *data, size_t len, uint32_t timeout);
HAL_StatusTypeDef board_uart_deinit();

#endif // BOARD_TINNYLOGGER_TINNYLOGGERSTM_UART_H