#include "sd_card.h"
#include "sd_commands.h"
#include "spi.h"
#include "stm32l432xx.h"
#include "stm32l4xx.h"
#include "stm32l4xx_ll_gpio.h"
#include <stdint.h>
#include <string.h>

#define TIMEOUT 10

static uint8_t response_R1;
static uint8_t response_R7_R3[4];

static SPI_TypeDef *SD_SPI = NULL;

static void sd_delay(uint32_t ms);
static void sd_deselect();
static void sd_select();
static void sd_power_on();
static bool sd_read_single_block(uint8_t* buff,size_t len);
void sd_card_init(SPI_TypeDef *SPIx)
{
    if(SPIx ==NULL) return;
    SD_SPI = SPIx;
    
    sd_power_on();

    sd_select();
    
    // Send CMD0 to reset the SD card
    response_R1 = sd_card_send_command(CMD0, 0);

    if (response_R1!=0x01) return; 

    // Send CMD8 to check voltage range and SD card version
    response_R1 = sd_card_send_command(CMD8, 0x1AA); // Argument for CMD8: 0x1AA (check voltage range)
    //TODO Add handling for v1 and MMC

    for(uint8_t i=0;i<sizeof(response_R7_R3);i++){
        spi_rx_byte(SD_SPI, &response_R7_R3[i]);
    }

    if (response_R7_R3[2] != 0x01 || response_R7_R3[3] != 0xAA) {
        // Invalid voltage range, SD card may not be compatible

        sd_deselect();
        return;
    }

    //Send CMD 55

    do {
        
        (void)sd_card_send_command(CMD55, 0);

        response_R1 = sd_card_send_command(ACMD41, 0x40000000); // Argument 0x40000000 to indicate host supports SDHC/SDXC     

    } while (response_R1!= 0x00);
    sd_card_send_command(CMD58, 0);  // Argument 0 dla CMD58

    for(uint8_t i=0;i<sizeof(response_R7_R3);i++){
        spi_rx_byte(SD_SPI, &response_R7_R3[i]);
    }
    sd_deselect();
}


ErrorStatus sd_card_send_command(uint8_t cmd, uint32_t arg)
{
    uint8_t command_packet[CMD_SIZE];
    uint8_t response_buffer[CMD_SIZE];
    uint8_t response = 0xFF;
    command_packet[0] = cmd;
    command_packet[1] = (arg >> 24) & 0xFF; 
    command_packet[2] = (arg >> 16) & 0xFF; 
    command_packet[3] = (arg >> 8) & 0xFF; 
    command_packet[4] = arg & 0xFF; 

    // Calculate CRC for CMD0 and CMD8, otherwise use dummy CRC
    command_packet[5] = (cmd == CMD0) ? SD_CMD0_CRC : (cmd == CMD8) ? SD_CMD8_CRC : SD_DUMMY_CRC;

    spi_tx_rx(SD_SPI, command_packet, response_buffer, CMD_SIZE);

    if(cmd==CMD12) spi_rx_byte(SD_SPI, response_buffer);
    do{
        
        spi_rx_byte(SD_SPI, &response);
    }while(response&0x80);

    return response;
}

uint8_t SD_disk_read(uint8_t pdr,uint8_t* buff,uint8_t sector,uint8_t count){

    sd_select();
    if(count==1){
        if((sd_card_send_command(CMD17, sector)==0)&&sd_read_single_block(buff,512)) count = 0 ;
    }
    else{
        if (sd_card_send_command(CMD18, sector)==0){
            do{
                if(!sd_read_single_block(buff, 512)) break;
                buff+=512
            }while(--count);

            sd_card_send_command(CMD12, 0);
        }
    }   
    sd_deselect();

    return count;
}

static bool sd_read_single_block(uint8_t* buff,size_t len)
{
    uint8_t retires= 200;
    uint8_t token;
    

    do{
        spi_rx_byte(SD_SPI, &token);
        
    }while(token==0xFF && retires--);
    
    if(token!=0xFE) return FALSE;

    uint8_t dummy_buff[512];
    memset(dummy_buff,SD_DUMMY_BYTE,SD_SECTOR_SIZE);

    spi_tx_rx(SD_SPI, dummy_buff, buff, SD_SECTOR_SIZE);

    //We can ommit CRC for now :D 
    spi_rx_byte(SD_SPI, &token);
    spi_rx_byte(SD_SPI, &token);

    return TRUE;
}
static void sd_delay(uint32_t ms){
    
    time_1ms_delay(ms);
}
static void sd_deselect(){
    spi_deselect(CS_PORT, CS_PIN);
    spi_tx_byte(SD_SPI, 0xFF);
}
static void sd_select(){
    spi_select(CS_PORT, CS_PIN);
    time_1ms_delay(1);
}
static void sd_power_on(){

    sd_delay(1000); // Delay to allow SD card to power up and stabilize

    do{
    sd_deselect();

    // Send 15*8 dummy clocks to initialize the SD card
    for(int i=0;i<DUMMY_CLOCKS;i++)
    {
       spi_tx_byte(SD_SPI, SD_DUMMY_BYTE); 
    }
    sd_select();
    
    // Send CMD0 to reset the SD card
    }while(sd_card_send_command(CMD0, 0)!=0x01);

    sd_deselect();
}