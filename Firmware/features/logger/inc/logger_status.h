#pragma once

typedef enum{
    LOG_OK = 0,
    LOG_ERR_INIT,
    LOG_ERR_WRITE,
    LOG_ERR_SYNC,
    LOG_ERR_NOT_INITIALIZED,
    LOG_ERR_READ,
    LOG_ERR_DEINIT,
    LOG_ERR_INVALID_PARAM,
}logger_status_t;
