#pragma once

#include "sd_commands.h"
#include "spi.h"
#include "stm32l432xx.h"
#include <stddef.h>
#include <stdint.h>
#include "platform.h"
#include <stdbool.h>
#define SD_CMD0_CRC 0x95
#define SD_CMD8_CRC 0x87
#define SD_DUMMY_CRC 0x01
#define SD_DUMMY_BYTE 0xFF
#define DUMMY_CLOCKS 15
#define CMD_SIZE 6
#define SD_SECTOR_SIZE 512



int SD_card_init();
int SD_disk_status();
int SD_card_send_command(uint8_t cmd, uint32_t arg);
int SD_disk_read(uint8_t* buff,uint32_t sector,int count);
int SD_disk_write(const uint8_t* buff,uint32_t sector,int count);