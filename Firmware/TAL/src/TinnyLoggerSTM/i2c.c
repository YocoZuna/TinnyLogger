#include "i2c.h"

i2c_status_t i2c_init(i2c_bus_t *bus, i2c_cfg_t *cfg)
{
    if(bus==NULL || cfg==NULL) return I2C_INVALID_PARAM;
    
    bus->hi2c.Instance = &bus->instance;
    bus->hi2c.Mode = cfg->mode;
    bus->hi2c.Init.Timing = cfg->timing;
    bus->hi2c.Init.OwnAddress1 = cfg->own_address_low;
    bus->hi2c.Init.AddressingMode = cfg->addressing_mode;
    bus->hi2c.Init.DualAddressMode = cfg->dual_address_mode;
    bus->hi2c.Init.OwnAddress2 = cfg->own_address_high;
    bus->hi2c.Init.OwnAddress2Masks = cfg->own_address_high_masks;
    bus->hi2c.Init.GeneralCallMode = cfg->general_call_mode;
    bus->hi2c.Init.NoStretchMode = cfg->no_stretch_mode;

    return (i2c_status_t)HAL_I2C_Init(&bus->hi2c);
}
i2c_status_t i2c_tx(i2c_bus_t *bus,const uint8_t slaveAddr, uint8_t *data,size_t size,uint32_t timeout)
{
    if(bus==NULL || data==NULL || size==0) return I2C_INVALID_PARAM;
    return (i2c_status_t)HAL_I2C_Master_Transmit(&bus->hi2c, slaveAddr, data, size, timeout);
}
i2c_status_t i2c_rx(i2c_bus_t *bus,const uint8_t slaveAddr, uint8_t * data, size_t size,uint32_t timeout){

    if(bus==NULL || data==NULL || size==0) return I2C_INVALID_PARAM;
    return (i2c_status_t)HAL_I2C_Master_Receive(&bus->hi2c, slaveAddr, data, size, timeout);
}
i2c_status_t i2c_write_mem(i2c_bus_t *bus, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    
    if(bus==NULL || data==NULL || size==0) return I2C_INVALID_PARAM;
    return (i2c_status_t)HAL_I2C_Mem_Write(&bus->hi2c, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, size, timeout);
}
i2c_status_t i2c_read_mem(i2c_bus_t *bus, const uint8_t slaveAddr, const uint16_t memAddr, uint8_t *data, size_t size, uint32_t timeout){
    
    if(bus==NULL || data==NULL || size==0) return I2C_INVALID_PARAM;
    return (i2c_status_t)HAL_I2C_Mem_Read(&bus->hi2c, slaveAddr, memAddr, I2C_MEMADD_SIZE_16BIT, data, size, timeout);
}
i2c_status_t i2c_deinit(i2c_bus_t *bus)
{
    if(bus==NULL) return I2C_INVALID_PARAM;
    return (i2c_status_t)HAL_I2C_DeInit(&bus->hi2c);
}


