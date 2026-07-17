#pragma once
#include "stm32l432xx.h"
#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_gpio.h"
#define CS_PIN LL_GPIO_PIN_11
#define CS_PORT GPIOA

/* SPI Status types derived from HAL_StatusTypeDef */
typedef enum
{
  SPI_OK       = 0x00,
  SPI_ERROR    = 0x01,
  SPI_BUSY     = 0x02,
  SPI_TIMEOUT  = 0x03,
  SPI_INVALID_PARAM = 0x04,
} SPI_StatusTypeDef;


SPI_StatusTypeDef spi_init(SPI_HandleTypeDef* hspi,SPI_TypeDef* SPIx,SPI_InitTypeDef* spi_init_struct);

SPI_StatusTypeDef spi_tx_rx( SPI_HandleTypeDef*SPIx, const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size,uint32_t timeout);

SPI_StatusTypeDef spi_tx( SPI_HandleTypeDef*SPIx, const uint8_t* data,size_t size,uint32_t timeout);

SPI_StatusTypeDef spi_rx( SPI_HandleTypeDef*SPIx, uint8_t* rx_buffer,size_t size,uint32_t timeout);

SPI_StatusTypeDef spi_deinit(SPI_HandleTypeDef*SPIx);

void spi_select(GPIO_TypeDef* GPIOx, uint32_t Pin);

void spi_deselect(GPIO_TypeDef* GPIOx, uint32_t Pin);
