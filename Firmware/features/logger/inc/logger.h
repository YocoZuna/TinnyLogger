#pragma  once

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include "fat_fs_backend.h"

/********************* CONIFG  *************************************/

#define LOGGER_INTERNAL_BUFFER_SIZE 512






/*****************************************************************/

#define LOGGER_BACKEND_CONSOLE console
#define LOGGER_BACKEND_FAT_FS fat

#define LOGGER_REGISTER(instance_name,_backend,_use_buffering) \
                        static _backend##_fs_storage_t s_##instance_name##_storage; \
                        static logger_t instance_name =  { \
                            .backend = &(_backend##_fs_backend), \
                            .storage = &(s_##instance_name##_storage), \
                            .use_buffering = _use_buffering, \
                        };   

#define MIN(a,b) (a>b ? b:a)

typedef struct{

    logger_status_t (*mount)(void* storage);
    logger_status_t (*open)(void* storage,const char* filename,const BYTE mode);
    logger_status_t (*write)(void* storage,const char* buffor,const size_t len);
    logger_status_t (*read)(void* storage,char* buffor,const size_t len);
    logger_status_t (*sync)(void* storage);
    logger_status_t (*close)(void* storage);
    logger_status_t (*unmount)(void* storage);

}logger_backend_t;

typedef struct{
    
    uint8_t buffor[LOGGER_INTERNAL_BUFFER_SIZE];
    size_t buffors_head;

}logger_storage_t;

typedef struct{

    const logger_backend_t* backend;
    logger_storage_t internal_storage;
    bool use_buffering;
    bool logger_ready;
    void* storage;

}logger_t;

logger_status_t logger_init(logger_t* logger);
logger_status_t logger_deinit(logger_t* logger);
logger_status_t logger_write(logger_t* logger, const char* buffor, const size_t len);
