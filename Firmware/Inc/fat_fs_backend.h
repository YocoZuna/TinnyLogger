#pragma once

#include <stddef.h>
#include "ff.h"
#include "logger_status.h"

typedef struct{
    FATFS fs;
    FIL file;
}fat_fs_storage_t;

logger_status_t fatfs_mount(void* storage);
logger_status_t fatfs_open(void* storage,const char* filename,const BYTE mode);
logger_status_t fatfs_write(void* storage,const char* buffor,const size_t len);
logger_status_t fatfs_read(void* storage,char* buffor,const size_t len);
logger_status_t fatfs_sync(void* storage);
logger_status_t fatfs_close(void* storage);

