#include "i2c.h"
#include "error.h"

error_status_t P_i2c_init(void){
    return map_hal_status_to_error(board_init_i2c());
}
error_status_t P_i2c_tx(const uint8_t slaveAddr, uint8_t *data, size_t size, uint32_t timeout){
    if(data==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_i2c_tx(slaveAddr, data,size,timeout));

}
error_status_t P_i2c_rx(const uint8_t slaveAddr, uint8_t *data, size_t size, uint32_t timeout){
    if(data==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_i2c_rx(slaveAddr,data,size,timeout));

}
error_status_t P_i2c_write_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    if(data==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_i2c_write_mem(slaveAddr,memAddr,data,size,timeout));

}
error_status_t P_i2c_read_mem(const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    if(data==NULL || size==0) return ERROR_INVALID_PARAM;
    return map_hal_status_to_error(board_i2c_read_mem(slaveAddr,memAddr,data,size,timeout));

}
error_status_t P_i2c_deinit(void){
    return map_hal_status_to_error(board_i2c_deinit());
}



