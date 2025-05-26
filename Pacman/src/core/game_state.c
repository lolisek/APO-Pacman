#include "../../include/core/game_state.h"
#include "../../include/entities/pacman.h"
#include "../../include/entities/ghost.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h" // Add this include for get_resource_path
#include "../../include/utils/timer.h"
#include <stdio.h>

void init_game_state(GameState *game_state)
{
    LOG_INFO("Initializing game state...");

    // Initialize Pac-Man
    pacman_init(&game_state->pacman, (Vector2D){PACMAN_START_X, PACMAN_START_Y});

    // Initialize ghosts
    ghost_init(&game_state->ghosts[0], GHOST_TYPE_BLINKY); // Blinky
    ghost_init(&game_state->ghosts[1], GHOST_TYPE_PINKY);  // Pinky
    ghost_init(&game_state->ghosts[2], GHOST_TYPE_INKY);   // Inky
    ghost_init(&game_state->ghosts[3], GHOST_TYPE_CLYDE);  // Clyde

    // Initialize score and lives
    game_state->score = PACMAN_START_SCORE;
    game_state->lives = PACMAN_START_LIVES;
    game_state->frightened_timer = 0;

    // Initialize the map
    map_init(&game_state->map);
    char map_path[256];
    get_resource_path(map_path, sizeof(map_path), "level1.txt");
    if (!map_load_from_file(&game_state->map, map_path))
    {
        LOG_ERROR("Failed to load map from file: %s", map_path);
        game_state->game_over = true;
        return;
    }

    game_state->game_over = false;
}

void reset_level(GameState *game_state)
{
    LOG_INFO("Resetting level...");

    // Reset Pac-Man
    pacman_init(&game_state->pacman, (Vector2D){PACMAN_START_X, PACMAN_START_Y});

    // Reset ghosts
    ghost_init(&game_state->ghosts[0], GHOST_TYPE_BLINKY); // Blinky
    ghost_init(&game_state->ghosts[1], GHOST_TYPE_PINKY);  // Pinky
    ghost_init(&game_state->ghosts[2], GHOST_TYPE_INKY);   // Inky
    ghost_init(&game_state->ghosts[3], GHOST_TYPE_CLYDE);  // Clyde

    // Reset score and lives
    game_state->score = 0;
    game_state->lives = PACMAN_START_LIVES;
    game_state->frightened_timer = 0;

    // Reload the map
    map_init(&game_state->map);
    char map_path[256];
    get_resource_path(map_path, sizeof(map_path), "level1.txt");
    if (!map_load_from_file(&game_state->map, map_path))
    {
        LOG_ERROR("Failed to reload map from file: %s", map_path);
        game_state->game_over = true;
        return;
    }

    game_state->game_over = false;
    LOG_INFO("Level reset successfully.");
}

void cleanup_game(GameState *game_state)
{
    LOG_INFO("Cleaning up game state...");

    // Free any allocated resources
    // For example, if you dynamically allocate memory for the map or entities,
    // you should free it here.

    LOG_INFO("Game state cleanup complete.");
}

void update_ghost_modes(GameState *game_state)
{
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        Ghost *ghost = &game_state->ghosts[i].specific.ghost;

        if (game_state->frightened_timer > 0)
        {
            ghost->mode = GHOST_MODE_FRIGHTENED;
        }
        else if (ghost->mode == GHOST_MODE_FRIGHTENED)
        {
            // Reset to normal mode after frightened state ends
            ghost->mode = GHOST_MODE_SCATTER;
        }
        else
        {
            // Use the global timer to alternate between SCATTER and CHASE modes
            uint64_t elapsed_time = timer_get_global_elapsed_ms();
            if ((elapsed_time / (SCATTER_MODE_DURATION * 1000)) % 2 == 0)
            {
                ghost->mode = GHOST_MODE_SCATTER;
            }
            else
            {
                ghost->mode = GHOST_MODE_CHASE;
            }
        }
    }
}
