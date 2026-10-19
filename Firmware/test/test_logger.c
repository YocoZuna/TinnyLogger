#include "logger_status.h"
#include "logger.h"
#include "unity.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "fake_backend.h"
#include "fat_fs_backend.h"
#include "mock_fake_backend.h"
#include <stdint.h>
#include <string.h>
#include "ff.h"

/* static values*/
static logger_t s_logger;
static fat_fs_storage_t s_storage;
static logger_backend_t fake_backend = {
    .init = fake_init,
    .write = fake_write,
    .read = fake_read,
    .sync = fake_sync,
    .deinit = fake_deinit,
};

#define MAX_NUMMBER_OF_CALLAS 10
static char expected_buffor[MAX_NUMMBER_OF_CALLAS][LOGGER_INTERNAL_BUFFER_SIZE];
static uint8_t write_called_times;


// helper functions 
static void prepare_logger(bool buffering_on){
    
    fake_init_ExpectAndReturn(s_logger.storage,LOG_OK);
    s_logger.use_buffering=buffering_on;
    logger_init(&s_logger);
}
/*
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
    
    fake_init_ExpectAndReturn(s_logger.storage,LOG_OK);

    TEST_ASSERT_EQUAL(LOG_OK,logger_init(&s_logger));
    TEST_ASSERT_EQUAL(s_logger.logger_ready,true);

}

void test_logger_init_NULL_logger(void){

    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(NULL));
    TEST_ASSERT_EQUAL(s_logger.logger_ready,false);
}

void test_logger_init_NULL_backend(void){

    s_logger.backend = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(&s_logger));
    TEST_ASSERT_EQUAL(s_logger.logger_ready,false);
}

void test_logger_init_NULL_storage(void){

    s_logger.storage = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_init(&s_logger));
    TEST_ASSERT_EQUAL(s_logger.logger_ready,false);
}

void test_logger_init_failes(void){

    fake_init_ExpectAndReturn(s_logger.storage,LOG_ERR_INIT);
    TEST_ASSERT_EQUAL(LOG_ERR_INIT,logger_init(&s_logger));
    TEST_ASSERT_EQUAL(s_logger.logger_ready,false);
}
void test_logger_init_bufforState(void){

    fake_init_ExpectAndReturn(s_logger.storage,LOG_OK);

    uint8_t tmp_buffor[LOGGER_INTERNAL_BUFFER_SIZE] = {[0 ... 5]='\0'};
    logger_init(&s_logger);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(tmp_buffor,s_logger.internal_storage.buffor,LOGGER_INTERNAL_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(0,s_logger.internal_storage.buffors_head);
    
}

void test_logger_deinit_NULL_logger(void){

    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_deinit(NULL));
}

void test_logger_deinit_NULL_backend(void){

    s_logger.backend = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_deinit(&s_logger));
}

void test_logger_deinit_NULL_storage(void){

    s_logger.storage = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_deinit(&s_logger));
}

void test_logger_deinit_logger_not_ready(void){

    TEST_ASSERT_EQUAL(LOG_ERR_NOT_INITIALIZED,logger_deinit(&s_logger));
}

void test_logger_deinit_logger_flush_failes(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,!LOG_OK);
    TEST_ASSERT_EQUAL(LOG_ERR_SYNC,logger_deinit(&s_logger));
}

void test_logger_deinit_logger_deinit_failes(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_deinit_ExpectAndReturn(s_logger.storage,LOG_ERR_DEINIT);
    TEST_ASSERT_EQUAL(LOG_ERR_DEINIT,logger_deinit(&s_logger));
}

void test_logger_deinit_logger_OK(void){

    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_deinit_ExpectAndReturn(s_logger.storage,LOG_OK); 
    TEST_ASSERT_EQUAL(LOG_OK,logger_deinit(&s_logger));
    TEST_ASSERT_EQUAL(false,s_logger.logger_ready);
}

void test_logger_write_NULL_logger(void){

    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(NULL,"",1));
}

void test_logger_write_NULL_backend(void){

    s_logger.backend = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(&s_logger,"",1));
}
void test_logger_write_NULL_storage(void){

    s_logger.storage = NULL;
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(&s_logger,"",1));
}

void test_logger_write_NULL_buffor(void){


    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(&s_logger,NULL,1));
}

void test_logger_write_invalid_len(void){
    
    TEST_ASSERT_EQUAL(LOG_ERR_INVALID_PARAM,logger_write(&s_logger,"",0));

}


void test_logger_write_write_failes(){
    TEST_ASSERT(false);
}

void test_logger_write_buffering_off(void){

    char tmp_buffor[10];
    memset(tmp_buffor,'A',10);
    prepare_logger(false);
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,tmp_buffor,10));
}