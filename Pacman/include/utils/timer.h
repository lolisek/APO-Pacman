#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <time.h>

// Macro to convert seconds to milliseconds
#define SECONDS_TO_MILLISECONDS(sec) ((sec) * 1000)

// Timer structure to track elapsed time
typedef struct
{
    struct timespec start_time;
    struct timespec end_time;
} Timer;

// Function to initialize a timer
void timer_start(Timer *timer);

// Function to stop a timer
void timer_stop(Timer *timer);

// Function to get the elapsed time in milliseconds
uint64_t timer_get_elapsed_ms(const Timer *timer);

// Function to sleep for a specified number of milliseconds
void timer_sleep_ms(uint64_t milliseconds);

#endif // TIMER_H