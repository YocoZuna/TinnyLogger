#ifndef BOARD_TINNYLOGGER_TINNYLOGGERSTM_SD_H
#define BOARD_TINNYLOGGER_TINNYLOGGERSTM_SD_H

#include <stdint.h>
#include <stddef.h>
#include "spi.h"


HAL_StatusTypeDef board_SD_init(void);
HAL_StatusTypeDef board_SD_tx_rx(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
HAL_StatusTypeDef board_SD_tx_byte(const uint8_t *tx_buffer,size_t size, uint32_t timeout);
HAL_StatusTypeDef board_SD_rx_byte(uint8_t *rx_buffer,size_t size,uint32_t timeout);
HAL_StatusTypeDef board_SD_deselect();
HAL_StatusTypeDef board_SD_select();

void board_SD_delay(uint32_t delay);


#endif