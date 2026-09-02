#include "uart.h"
#include "error.h"

error_status_t P_uart_init(){
    return map_hal_status_to_error(board_init_uart());
}
error_status_t P_uart_tx(const uint8_t *tx_buffer,size_t size, uint32_t timeout){
    if(tx_buffer==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_uart_tx(tx_buffer,size,timeout));
}
error_status_t P_uart_rx(uint8_t *rx_buffer,size_t size,uint32_t timeout){
    if(rx_buffer==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_uart_rx(rx_buffer,size,timeout));
}
error_status_t P_uart_deinit(){
    return map_hal_status_to_error(board_uart_deinit());
}
