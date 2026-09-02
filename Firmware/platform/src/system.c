#include "system.h"
#include "stm32l4xx.h"

void P_init_FPU(void)
{
    /* P_init_time_base(); */
    SCB->CPACR |= (0xF << 20);
    __DSB();
    __ISB();
}
