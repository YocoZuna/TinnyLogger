#pragma  once

#include "sd_card.h"
#include "spi.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "time.h"
#include "i2c.h"
#include "uart.h"

#define SD_CARD_SPI SPI3
#define MOSI_PIN LL_GPIO_PIN_3
#define MISO_PIN LL_GPIO_PIN_4
#define SCK_PIN LL_GPIO_PIN_5

#define CONSOLE_UART USART2
#define CONSOLE_TX_PIN LL_GPIO_PIN_2
#define CONSOLE_RX_PIN LL_GPIO_PIN_15


static inline void P_init_SD_SPI();
static inline void P_init_time_base();
static inline void P_init_FPU();
static inline void P_init_I2CX();


static inline void P_init_SD_SPI(){
    LL_SPI_InitTypeDef spi = {
    .TransferDirection = LL_SPI_FULL_DUPLEX,
    .Mode = LL_SPI_MODE_MASTER,
    .DataWidth = LL_SPI_DATAWIDTH_8BIT,
    .ClockPolarity = LL_SPI_POLARITY_LOW,
    .ClockPhase = LL_SPI_PHASE_1EDGE,
    .NSS = LL_SPI_NSS_SOFT,
    .BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2, // Szybsze dla SD
    .BitOrder = LL_SPI_MSB_FIRST
    };

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = MOSI_PIN | MISO_PIN | SCK_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_6; // AF6 for SPI3
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    GPIO_InitStruct.Pin = CS_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    LL_GPIO_SetPinPull(CS_PORT, CS_PIN, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, MISO_PIN, LL_GPIO_PULL_UP);
    
    spi_init(SD_CARD_SPI, &spi);
}
static inline void P_init_time_base(){
    timeInit();
}
static inline void P_init_FPU(){
    P_init_time_base();
    /* Init FPU*/
    SCB->CPACR |= (0xF << 20);
    __DSB(); __ISB();
}


static inline void P_init_I2CX() {


    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4; // AF4 for I2C1
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    LL_I2C_InitTypeDef i2c_init_struct = {
        .OwnAddress1 = 0,
        .TypeAcknowledge = LL_I2C_ACK,
        .OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT,
        .PeripheralMode = LL_I2C_MODE_I2C,
        .Timing = 0x00100D14 // Timing value for 100kHz with 4MHz PCLK1 (calculated with STM32CubeMX) --- IGNORE ---
    };

    i2c_init(I2C_PORT, &i2c_init_struct);
}

static inline void P_init_UART() {
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = CONSOLE_TX_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7; // AF7 for USART2
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = CONSOLE_RX_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_3; // AF3 for USART2
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_USART_InitTypeDef uart_init_struct = {
        .BaudRate = 115200,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX_RX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    };

    uart_init(CONSOLE_UART, &uart_init_struct);
}
