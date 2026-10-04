#include "system.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"


void P_init_System(void){
    HAL_Init();
}

static void init_FPU(void)
{
    /* P_init_time_base(); */
    SCB->CPACR |= (0xF << 20);
    __DSB();
    __ISB();
}

