
#include "ff.h"
#include "unity.h"
#include "mock_ff.h"
#include "fat_fs_backend.h"

fat_fs_storage_t storage;
void setUp(void)
{

}

void tearDown(void)
{
    
}

void test_fatfs_backend_mount_OK(void)
{
    
    f_mount_ExpectAndReturn(&storage.fs, FF_VOLUME, 1, FR_OK);
    logger_status_t res = fatfs_mount(&storage);
    TEST_ASSERT_EQUAL(LOG_OK, res);
}

void test_fatfs_backend_mount_Failed(void)
{

    f_mount_ExpectAndReturn(&storage.fs, FF_VOLUME, 1, FR_DISK_ERR);
    logger_status_t res = fatfs_mount(&storage);
    TEST_ASSERT_EQUAL(LOG_ERR_MOUNT, res);
}

void test_fatfs_backend_open_OK(void)
{
    TEST_ASSERT_TRUE(0);
}


void test_fatfs_backend_write_OK(void)
{
    UINT bytes_written = 5;
    /*
    
    It would be wrth to check f_write parameters, but it is not possible to use ExpectAndReturn, because of pointer parameters.
    We could etiher use ExpectANyArgsAndReturn and get bytes_written value through ReturnThruPtr
    Or create callback which will take parameters do TEST_ASSERT_..  and then return FR_OK. I think I will choose first option
    */
    f_write_ExpectAndReturn(&storage.fs, "Hello", 5, &bytes_written, FR_OK);
    f_write_IgnoreArg_bw();
    f_write_ReturnThruPtr_bw(&bytes_written);
    logger_status_t res = fatfs_write(&storage,"Hello",5);
    TEST_ASSERT_EQUAL(LOG_OK, res);  
}

void test_fatfs_backend_write_bytes_written_OK(void)
{
    UINT bytes_written = 5;
    f_write_ExpectAnyArgsAndReturn(FR_OK);
    f_write_ReturnThruPtr_bw(&bytes_written);
    logger_status_t res = fatfs_write(&storage,"Hello",5);
    TEST_ASSERT_EQUAL(LOG_OK, res);  
}

void test_fatfs_backend_write_bytes_written_NOK(void)
{
    UINT bytes_written = 3;
    f_write_ExpectAnyArgsAndReturn(FR_OK);
    f_write_ReturnThruPtr_bw(&bytes_written);
    logger_status_t res = fatfs_write(&storage,"Hello",5);
    TEST_ASSERT_EQUAL(LOG_ERR_WRITE, res);  
}