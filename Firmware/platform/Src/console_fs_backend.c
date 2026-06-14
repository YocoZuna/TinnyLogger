#include "console_fs_backend.h"
#include "logger.h"
#include <stdio.h>
#include "platform.h"


logger_status_t console_mount(void* storage){
    return LOG_OK;
}

logger_status_t console_open(void* storage,const char* filename,const BYTE mode){
    
    P_init_UART();
    return LOG_OK;
}
logger_status_t console_write(void* storage,const char* buffor,const size_t len){

    int ret = 0;
    size_t bytes_written;
    console_fs_storage_t* console_storage = (console_fs_storage_t*)storage;
    for(bytes_written = 0;bytes_written<len;bytes_written++){
        uart_tx_byte(console_storage->file,buffor[bytes_written]);
    }
    if(ret!=0 || bytes_written!=len) return LOG_ERR_WRITE;
    return LOG_OK;
}
logger_status_t console_read(void* storage,char* buffor,const size_t len){


    return LOG_OK;
}
logger_status_t console_sync(void* storage){

    return LOG_OK;
}
logger_status_t console_close(void* storage){
    console_fs_storage_t* console_storage = (console_fs_storage_t*)storage;
    uart_deinit(console_storage->file);
    return LOG_OK;
}
logger_status_t console_umount(void* storage){

    if(f_unmount(FF_VOLUME)!=FR_OK) return LOG_ERR_UMOUNT;
    return LOG_OK;
}
const logger_backend_t console_backend = {
    .mount = console_mount,
    .open = console_open,
    .write = console_write,
    .read = console_read,
    .sync = console_sync,
    .close = console_close
};