#include "error.h"


#if defined(TINNY_LOGGER_STM)
#include "stm32l4xx_hal_def.h"
    error_status_t map_hal_status_to_error(uint32_t status){
    switch (status){
        case HAL_OK:
            return ERROR_OK;
        case HAL_ERROR:
            return ERROR_NOK;
        case HAL_BUSY:
            return ERROR_BUSY;
        case HAL_TIMEOUT:
            return  ERROR_TIMEOUT;
        default:
            return ERROR_UNKNOWN;
    }
}
#endif