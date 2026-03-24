#pragma once

#include "sd_commands.h"
#include "spi.h"
#include "stm32l432xx.h"
#include <stddef.h>

#define SD_CMD0_CRC 0x95
#define SD_CMD8_CRC 0x87
#define SD_DUMMY_CRC 0x01
#define DUMMY_CLOCKS 76
#define DUMMY_BYTE 0xFF

static SPI_TypeDef *SD_SPI = NULL;
void sd_card_init(SPI_TypeDef *SPIx);
//inline _Bool sd_card_check_presence();
/*
    @brief Sends a command to the SD card over SPI. This function constructs the command packet according to the SD card protocol and transmits it using the SPI interface.
    @param cmd: The command index (e.g., CMD0, CMD8) to be sent to the SD card.
    @param arg: The argument associated with the command, which may be required for certain commands (e.g., CMD8 requires an argument to specify voltage range).

    0 CMD ARGUMENT CRC END_BIT
*/
void sd_card_send_command(uint8_t cmd, uint32_t arg);

static inline void send_dummy_clocks(){
    uint8_t dummy_buffer[DUMMY_CLOCKS];
    for(int i = 0; i < DUMMY_CLOCKS; i++){
        dummy_buffer[i] = DUMMY_BYTE;
    }
    spi_tx(SD_SPI, dummy_buffer, DUMMY_CLOCKS);
}


//Disable optimalization, compiler can optimazie out this function
#pragma GCC push_options
#pragma GCC optimize ("O0")
static inline void sd_delay(uint32_t ms){
    
    uint32_t delay_cycles = (4000000/1000); // Approximate delay loop, adjust as needed for timing accuracy
    for(volatile uint32_t i = 0; i < ms * delay_cycles; i++)
    {
        __NOP(); // No operation, just waste time
    }
}
#pragma GCC pop_options