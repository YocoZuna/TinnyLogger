#include "logger.h"
#include "Third_party/FatFs/source/diskio.h"
#include "Third_party/FatFs/source/ff.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>



static bool logger_ready = false;

static logger_status_t logger_flush();

FATFS fs;
FIL fil;
PARTITION VolToPart[FF_VOLUMES] = {
    {1, 0},   // "0:" → physical drive 0, whole drive
};

logger_status_t logger_init()
{


    if(f_mount(&fs, FF_VOLUME, 1)!=FR_OK) return LOG_ERR_MOUNT;
    
    if(f_open(&fil, "0:TinnyLogger.txt", FA_WRITE | FA_OPEN_APPEND)!=FR_OK) return LOG_ERR_OPEN;
    
    logger_ready = true;
    return LOG_OK;

}
logger_status_t logger_deinit(){
    
    if(!logger_ready) return LOG_ERR_NOT_INITIALIZED;
    
    logger_ready = false;
    if(logger_flush()!=LOG_OK) return LOG_ERR_SYNC;
    f_close(&fil);
    f_unmount(FF_VOLUME);

    return LOG_OK;
}

logger_status_t logger_write(const char* buffor,size_t len){
    
    FRESULT res;
    UINT bytes_written;
    
    if(!logger_ready) return LOG_ERR_NOT_INITIALIZED;
    
    res = f_write(&fil, buffor, len,&bytes_written );
    if(res!=FR_OK || bytes_written!=len) return LOG_ERR_WRITE;

    return logger_flush();
}
logger_status_t logger_printf(const char* format,...){

    char buffer[512];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len < 0) return LOG_ERR_WRITE;

    // Handle truncation
    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
    }

    return logger_write(buffer, len);
}
static logger_status_t logger_flush(){
    
    if(!logger_ready) return LOG_ERR_NOT_INITIALIZED;

    if(f_sync(&fil)!=FR_OK) return LOG_ERR_SYNC;
    
    return LOG_OK;
}