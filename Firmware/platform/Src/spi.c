#include "spi.h"
#include "sd_commands.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_spi.h"
#include <stddef.h>
#include <stdint.h>



void spi_init( SPI_TypeDef*SPIx,LL_SPI_InitTypeDef* spi_init_struct)
{
    LL_SPI_Disable(SPIx);
    if (SPIx == SPI1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    else if (SPIx == SPI3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
    
    LL_SPI_Init(SPIx, spi_init_struct);
    LL_SPI_Enable(SPIx);
}

int8_t spi_tx_rx( SPI_TypeDef*SPIx, const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t size,uint32_t timeout)
{
    uint8_t recv;
    __IO uint32_t start_time = getTime();
    for (size_t i = 0; i < size; i++) {
        while (!LL_SPI_IsActiveFlag_TXE(SPIx)){
            if(getTime() - start_time >= timeout) {
                return -1;
            }
        };

        LL_SPI_TransmitData8(SPIx, tx_buffer[i]);
        start_time = getTime();

        while (!LL_SPI_IsActiveFlag_RXNE(SPIx)&& (getTime() - start_time) < timeout){
            if(getTime() - start_time >= timeout) {
                return -1; 
            }
        }
        
        recv=LL_SPI_ReceiveData8(SPIx); // discard

        if(rx_buffer!=NULL) rx_buffer[i]=recv;

        start_time = getTime();
        while (LL_SPI_IsActiveFlag_BSY(SPIx)){
            if(getTime() - start_time >= timeout) {
                return -1; 
            }
        };
    }

    return 0;
}

int8_t spi_tx_byte(SPI_TypeDef *SPIx, const uint8_t data,uint32_t timeout) {

    __IO uint32_t start_time = getTime();
    while (!LL_SPI_IsActiveFlag_TXE(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    }

    LL_SPI_TransmitData8(SPIx, data);

    start_time = getTime();
    while (!LL_SPI_IsActiveFlag_RXNE(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    }

    (void)LL_SPI_ReceiveData8(SPIx); // discard

    start_time = getTime();
    while (LL_SPI_IsActiveFlag_BSY(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    };

    return 0;
}

int8_t spi_rx_byte(SPI_TypeDef *SPIx, uint8_t *data,uint32_t timeout) {

    __IO uint32_t start_time = getTime();
    while (!LL_SPI_IsActiveFlag_TXE(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    };

    LL_SPI_TransmitData8(SPIx, 0xFF); // dummy

    start_time = getTime();
    while (!LL_SPI_IsActiveFlag_RXNE(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    }

    *data = LL_SPI_ReceiveData8(SPIx);
    
    start_time = getTime();
    while (LL_SPI_IsActiveFlag_BSY(SPIx)){
        if(getTime() - start_time >= timeout) {
            return -1; 
        }
    };
    return 0;
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
