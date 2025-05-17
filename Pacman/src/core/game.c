#include "../include/core/game.h"
#include "../include/core/renderer.h"
#include "../include/utils/timer.h"
#include "../include/core/input.h"
#include "../include/utils/logger.h"
#include "../include/gui/display_scoreboard.h"
#include "../include/utils/constants.h" // Add this include for get_resource_path
#include <stdio.h>
#include <unistd.h>

void run_game_loop(uint16_t *shared_fb)
{
    // Create a dedicated game frame buffer
    uint16_t game_fb[LCD_SIZE];

    GameState game_state;
    init_game_state(&game_state);
    render_init(); // Ensure this initializes LCD if needed

    Timer frame_timer;
    bool running = true;

    while (running)
    {
        if (game_state.game_over)
        {
            // Handle game over state
            printf("Game Over! Final Score: %d\n", game_state.score);

            char gameover_path[256];
            get_resource_path(gameover_path, sizeof(gameover_path), "gameover.ppm");
            ppm_image_t *game_over = load_ppm(gameover_path);
            if (!game_over)
            {
                fprintf(stderr, "Failed to load game over image\n");
                break;
            }

            memcpy(game_fb, game_over->pixels, game_over->width * game_over->height * sizeof(uint16_t));
            lcd_update(game_fb);

            while (1)
            {
                if (red_knob_is_pressed())
                {
                    char *name = handle_keyboard_input(game_fb, &font_winFreeSystem14x16);
                    if (name)
                    {
                        int saved = save_score(name, game_state.score);
                        if (saved == 0)
                        {
                            printf("Score saved successfully!\n");
                        }
                        else
                        {
                            printf("Failed to save score.\n");
                        }
                        free(name);
                    }
                    break; // Exit game over loop
                }
                if (blue_knob_is_pressed())
                {
                    break; // Exit game loop
                }
            }
            break;
        }

        timer_start(&frame_timer);

        // Clear game frame buffer
        memset(game_fb, 0, sizeof(game_fb));

        handle_input(&game_state, &running);
        update_game_state(&game_state);
        render(&game_state, game_fb);

        // Copy to shared frame buffer if needed
        memcpy(shared_fb, game_fb, sizeof(game_fb));

        lcd_update(shared_fb);

        // Frame rate control
        timer_stop(&frame_timer);
        uint64_t elapsed_ms = timer_get_elapsed_ms(&frame_timer);
        if (elapsed_ms < 16)
        {
            timer_sleep_ms(16 - elapsed_ms);
        }
    }

    cleanup_game(&game_state);
}

void update_game_state(GameState *game_state)
{
    // Update Pac-Man
    entity_update(&game_state->pacman, game_state);

    // Update ghosts
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        entity_update(&game_state->ghosts[i], game_state);
    }

    // Check for collisions between Pac-Man and ghosts
    check_collisions(game_state);

    // Debug: Print Pac-Man's position
    printf("Pac-Man position: (%d, %d)\n", game_state->pacman.position.x, game_state->pacman.position.y);
}

void check_collisions(GameState *game_state)
{
    // Check for collisions between Pac-Man and ghosts
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        if (game_state->ghosts[i].position.x == game_state->pacman.position.x &&
            game_state->ghosts[i].position.y == game_state->pacman.position.y)
        {
            // Handle collision with ghost
            Ghost *ghost = &game_state->ghosts[i].specific.ghost;
            if (ghost->mode == GHOST_MODE_FRIGHTENED)
            {
                // Ghost is frightened, Pac-Man eats the ghost
                game_state->score += 200;       // Increase score
                ghost->mode = GHOST_MODE_EATEN; // Change ghost mode to eaten
                LOG_INFO("Pac-Man ate a ghost! Score: %d", game_state->score);
            }
            else
            {
                // Pac-Man loses a life
                game_state->lives--;
                LOG_INFO("Pac-Man hit by a ghost! Lives remaining: %d", game_state->lives);
                if (game_state->lives <= 0)
                {
                    game_state->game_over = true; // Game over
                    LOG_INFO("Game Over!");
                }

                // Reset Pac-Man's position
                game_state->pacman.position.x = PACMAN_START_X;
                game_state->pacman.position.y = PACMAN_START_Y;

                // Reset ghost positions
                for (int j = 0; j < NUM_GHOSTS; j++)
                {
                    game_state->ghosts[j].position.x = GHOST_START_X + j;
                    game_state->ghosts[j].position.y = GHOST_START_Y;
                }

                // Reset frightened timer
                game_state->frightened_timer = 0;
            }
        }
    }
}