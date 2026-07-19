#include "logger_status.h"
#include "logger.h"
#include "unity.h"
#include <stdint.h>
#include "fake_backend.h"
#include "mock_fake_backend.h"
// What test I would like to have?
/*
    1. logger init:
        - all fine 
        - mount failes
        - oopen failes
    2. deinit
        - logger not ready 
        -close faiiles
        -unmount 
        failes
        -all good
    3. Write 
        - all wirte failes
        - all good
    4 logger printf 
        - is formating fine?
        - what if len <0
        - what if len > buffer size
    5. logger flush 
        - all good 
        - check what if sync failes

    I cant use CMock fake_backedn has to e used in orde to test this layer

*/

/* static values*/
static uint8_t initialized = 0;
static logger_t s_logger;
static fat_fs_storage_t s_storage;
logger_backend_t fake_backend = {
    .mount = fake_mount,
    .open = fake_open,
    .write = fake_write,
    .read = fake_read,
    .sync = fake_sync,
    .close = fake_close,
    .unmount = fake_unmount,
};

/* helper functions */
static void prepare_logger(){
    
    fake_mount_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_open_ExpectAndReturn(s_logger.storage,"log.txt", FA_WRITE | FA_OPEN_ALWAYS,LOG_OK);
    logger_init(&s_logger);
}

void setUp(void)
{

        
    s_logger.backend = &fake_backend;
    s_logger.storage = &s_storage;
    s_logger.logger_ready = false;
    
}


void tearDown(void)
{
    s_logger.backend = NULL;
    s_logger.storage = NULL;
    s_logger.logger_ready = false;
}

void test_logger_init_OK(void){
    
    fake_mount_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_open_ExpectAndReturn(s_logger.storage,"log.txt", FA_WRITE | FA_OPEN_ALWAYS,LOG_OK);

    TEST_ASSERT_EQUAL(LOG_OK,logger_init(&s_logger));

}

void test_logger_init_mount_failes(void){

    fake_mount_ExpectAndReturn(s_logger.storage,LOG_ERR_MOUNT);
    TEST_ASSERT_EQUAL(LOG_ERR_MOUNT,logger_init(&s_logger));
}

void test_logger_init_open_failes(void){

    fake_mount_ExpectAndReturn(s_logger.storage,LOG_OK);

    fake_open_ExpectAndReturn(s_logger.storage,"log.txt", FA_WRITE | FA_OPEN_ALWAYS,LOG_ERR_OPEN);
    TEST_ASSERT_EQUAL(LOG_ERR_OPEN,logger_init(&s_logger));
}

void test_logger_deInit_logger_not_ready(void){

    TEST_ASSERT_EQUAL(LOG_ERR_NOT_INITIALIZED,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_flush_failes(void){

    prepare_logger();
    fake_sync_ExpectAndReturn(s_logger.storage,!LOG_OK);
    TEST_ASSERT_EQUAL(LOG_ERR_SYNC,logger_deinit(&s_logger));
}

void test_logger_deInit_logger_close_failes(void){

    prepare_logger();
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_ERR_CLOSE);
    TEST_ASSERT_EQUAL(LOG_ERR_CLOSE,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_unmount_failes(void){

    prepare_logger();
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_unmount_ExpectAndReturn(s_logger.storage,LOG_ERR_UNMOUNT);
    TEST_ASSERT_EQUAL(LOG_ERR_UNMOUNT,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_OK(void){

    prepare_logger();
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_unmount_ExpectAndReturn(s_logger.storage,LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_deinit(&s_logger));
    TEST_ASSERT_EQUAL(false,s_logger.logger_ready);
}

void test_logger_write_failes(void){
    
    prepare_logger();
    fake_write_ExpectAnyArgsAndReturn(LOG_ERR_WRITE);
    TEST_ASSERT_EQUAL(LOG_ERR_WRITE,logger_write(&s_logger, (char[]){"1"}, 2));
}

void test_logger_write_OK(void){
    
    prepare_logger();
    fake_write_ExpectAnyArgsAndReturn(LOG_OK);
    fake_sync_ExpectAnyArgsAndReturn(LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger, (char[]){"1"}, 2));
}

void test_logger_printf_formating(void)
{
    char expectedString[20] = "123TestBuffor\n\rTest";
    prepare_logger();
    fake_write_ExpectAndReturn(&s_logger,expectedString,sizeof(expectedString),LOG_OK);
    logger_printf(&s_logger,"%dTest%s\n\r%s",123,"Buffor","Test");
}