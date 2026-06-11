
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
    f_open_ExpectAndReturn(&storage.file, "0:test.txt", FA_WRITE | FA_CREATE_ALWAYS, FR_OK);
    logger_status_t res = fatfs_open(&storage, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    TEST_ASSERT_EQUAL(LOG_OK, res);
}

void test_fatfs_backend_open_Failed(void)
{
    f_open_ExpectAnyArgsAndReturn(FR_DISK_ERR);
    logger_status_t res = fatfs_open(&storage, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    TEST_ASSERT_EQUAL(LOG_ERR_OPEN, res);
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

void test_fatfs_backend_sync_OK(void)
{
    f_sync_ExpectAndReturn(&storage.file, FR_OK);
    logger_status_t res = fatfs_sync(&storage);
    TEST_ASSERT_EQUAL(LOG_OK, res);  
}

void test_fatfs_backend_sync_Failed(void)
{
    f_sync_ExpectAndReturn(&storage.file, FR_DISK_ERR);
    logger_status_t res = fatfs_sync(&storage);
    TEST_ASSERT_EQUAL(LOG_ERR_SYNC, res);  
}

void test_fatfs_close_OK(void)
{
    f_close_ExpectAndReturn(&storage.file, FR_OK);
    logger_status_t res = fatfs_close(&storage);
    TEST_ASSERT_EQUAL(LOG_OK, res);  
}

void test_fatfs_close_Failed(void)
{
    f_close_ExpectAndReturn(&storage.file, FR_DISK_ERR);
    logger_status_t res = fatfs_close(&storage);
    TEST_ASSERT_EQUAL(LOG_ERR_CLOSE, res);  
}

void test_fatfs_backend_umount_OK(void)
{
    f_mount_ExpectAndReturn(0,FF_VOLUME,0, FR_OK);
    logger_status_t res = fatfs_umount(&storage);
    TEST_ASSERT_EQUAL(LOG_OK, res);  
}

void test_fatfs_backend_umount_Failed(void)
{
    f_mount_ExpectAndReturn(0,FF_VOLUME,0, FR_DISK_ERR);
    logger_status_t res = fatfs_umount(&storage);
    TEST_ASSERT_EQUAL(LOG_ERR_UMOUNT, res);  
}