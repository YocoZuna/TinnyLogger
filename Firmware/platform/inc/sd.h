#ifndef PLATFORM_SD_H
#define PLATFORM_SD_H

#include "TAL/inc/sd.h"

extern SD_IO_t P_hSD_card;



void P_init_SD_card();
void P_deinit_SD_card();

#endif /*PLATFORM_SD_H*/