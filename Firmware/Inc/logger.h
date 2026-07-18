#pragma  once

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include "fat_fs_backend.h"

typedef struct{

    logger_status_t (*mount)(void* storage);
    logger_status_t (*open)(void* storage,const char* filename,const BYTE mode);
    logger_status_t (*write)(void* storage,const char* buffor,const size_t len);
    logger_status_t (*read)(void* storage,char* buffor,const size_t len);
    logger_status_t (*sync)(void* storage);
    logger_status_t (*close)(void* storage);
    logger_status_t (*umount)(void* storage);
}logger_backend_t;

typedef struct{
    const logger_backend_t* backend;
    void* storage;
    bool logger_ready;
}logger_t;

logger_status_t logger_init(logger_t* logger);
logger_status_t logger_deinit(logger_t* logger);
logger_status_t logger_write(logger_t* logger, const char* buffor, const size_t len);
logger_status_t logger_printf(logger_t* logger, const char* format,...);