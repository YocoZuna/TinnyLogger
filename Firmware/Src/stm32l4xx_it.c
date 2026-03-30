
#include "stm32l432xx.h"
extern __IO uint32_t mTick;


void SysTick_Handler(void)
{
    mTick++;
}