#include "ff.h"
#include "unity.h"
#include "logger.h"
#include "mock_ff.h"

extern FATFS fs;
extern FIL fil;
extern PARTITION VolToPart[FF_VOLUMES];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_logger_write_returns_error_when_not_initialized(void)
{
    TEST_ASSERT_EQUAL(LOG_ERR_NOT_INITIALIZED, logger_write("abc", 3));
}



void test_logger_write_succeeds_when_fatfs_write_and_sync_ok(void)
{
    UINT bytes_written = 2;

    f_mount_IgnoreAndReturn(FR_OK);
    f_open_IgnoreAndReturn(FR_OK);
    TEST_ASSERT_EQUAL(LOG_OK, logger_init());

    f_write_ExpectAnyArgsAndReturn(FR_OK);
    f_write_ReturnThruPtr_bw(&bytes_written);

    f_sync_IgnoreAndReturn(FR_OK);

    TEST_ASSERT_EQUAL(LOG_OK, logger_write("abc", bytes_written));
}

void test_logger_init_arguments_check(void){

    f_mount_ExpectAndReturn(&fs, FF_VOLUME, 1, FR_OK);
    f_open_ExpectAndReturn(&fil, "0:TinnyLogger.txt", FA_WRITE | FA_OPEN_APPEND, FR_OK);
    TEST_ASSERT_EQUAL(LOG_OK, logger_init());
}

void test_logger_init_return_errors(void){

    // Error when not mouinting. The error value is not important, logger will faile when !FR_OK
    f_mount_IgnoreAndReturn(FR_WRITE_PROTECTED);
    TEST_ASSERT_EQUAL(LOG_ERR_MOUNT, logger_init());
    
    // Error when not opening file. The error value is not important, logger will faile when !FR_OK
    f_mount_IgnoreAndReturn(FR_OK);
    f_open_IgnoreAndReturn(FR_NO_FILE);
    TEST_ASSERT_EQUAL(LOG_ERR_OPEN, logger_init());

}   
