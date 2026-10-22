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

static char mocked_buffor[LOGGER_INTERNAL_BUFFER_SIZE];
#define MAX_NUMMBER_OF_CALLAS 10
static char expected_buffor[MAX_NUMMBER_OF_CALLAS][LOGGER_INTERNAL_BUFFER_SIZE];
static uint8_t write_called_times;


static logger_status_t mocked_write(void* storage,const char* buffor,const size_t len){

    memcpy(mocked_buffor,buffor, len);
    return LOG_OK;

}

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
    memset(mocked_buffor, '\0', LOGGER_INTERNAL_BUFFER_SIZE);
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


void test_logger_write_buffering_off_failes(){

    char *dataToWrite = "DEADBEEF";
    prepare_logger(false);
    fake_write_ExpectAndReturn(s_logger.storage, dataToWrite, strlen(dataToWrite),LOG_ERR_WRITE);
    TEST_ASSERT_EQUAL(LOG_ERR_WRITE,logger_write(&s_logger,dataToWrite,strlen(dataToWrite)));
}

void test_logger_write_buffering_on_failes(){

    char dataToWrite[512] = {[0 ... 511] = 'A'};
    prepare_logger(false);
    fake_write_ExpectAndReturn(s_logger.storage, dataToWrite, LOGGER_INTERNAL_BUFFER_SIZE,LOG_ERR_WRITE);
    TEST_ASSERT_EQUAL(LOG_ERR_WRITE,logger_write(&s_logger,dataToWrite,LOGGER_INTERNAL_BUFFER_SIZE));
}
void test_logger_write_buffering_off(void){

    s_logger.backend->write = mocked_write;
    char *dataToWrite = "DEADBEEF";
    prepare_logger(false);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,dataToWrite,strlen(dataToWrite)));
    TEST_ASSERT_EQUAL_MEMORY(mocked_buffor,dataToWrite,strlen(dataToWrite));
    
}


void test_logger_write_buffering_on_less_than_buff_size(void){


    s_logger.backend->write = mocked_write;
    const char *dataToWrite = "DEADBEEF";
    prepare_logger(true);
    
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,dataToWrite,strlen(dataToWrite)));
   
    TEST_ASSERT_EQUAL_MEMORY(dataToWrite,
                         s_logger.internal_storage.buffor,
                         strlen(dataToWrite));
    TEST_ASSERT_EQUAL(strlen(dataToWrite),s_logger.internal_storage.buffors_head);


}

void test_logger_write_buffering_on_write_concat(void){


    s_logger.backend->write = mocked_write;
    char *first_write = "AAAAAAAAAAAA";
    char *second_write = "$$DEADBEEF$$";
    
    char dataToWrite[LOGGER_INTERNAL_BUFFER_SIZE];
    prepare_logger(true);

    memset(dataToWrite, 0, sizeof(dataToWrite));
    memcpy(dataToWrite, first_write, strlen(first_write));
    memcpy(dataToWrite + strlen(first_write), second_write, strlen(second_write));

    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,first_write,strlen(first_write)));
    TEST_ASSERT_EQUAL_MEMORY(first_write,
                            s_logger.internal_storage.buffor,
                            strlen(first_write));
    TEST_ASSERT_EQUAL(strlen(first_write),s_logger.internal_storage.buffors_head);
    
    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,second_write,strlen(second_write)));
    TEST_ASSERT_EQUAL_MEMORY(dataToWrite,
                            s_logger.internal_storage.buffor,
                            strlen(dataToWrite));
    TEST_ASSERT_EQUAL(strlen(dataToWrite),s_logger.internal_storage.buffors_head);

}


void test_logger_write_buffering_on_twice_buff_size(void){


    s_logger.backend->write = mocked_write;
    char dataToWrite[LOGGER_INTERNAL_BUFFER_SIZE*2];
    const char expected_buffor[LOGGER_INTERNAL_BUFFER_SIZE] = {[ 0 ... LOGGER_INTERNAL_BUFFER_SIZE-1]='\0'};
    memset(dataToWrite,'A',LOGGER_INTERNAL_BUFFER_SIZE*2);
    prepare_logger(true);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);

    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,dataToWrite,sizeof(dataToWrite)));

    TEST_ASSERT_EQUAL_MEMORY(expected_buffor,s_logger.internal_storage.buffor,LOGGER_INTERNAL_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(0,s_logger.internal_storage.buffors_head);


}

void test_logger_write_buffering_on_more_buff_size(void){

    const uint32_t dataToWrite_len = LOGGER_INTERNAL_BUFFER_SIZE+256;
    s_logger.backend->write = mocked_write;
    char dataToWrite[dataToWrite_len];
    const char expected_buffor[256] = {[ 0 ... (255)]='A'};
    memset(dataToWrite,'A',dataToWrite_len);
    prepare_logger(true);
    fake_sync_ExpectAndReturn(s_logger.storage,LOG_OK);

    TEST_ASSERT_EQUAL(LOG_OK,logger_write(&s_logger,dataToWrite,sizeof(dataToWrite)));
    TEST_ASSERT_EQUAL_MEMORY(expected_buffor,s_logger.internal_storage.buffor,256);
    TEST_ASSERT_EQUAL(256,s_logger.internal_storage.buffors_head);


}