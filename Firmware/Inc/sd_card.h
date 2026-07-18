#pragma once

#include "sd_commands.h"
#include "spi.h"
#include "string.h"
#include "platform.h"
#include <stdbool.h>
#include "ff.h"
#include "diskio.h"

#define SD_CMD0_CRC 0x95
#define SD_CMD8_CRC 0x87
#define SD_DUMMY_CRC 0x01
#define DUMMY_CLOCKS 15
#define CMD_SIZE 6
#define SD_SECTOR_SIZE 512
extern SPI_HandleTypeDef SD_CARD_SPI;

typedef enum{
    SDSC = 0,
    SDHC = 1
}SD_cardtype_t;

DSTATUS SD_card_init();
DSTATUS SD_disk_status();
DRESULT SD_disk_read(BYTE* buff, LBA_t sector, UINT count);
DRESULT SD_disk_write(const BYTE* buff, LBA_t sector, UINT count);