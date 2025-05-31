/**
 * @file logger.h
 * @brief Provides logging functionality for debugging and error reporting.
 */

// #define DEBUG_MODE /**< Uncomment to enable debug logging. */

#ifndef APO_PACMAN_LOGGER_H
#define APO_PACMAN_LOGGER_H

#include <stdio.h>

/**
 * @def LOG_DEBUG
 * @brief Logs a debug message with file and line information.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#ifdef DEBUG_MODE
    #define LOG_DEBUG(fmt, ...) logger_log("DEBUG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) /**< No-op when DEBUG_MODE is not defined. */
#endif

/**
 * @def LOG_INFO
 * @brief Logs an informational message with file and line information.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define LOG_INFO(fmt, ...) logger_log("INFO", __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @def LOG_ERROR
 * @brief Logs an error message with file and line information.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define LOG_ERROR(fmt, ...) logger_log("ERROR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Logs a message with the specified log level, file, and line information.
 *
 * @param level Log level (e.g., "DEBUG", "INFO", "ERROR").
 * @param file Name of the source file where the log is generated.
 * @param line Line number in the source file where the log is generated.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
void logger_log(const char *level, const char *file, int line, const char *fmt, ...);

#endif // APO_PACMAN_LOGGER_H