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

void update_game_state(GameState *game_state)
{
    static int ghost_tick_counter = 0;

    // Decrement the global frightened timer
    if (game_state->frightened_timer > 0)
    {
        game_state->frightened_timer--;
        if (game_state->frightened_timer == 0)
        {
            LOG_INFO("Global frightened mode ended.");
        }
    }

    entity_update(&game_state->pacman, game_state);

    const int GHOST_MOVEMENT_INTERVAL = 2;
    if (ghost_tick_counter % GHOST_MOVEMENT_INTERVAL == 0)
    {
        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            entity_update(&game_state->ghosts[i], game_state);
        }
    }

    ghost_tick_counter++;
    check_collisions(game_state);
}

void check_collisions(GameState *game_state)
{
    const float COLLISION_TOLERANCE = 0.5f; // Allow a small tolerance for collision detection

    // Check for collisions between Pac-Man and ghosts
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        float dx = fabsf(game_state->ghosts[i].position.x - game_state->pacman.position.x);
        float dy = fabsf(game_state->ghosts[i].position.y - game_state->pacman.position.y);

        if (dx <= COLLISION_TOLERANCE && dy <= COLLISION_TOLERANCE)
        {
            // Handle collision with ghost
            Ghost *ghost = &game_state->ghosts[i].specific.ghost;
            if (ghost->mode == GHOST_MODE_FRIGHTENED)
            {
                // Ghost is frightened, Pac-Man eats the ghost
                game_state->score += 200;                     // Increase score
                ghost->mode = GHOST_MODE_EATEN;               // Change ghost mode to eaten
                ghost->waiting_timer = GHOST_EATEN_WAIT_TIME; // Set wait time
                ghost->frightened_timer = 0;                  // Reset frightened timer
                LOG_INFO("Pac-Man ate a ghost! Score: %d", game_state->score);
            }
            else if (ghost->mode != GHOST_MODE_EATEN)
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
                    game_state->ghosts[j].position.x = game_state->ghosts[j].specific.ghost.starting_position.x;
                    game_state->ghosts[j].position.y = game_state->ghosts[j].specific.ghost.starting_position.y;
                    game_state->ghosts[j].specific.ghost.mode = GHOST_MODE_EXITING; // Reset ghost mode to exiting
                }

                // Reset frightened timer
                game_state->frightened_timer = 0;
            }
        }
    }
}
