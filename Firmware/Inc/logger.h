#pragma  once

#include <stddef.h>
#include "Third_party/FatFs/source/ff.h"
#include "Third_party/FatFs/source/storage.h"

typedef enum{
    LOG_OK = 0,
    LOG_ERR_MOUNT,
    LOG_ERR_OPEN,
    LOG_ERR_WRITE,
    LOG_ERR_SYNC,
    LOG_ERR_NOT_INITIALIZED,
}logger_status_t;


logger_status_t logger_init();
logger_status_t logger_deinit();
logger_status_t logger_write(const char* buffor,size_t len);
logger_status_t logger_printf(const char* format,...);