#include "spi.h"
#include "error.h"


error_status_t P_spi_init(){
	return map_hal_status_to_error(board_init_spi());
}
error_status_t P_spi_tx(const uint8_t *data, size_t size, uint32_t timeout){
	if(data==NULL || size==0) return ERROR_INVALID_PARAM;
	return map_hal_status_to_error(board_spi_tx(data, size, timeout));
}
error_status_t P_spi_rx(uint8_t *data, size_t size, uint32_t timeout){
	if(data==NULL || size==0) return ERROR_INVALID_PARAM;
	return map_hal_status_to_error(board_spi_rx(data, size, timeout));
}
error_status_t P_spi_tx_rx(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout){
	if(tx_buffer==NULL || rx_buffer==0 || size==0) return ERROR_INVALID_PARAM;
	return map_hal_status_to_error(board_spi_tx_rx(tx_buffer,rx_buffer, size, timeout));
}
error_status_t P_spi_deinit(){
	return map_hal_status_to_error(board_spi_deinit());
}

void P_spi_select(const spi_bus_t *cs){
	board_spi_select(cs);
}
void P_spi_deselect(const spi_bus_t *cs){
	board_spi_deselect(cs);
}
