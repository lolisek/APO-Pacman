/**
 * @file timer.h
 * @brief Provides a Timer structure and utility functions for time management.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <time.h>

/**
 * @brief Structure to track elapsed time using start and end timestamps.
 */
typedef struct
{
    struct timespec start_time; /**< Start time of the timer. */
    struct timespec end_time;   /**< End time of the timer. */
} Timer;

/**
 * @brief Initializes the global timer.
 */
void timer_init_global();

/**
 * @brief Gets the elapsed time in milliseconds since the global timer started.
 *
 * @return uint64_t Elapsed time in milliseconds.
 */
uint64_t timer_get_global_elapsed_ms();

/**
 * @brief Starts the given timer by recording the current time as the start time.
 *
 * @param timer Pointer to the Timer structure to start.
 */
void timer_start(Timer *timer);

/**
 * @brief Stops the given timer by recording the current time as the end time.
 *
 * @param timer Pointer to the Timer structure to stop.
 */
void timer_stop(Timer *timer);

/**
 * @brief Gets the elapsed time in milliseconds between the start and end times of the timer.
 *
 * @param timer Pointer to the Timer structure.
 * @return uint64_t Elapsed time in milliseconds.
 */
uint64_t timer_get_elapsed_ms(const Timer *timer);

/**
 * @brief Pauses execution for the specified number of milliseconds.
 *
 * @param milliseconds Duration to sleep in milliseconds.
 */
void timer_sleep_ms(uint64_t milliseconds);

#endif // TIMER_H