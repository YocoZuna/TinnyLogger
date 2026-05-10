#ifndef AHT20_TYPEDEFS_H
#define AHT20_TYPEDEFS_H

// AHT2- default address
#define AHT20_ADDRESS 0x38u


#define INIT_CALIB_CMD 0xBEu
#define INIT_CALIB_LOW 0x00u
#define INIT_CALIB_HIGH 0x08u

/*
In order to trigger measumrent we need to 0xAC command with two parameters
0x33 and 0x00
*/
#define TRIGGER_MEASURMENT_CMD 0xACu
#define TRIGGER_MEASURMENT_LOW 0x00u
#define TRIGGER_MEASURMENT_HIGH 0x33u

#define SOFT_RESET_CMD 0xBAu
#define READ_STATUS_REGISTER_CMD 0x71u
/*

Bit  |      Definition      |   Description 
 7   |   busy indication    |   1 -> busy in measurment, 0-> free in dormant state
 6   |   reserved           |   remained
 5   |   reserved           |   remained
 4   |   reserved           |   remained
 3   |   calibration status |   1-> calibrated, 0 -> uncalibrated
 2   |   reserved           |   remained
 1   |   reserved           |   remained
 0   |   reserved           |   remained
*/
#define AHT20_BUSY_IN_MEASURMENT(status) ((status)&0x80)
#define AHT20_CHECK_CALIBRATION(status) ((status)&0x08)

typedef enum {
    AHT20_OK = 1,
    AHT20_ERROR_COMM = -1,
    AHT20_ERROR_CRC = -2,
    AHT20_ERROR_TIMEOUT = -3,
    AHT20_ERROR_NULL_PTR = -4,
    AHT20_ERROR_INVALID_ARG = -5
} AHT20_Status_t;


#endif