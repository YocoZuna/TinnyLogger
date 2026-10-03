#ifndef SD_CARD_H
#define SD_CARD_H

#include "sd.h"
#include "sd_cmds.h"
#include <stdbool.h>
#include "diskio.h"
#include "ff.h"



#define SD_CMD0_CRC 0x95
#define SD_CMD8_CRC 0x87
#define SD_DUMMY_CRC 0x01
#define DUMMY_CLOCKS 15
#define CMD_SIZE 6
#define SD_SECTOR_SIZE 512

#define SD_DUMMY_BYTE           ((uint8_t[]){0xFF})
#define TIMEOUT                 100U

typedef enum{
    SDSC = 0,
    SDHC = 1
}sd_cardtype_t;

typedef enum{
  SD_OK       = 0x00,
  SD_ERROR    = 0x01,
  SD_BUSY     = 0x02,
  SD_TIMEOUT  = 0x03,
  SD_INVALID_PARAM = 0x04,
  SD_UNKNOWN_ERROR = 0x5
}sd_status_t;

typedef enum{
	SD_NotInitialized=0,
	SD_Initialized,
}sd_init_state_t;

typedef sd_status_t (*sd_tx_rx_t)(const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
typedef	sd_status_t (*sd_tx_byte_t)(const uint8_t *tx_buffer,size_t size, uint32_t timeout);
typedef	sd_status_t (*sd_rx_byte_t)(uint8_t *rx_buffer,size_t size,uint32_t timeout);
typedef	sd_status_t (*sd_deselect_t)();
typedef	sd_status_t (*sd_select_t)();	

typedef struct{
	sd_tx_rx_t tx_rx;
	sd_tx_byte_t tx_byte;
	sd_rx_byte_t rx_byte;
	sd_deselect_t deselect;
	sd_select_t select;
	sd_init_state_t init;
}sd_IO_t;

DSTATUS SD_disk_status();
DRESULT SD_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT SD_disk_write(const BYTE *buff, LBA_t sector, UINT count);
DSTATUS SD_card_init();
sd_init_state_t SD_init_IO(sd_IO_t io_handlers);

#endif