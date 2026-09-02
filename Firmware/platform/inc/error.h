#ifndef PLATFORM_ERROR_H
#define PLATFORM_ERROR_H

#include <stdint.h>
#include <stddef.h>

/* ERROR   Status types derived from HAL_StatusTypeDef */
typedef enum
{
  ERROR_OK       = 0x00,
  ERROR_NOK  = 0x01,
  ERROR_BUSY     = 0x02,
  ERROR_TIMEOUT  = 0x03,
  ERROR_INVALID_PARAM = 0x04,
  ERROR_UNKNOWN  = 0xFF
} error_status_t;

error_status_t map_hal_status_to_error(uint32_t status);

#endif /* PLATFORM_ERROR_H */

