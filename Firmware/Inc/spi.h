#pragma once


#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"


#define CS_PIN LL_GPIO_PIN_11
#define CS_PORT GPIOB

/*
    * @brief Initializes the SPI peripheral.
    *
    * @param SPIx: Pointer to the SPI peripheral.
*/
void spi_init(SPI_TypeDef*SPIx,LL_SPI_InitTypeDef* spi_init_struct);
/*
    * @brief Transmits and receives data over SPI. This function sends data from the tx_buffer and simultaneously receives data into the rx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param tx_buffer: Pointer to the buffer containing data to be transmitted.
    * @param rx_buffer: Pointer to the buffer where received data will be stored.
    * @param size: Number of bytes to transmit and receive.
*/
void spi_tx_rx(SPI_TypeDef*SPIx, uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t size);
/*
    * @brief Transmits  single byte over SPI.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param byte: The byte to be transmitted.
*/
void spi_tx_byte(SPI_TypeDef*SPIx, uint8_t byte);
/*
    * @brief Receives a single byte over SPI.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param byte: Pointer to the variable where the received byte will be stored.
    * @return The received byte.
*/
uint8_t spi_rx_byte(SPI_TypeDef*SPIx, uint8_t* byte);
/*
    * @brief Transmits data over SPI. This function sends data from the tx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param tx_buffer: Pointer to the buffer containing data to be transmitted.
    * @param size: Number of bytes to transmit.
*/
void spi_tx(SPI_TypeDef*SPIx, uint8_t* tx_buffer, uint16_t size);
/*
    * @brief Receives data over SPI. This function receives data into the rx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param rx_buffer: Pointer to the buffer where received data will be stored.
    * @param size: Number of bytes to receive.
*/
void spi_rx(SPI_TypeDef*SPIx, uint8_t* rx_buffer, uint16_t size);
/*
    * @brief Deinitializes the SPI peripheral.
    *
    * @param SPIx: Pointer to the SPI peripheral.
*/
void spi_deinit(SPI_TypeDef*SPIx);
/*
    * @brief Selects the SPI slave by setting the corresponding GPIO pin low.
    *
    * @param GPIOx: Pointer to the GPIO port where the slave select pin is located.
    * @param Pin: Specifies the GPIO pin number used for slave select (e.g., LL_GPIO_PIN_0).
*/
void spi_select(GPIO_TypeDef* GPIOx, uint32_t Pin);
/*
    * @brief Deselects the SPI slave by setting the corresponding GPIO pin high.
    *
    * @param GPIOx: Pointer to the GPIO port where the slave select pin is located.
    * @param Pin: Specifies the GPIO pin number used for slave select (e.g., LL_GPIO_PIN_0).
*/
void spi_deselect(GPIO_TypeDef* GPIOx, uint32_t Pin);

