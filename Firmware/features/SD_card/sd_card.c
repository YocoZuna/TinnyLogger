#include "sd_card.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sd_cmds.h"


#define MAX_RETIRES 10

sd_IO_t hSD_card = {0};
static DSTATUS SD_card_status = STA_NOINIT;
static sd_cardtype_t CardType =SDHC; 
static uint8_t PowerOnFlag = 0; 
static uint8_t response_R1;
static uint8_t response_R7_R3[4];
static uint8_t dummy_buff[SD_SECTOR_SIZE];
static sd_status_t status = SD_ERROR;

static sd_status_t card_send_command(uint8_t cmd, uint32_t arg,uint8_t* response) {
  uint8_t command_packet[CMD_SIZE];
  uint8_t response_buffer[CMD_SIZE];
  command_packet[0] = cmd;
  command_packet[1] = (arg >> 24) & 0xFF;
  command_packet[2] = (arg >> 16) & 0xFF;
  command_packet[3] = (arg >> 8) & 0xFF;
  command_packet[4] = arg & 0xFF;

  // Use predefined CRC if any of this CMD has to be sent
  command_packet[5] = (cmd == CMD0)   ? SD_CMD0_CRC
                      : (cmd == CMD8) ? SD_CMD8_CRC
                                      : SD_DUMMY_CRC;
  
  if((status=hSD_card.tx_rx(command_packet, response_buffer, CMD_SIZE, TIMEOUT))!=SD_OK){
    return status;
  }

  if (cmd == CMD12) {
      if((status=hSD_card.rx_byte(response_buffer, 1, TIMEOUT))!=SD_OK){;
      return status;
    }
  }
  do {

    if((status=hSD_card.rx_byte(response, 1, TIMEOUT))!=SD_OK){
      return status;
    }
    
  } while ((*response & 0x80));

  return status;
}


#define SPI_OK 0 
static DRESULT power_on() {
  uint8_t retries = 10;
  
  do {
    
    hSD_card.deselect();

    // Send 15*8 dummy clocks to initialize the SD card
    for (int i = 0; i < DUMMY_CLOCKS; i++) {
        hSD_card.tx_byte(SD_DUMMY_BYTE, sizeof(SD_DUMMY_BYTE), TIMEOUT);

    }
    hSD_card.select();

    if (retries-- == 0) {
      
      return RES_NOTRDY;
    }
    // Send CMD0 to reset the SD card
    if (card_send_command(CMD0, 0,&response_R1)!=SD_OK) return RES_NOTRDY;
  } while (response_R1 != 0x01);

  hSD_card.deselect();
  PowerOnFlag = 1;

  return RES_OK;
}
static bool read_single_block(uint8_t *buff, size_t len) {
  uint8_t max_retires = MAX_RETIRES;
  uint8_t token;
  bool state = false;

  do {
    status = hSD_card.rx_byte(&token, sizeof(token), TIMEOUT);
  } while (token == 0xFF && max_retires-- && status==SD_OK);

  /* Invalid response */
  if (token != 0xFE) return state;

  memset(dummy_buff, SD_DUMMY_BYTE[0], SD_SECTOR_SIZE);

  
  if(hSD_card.tx_rx(dummy_buff, buff, SD_SECTOR_SIZE, TIMEOUT)!=SD_OK) return state;

  if(hSD_card.rx_byte(&token, sizeof(token), TIMEOUT)!=SD_OK) return state;
  if(hSD_card.rx_byte(&token, sizeof(token), TIMEOUT)!=SD_OK) return state;

  return !state;
}

static bool write_single_block(const uint8_t *buff, size_t len) {
  uint8_t res;
  bool state = false;

  do {
    status = hSD_card.rx_byte(&res, sizeof(res), TIMEOUT);
  } while (res != 0xFF && state==SD_OK);

  if(hSD_card.tx_byte((uint8_t[]){0xFE}, sizeof(res), TIMEOUT)!=SD_OK) return state;

  if(hSD_card.tx_rx(buff, dummy_buff, len, TIMEOUT)!=SD_OK) return state;

  if(hSD_card.tx_byte((uint8_t[]){0xFF}, 1, TIMEOUT)!=SD_OK) return state;
  if(hSD_card.tx_byte((uint8_t[]){0xFF}, 1, TIMEOUT)!=SD_OK) return state;

  if(hSD_card.rx_byte(&res, sizeof(res), TIMEOUT)!=SD_OK) return state;
  
  if ((res & 0x1F) != 0x05) {
    return state;
  }

  do {
    status = hSD_card.rx_byte(&res, sizeof(res), TIMEOUT);
  } while (res == 0x00 && state==SD_OK);

  return !state;
}

