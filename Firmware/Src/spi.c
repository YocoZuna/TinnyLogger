#include "spi.h"
#include "sd_commands.h"
#include "stm32l4xx_ll_bus.h"


void spi_init(SPI_TypeDef*SPIx,LL_SPI_InitTypeDef* spi_init_struct)
{
    if (SPIx == SPI1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    else if (SPIx == SPI3) LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
    LL_SPI_Init(SPIx, spi_init_struct);
    LL_SPI_Enable(SPIx);
}
void spi_tx_rx(SPI_TypeDef*SPIx, uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++){
        spi_tx_byte(SPIx, tx_buffer[i]);
        spi_rx_byte(SPIx, &rx_buffer[i]);
    }
}
void spi_tx_byte(SPI_TypeDef*SPIx, uint8_t byte)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPIx));
    LL_SPI_TransmitData8(SPIx, byte);
}
uint8_t spi_rx_byte(SPI_TypeDef*SPIx, uint8_t* byte)
{
    while (!LL_SPI_IsActiveFlag_RXNE(SPIx));
    *byte = LL_SPI_ReceiveData8(SPIx);
    return *byte;
}
void spi_tx(SPI_TypeDef*SPIx, uint8_t* tx_buffer, uint16_t size){
    for (uint16_t i = 0; i < size; i++){
        spi_tx_byte(SPIx, tx_buffer[i]);
    }
}
void spi_rx(SPI_TypeDef*SPIx, uint8_t* rx_buffer, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++){
        spi_rx_byte(SPIx, &rx_buffer[i]);
    }
}
void spi_deinit(SPI_TypeDef*SPIx)
{
    if (SPIx == SPI1) LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);
    else if (SPIx == SPI3) LL_APB2_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI3);
    LL_SPI_Disable(SPIx);
}
void spi_select(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    LL_GPIO_ResetOutputPin(GPIOx, Pin);
}
void spi_deselect(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    LL_GPIO_SetOutputPin(GPIOx, Pin);
}
