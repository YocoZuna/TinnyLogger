#include "diskio.h"
#include "ff.h"
#include "sd_card.h"
#include "platform.h"

/* FLASH disk not present always return Not ready error*/
static inline int FLASH_disk_status(){return 3;}
static inline int FLASH_disk_initialize(){return 3;}
static inline int FLASH_disk_read(BYTE* buff, LBA_t sector, UINT count){return 3;}
static inline int FLASH_disk_write(const BYTE* buff, LBA_t sector, UINT count){return 3;}
static inline int FLASH_disk_ioctl(void* buff,BYTE cmd){return 3;}

/* USB disk not present always return Not ready error*/
static inline int USB_disk_status(){return 3;}
static inline int USB_disk_initialize(){return 3;}
static inline int USB_disk_read(BYTE* buff, LBA_t sector, UINT count){return 3;}
static inline int USB_disk_write(const BYTE* buff, LBA_t sector, UINT count){return 3;}
static inline int USB_disk_ioctl(void* buff,BYTE cmd){return 3;}

static inline int MMC_disk_status();
static inline int MMC_disk_initialize();
static inline int MMC_disk_read(BYTE* buff, LBA_t sector, UINT count);
static inline int MMC_disk_write(const BYTE* buff, LBA_t sector, UINT count);
static inline int MMC_disk_ioctl(void* buff,BYTE cmd){return 0;}


static inline int MMC_disk_status(){
    return SD_disk_status();
}

static inline int MMC_disk_initialize(){
    P_init_SD_SPI();
    return SD_card_init();
}

static inline int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_read(buff, sector, count);
}

static inline int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_write(buff,sector,count);
}