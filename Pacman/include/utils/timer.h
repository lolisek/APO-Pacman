#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <time.h>

// Timer structure to track elapsed time
typedef struct
{
    struct timespec start_time;
    struct timespec end_time;
} Timer;

// Initialize the global timer
void timer_init_global();

// Get the elapsed time in milliseconds since the global timer started
uint64_t timer_get_global_elapsed_ms();

// Existing functions
void timer_start(Timer *timer);
void timer_stop(Timer *timer);
uint64_t timer_get_elapsed_ms(const Timer *timer);
void timer_sleep_ms(uint64_t milliseconds);

#endif // TIMER_H