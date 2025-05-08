#include <i386-linux-gnu/bits/time.h>
#include "../../include/utils/timer.h"
#include <time.h>
#include <unistd.h>

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
    usleep(ms * 1000); // Convert milliseconds to microseconds
}