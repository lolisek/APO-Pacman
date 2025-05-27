#include "../include/core/game.h"
#include "../include/core/renderer.h"
#include "../include/utils/timer.h"
#include "../include/core/input.h"
#include "../include/utils/logger.h"
#include "../include/gui/display_scoreboard.h"
#include "../include/utils/constants.h"
#include "../include/gui/custom_keyboard.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Shared game state and mutex
static GameState game_state;
static pthread_mutex_t game_state_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool running = true;

void *game_loop(void *arg)
{
    while (running)
    {
        uint64_t frame_start_time = timer_get_global_elapsed_ms();

        pthread_mutex_lock(&game_state_mutex);
        update_game_state(&game_state);
        pthread_mutex_unlock(&game_state_mutex);

        uint64_t frame_end_time = timer_get_global_elapsed_ms();
        uint64_t frame_elapsed_time = frame_end_time - frame_start_time;

        if (frame_elapsed_time < GAME_LOGIC_INTERVAL_MS)
        {
            timer_sleep_ms(GAME_LOGIC_INTERVAL_MS - frame_elapsed_time);
        }
    }
    return NULL;
}

void *render_loop(void *arg)
{
    uint16_t *shared_fb = (uint16_t *)arg;
    uint16_t render_fb[LCD_SIZE];

    while (running)
    {
        pthread_mutex_lock(&game_state_mutex);
        render(&game_state, render_fb);
        pthread_mutex_unlock(&game_state_mutex);

        memcpy(shared_fb, render_fb, sizeof(render_fb));
        lcd_update(shared_fb);

        timer_sleep_ms(FRAME_DURATION_MS); // Maintain 30 FPS
    }
    return NULL;
}

void run_game_loop(uint16_t *shared_fb)
{
    // Initialize game state and renderer
    init_game_state(&game_state);
    render_init();

    // Start game and render threads
    pthread_t game_thread, render_thread;
    pthread_create(&game_thread, NULL, game_loop, NULL);
    pthread_create(&render_thread, NULL, render_loop, shared_fb);

    // Handle input in the main thread
    while (running)
    {
        handle_input(&game_state, &running);
    }

    // Wait for threads to finish
    running = false; // Ensure threads exit cleanly
    pthread_join(game_thread, NULL);
    pthread_join(render_thread, NULL);

    cleanup_game(&game_state);
}
