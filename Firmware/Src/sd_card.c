#include "sd_card.h"
#include "spi.h"
#include "stm32l432xx.h"
#include "stm32l4xx_ll_gpio.h"

void sd_card_init(SPI_TypeDef *SPIx)
{
    if(SPIx ==NULL) return;
    SD_SPI = SPIx;
    
    sd_delay(1);

    // Deselect the SD card by setting CS high
    spi_deselect(CS_PORT, CS_PIN);
    // Send 80 dummy clocks to initialize the SD card
    send_dummy_clocks();
    // Send CMD0 to reset the SD card
    spi_select(CS_PORT, CS_PIN);
    sd_card_send_command(CMD0, 0);
    // Wait for the SD card to enter idle state (response should be 0x01)
    uint8_t response;
    do {
        spi_rx_byte(SD_SPI, &response);
    } while (response != 0x01);
    // Send CMD8 to check voltage range and SD card version
    sd_card_send_command(CMD8, 0x1AA); // Argument for CMD8: 0x1AA (check voltage range)
    // Wait for the response to CMD8 (should be 0x01 for valid SD card)
    uint8_t r7_response[5];
    do {
        spi_rx(SD_SPI, r7_response, 5);
    } while (r7_response[0] != 0x01);

}

void sd_card_send_command(uint8_t cmd, uint32_t arg)
{
    uint8_t command_packet[6];
    command_packet[0] = cmd;
    command_packet[1] = (arg >> 24) & 0xFF; 
    command_packet[2] = (arg >> 16) & 0xFF; 
    command_packet[3] = (arg >> 8) & 0xFF; 
    command_packet[4] = arg & 0xFF; 

    // Calculate CRC for CMD0 and CMD8, otherwise use dummy CRC
    if (cmd == CMD0) {
        command_packet[5] = SD_CMD0_CRC;
    } else if (cmd == CMD8) {
        command_packet[5] = SD_CMD8_CRC;
    } else {
        command_packet[5] = SD_DUMMY_CRC; // Dummy CRC for other commands
    }

    spi_tx(SD_SPI, command_packet, sizeof(command_packet));
}