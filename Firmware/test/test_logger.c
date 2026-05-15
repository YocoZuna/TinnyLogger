#include "unity.h"
#include "logger.h"
#include "mock_ff.h"

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
    UINT bytes_written = 3;

    f_mount_IgnoreAndReturn(FR_OK);
    f_open_IgnoreAndReturn(FR_OK);
    TEST_ASSERT_EQUAL(LOG_OK, logger_init());

    f_write_ExpectAnyArgsAndReturn(FR_OK);
    f_write_ReturnThruPtr_bw(&bytes_written);

    f_sync_IgnoreAndReturn(FR_OK);

    TEST_ASSERT_EQUAL(LOG_OK, logger_write("abc", bytes_written));
}

