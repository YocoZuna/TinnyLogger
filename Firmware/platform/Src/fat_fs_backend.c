#include "fat_fs_backend.h"
#include "logger.h"
#include <stdio.h>

#define FF_VOLUME "0:"

PARTITION VolToPart[FF_VOLUMES] = {
    {1, 0},   // "0:" → physical drive 0, whole drive
};

logger_status_t fatfs_mount(void* storage){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FATFS* fs = &fat_storage->fs;
    
    if(f_mount(fs, FF_VOLUME, 1)!=FR_OK) return LOG_ERR_MOUNT;
    return LOG_OK;
}
logger_status_t fatfs_open(void* storage,const char* filename,const BYTE mode){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s", FF_VOLUME,filename);
    if(f_open(file, file_path, mode)!=FR_OK) return LOG_ERR_OPEN;
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
logger_status_t fatfs_close(void* storage){
    fat_fs_storage_t* fat_storage = (fat_fs_storage_t*)storage;
    FIL* file = &fat_storage->file;
    if(f_close(file)!=FR_OK) return LOG_ERR_CLOSE;
    return LOG_OK;
}
logger_status_t fatfs_umount(void* storage){

    if(f_unmount(FF_VOLUME)!=FR_OK) return LOG_ERR_UMOUNT;
    return LOG_OK;
}
const logger_backend_t fat_fs_backend = {
    .mount = fatfs_mount,
    .open = fatfs_open,
    .write = fatfs_write,
    .read = fatfs_read,
    .sync = fatfs_sync,
    .close = fatfs_close
};