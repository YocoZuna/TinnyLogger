#pragma once

#ifdef USE_CONSOLE_FS_BACKEND
#include "logger_status.h"
#include "uart.h"

typedef struct{
    USART_TypeDef *USART;
}console_fs_storage_t;

logger_status_t console_mount(void* storage);
logger_status_t console_open(void* storage,const char* filename,const BYTE mode);
logger_status_t console_write(void* storage,const char* buffor,const size_t len);
logger_status_t console_read(void* storage,char* buffor,const size_t len);
logger_status_t console_sync(void* storage);
logger_status_t console_close(void* storage);
#endif // USE_CONSOLE_FS_BACKEND