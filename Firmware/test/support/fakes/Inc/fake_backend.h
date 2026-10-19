#pragma once

#include "logger.h"
#include "logger_status.h"

typedef struct{
    uint8_t  *fake;
}fake_fs_storage_t;

logger_status_t fake_init(void* storage);
logger_status_t fake_write(void* storage,const char* buffor,const size_t len);
logger_status_t fake_read(void* storage,char* buffor,const size_t len);
logger_status_t fake_sync(void* storage);
logger_status_t fake_deinit(void* storage);
