#pragma once

typedef enum{
    LOG_OK = 0,
    LOG_ERR_MOUNT,
    LOG_ERR_OPEN,
    LOG_ERR_WRITE,
    LOG_ERR_SYNC,
    LOG_ERR_NOT_INITIALIZED,
    LOG_ERR_READ,
    LOG_ERR_CLOSE,
    LOG_ERR_UMOUNT,
}logger_status_t;
