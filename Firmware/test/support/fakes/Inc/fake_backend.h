#pragma once

#include "logger.h"
#include "logger_status.h"

typedef struct{
    uint8_t  *fake;
}fake_fs_storage_t;

logger_status_t fake_mount(void* storage);
logger_status_t fake_open(void* storage,const char* filename,const BYTE mode);
logger_status_t fake_write(void* storage,const char* buffor,const size_t len);
logger_status_t fake_read(void* storage,char* buffor,const size_t len);
logger_status_t fake_sync(void* storage);
logger_status_t fake_close(void* storage);
logger_status_t fake_unmount(void* storage);

extern logger_backend_t fake_backend;
