#include "logger.h"
#include "logger_status.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>



static logger_status_t logger_flush(logger_t* logger);
static logger_status_t logger_status = LOG_ERR_NOT_INITIALIZED;

logger_status_t logger_init(logger_t* logger)
{
    if (logger == NULL || logger->backend == NULL || logger->storage == NULL) {
        return LOG_ERR_INVALID_PARAM;
    }

    logger->internal_storage.buffors_head = 0;
    memset(logger->internal_storage.buffor, '\0', LOGGER_INTERNAL_BUFFER_SIZE);
    
    //TODO: Think how to encapsulate open for FATFs and where to define 
    if((logger_status = logger->backend->mount(logger->storage))!=LOG_OK) return logger_status;
    
    if((logger_status = logger->backend->open(logger->storage, "log.txt", FA_WRITE | FA_OPEN_ALWAYS))!=LOG_OK) return logger_status;
   
    logger->logger_ready = true;
    return LOG_OK;

}
logger_status_t logger_deinit(logger_t* logger){
    
    if (logger == NULL || logger->backend == NULL || logger->storage == NULL) {
        return LOG_ERR_INVALID_PARAM;
    }
    if(!logger->logger_ready) return LOG_ERR_NOT_INITIALIZED;
    
    
    if(logger_flush(logger)!=LOG_OK) return LOG_ERR_SYNC;

    if((logger_status = logger->backend->close(logger->storage))!=LOG_OK) return logger_status;

    if((logger_status = logger->backend->unmount(logger->storage))!=LOG_OK) return logger_status;

    logger->logger_ready = false;
    return LOG_OK;
}

logger_status_t logger_write(logger_t* logger,const char* buffor,size_t len){
 
    if(len==0 || buffor==NULL || logger == NULL || logger->backend == NULL 
        || logger->storage == NULL) return LOG_ERR_INVALID_PARAM;
    if(!logger->logger_ready) return LOG_ERR_NOT_INITIALIZED;

    if(logger->use_buffering==false){

        if((logger_status=logger->backend->write(logger->storage, buffor, len))!=LOG_OK) return logger_status;

        return logger_flush(logger);
    }
    else{
        
        while(len>0){
            size_t free = LOGGER_INTERNAL_BUFFER_SIZE-(logger->internal_storage.buffors_head);
            size_t copy = MIN(len,free);

            memcpy((logger->internal_storage.buffor+logger->internal_storage.buffors_head),buffor,copy);

            logger->internal_storage.buffors_head+=copy;
            buffor+= copy;
            len-=copy;

            if(logger->internal_storage.buffors_head==LOGGER_INTERNAL_BUFFER_SIZE){
            
                if((logger_status=logger->backend->write(logger->storage, logger->internal_storage.buffor, LOGGER_INTERNAL_BUFFER_SIZE))!=LOG_OK) return logger_status;
                if((logger_status=logger_flush(logger))!=LOG_OK) return LOG_ERR_SYNC;
                logger->internal_storage.buffors_head = 0;
                memset(logger->internal_storage.buffor, '\0', LOGGER_INTERNAL_BUFFER_SIZE);
            }
            
        }

        return LOG_OK;
    }
}

static logger_status_t logger_flush(logger_t* logger){
    
    if (logger == NULL || logger->backend == NULL || logger->storage == NULL) {
        return LOG_ERR_INVALID_PARAM;
    }
    if(logger->backend->sync(logger->storage)!=LOG_OK) return LOG_ERR_SYNC;
    
    return LOG_OK;
} 