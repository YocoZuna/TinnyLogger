#pragma once

#include <stddef.h>
#include "ff.h"
#include "logger_status.h"
#include "logger.h"

#define FF_VOLUME "0:"
#define FILE_NAME "TinnyLogger.txt"

typedef struct{
    FATFS fs;
    FIL file;
}fat_fs_storage_t;

logger_status_t fatfs_init(void* storage);
logger_status_t fatfs_write(void* storage,const char* buffor,const size_t len);
logger_status_t fatfs_read(void* storage,char* buffor,const size_t len);
logger_status_t fatfs_sync(void* storage);
logger_status_t fatfs_deinit(void* storage);

extern const logger_backend_t fat_fs_backend;