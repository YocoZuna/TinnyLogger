#include "sd_card.h"
#include "spi.h"
#include "stm32l432xx.h"
#include "stm32l4xx_ll_gpio.h"
#include <string.h>

#define TIMEOUT 1000

static void sd_delay(uint32_t ms);

void sd_card_init(SPI_TypeDef *SPIx)
{
    if(SPIx ==NULL) return;
    SD_SPI = SPIx;
    
    sd_delay(1000)  ; // Delay to allow SD card to power up and stabilize

    spi_deselect(CS_PORT, CS_PIN);// Deselect the SD card by setting CS high

    // Send 80 dummy clocks to initialize the SD card
    send_dummy_clocks();
    
    // Select the SD card by setting CS low
    spi_select(CS_PORT, CS_PIN);
    
    // Send CMD0 to reset the SD card
    sd_card_send_command(CMD0, 0);

    // Wait for the SD card to enter idle state (response should be 0x01)
    uint8_t response;
    do {
        sd_card_receive_R1(&response, TIMEOUT);
    } while (response != 0x01);

    // Send CMD8 to check voltage range and SD card version
    sd_card_send_command(CMD8, 0x1AA); // Argument for CMD8: 0x1AA (check voltage range)
    
    uint8_t r7_response[4] = {0xFF};
    sd_card_receive_R7_R3(r7_response, sizeof(r7_response), TIMEOUT); // Receive R7 response (4 bytes)

    if (r7_response[2] != 0x01 || r7_response[3] != 0xAA) {
        // Invalid voltage range, SD card may not be compatible
        spi_deselect(CS_PORT, CS_PIN);
        return;
    }

    //Send CMD 55

    do {
    do {

        sd_card_send_command(CMD55, 0);
        sd_card_receive_R1(&response, TIMEOUT);
    } while (response != 0x01);


        sd_card_send_command(ACMD41, 0x0000000); // Argument 0x40000000 to indicate host supports SDHC/SDXC
        
        sd_card_receive_R1(&response, TIMEOUT);
    } while (response != 0x00);
    sd_card_send_command(CMD58, 0);  // Argument 0 dla CMD58
    uint8_t ocr_response[4];

    sd_card_receive_R7_R3(ocr_response, sizeof(ocr_response), TIMEOUT); // Receive R3 response (1 byte R1 + 4 bytes OCR)

}


ErrorStatus sd_card_send_command(uint8_t cmd, uint32_t arg)
{
    uint8_t command_packet[CMD_SIZE+1];
    command_packet[0] = cmd;
    command_packet[1] = (arg >> 24) & 0xFF; 
    command_packet[2] = (arg >> 16) & 0xFF; 
    command_packet[3] = (arg >> 8) & 0xFF; 
    command_packet[4] = arg & 0xFF; 

    // Calculate CRC for CMD0 and CMD8, otherwise use dummy CRC
    command_packet[5] = (cmd == CMD0) ? SD_CMD0_CRC : (cmd == CMD8) ? SD_CMD8_CRC : SD_DUMMY_CRC;

    if (spi_tx(SD_SPI, command_packet, sizeof(command_packet),TIMEOUT) != SUCCESS) {
        return ERROR; // Handle error
    }
    return SUCCESS;
}

ErrorStatus sd_card_receive_R1(uint8_t* response, uint32_t timeout)
{
    *response = SD_DUMMY_BYTE; // Initialize response to dummy value
    uint8_t dummy_byte = SD_DUMMY_BYTE;
    do {
        if (spi_tx_rx(SD_SPI, &dummy_byte, response, 1, timeout) != SUCCESS) {
            return ERROR;// Handle error
        }
    } while (*response == SD_DUMMY_BYTE); // Wait for a valid response (not 0xFF)
    return SUCCESS;
}

ErrorStatus sd_card_receive_R7_R3(uint8_t* responseBuffer,uint8_t buffer_size, uint32_t timeout)
{
    if (responseBuffer == NULL || buffer_size < CMD_SIZE) {
        return ERROR; // Invalid buffer
    }
    if (sd_card_receive_R1(responseBuffer, timeout) != SUCCESS) {
        return ERROR;// Handle error
    }

    uint8_t dummy_byte[CMD_SIZE-1] = {SD_DUMMY_BYTE,SD_DUMMY_BYTE,SD_DUMMY_BYTE,SD_DUMMY_BYTE};
    
    responseBuffer = responseBuffer + 1; // Move pointer to store data after R1 byte

    do {
        if (spi_tx_rx(SD_SPI, dummy_byte, responseBuffer, buffer_size, timeout) != SUCCESS) {
            return ERROR;// Handle error
        }
    } while (*responseBuffer == SD_DUMMY_BYTE); // Wait for a valid response (not 0xFF)

    return SUCCESS;
}
static void sd_delay(uint32_t ms){
    
    time_1ms_delay(ms);
}