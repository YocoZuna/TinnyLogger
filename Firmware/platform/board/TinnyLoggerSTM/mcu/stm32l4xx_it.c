
#include "stm32l4xx_hal.h"
extern __IO uint32_t mTick;


void SysTick_Handler(void)
{
    HAL_IncTick();
}