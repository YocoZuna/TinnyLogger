#include "spi.h"
#include "error.h"

extern spi_bus_t spi1;

error_status_t P_spi_init(){return ERROR_NOK;}
error_status_t P_spi_tx(const uint8_t *data, size_t len, uint32_t timeout){return ERROR_NOK;}
error_status_t P_spi_rx(uint8_t *data, size_t len, uint32_t timeout){return ERROR_NOK;}
error_status_t P_spi_tx_rx(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout){return ERROR_NOK;}
error_status_t P_spi_deinit(){return ERROR_NOK;}

void P_spi_select(const spi_bus_t *cs){}
void P_spi_deselect(const spi_bus_t *cs){}
