#ifndef SD_H
#define SD_H

#ifdef TINNY_LOGGER
	#include "board/TinnyLogger/i2c.h"
#endif

#include "sd_commands.h"
#include "ff.h"
#include "diskio.h"


#define SD_DUMMY_BYTE           ((uint8_t[]){0xFF})
#define TIMEOUT                 100U

typedef enum{
    SDSC = 0,
    SDHC = 1
}SD_cardtype_t;

typedef enum{
  SD_OK       = 0x00,
  SD_ERROR    = 0x01,
  SD_BUSY     = 0x02,
  SD_TIMEOUT  = 0x03,
  SD_INVALID_PARAM = 0x04,
  SD_UNKNOWN_ERROR = 0x5
}SD_status_t;


typedef struct SD_IO_t SD_IO_t;
struct SD_IO_t{

    void *ctx;
    SD_status_t (*tx_rx)(SD_IO_t* card,const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
    SD_status_t (*tx_byte)(SD_IO_t* card,const uint8_t *tx_buffer,size_t size, uint32_t timeout);
    SD_status_t (*rx_byte)(SD_IO_t* card,uint8_t *rx_buffer,size_t size,uint32_t timeout);
    SD_status_t (*deselect)(SD_IO_t* card);
    void (*select)(SD_IO_t* card);
    void (*delay)(uint32_t delay);
};

DSTATUS SD_card_init();
DSTATUS SD_disk_status();
DRESULT SD_disk_read(BYTE* buff, LBA_t sector, UINT count);
DRESULT SD_disk_write(const BYTE* buff, LBA_t sector, UINT count);

#endif /*SD_H*/


