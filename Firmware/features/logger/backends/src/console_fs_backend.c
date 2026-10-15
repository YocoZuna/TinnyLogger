#include "console_fs_backend.h"
#include "logger.h"
#include <stdio.h>
#include "logger_status.h"
#include "uart.h"
#include "error.h"


static logger_status_t console_close(void* storage){
    
    console_fs_storage_t* console_storage = (console_fs_storage_t*)storage;
    if(P_uart_deinit(console_storage->file)!=ERROR_OK) return LOG_ERR_DEINIT;
    return LOG_OK;
}

logger_status_t console_init(void* storage){
    
    if(P_uart_init()!=ERROR_OK) return LOG_ERR_INIT;
    return LOG_OK;
    
}
logger_status_t console_write(void* storage,const char* buffor,const size_t len){

    int ret = 0;
    size_t bytes_written;
    //console_fs_storage_t* console_storage = (console_fs_storage_t*)storage;
    for(bytes_written = 0;bytes_written<len;bytes_written++){
        //P_uart_tx(const uint8_t *tx_buffer, size_t size, uint32_t timeout)
        //uart_tx_byte(console_storage->file,buffor[bytes_written]);
        //P_uart_tx(buffor[bytes_written], size_t size, uint32_t timeout)
    }
    if(ret!=0 || bytes_written!=len) return LOG_ERR_WRITE;
    return LOG_OK;
}
logger_status_t console_read(void* storage,char* buffor,const size_t len){
    
    return LOG_ERR_READ;
}
logger_status_t console_sync(void* storage){

    return LOG_ERR_SYNC;
}

logger_status_t console_deinit(void* storage){

    if(console_close(storage)!=LOG_OK) return LOG_ERR_DEINIT;
    return LOG_OK;
}