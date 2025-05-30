#include "../../include/core/game_initializer.h"
#include "../../include/entities/pacman.h"
#include "../../include/entities/ghost.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h"
#include "../../include/map/map.h"
#include <stdio.h>

void initialize_game_state(GameState *game_state)
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
