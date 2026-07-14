#pragma  once

#include "sd_card.h"
#include "spi.h"
#include "stm32l4xx_hal_rcc.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_spi.h"
#include "time.h"
#include "i2c.h"
#include "uart.h"



#define MOSI_PIN LL_GPIO_PIN_3
#define MISO_PIN LL_GPIO_PIN_4
#define SCK_PIN LL_GPIO_PIN_5

#define CONSOLE_UART USART2
#define CONSOLE_TX_PIN LL_GPIO_PIN_2
#define CONSOLE_RX_PIN LL_GPIO_PIN_15


void P_init_SD_SPI();
void P_init_time_base();
void P_init_FPU();
void P_init_I2CX();
void P_init_SD_SPI();
void P_init_time_base();
void P_init_FPU();
void P_init_I2CX();
void P_init_UART();
void P_deinit_UART();