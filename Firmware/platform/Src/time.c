#include "time.h"
#include "stm32l432xx.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_bus.h"


__IO uint32_t mTick = 0;
void timeInit()
{
    LL_Init1msTick(4000000); // Assuming HCLK is 4 MHz, adjust if different
    LL_SYSTICK_EnableIT();
}
uint32_t getTime()
{
    return mTick;
}
void time_1ms_delay(const uint32_t delay)
{
    LL_mDelay(delay);
}
