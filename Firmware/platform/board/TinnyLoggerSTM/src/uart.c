#include "uart.h"
#include <string.h>
#include "board_config.h"

static UART_HandleTypeDef uart = {0};

HAL_StatusTypeDef board_init_uart()
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    uart.Instance = CONSOLE_UART_INSTANCE;
    uart.Init.BaudRate = 115200;
    uart.Init.WordLength = UART_WORDLENGTH_8B;
    uart.Init.StopBits = UART_STOPBITS_1;
    uart.Init.Parity = UART_PARITY_NONE;
    uart.Init.Mode = UART_MODE_TX_RX;
    uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart.Init.OverSampling = UART_OVERSAMPLING_16;
#if defined(USART_PRESC_PRESCALER)
    uart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
#endif
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = CONSOLE_TX_PIN | CONSOLE_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(CONSOLE_GPIO_PORT, &GPIO_InitStruct);

    return HAL_UART_Init(&uart);
}
HAL_StatusTypeDef board_uart_tx(const uint8_t *data, size_t len, uint32_t timeout)
{
    return HAL_UART_Transmit(&uart,data, len, timeout);

}

HAL_StatusTypeDef board_uart_rx(uint8_t *data, size_t len, uint32_t timeout)
{

    return HAL_UART_Receive(&uart, data, len, timeout);

}
HAL_StatusTypeDef board_uart_deinit()
{

    return HAL_UART_DeInit(&uart);
}

