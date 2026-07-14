#include "spi.h"




SPI_StatusTypeDef spi_init(SPI_HandleTypeDef*SPIx,HAL_SPI_InitTypeDef* spi_init_struct)
{
    return HAL_SPI_Init(SPIx, spi_init_struct);
}

SPI_StatusTypeDef spi_tx_rx( SPI_HandleTypeDef*SPIx, const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size,uint32_t timeout)
{
    return HAL_SPI_TransmitReceive(SPIx, (uint8_t*)tx_buffer, rx_buffer, size, timeout);

}

SPI_StatusTypeDef spi_tx(SPI_HandleTypeDef *SPIx, const uint8_t data,uint32_t timeout) {

    return HAL_SPI_Transmit(SPIx, (uint8_t*)&data, 1, timeout);

}

SPI_StatusTypeDef spi_rx(SPI_HandleTypeDef *SPIx, uint8_t *data,uint32_t timeout) {

    return HAL_SPI_Receive(SPIx, data, 1, timeout);

}
SPI_StatusTypeDef spi_deinit(SPI_HandleTypeDef*SPIx)
{
    return HAL_SPI_DeInit(SPIx);
}

void spi_select(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    LL_GPIO_ResetOutputPin(GPIOx, Pin);
}

void spi_deselect(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    LL_GPIO_SetOutputPin(GPIOx, Pin);
}
