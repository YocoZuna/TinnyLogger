#pragma once

#include "sd_commands.h"
#include "spi.h"
#include "stm32l432xx.h"
#include <stddef.h>

#define SD_CMD0_CRC 0x95
#define SD_CMD8_CRC 0x87
#define DUMMY_CLOCKS 76
#define DUMMY_BYTE 0xFF

SPI_TypeDef *SD_SPI = NULL;

void sd_card_init(SPI_TypeDef *SPIx);
//inline _Bool sd_card_check_presence();
void sd_card_send_command(uint8_t cmd, uint32_t arg);

inline void send_dummy_clocks(){
    uint8_t dummy_buffer[DUMMY_CLOCKS];
    for(int i = 0; i < DUMMY_CLOCKS; i++){
        dummy_buffer[i] = DUMMY_BYTE;
    }
    spi_tx(SD_SPI, dummy_buffer, DUMMY_CLOCKS);
}