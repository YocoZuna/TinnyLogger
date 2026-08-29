#include "sd.h"
#include "board/TinnyLogger/TinnyLoggerSTM/spi.h"
#include "spi.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>

static SD_status_t sd_tx_rx(SD_IO_t* card,const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout);
static SD_status_t sd_tx(SD_IO_t* card,const uint8_t *tx_buffer,size_t size, uint32_t timeout);
static SD_status_t sd_rx(SD_IO_t* card,uint8_t *rx_buffer,size_t size,uint32_t timeout);
static SD_status_t sd_deselect(SD_IO_t* card);
static void sd_select(SD_IO_t* card);
static void sd_delay(uint32_t delay);



static SD_status_t sd_status_from_io(uint32_t status){

  #if defined (TINNY_LOGGER_STM)
    switch(status){

      case HAL_OK:
        return SD_OK;
      case HAL_ERROR:
        return SD_ERROR;
      case HAL_BUSY:
        return SD_BUSY;
      case HAL_TIMEOUT:
        return SD_TIMEOUT;
      
      default:
        return SD_UNKNOWN_ERROR;
    }
  #endif
}

SD_status_t sd_init(SD_IO_t* card){
  
  if(card==NULL) return SD_INVALID_PARAM;

  card->tx_rx = sd_tx_rx;
  card->tx_byte = sd_tx;
  card->rx_byte = sd_rx;
  card->deselect = sd_deselect;
  card->select = sd_select;
  card->delay = sd_delay;

  return SD_OK;
}

static SD_status_t sd_tx_rx(SD_IO_t* card,const uint8_t *tx_buffer,uint8_t *rx_buffer,size_t size,uint32_t timeout){
  
  if(tx_buffer==NULL || rx_buffer==NULL || size==0) return SD_INVALID_PARAM;
  return sd_status_from_io(spi_tx_rx(card->ctx, tx_buffer, rx_buffer, size, timeout));
}
static SD_status_t sd_tx(SD_IO_t* card,const uint8_t *tx_buffer,size_t size, uint32_t timeout){
  
  if(tx_buffer==NULL || size==0) return SD_INVALID_PARAM;
  return sd_status_from_io(spi_tx(card->ctx, tx_buffer, size, timeout));
}
static SD_status_t sd_rx(SD_IO_t* card,uint8_t *rx_buffer,size_t size,uint32_t timeout){
  
  if(rx_buffer==NULL || size==0) return SD_INVALID_PARAM;
  return sd_status_from_io(spi_rx(card->ctx,rx_buffer, size, timeout));
}

static SD_status_t sd_deselect(SD_IO_t* card){
  
  spi_deselect(card->ctx);
  return sd_status_from_io(sd_tx(card,SD_DUMMY_BYTE, sizeof(SD_DUMMY_BYTE),TIMEOUT));
}

static void sd_select(SD_IO_t* card){
  spi_select(card->ctx);
  sd_delay(1);
}
static void sd_delay(uint32_t delay){
  
  HAL_Delay(delay);
}


