#include "sd.h"
#include "error.h"
#include <stdint.h>
error_status_t P_SD_init(void)
{
  return map_hal_status_to_error(board_SD_init());
}

error_status_t P_SD_tx_rx(const uint8_t *tx_buffer, uint8_t *rx_buffer, size_t size, uint32_t timeout)
{
  if(tx_buffer == NULL || rx_buffer == NULL || size == 0) return ERROR_INVALID_PARAM;
  return map_hal_status_to_error(board_SD_tx_rx(tx_buffer, rx_buffer, size, timeout));
}

error_status_t P_SD_tx_byte(const uint8_t *tx_buffer, size_t size, uint32_t timeout)
{
  if(tx_buffer == NULL || size == 0) return ERROR_INVALID_PARAM;
  return map_hal_status_to_error(board_SD_tx_byte(tx_buffer, size, timeout));
}

error_status_t P_SD_rx_byte(uint8_t *rx_buffer, size_t size, uint32_t timeout)
{
  if(rx_buffer == NULL || size == 0) return ERROR_INVALID_PARAM;
  return map_hal_status_to_error(board_SD_rx_byte(rx_buffer, size, timeout));
}

error_status_t P_SD_deselect(void)
{
  error_status_t ret = map_hal_status_to_error(board_SD_deselect());
  if (ret!=ERROR_OK) return ret;

  /*MUST SEND X DATA CLOCS AFTER DESELECT!!! CARD WILL FAIL WITHOUT THIOS*/
  uint8_t tx_buffor[6] = {[0 ... 5]=0xFF};
  return map_hal_status_to_error(board_SD_tx_byte(tx_buffor,sizeof(tx_buffor),100));
  
}

error_status_t P_SD_select(void)
{
  return map_hal_status_to_error(board_SD_select());
}

void P_SD_delay(uint32_t delay)
{
  board_SD_delay(delay);
}

void P_SD_change_CLK_after_init(){
  board_SD_change_CLK_after_init();
}   
