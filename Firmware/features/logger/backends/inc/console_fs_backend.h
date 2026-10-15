#pragma once


#include "logger_status.h"
#include "uart.h"
#include <stddef.h>
#include "ff.h"
typedef struct{
    USART_TypeDef *file;
}console_fs_storage_t;

logger_status_t console_init(void* storage);
logger_status_t console_write(void* storage,const char* buffor,const size_t len);
logger_status_t console_read(void* storage,char* buffor,const size_t len);
logger_status_t console_sync(void* storage);
logger_status_t console_deinit(void* storage);
