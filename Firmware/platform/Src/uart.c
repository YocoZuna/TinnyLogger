#include "uart.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_usart.h"


void uart_init(USART_TypeDef *USART, LL_USART_InitTypeDef *uart_init_struct){
    if (USART == USART1) {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    } else if (USART == USART2) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    }

    LL_USART_Init(USART, uart_init_struct);
    LL_USART_Enable(USART);
}
void uart_tx_byte(USART_TypeDef *USART, const uint8_t data){
    while (!LL_USART_IsActiveFlag_TXE(USART));
    LL_USART_TransmitData8(USART, data);
}
void uart_rx_byte(USART_TypeDef *USART, uint8_t *data){
    while (!LL_USART_IsActiveFlag_RXNE(USART));
    *data = LL_USART_ReceiveData8(USART);
}
void uart_deinit(USART_TypeDef *USART){
    LL_USART_Disable(USART);
    if (USART == USART1) {
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
    } else if (USART == USART2) {
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
    }
}