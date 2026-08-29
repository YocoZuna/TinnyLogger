#include "uart.h"
#include <string.h>


uart_status_t uart_init(uart_bus_t *bus, const uart_cfg_t *cfg)
{
    if (bus == NULL || cfg == NULL) return UART_INVALID_PARAM;

	bus->huart.Instance = &bus->instance;
    bus->huart.Init.BaudRate= cfg->baudrate;
    bus->huart.Init.WordLength = cfg->word_length;
    bus->huart.Init.StopBits = cfg->stop_bits;
    bus->huart.Init.Parity = cfg->parity;
    bus->huart.Init.Mode = cfg->mode;
    bus->huart.Init.HwFlowCtl = cfg->hw_flow_ctl;
    bus->huart.Init.OverSampling = cfg->over_sampling;
    bus->huart.Init.OneBitSampling = cfg->one_bit_sampling;
    #if defined(USART_PRESC_PRESCALER)
    bus->huart.Init.ClockPrescaler = cfg->clock_prescaler;
    #endif
    return (uart_status_t)HAL_UART_Init(&bus->huart);
}

uart_status_t uart_tx(uart_bus_t *bus, const uint8_t *data, size_t len, uint32_t timeout)
{
    if(bus==NULL || data==NULL || len==0) return UART_INVALID_PARAM;
    return (uart_status_t)HAL_UART_Transmit(&bus->huart, (uint8_t*)data, len, timeout);

}

uart_status_t uart_rx(uart_bus_t *bus, uint8_t *data, size_t len, uint32_t timeout)
{
    if(bus==NULL || data==NULL || len==0) return UART_INVALID_PARAM;
    memset((void*)data, 0xFF, len);
    return (uart_status_t)HAL_UART_Receive(&bus->huart, data, len, timeout);

}
uart_status_t uart_deinit(uart_bus_t *bus)
{
    if(bus==NULL) return UART_INVALID_PARAM;
    return (uart_status_t)HAL_UART_DeInit(&bus->huart);
}

