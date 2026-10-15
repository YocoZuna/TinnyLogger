#include "fat_fs_backend.h"
#include "logger.h"
#include <stdio.h>
#include "ff.h"
#include "logger_status.h"


#define MAKE_PATH(base,filename) base filename

PARTITION VolToPart[FF_VOLUMES] = {
    {1, 0},   // "0:" → physical drive 0, whole drive
};

static logger_status_t fatfs_mount(void* storage){

    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FATFS* fs = &fat_storage->fs;
    
    if(f_mount(fs, FF_VOLUME, 1)!=FR_OK) return LOG_ERR_INIT;
    return LOG_OK;
}
static logger_status_t fatfs_open(void* storage,const char* filename,const BYTE mode){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;

    if(f_open(file, MAKE_PATH(FF_VOLUME, FILE_NAME), mode)!=FR_OK) return LOG_ERR_INIT;
    return LOG_OK;
}
static logger_status_t fatfs_close(void* storage){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    if(f_close(file)!=FR_OK) return LOG_ERR_DEINIT;
    return LOG_OK;
}
static logger_status_t fatfs_umount(void* storage){

    if(f_unmount(FF_VOLUME)!=FR_OK) return LOG_ERR_DEINIT;
    return LOG_OK;
}

logger_status_t fatfs_init(void*storage){

    logger_status_t status;
    if((status =  fatfs_mount(storage))!=LOG_OK) return status;
    if((status =  fatfs_open(storage, FILE_NAME, FA_WRITE | FA_OPEN_APPEND))!=LOG_OK) return status;

   return LOG_OK;
}

logger_status_t fatfs_write(void* storage,const char* buffor,const size_t len){

    UINT bytes_written;
    FRESULT ret;
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    ret = f_write(file, buffor, len, &bytes_written);
    if(ret!=FR_OK || bytes_written!=len) return LOG_ERR_WRITE;
    return LOG_OK;
}
logger_status_t fatfs_read(void* storage,char* buffor,const size_t len){

    UINT bytes_read;
    FRESULT ret;
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    ret= f_read(file, buffor, len, &bytes_read);
    if(ret!=FR_OK || bytes_read!=len) return LOG_ERR_READ;
    return LOG_OK;
}
logger_status_t fatfs_sync(void* storage){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    if(f_sync(file)!=FR_OK) return LOG_ERR_SYNC;
    return LOG_OK;
}
logger_status_t fatfs_deinit(void*storage){

    logger_status_t status;
    if((status =  fatfs_close(storage))!=LOG_OK) return status;
    if((status =  fatfs_umount(storage))!=LOG_OK) return status;

   return LOG_OK;
}
