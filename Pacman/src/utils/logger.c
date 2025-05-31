/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  logger.c      - Logger utility for debugging and information output

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/


#include "../../include/utils/logger.h"
#include <stdarg.h>
#include <time.h>

void logger_log(const char *level, const char *file, int line, const char *fmt, ...) {
    // Get the current time
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    // Print the timestamp, log level, file, and line number
    printf("[%02d:%02d:%02d] [%s] (%s:%d): ", 
           local_time->tm_hour, local_time->tm_min, local_time->tm_sec, 
           level, file, line);

    // Print the formatted log message
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}