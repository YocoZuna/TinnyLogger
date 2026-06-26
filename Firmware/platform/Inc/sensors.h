#pragma once

#include "i2c.h"
#include "itf_AHT20.h"
#include "AHT20_typedefs.h"

AHT20_Status_t itf_AHT20_delay(uint32_t ms);
AHT20_Status_t itf_AHT20_read(uint8_t deviceAddres,uint8_t* data,uint8_t len);
AHT20_Status_t itf_AHT20_write(uint8_t deviceAddres,uint8_t* data,uint8_t len);
