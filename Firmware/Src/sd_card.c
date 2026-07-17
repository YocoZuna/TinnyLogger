#include "sd_card.h"
#include "sd_commands.h"
#include "spi.h"



#define TIMEOUT 10
uint8_t SD_DUMMY_BYTE[] = {0xFFU};


static uint8_t response_R1;
static uint8_t response_R7_R3[4];
static uint8_t dummy_buff[512];
extern SPI_HandleTypeDef SD_CARD_SPI;

static void sd_power_on();
static SPI_StatusTypeDef SD_spi_tx_rx(const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size,uint32_t timeout) {
    return spi_tx_rx(&SD_CARD_SPI, tx_buffer, rx_buffer, size, timeout);
}

static SPI_StatusTypeDef SD_spi_tx_byte(const uint8_t* data,size_t len,uint32_t timeout) {
    return spi_tx(&SD_CARD_SPI, data, len, timeout);
}

static SPI_StatusTypeDef SD_spi_rx_byte(uint8_t* rx_buffer,size_t len,uint32_t timeout) {
    return spi_rx(&SD_CARD_SPI, rx_buffer, len, timeout);
}
static void SD_deselect() {
    spi_deselect(CS_PORT, CS_PIN);
    SD_spi_tx_byte((uint8_t[]){0xFF},1,TIMEOUT);
}

static void SD_select() {
    spi_select(CS_PORT, CS_PIN);
    time_1ms_delay(1);
}
static void sd_delay(uint32_t ms){
    
    time_1ms_delay(ms);
}

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

    SD_select();
    
    // Send CMD0 to reset the SD card
    response_R1 = SD_card_send_command(CMD0, 0);

    if (response_R1!=0x01) return -3; 

    // Send CMD8 to check voltage range and SD card version
    response_R1 = SD_card_send_command(CMD8, 0x1AA); // Argument for CMD8: 0x1AA (check voltage range)
    //TODO Add handling for v1 and MMC


    SD_spi_rx_byte(response_R7_R3,sizeof(response_R7_R3),TIMEOUT);
 

    if (response_R7_R3[2] != 0x01 || response_R7_R3[3] != 0xAA) {
        // Invalid voltage range, SD card may not be compatible

        SD_deselect();
        return 3;
    }

    //Send CMD 55

    do {
        
        (void)SD_card_send_command(CMD55, 0);

        response_R1 = SD_card_send_command(ACMD41, 0x40000000); // Argument 0x40000000 to indicate host supports SDHC/SDXC     

    } while (response_R1!= 0x00);
    SD_card_send_command(CMD58, 0);  // Argument 0 dla CMD58


    SD_spi_rx_byte(response_R7_R3,sizeof(response_R7_R3),TIMEOUT);

    SD_deselect();

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

    SD_spi_tx_rx(command_packet, response_buffer, CMD_SIZE, TIMEOUT);

    if(cmd==CMD12) SD_spi_rx_byte(response_buffer,1,TIMEOUT);
    do{
        
        SD_spi_rx_byte(&response,1,TIMEOUT);
    }while(response&0x80);

    return response;
}

int SD_disk_read(uint8_t* buff,uint32_t sector,int count){

    SD_select();
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
    SD_deselect();

    return count;
}

static bool sd_read_single_block(uint8_t* buff,size_t len)
{
    uint8_t retires= 200;
    uint8_t token;

    do{
        SD_spi_rx_byte(&token,sizeof(token),TIMEOUT);
        
    }while(token==0xFF && retires--);
    
    if(token!=0xFE) return false;

    memset(dummy_buff,SD_DUMMY_BYTE[0],SD_SECTOR_SIZE);

    SD_spi_tx_rx(dummy_buff, buff, SD_SECTOR_SIZE, TIMEOUT);

    //We can ommit CRC for now :D 
    SD_spi_rx_byte(&token,sizeof(token),TIMEOUT);
    SD_spi_rx_byte(&token,sizeof(token),TIMEOUT);

    return true;
}


static bool sd_write_single_block(const uint8_t* buff, size_t len) {
    uint8_t res;

    do {
        SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
    } while (res != 0xFF);

    SD_spi_tx_byte((uint8_t[]){0xFE},sizeof(res),TIMEOUT);

    SD_spi_tx_rx(buff, dummy_buff, len,TIMEOUT); 

    SD_spi_tx_byte((uint8_t[]){0xFF},1,TIMEOUT);
    SD_spi_tx_byte((uint8_t[]){0xFF},1,TIMEOUT);

    // 5. Odbierz Data Response Token
    SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
    if ((res & 0x1F) != 0x05) {
        return false; 
    }

    do {
        SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
    } while (res == 0x00);

    return true;
}

static bool sd_write_multiple_blocks(uint32_t sector_addr, const uint8_t*  buff, size_t count) {
    uint8_t res;

    if (SD_card_send_command(CMD25,sector_addr) != 0x00) return false;

    for (size_t i = 0; i < count; i++) {
        do {
            SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
        } while (res != 0xFF);

        SD_spi_tx_byte((uint8_t[]){0xFC},1,TIMEOUT);

        SD_spi_tx_rx(&buff[i * 512], dummy_buff, 512,TIMEOUT);

        SD_spi_tx_byte((uint8_t[]){0xFF},1,TIMEOUT);
        SD_spi_tx_byte((uint8_t[]){0xFF},1,TIMEOUT);

        SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
        if ((res & 0x1F) != 0x05) return false; // Błąd zapisu bloku
        
        do {
            SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
        } while (res == 0x00);
    }

    SD_spi_tx_byte((uint8_t[]){0xFD},1,TIMEOUT);

    do {
        SD_spi_rx_byte(&res,sizeof(res),TIMEOUT);
    } while (res == 0x00);

    return true;
}

int SD_disk_write(const uint8_t* buff,uint32_t sector,int count){

    SD_select();

    if(count==1){
        if((SD_card_send_command(CMD24, sector)==0)&&sd_write_single_block(buff,512)) count = 0 ;
    }
    else{
        sd_write_multiple_blocks(sector,buff,count);
    }

    SD_deselect();
    return count;
}

static void sd_power_on(){


    do{
    SD_deselect();

    // Send 15*8 dummy clocks to initialize the SD card
    for(int i=0;i<DUMMY_CLOCKS;i++)
    {
       if(SD_spi_tx_byte(SD_DUMMY_BYTE,sizeof(SD_DUMMY_BYTE),TIMEOUT)!=SPI_OK) {
           return ;
       }
    }
    SD_select();
    
    // Send CMD0 to reset the SD card
    }while(SD_card_send_command(CMD0, 0)!=0x01);

    SD_deselect();
}
