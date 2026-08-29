#include "spi.h"
#include <string.h>

spi_status_t spi_init(spi_bus_t *bus, const spi_cfg_t *cfg)
{
    if (bus == NULL || cfg == NULL) return SPI_INVALID_PARAM;

	bus->hspi.Instance = &bus->instance;
    bus->hspi.Init.Mode = cfg->mode;
    bus->hspi.Init.Direction = cfg->direction;
    bus->hspi.Init.DataSize = cfg->data_size;
    bus->hspi.Init.CLKPolarity = cfg->clock_polarity;
    bus->hspi.Init.CLKPhase = cfg->clock_phase;
    bus->hspi.Init.NSS = cfg->nss;
    bus->hspi.Init.BaudRatePrescaler = cfg->baudrate_prescaler;
    bus->hspi.Init.FirstBit = cfg->first_bit;

    return (spi_status_t)HAL_SPI_Init(&bus->hspi);
}

spi_status_t spi_tx_rx(spi_bus_t *bus,const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t len,uint32_t timeout)
{
    if(bus==NULL || tx_buffer==NULL || rx_buffer==NULL || len==0) return SPI_INVALID_PARAM;
    memset((void*)rx_buffer, 0xFF, len);
    return (spi_status_t)HAL_SPI_TransmitReceive(&bus->hspi,(uint8_t*)tx_buffer, rx_buffer, len, timeout);

}

spi_status_t spi_tx(spi_bus_t *bus, const uint8_t *data, size_t len, uint32_t timeout)
{
    if(bus==NULL || data==NULL || len==0) return SPI_INVALID_PARAM;
    return (spi_status_t)HAL_SPI_Transmit(&bus->hspi, (uint8_t*)data, len, timeout);

}

spi_status_t spi_rx(spi_bus_t *bus, uint8_t *data, size_t len, uint32_t timeout)
{
    if(bus==NULL || data==NULL || len==0) return SPI_INVALID_PARAM;
    memset((void*)data, 0xFF, len);
    return (spi_status_t)HAL_SPI_Receive(&bus->hspi, data, len, timeout);

}
spi_status_t spi_deinit(spi_bus_t *bus)
{
    if(bus==NULL) return SPI_INVALID_PARAM;
    return (spi_status_t)HAL_SPI_DeInit(&bus->hspi);
}

void spi_select(const spi_bus_t *bus)
{
    HAL_GPIO_WritePin(bus->cs_port,bus->cs_pin, GPIO_PIN_RESET);
}

void spi_deselect(const spi_bus_t *bus)
{
    HAL_GPIO_WritePin(bus->cs_port,bus->cs_pin, GPIO_PIN_SET);
}
