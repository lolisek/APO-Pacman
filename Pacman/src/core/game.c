#include "../include/core/game.h"
#include "../include/core/renderer.h"
#include "../include/utils/timer.h"
#include "../include/core/input.h"
#include "../include/utils/logger.h"
#include <stdio.h>

void run_game_loop()
{
    GameState game_state;
    init_game_state(&game_state);

    // Main game loop
    Timer frame_timer;
    bool running = true;

    while (running)
    {
        timer_start(&frame_timer);

        // Handle input
        handle_input(&game_state, &running);

        // Update game state
        update_game_state(&game_state);

        // Render the game
        render(&game_state);

        // Control frame rate (e.g., 60 FPS)
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