static bool write_multiple_blocks(uint32_t sector_addr, const uint8_t *buff,
                                  size_t* count) {
  uint8_t res;
  bool state = false;

  if (card_send_command(CMD25, sector_addr,&response_R1)!=SD_OK || response_R1!=0x00) return state;

  for (size_t i = 0; i < *count; i++) {
    do {
      state = hSD_card.rx_byte(&res, sizeof(res), TIMEOUT);
    } while (res != 0xFF && status == SD_OK);

    if(hSD_card.tx_byte((uint8_t[]){0xFC}, 1, TIMEOUT)!=SD_OK) return state;

    if(hSD_card.tx_rx(&buff[i * SD_SECTOR_SIZE], dummy_buff, SD_SECTOR_SIZE,TIMEOUT)!=SD_OK) return state;

    if(hSD_card.tx_byte((uint8_t[]){0xFF}, 1, TIMEOUT)!=SD_OK) return state;
    if(hSD_card.tx_byte((uint8_t[]){0xFF}, 1, TIMEOUT)!=SD_OK) return state;

    if(hSD_card.rx_byte(&res, sizeof(res), TIMEOUT)!=SD_OK) return state;
    if ((res & 0x1F) != 0x05) return false;

    do {
      status = hSD_card.rx_byte(&res, sizeof(res), TIMEOUT);
    } while (res == 0x00 && status == SD_OK);
  }

  if(hSD_card.tx_byte((uint8_t[]){0xFD}, 1, TIMEOUT)!=SD_OK) return state;

  do {
    status = hSD_card.rx_byte(&res, sizeof(res), TIMEOUT);
  } while (res == 0x00 && status==SD_OK);

  return !state;
}

sd_init_state_t SD_init_IO(sd_IO_t io_handlers) {
  if ((io_handlers.tx_rx == NULL )  |
      (io_handlers.tx_byte == NULL) |
      (io_handlers.rx_byte == NULL) |
      (io_handlers.deselect == NULL)|
      (io_handlers.select == NULL)) {
    return 1;
  }
  if(hSD_card.init == SD_Initialized ) return SD_Initialized; // Retarded signleton ;)

  hSD_card = io_handlers;
  hSD_card.init= SD_Initialized;

  return hSD_card.init;
}

DSTATUS
SD_disk_status() { return SD_card_status; }

DRESULT
SD_disk_read(BYTE *buff, LBA_t sector, UINT count) {
  if (count == 0) return RES_PARERR;
  if (SD_card_status & STA_NOINIT) return RES_NOTRDY;
  if (CardType == SDSC) sector *= 512;

  hSD_card.select();
  if (count == 1) {
    if (card_send_command(CMD17, sector,&response_R1)==SD_OK && response_R1 == 0 && read_single_block(buff, SD_SECTOR_SIZE))
      count = 0;
  } else {
    if (card_send_command(CMD18, sector,&response_R1)==SD_OK && response_R1 == 0) {
      do {
        if (!read_single_block(buff, SD_SECTOR_SIZE)) break;
        buff += SD_SECTOR_SIZE;
      } while (--count);

      card_send_command(CMD12, 0,&response_R1);
    }
  }
  hSD_card.deselect();

  return count ? RES_ERROR : RES_OK;
}

DRESULT
SD_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
  if (count == 0) return RES_PARERR;
  if (SD_card_status & STA_NOINIT) return RES_NOTRDY;
  if (SD_card_status & STA_PROTECT) return RES_WRPRT;
  if (CardType == SDSC) sector *= 512;

  hSD_card.select();

  if (count == 1) {
    if (card_send_command(CMD24, sector,&response_R1)==SD_OK && response_R1 ==0 && write_single_block(buff, SD_SECTOR_SIZE))
      count = 0;
  } else {
    write_multiple_blocks(sector, buff, &count);
  }

  hSD_card.deselect();
  return count ? RES_ERROR : RES_OK;
}

DSTATUS
SD_card_init() {

  uint8_t max_retries = MAX_RETIRES;
  if (SD_card_status & STA_NODISK) return SD_card_status;

  if (power_on() != RES_OK) {
    goto init_done;
  }

  hSD_card.select();

  // Send CMD0 to reset the SD card
  if (card_send_command(CMD0, 0,&response_R1)!=SD_OK || response_R1 != 0x01){
    goto init_done;
  }

  if(card_send_command(CMD8, 0x1AA, &response_R1)!=SD_OK || response_R1!=0x01){
    goto init_done;
  }

  if(hSD_card.rx_byte(response_R7_R3, sizeof(response_R7_R3), TIMEOUT)!=SD_OK ||
    response_R7_R3[2] != 0x01 || response_R7_R3[3] != 0xAA){
      goto init_done;
    }  

  do {

    if(card_send_command(CMD55, 0,&response_R1)!=SD_OK){
      goto init_done;
    }

    if(card_send_command(ACMD41,0x40000000,&response_R1)!=SD_OK){  // Argument 0x40000000 to indicate host supports SDHC/SDXC
      goto  init_done;
    }

  } while (response_R1 != 0x00 || max_retries--);

  
  if(card_send_command(CMD58, 0,&response_R1)!=SD_OK){
    goto  init_done;
  }

  if(hSD_card.rx_byte(response_R7_R3, sizeof(response_R7_R3), TIMEOUT)!=SD_OK || (!(response_R7_R3[0]&0x80))){
    goto  init_done;
  }

  CardType = (response_R7_R3[0] & 0x40)
                 ? SDHC
                 : SDSC;  // Check if the CCS (Card Capacity Status) bit is set

  SD_card_status &= ~STA_NOINIT;
  
init_done:
  
  hSD_card.deselect();
  return SD_card_status;
}