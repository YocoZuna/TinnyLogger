#include "spi.h"
#include <string.h>




SPI_StatusTypeDef spi_init(SPI_HandleTypeDef* hspi,SPI_TypeDef* SPIx,SPI_InitTypeDef* spi_init_struct)
{
    if(hspi==NULL || SPIx==NULL || spi_init_struct==NULL) return SPI_INVALID_PARAM;
    hspi->Instance = SPIx;
    hspi->Init = *spi_init_struct;
    return (SPI_StatusTypeDef)HAL_SPI_Init(hspi);
}

SPI_StatusTypeDef spi_tx_rx( SPI_HandleTypeDef*SPIx, const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size,uint32_t timeout)
{
    if(SPIx==NULL || tx_buffer==NULL || rx_buffer==NULL || size==0) return SPI_INVALID_PARAM;
    memset((void*)rx_buffer, 0xFF, size);
    return (SPI_StatusTypeDef)HAL_SPI_TransmitReceive(SPIx, (uint8_t*)tx_buffer, rx_buffer, size, timeout);

}

SPI_StatusTypeDef spi_tx( SPI_HandleTypeDef*SPIx, const uint8_t* data,size_t size,uint32_t timeout)
{
    if(SPIx==NULL || data==NULL || size==0) return SPI_INVALID_PARAM;
    return (SPI_StatusTypeDef)HAL_SPI_Transmit(SPIx, (uint8_t*)data, size, timeout);

}

SPI_StatusTypeDef spi_rx( SPI_HandleTypeDef*SPIx, uint8_t* rx_buffer,size_t size,uint32_t timeout)
{
    if(SPIx==NULL || rx_buffer==NULL || size==0) return SPI_INVALID_PARAM;
    memset((void*)rx_buffer, 0xFF, size);
    return (SPI_StatusTypeDef)HAL_SPI_Receive(SPIx, rx_buffer, size, timeout);

}
SPI_StatusTypeDef spi_deinit(SPI_HandleTypeDef*SPIx)
{
    if(SPIx==NULL) return SPI_INVALID_PARAM;
    return (SPI_StatusTypeDef)HAL_SPI_DeInit(SPIx);
}

void spi_select(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    if(GPIOx==NULL) return;
    HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_RESET);
}

void spi_deselect(GPIO_TypeDef* GPIOx, uint32_t Pin)
{
    if(GPIOx==NULL) return;
    HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_SET);
}
