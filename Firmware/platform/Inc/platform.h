#pragma  once

#include "sd_card.h"
#include "spi.h"
#include "time.h"

#define SD_CARD_SPI SPI3
#define MOSI_PIN LL_GPIO_PIN_3
#define MISO_PIN LL_GPIO_PIN_4
#define SCK_PIN LL_GPIO_PIN_5

static inline void P_init_SD_SPI();
static inline void P_init_time_base();
static inline void P_init_platform();


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
static inline void P_init_platform(){
    P_init_time_base();
}