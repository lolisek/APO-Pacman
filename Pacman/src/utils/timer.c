//#include <i386-linux-gnu/bits/time.h>
#define _POSIX_C_SOURCE 199309L
#include "../../include/utils/timer.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

static struct timespec global_start_time;

// Initialize the global timer
void timer_init_global()
{
    clock_gettime(CLOCK_MONOTONIC, &global_start_time);
}

// Get the elapsed time in milliseconds since the global timer started
uint64_t timer_get_global_elapsed_ms()
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return (current_time.tv_sec - global_start_time.tv_sec) * 1000 +
           (current_time.tv_nsec - global_start_time.tv_nsec) / 1000000;
}

// Start the timer
void timer_start(Timer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->start_time);
}

// Stop the timer
void timer_stop(Timer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->end_time);
}

// Get the elapsed time in milliseconds
uint64_t timer_get_elapsed_ms(const Timer *timer)
{
    return (timer->end_time.tv_sec - timer->start_time.tv_sec) * 1000 +
           (timer->end_time.tv_nsec - timer->start_time.tv_nsec) / 1000000;
}

// Sleep for a specified number of milliseconds
void timer_sleep_ms(uint64_t ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}