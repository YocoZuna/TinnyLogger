#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_bus.h"


void timeInit();
uint32_t getTime();
void time_1ms_delay(const uint32_t delay);