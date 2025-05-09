#define DEBUG_MODE // Uncomment to enable debug logging

#ifndef APO_PACMAN_LOGGER_H
#define APO_PACMAN_LOGGER_H

#include <stdio.h>

// Define DEBUG_MODE to enable or disable logging
#ifdef DEBUG_MODE
    #define LOG_DEBUG(fmt, ...) logger_log("DEBUG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...) // No-op when DEBUG_MODE is not defined
#endif

#define LOG_INFO(fmt, ...) logger_log("INFO", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log("ERROR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// Function prototype
void logger_log(const char *level, const char *file, int line, const char *fmt, ...);

#endif // APO_PACMAN_LOGGER_H