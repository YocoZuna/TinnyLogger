#include "logger_status.h"
#include "logger.h"
#include "unity.h"
#include <stdint.h>
#include "fake_backend.h"
#include "mock_fake_backend.h"
#include <string.h>


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


//TODO dodac testy na invalid parametry

#define MAX_NUMMBER_OF_CALLAS 10
static char expected_buffor[MAX_NUMMBER_OF_CALLAS][LOGGER_INTERNAL_BUFFER_SIZE];
static uint8_t write_called_times;

/* helper functions */
static void prepare_logger(bool buffering_on){
    
    fake_mount_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_open_ExpectAndReturn(s_logger.storage,"log.txt", FA_WRITE | FA_OPEN_ALWAYS,LOG_OK);
    s_logger.use_buffering=buffering_on;
    logger_init(&s_logger);
}
static logger_status_t write_check(void* storage,const char* buffor,const size_t len){
    

    TEST_ASSERT_EQUAL(LOGGER_INTERNAL_BUFFER_SIZE,len);
    TEST_ASSERT_EQUAL(s_logger.storage,storage);
    TEST_ASSERT_EQUAL_MEMORY(expected_buffor[write_called_times], buffor, LOGGER_INTERNAL_BUFFER_SIZE);
    write_called_times++;

    return  LOG_OK;
}
/************************************************************************************ */
void setUp(void)
{

        
    s_logger.backend = &fake_backend;
    s_logger.storage = &s_storage;
    s_logger.logger_ready = false;
    write_called_times=0;
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

void test_logger_init_NULL_logger(void){

    memset(&s_logger, NULL, sizeof(s_logger));
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(&s_logger));
}

void test_logger_init_NULL_backend(void){

    memset(&s_logger.backend, NULL, sizeof(&s_logger.backend));
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(&s_logger));
}
void test_logger_init_NULL_storage(void){

    memset(&s_logger.storage, NULL, sizeof(&s_logger.storage));
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(&s_logger));
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

void test_logger_deinit_NULL_logger(void){

    //memset(&s_logger, NULL, sizeof(s_logger));
    //TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_deinit(&s_logger));
}


void test_logger_deInit_logger_not_ready(void){

    TEST_ASSERT_EQUAL(LOG_ERR_NOT_INITIALIZED,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_flush_failes(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,!LOG_OK);
    TEST_ASSERT_EQUAL(LOG_ERR_SYNC,logger_deinit(&s_logger));
}

void test_logger_deInit_logger_close_failes(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_ERR_CLOSE);
    TEST_ASSERT_EQUAL(LOG_ERR_CLOSE,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_unmount_failes(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_unmount_ExpectAndReturn(s_logger.storage,LOG_ERR_UNMOUNT);
    TEST_ASSERT_EQUAL(LOG_ERR_UNMOUNT,logger_deinit(&s_logger));
}
void test_logger_deInit_logger_OK(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_close_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_unmount_ExpectAndReturn(s_logger.storage,LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_deinit(&s_logger));
    TEST_ASSERT_EQUAL(false,s_logger.logger_ready);
}

void test_logger_write_failes_buffering_off(void){
    
    prepare_logger(false);
    fake_write_ExpectAnyArgsAndReturn(LOG_ERR_WRITE);
    TEST_ASSERT_EQUAL(LOG_ERR_WRITE,logger_write(&s_logger, (char[]){"1"}, 2));
}

void test_logger_write_OK_buffering_off(void){
    
    prepare_logger(false);
    fake_write_ExpectAndReturn(s_logger.storage, (char[]){"1"}, 2,LOG_OK);
    fake_sync_ExpectAnyArgsAndReturn(LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger, (char[]){"1"}, 2));
}

void test_logger_write_OK_buffering_write_full_buffer_on(void){
    
    prepare_logger(true);
    char buffor[LOGGER_INTERNAL_BUFFER_SIZE];

    memset(buffor,'A',LOGGER_INTERNAL_BUFFER_SIZE);
    memcpy(expected_buffor,buffor,LOGGER_INTERNAL_BUFFER_SIZE);

    fake_write_StubWithCallback(write_check);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger, buffor, LOGGER_INTERNAL_BUFFER_SIZE));
    TEST_ASSERT_EQUAL(0,s_logger.internal_storage.buffors_head);

}
void test_logger_write_OK_buffering_write_buffer_on(void){
    
    prepare_logger(true);   
    char buffor[10];
    memset(buffor,'A',sizeof(buffor));

    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger, buffor, sizeof(buffor)));
    TEST_ASSERT_EQUAL(10,s_logger.internal_storage.buffors_head);
    TEST_ASSERT_EQUAL_STRING(buffor,s_logger.internal_storage.buffor);
}

void test_logger_write_write_twice_than_buff_size_buffering_on(void){
    
    prepare_logger(true);
    char buffor[(LOGGER_INTERNAL_BUFFER_SIZE*2)+10];
    
    memset(buffor,'A',LOGGER_INTERNAL_BUFFER_SIZE);
    memcpy(expected_buffor[0],buffor,LOGGER_INTERNAL_BUFFER_SIZE);

    memset(buffor+LOGGER_INTERNAL_BUFFER_SIZE,'B',LOGGER_INTERNAL_BUFFER_SIZE);
    memcpy(expected_buffor[1],buffor+(LOGGER_INTERNAL_BUFFER_SIZE),LOGGER_INTERNAL_BUFFER_SIZE);

    memset(buffor+(2*LOGGER_INTERNAL_BUFFER_SIZE),'C',10);
    memcpy(expected_buffor[2],buffor+(2*LOGGER_INTERNAL_BUFFER_SIZE),10);


    fake_write_StubWithCallback(write_check);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_write_StubWithCallback(write_check);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,buffor,sizeof(buffor)));
    TEST_ASSERT_EQUAL(10,s_logger.internal_storage.buffors_head);
    TEST_ASSERT_EQUAL(2,write_called_times);
    
}

void test_logger_write_NULL_logger(void){

    memset(&s_logger, NULL, sizeof(s_logger));
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(&s_logger,"1",2));
}

