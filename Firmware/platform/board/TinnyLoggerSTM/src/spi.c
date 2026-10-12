#include "spi.h"
#include "board_config.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_spi.h"
#include <string.h>

SPI_HandleTypeDef spi_bus = {0};

HAL_StatusTypeDef board_init_spi(void)
{
    __HAL_RCC_SPI1_CLK_ENABLE();
    spi_bus.Instance = SPI_INSTANCE;
    spi_bus.Init.Mode = SPI_MODE_MASTER;
    spi_bus.Init.Direction = SPI_DIRECTION_2LINES;
    spi_bus.Init.DataSize = SPI_DATASIZE_8BIT;
    spi_bus.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi_bus.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi_bus.Init.NSS = SPI_NSS_SOFT;
    spi_bus.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    spi_bus.Init.FirstBit = SPI_FIRSTBIT_MSB;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SPI_MOSI_PIN | SPI_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI_SPI_PORT, &GPIO_InitStruct);

    memset((void*)&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.Pin = SPI_MISO_PIN ;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI_SPI_PORT, &GPIO_InitStruct);

    memset((void*)&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.Pin = SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_CS_PORT, &GPIO_InitStruct);

    return HAL_SPI_Init(&spi_bus);
}

HAL_StatusTypeDef board_spi_tx_rx(const uint8_t *tx_buffer, uint8_t *rx_buffer, size_t len, uint32_t timeout)
{
    memset((void*)rx_buffer, 0xFF, len);
    return HAL_SPI_TransmitReceive(&spi_bus, (uint8_t*)tx_buffer, rx_buffer, len, timeout);
}

HAL_StatusTypeDef board_spi_tx(const uint8_t *data, size_t len, uint32_t timeout)
{
    return HAL_SPI_Transmit(&spi_bus, (uint8_t*)data, len, timeout);
}

HAL_StatusTypeDef board_spi_rx(uint8_t *data, size_t len, uint32_t timeout)
{
    memset((void*)data, 0xFF, len);
    return HAL_SPI_Receive(&spi_bus, data, len, timeout);
}

HAL_StatusTypeDef board_spi_deinit(void)
{
    return HAL_SPI_DeInit(&spi_bus);
}

void board_spi_select(const spi_slave_io_t *cs)
{
    HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_RESET);
}

void board_spi_deselect(const spi_slave_io_t *cs)
{
    HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_SET);
}


