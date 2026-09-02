#ifndef BOARD_TINNYLOGGER_SD_H
#define BOARD_TINNYLOGGER_SD_H

#if defined(TINNY_LOGGER_STM) && defined(USE_SD_CARD)
    #include "board_config.h"
    #include "TinnyLoggerSTM/inc/sd.h"
#else
    #error "SD card driver is not supported for this platform"
#endif

#endif /* BOARD_TINNYLOGGER_SD_H */
