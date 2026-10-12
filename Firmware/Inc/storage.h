#include "sd.h"
#include "features/SD_card/sd_card.h"

static void IO_handlers_init(){
    SD_init_IO((sd_IO_t){.tx_rx=(sd_tx_rx_t)P_SD_tx_rx,
        .tx_byte=(sd_tx_byte_t)P_SD_tx_byte,
        .rx_byte=(sd_rx_byte_t)P_SD_rx_byte,
        .deselect=(sd_deselect_t)P_SD_deselect,
        .select=(sd_select_t)P_SD_select,
                  
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
    P_SD_init();
    IO_handlers_init();
    return SD_card_init();
}

static inline int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_read(buff, sector, count);
}

static inline int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count){
    return SD_disk_write(buff,sector,count);
}
