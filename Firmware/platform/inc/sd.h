#ifndef PLATFORM_SD_H
#define PLATFORM_SD_H

#if defined(TINNY_LOGGER_STM)
	#include "board/board_sd.h"
#endif

#include "error.h"
error_status_t P_SD_init();
error_status_t P_SD_tx_rx(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
error_status_t P_SD_tx_byte(const uint8_t *tx_buffer,size_t size, uint32_t timeout);
error_status_t P_SD_rx_byte(uint8_t *rx_buffer,size_t size,uint32_t timeout);
error_status_t P_SD_deselect();
error_status_t P_SD_select();
void P_SD_delay(uint32_t delay);
void P_SD_change_CLK_after_init();

#endif /*PLATFORM_SD_H*/


