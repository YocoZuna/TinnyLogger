#include "sd.h"
#include "TAL/inc/board/TinnyLogger/TinnyLoggerSTM/sd.h"
#include "board/TinnyLogger/TinnyLoggerSTM/spi.h"
#include "spi.h"
#include "stm32l4xx_hal_spi.h"

/* SD SPI pins*/
#define SD_CS_PIN  GPIO_PIN_11
#define SD_CS_PORT GPIOA

#define SD_CARD_SPI_INSTANCE SPI3
#define SD_MOSI_PIN GPIO_PIN_3
#define SD_MISO_PIN GPIO_PIN_4
#define SD_SCK_PIN GPIO_PIN_5
#define SD_SPI_PORT GPIOB

SD_IO_t P_hSD_card;

void P_init_SD_card(){

	spi_bus_t sd_card_spi = {
		.cs_pin = SD_CS_PIN,
		.cs_port = SD_CS_PORT,
		.instance = SD_SPI_PORT
	}

	spi_cfg_t sd_card_cfg = {
		.baudrate_prescaler = SPI_BAUDRATEPRESCALER_2
	}

	spi_init(spi_bus_t *bus, const spi_cfg_t *cfg)
}
void P_deinit_SD_card();