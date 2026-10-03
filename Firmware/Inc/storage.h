#include "sd.h"
#include "features/sd_card.h"

static void IO_handlers_init(){
    SD_init_IO((SD_IO_t){P_SD_tx_rx,
        P_SD_tx_byte,
        P_SD_rx_byte,
        P_SD_deselect,
        P_SD_select,
        0          
        });
        
}

/* FLASH disk not present always return Not ready error*/
static inline int FLASH_disk_status(){return STA_NODISK;}
static inline int FLASH_disk_initialize(){return STA_NODISK;}
static inline int FLASH_disk_read(BYTE* buff, LBA_t sector, UINT count){return STA_NODISK;}
static inline int FLASH_disk_write(const BYTE* buff, LBA_t sector, UINT count){return STA_NODISK;}
static inline int FLASH_disk_ioctl(void* buff,BYTE cmd){return STA_NODISK;}

/* USB disk not present always return Not ready error*/
static inline int USB_disk_status(){return STA_NODISK;}
static inline int USB_disk_initialize(){return STA_NODISK;}
static inline int USB_disk_read(BYTE* buff, LBA_t sector, UINT count){return STA_NODISK;}
static inline int USB_disk_write(const BYTE* buff, LBA_t sector, UINT count){return STA_NODISK;}
static inline int USB_disk_ioctl(void* buff,BYTE cmd){return STA_NODISK;}

static inline int MMC_disk_status();
static inline int MMC_disk_initialize();
static inline int MMC_disk_read(BYTE* buff, LBA_t sector, UINT count);
static inline int MMC_disk_write(const BYTE* buff, LBA_t sector, UINT count);
static inline int MMC_disk_ioctl(void* buff,BYTE cmd){return 0;}


static inline int MMC_disk_status(){
    return SD_disk_status();
}

static inline int MMC_disk_initialize(){
    P_SD_SPI();
    IO_handlers_init();
    return SD_card_init();
}

static inline int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_read(buff, sector, count);
}

static inline int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_write(buff,sector,count);
}
