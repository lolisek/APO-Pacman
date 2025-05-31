#include "../include/core/game.h"
#include "../include/core/renderer.h"
#include "../include/utils/timer.h"
#include "../include/core/input.h"
#include "../include/utils/logger.h"
#include "../include/gui/display_scoreboard.h"
#include "../include/utils/constants.h"
#include "../include/gui/custom_keyboard.h"
#include "../include/gui/resource_manager.h"
#include "../include/utils/led_manager.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // For rand()
#include <unistd.h>

// Shared game state and mutex
static GameState game_state;
static pthread_mutex_t game_state_mutex = PTHREAD_MUTEX_INITIALIZER;

void spawn_random_pellet(GameState *game_state)
{
    int empty_tiles[NUM_TILES_X * NUM_TILES_Y][2];
    int empty_count = 0;

    // Find all empty tiles
    for (int y = 0; y < game_state->map.height; y++)
    {
        for (int x = 0; x < game_state->map.width; x++)
        {
            if (game_state->map.tiles[y][x].type == TILE_EMPTY)
            {
                empty_tiles[empty_count][0] = x;
                empty_tiles[empty_count][1] = y;
                empty_count++;
            }
        }
    }

    // If there are empty tiles, randomly select one and spawn a pellet
    if (empty_count > 0)
    {
        int random_index = rand() % empty_count;
        int spawn_x = empty_tiles[random_index][0];
        int spawn_y = empty_tiles[random_index][1];
        game_state->map.tiles[spawn_y][spawn_x].type = TILE_PELLET;
        LOG_DEBUG("Spawned pellet at (%d, %d)", spawn_x, spawn_y);
    }
}

void spawn_random_power_pellet(GameState *game_state)
{
    int empty_tiles[NUM_TILES_X * NUM_TILES_Y][2];
    int empty_count = 0;

    // Find all empty tiles
    for (int y = 0; y < game_state->map.height; y++)
    {
        for (int x = 0; x < game_state->map.width; x++)
        {
            if (game_state->map.tiles[y][x].type == TILE_EMPTY)
            {
                empty_tiles[empty_count][0] = x;
                empty_tiles[empty_count][1] = y;
                empty_count++;
            }
        }
    }

    // If there are empty tiles, randomly select one and spawn a power pellet
    if (empty_count > 0)
    {
        int random_index = rand() % empty_count;
        int spawn_x = empty_tiles[random_index][0];
        int spawn_y = empty_tiles[random_index][1];
        game_state->map.tiles[spawn_y][spawn_x].type = TILE_POWER_PELLET;
        LOG_DEBUG("Spawned power pellet at (%d, %d)", spawn_x, spawn_y);
    }
}

void *game_loop(void *arg)
{
    uint64_t last_pellet_spawn_time = 0;
    uint64_t last_power_pellet_spawn_time = 0;

    while (game_state.game_over == false)
    {
        uint64_t frame_start_time = timer_get_global_elapsed_ms();

        pthread_mutex_lock(&game_state_mutex);
        update_game_state(&game_state);

        // Randomly spawn pellets at intervals
        if (frame_start_time - last_pellet_spawn_time > 8000) // 8 seconds interval
        {
            spawn_random_pellet(&game_state);
            last_pellet_spawn_time = frame_start_time;
        }

        if (frame_start_time - last_power_pellet_spawn_time > 20000) // 20 seconds interval
        {
            spawn_random_power_pellet(&game_state);
            last_power_pellet_spawn_time = frame_start_time;
        }

        update_leds(&game_state); // Refactored LED logic
        pthread_mutex_unlock(&game_state_mutex);

        uint64_t frame_end_time = timer_get_global_elapsed_ms();
        uint64_t frame_elapsed_time = frame_end_time - frame_start_time;

        if (frame_elapsed_time < GAME_LOGIC_INTERVAL_MS)
        {
            timer_sleep_ms(GAME_LOGIC_INTERVAL_MS - frame_elapsed_time);
        }
    }

    // Turn off all LEDs when the game loop ends
    set_all_leds_off();

    return NULL;
}

void *render_loop(void *arg)
{
    uint16_t *shared_fb = (uint16_t *)arg;
    uint16_t render_fb[LCD_SIZE];

    while (game_state.game_over == false)
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

void handle_game_over(uint16_t *shared_fb, int score)
{
    // Render the preloaded game over screen
    ppm_image_t *gameover_screen = get_gameover_screen();
    if (gameover_screen)
    {
        memcpy(shared_fb, gameover_screen->pixels, LCD_SIZE * sizeof(uint16_t));
        lcd_update(shared_fb);
    }

    while (1)
    {
        if (red_knob_is_pressed())
        {
            // Render the keyboard for name input
            char *player_name = handle_keyboard_input(shared_fb, &font_winFreeSystem14x16);
            if (player_name)
            {
                save_score(player_name, score);
                free(player_name);
            }
            break;
        }
        else if (blue_knob_is_pressed())
        {
            break;
        }
        timer_sleep_ms(100);
    }

    // Wait for the blue button to be released before continuing
    while (blue_knob_is_pressed())
    {
        timer_sleep_ms(100);
    }
}

void cleanup_game(GameState *game_state) {
    LOG_INFO("Cleaning up game state...");

    // Reset Pac-Man
    memset(&game_state->pacman, 0, sizeof(Entity));

    // Reset ghosts
    for (int i = 0; i < NUM_GHOSTS; i++) {
        memset(&game_state->ghosts[i], 0, sizeof(Entity));
    }

    // Reset other game state variables
    game_state->score = 0;
    game_state->lives = 0;
    game_state->frightened_timer = 0;
    game_state->game_over = false;

    LOG_INFO("Game state cleanup complete.");
}

void run_game_loop(uint16_t *shared_fb)
{
    // Initialize game state and resources
    init_game_state(&game_state);
    init_resources(); // Use resource manager

    // Start game and render threads
    pthread_t game_thread, render_thread;
    pthread_create(&game_thread, NULL, game_loop, NULL);
    pthread_create(&render_thread, NULL, render_loop, shared_fb);

    // Handle input in the main thread
    while (game_state.game_over == false)
    {
        handle_input(&game_state);
    }

    // Wait for a short duration before checking button presses
    timer_sleep_ms(100);

    // Handle game over or exit
    handle_game_over(shared_fb, game_state.score);

    // Wait for threads to finish
    pthread_join(game_thread, NULL);
    pthread_join(render_thread, NULL);

    cleanup_resources(); // Use resource manager
    cleanup_game(&game_state);
}
