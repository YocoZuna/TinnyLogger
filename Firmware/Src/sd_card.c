#include "sd_card.h"
#include "sd_commands.h"
#include "spi.h"
#include "stm32l432xx.h"
#include "stm32l4xx.h"
#include "stm32l4xx_ll_gpio.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define TIMEOUT 10

static uint8_t response_R1;
static uint8_t response_R7_R3[4];
static SPI_TypeDef *SD_SPI = SPI3;


static void sd_delay(uint32_t ms);
static void sd_deselect();
static void sd_select();
static void sd_power_on();

static bool sd_read_single_block(uint8_t* buff,size_t len);
static bool sd_write_single_block(const uint8_t* buff,size_t len);
static bool sd_write_multiple_blocks(uint32_t sector_addr, const uint8_t* buff, size_t count);
int SD_disk_status()
{
    //TODO: Implement SD card status check
    return 0;
}
int SD_card_init(){


    sd_power_on();

    sd_select();
    
    // Send CMD0 to reset the SD card
    response_R1 = SD_card_send_command(CMD0, 0);

    if (response_R1!=0x01) return -3; 

    // Send CMD8 to check voltage range and SD card version
    response_R1 = SD_card_send_command(CMD8, 0x1AA); // Argument for CMD8: 0x1AA (check voltage range)
    //TODO Add handling for v1 and MMC

    for(uint8_t i=0;i<sizeof(response_R7_R3);i++){
        spi_rx_byte(SD_SPI, &response_R7_R3[i],TIMEOUT);
    }

    if (response_R7_R3[2] != 0x01 || response_R7_R3[3] != 0xAA) {
        // Invalid voltage range, SD card may not be compatible

        sd_deselect();
        return 3;
    }

    //Send CMD 55

    do {
        
        (void)SD_card_send_command(CMD55, 0);

        response_R1 = SD_card_send_command(ACMD41, 0x40000000); // Argument 0x40000000 to indicate host supports SDHC/SDXC     

    } while (response_R1!= 0x00);
    SD_card_send_command(CMD58, 0);  // Argument 0 dla CMD58

    for(uint8_t i=0;i<sizeof(response_R7_R3);i++){
        spi_rx_byte(SD_SPI, &response_R7_R3[i],TIMEOUT);
    }
    sd_deselect();

    return 0;
}


int SD_card_send_command(uint8_t cmd, uint32_t arg)
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

    spi_tx_rx(SD_SPI, command_packet, NULL, CMD_SIZE, TIMEOUT);

    if(cmd==CMD12) spi_rx_byte(SD_SPI, response_buffer,TIMEOUT);
    do{
        
        spi_rx_byte(SD_SPI, &response,TIMEOUT);
    }while(response&0x80);

    return response;
}

int SD_disk_read(uint8_t* buff,uint32_t sector,int count){

    sd_select();
    if(count==1){
        if((SD_card_send_command(CMD17, sector)==0)&&sd_read_single_block(buff,512)) count = 0 ;
    }
    else{
        if (SD_card_send_command(CMD18, sector)==0){
            do{
                if(!sd_read_single_block(buff, 512)) break;
                buff+=512;
            }while(--count);

            SD_card_send_command(CMD12, 0);
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
        spi_rx_byte(SD_SPI, &token,TIMEOUT);
        
    }while(token==0xFF && retires--);
    
    if(token!=0xFE) return false;

    uint8_t dummy_buff[512];
    memset(dummy_buff,SD_DUMMY_BYTE,SD_SECTOR_SIZE);

    spi_tx_rx(SD_SPI, dummy_buff, buff, SD_SECTOR_SIZE, TIMEOUT);

    //We can ommit CRC for now :D 
    spi_rx_byte(SD_SPI, &token,TIMEOUT);
    spi_rx_byte(SD_SPI, &token,TIMEOUT);

    return true;
}


static bool sd_write_single_block(const uint8_t* buff, size_t len) {
    uint8_t res;

    do {
        spi_rx_byte(SD_SPI, &res,TIMEOUT);
    } while (res != 0xFF);

    spi_tx_byte(SD_SPI, 0xFE,TIMEOUT);

    spi_tx_rx(SD_SPI, buff, NULL, len,TIMEOUT); 

    spi_tx_byte(SD_SPI, 0xFF,TIMEOUT);
    spi_tx_byte(SD_SPI, 0xFF,TIMEOUT);

    // 5. Odbierz Data Response Token
    spi_rx_byte(SD_SPI, &res,TIMEOUT);
    if ((res & 0x1F) != 0x05) {
        return false; 
    }

    do {
        spi_rx_byte(SD_SPI, &res,TIMEOUT);
    } while (res == 0x00);

    return true;
}

static bool sd_write_multiple_blocks(uint32_t sector_addr, const uint8_t*  buff, size_t count) {
    uint8_t res;

    if (SD_card_send_command(CMD25,sector_addr) != 0x00) return false;

    for (size_t i = 0; i < count; i++) {
        do {
            spi_rx_byte(SD_SPI, &res,TIMEOUT);
        } while (res != 0xFF);

        spi_tx_byte(SD_SPI, 0xFC,TIMEOUT);

        spi_tx_rx(SD_SPI, &buff[i * 512], NULL, 512,TIMEOUT);

        spi_tx_byte(SD_SPI, 0xFF,TIMEOUT);
        spi_tx_byte(SD_SPI, 0xFF,TIMEOUT);

        spi_rx_byte(SD_SPI, &res,TIMEOUT);
        if ((res & 0x1F) != 0x05) return false; // Błąd zapisu bloku
        
        do {
            spi_rx_byte(SD_SPI, &res,TIMEOUT);
        } while (res == 0x00);
    }

    spi_tx_byte(SD_SPI, 0xFD,TIMEOUT);

    do {
        spi_rx_byte(SD_SPI, &res,TIMEOUT);
    } while (res == 0x00);

    return true;
}

int SD_disk_write(const uint8_t* buff,uint32_t sector,int count){

    sd_select();

    if(count==1){
        if((SD_card_send_command(CMD24, sector)==0)&&sd_write_single_block(buff,512)) count = 0 ;
    }
    else{
        sd_write_multiple_blocks(sector,buff,count);
    }

    sd_deselect();
    return count;
}
static void sd_delay(uint32_t ms){
    
    time_1ms_delay(ms);
}
static void sd_deselect(){
    spi_deselect(CS_PORT, CS_PIN);
    spi_tx_byte(SD_SPI, 0xFF,TIMEOUT);
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
       spi_tx_byte(SD_SPI, SD_DUMMY_BYTE,TIMEOUT); 
    }
    sd_select();
    
    // Send CMD0 to reset the SD card
    }while(SD_card_send_command(CMD0, 0)!=0x01);

    sd_deselect();
}
