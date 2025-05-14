#include "../../include/core/game_state.h"
#include "../../include/entities/pacman.h"
#include "../../include/entities/ghost.h"
#include "../../include/utils/logger.h"
#include <stdio.h>

void init_game_state(GameState *game_state)
{
    LOG_INFO("Initializing game state...");

    // Initialize Pac-Man
    pacman_init(&game_state->pacman, (Vector2D){PACMAN_START_X, PACMAN_START_Y});

    // Initialize ghosts
    ghost_init(&game_state->ghosts[0], (Vector2D){GHOST_START_X, GHOST_START_Y}, GHOST_TYPE_BLINKY);    // Blinky
    ghost_init(&game_state->ghosts[1], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_PINKY); // Pinky
    ghost_init(&game_state->ghosts[2], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_INKY);  // Inky
    ghost_init(&game_state->ghosts[3], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_CLYDE); // Clyde

    // Initialize score and lives
    game_state->score = PACMAN_START_SCORE;
    game_state->lives = PACMAN_START_LIVES;
    game_state->frightened_timer = 0;

    // Initialize the map
    map_init(&game_state->map);
    if (!map_load_from_file(&game_state->map, "/tmp/veru/assets/maps/level1.txt"))
    {
        LOG_ERROR("Failed to load map from file: /assets/maps/level1.txt");
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

    ghost_init(&game_state->ghosts[0], (Vector2D){GHOST_START_X, GHOST_START_Y}, GHOST_TYPE_BLINKY);    // Blinky
    ghost_init(&game_state->ghosts[1], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_PINKY); // Pinky
    ghost_init(&game_state->ghosts[2], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_INKY);  // Inky
    ghost_init(&game_state->ghosts[3], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}, GHOST_TYPE_CLYDE); // Clyde

    // Reset score and lives
    game_state->score = 0;
    game_state->lives = PACMAN_START_LIVES;
    game_state->frightened_timer = 0;

    // Reload the map
    map_init(&game_state->map);
    if (!map_load_from_file(&game_state->map, "./assets/maps/level1.txt"))
    {
        LOG_ERROR("Failed to reload map from file: ./assets/maps/level1.txt");
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
