#pragma once


#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "time.h"
#include <stddef.h>

#define CS_PIN LL_GPIO_PIN_11
#define CS_PORT GPIOA

/*
    * @brief Initializes the SPI peripheral.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param spi_init_struct: Pointer to a structure containing the SPI initialization parameters (e.g., mode, baud rate, data width).
    * @return void indicating success or failure of the initialization.
*/
void spi_init(SPI_TypeDef*SPIx,LL_SPI_InitTypeDef* spi_init_struct);
/*
    * @brief Transmits and receives data over SPI. This function sends data from the tx_buffer and simultaneously receives data into the rx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param tx_buffer: Pointer to the buffer containing data to be transmitted.
    * @param rx_buffer: Pointer to the buffer where received data will be stored.
    * @param size: Number of bytes to transmit and receive.
    * @param timeout: Timeout value for the operation.
    * @return void indicating success or failure of the operation.
*/
void spi_tx_rx( SPI_TypeDef*SPIx, const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size);
/*
    * @brief Transmits data over SPI. This function sends data from the tx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param tx_buffer: Pointer to the buffer containing data to be transmitted.
    * @param size: Number of bytes to transmit.
    * @param timeout: Timeout value for the operation.
    * @return void indicating success or failure of the operation.
*/
void spi_tx_byte( SPI_TypeDef*SPIx, const uint8_t data);
/*
    * @brief Receives data over SPI. This function receives data into the rx_buffer.
    *
    * @param SPIx: Pointer to the SPI peripheral.
    * @param rx_buffer: Pointer to the buffer where received data will be stored.
    * @param size: Number of bytes to receive.
    * @param timeout: Timeout value for the operation.
    * @return void indicating success or failure of the operation.
*/
void spi_rx_byte( SPI_TypeDef*SPIx, uint8_t* rx_buffer);
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

