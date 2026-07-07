#include "logger.h"
#include "diskio.h"
#include "ff.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>



static logger_status_t logger_flush(logger_t* logger);


logger_status_t logger_init(logger_t* logger)
{
    if (logger == NULL || logger->backend == NULL || logger->storage == NULL) {
        return LOG_ERR_INVALID_PARAM;
    }

    //TODO: Think how to encapsulate open for FATFs and where to define filename
    logger->backend->mount(logger->storage);
    logger->backend->open(logger->storage, "log.txt", FA_WRITE | FA_OPEN_ALWAYS);
    logger->logger_ready = true;
    return LOG_OK;

}
logger_status_t logger_deinit(logger_t* logger){
    
    if(!logger->logger_ready) return LOG_ERR_NOT_INITIALIZED;
    
    logger->logger_ready = false;
    if(logger_flush(logger)!=LOG_OK) return LOG_ERR_SYNC;
    logger->backend->close(logger->storage);
    logger->backend->umount(logger->storage);

    return LOG_OK;
}

logger_status_t logger_write(logger_t* logger,const char* buffor,size_t len){
    
    logger_status_t res;
    
    if(!logger->logger_ready) return LOG_ERR_NOT_INITIALIZED;
    
    res = logger->backend->write(logger->storage, buffor, len);
    if(res!=LOG_OK) return LOG_ERR_WRITE;

    return logger_flush(logger);
}
logger_status_t logger_printf(logger_t* logger, const char* format,...){

    char buffer[512];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len < 0) return LOG_ERR_WRITE;

    // Handle truncation
    if (len >= (int)sizeof(buffer)) {
        len = sizeof(buffer) - 1;
    }

    return logger_write(logger, buffer, len);
}
static logger_status_t logger_flush(logger_t* logger){
    
    if(!logger->logger_ready) return LOG_ERR_NOT_INITIALIZED;

    if(logger->backend->sync(logger->storage)!=LOG_OK) return LOG_ERR_SYNC;
    
    return LOG_OK;
}