#include "sd.h"
#include "board_config.h"
#include "cmsis_gcc.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include "stm32l432xx.h"
#include "stm32l4xx_hal_spi.h"
static SPI_HandleTypeDef sd_spi_bus = {0};

HAL_StatusTypeDef board_SD_init(void)
{
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    sd_spi_bus.Instance = SD_CARD_SPI_INSTANCE;
    sd_spi_bus.Init.Mode = SPI_MODE_MASTER;
    sd_spi_bus.Init.Direction = SPI_DIRECTION_2LINES;
    sd_spi_bus.Init.DataSize = SPI_DATASIZE_8BIT;
    sd_spi_bus.Init.CLKPolarity = SPI_POLARITY_LOW;
    sd_spi_bus.Init.CLKPhase = SPI_PHASE_1EDGE;
    sd_spi_bus.Init.NSS = SPI_NSS_SOFT;
    sd_spi_bus.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    sd_spi_bus.Init.FirstBit = SPI_FIRSTBIT_MSB;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SD_MOSI_PIN | SD_MISO_PIN | SD_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(SD_SPI_PORT, &GPIO_InitStruct);

    memset((void*)&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.Pin = SD_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SD_CS_PORT, &GPIO_InitStruct);

    return HAL_SPI_Init(&sd_spi_bus);
}

HAL_StatusTypeDef board_SD_tx_rx(const uint8_t *tx_buffer, uint8_t *rx_buffer, size_t size, uint32_t timeout)
{
    memset((void*)rx_buffer, 0xFF, size);
    __disable_irq();
    return HAL_SPI_TransmitReceive(&sd_spi_bus, (uint8_t *)tx_buffer, rx_buffer, size, timeout);
    __enable_irq();
}

HAL_StatusTypeDef board_SD_tx_byte(const uint8_t *tx_buffer, size_t size, uint32_t timeout)
{
    __disable_irq();  
    return HAL_SPI_Transmit(&sd_spi_bus, tx_buffer, size, timeout);
    __enable_irq();
}

HAL_StatusTypeDef board_SD_rx_byte(uint8_t *rx_buffer, size_t size, uint32_t timeout)
{
    memset((void*)rx_buffer, 0xFF, size);
    __disable_irq();
    return HAL_SPI_Receive(&sd_spi_bus, rx_buffer, size, timeout);
    __enable_irq();
}

HAL_StatusTypeDef board_SD_deselect(void)
{
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
    return HAL_OK;
}

HAL_StatusTypeDef board_SD_select(void)
{
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
    return HAL_OK;
}

void board_SD_delay(uint32_t delay)
{
    HAL_Delay(delay);
}

void board_SD_change_CLK_after_init(){
    __HAL_SPI_DISABLE(&sd_spi_bus);
    sd_spi_bus.Instance->CR1 &= ~(SPI_CR1_BR_Msk);
    sd_spi_bus.Instance->CR1 |= SPI_BAUDRATEPRESCALER_2;
    __HAL_SPI_ENABLE(&sd_spi_bus);
}   