#include "fake_backend.h"

logger_backend_t fake_backend = {
    .mount = fake_mount,
    .open = fake_open,
    .write = fake_write,
    .read = fake_read,
    .sync = fake_sync,
    .close = fake_close
};


logger_status_t fake_mount(void* storage){};
logger_status_t fake_open(void* storage,const char* filename,const BYTE mode){};
logger_status_t fake_write(void* storage,const char* buffor,const size_t len){};
logger_status_t fake_read(void* storage,char* buffor,const size_t len){};
logger_status_t fake_sync(void* storage){};
logger_status_t fake_close(void* storage){};
logger_status_t fake_umount(void* storage){};