#include "../../include/core/game_state.h"
#include "../../include/entities/pacman.h"
#include "../../include/entities/ghost.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/timer.h"
#include <stdio.h>

void init_game_state(GameState *game_state)
{
    LOG_INFO("Initializing game state...");

    // Initialize Pac-Man
    pacman_init(&game_state->pacman, (Vector2D){PACMAN_START_X, PACMAN_START_Y});

    // Initialize ghosts
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        ghost_init(&game_state->ghosts[i], (GhostType)i);
    }

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
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        ghost_init(&game_state->ghosts[i], game_state->ghosts[i].specific.ghost.type);
        game_state->ghosts[i].specific.ghost.waiting_timer = GHOST_EATEN_WAIT_TIME * (i + 1); // Staggered respawn timers
    }

    game_state->frightened_timer = 0;
    game_state->game_over = false;

    LOG_INFO("Level reset successfully.");
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

        // Synchronize all ghost timers with the global timer
        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            Ghost *ghost = &game_state->ghosts[i].specific.ghost;
            if (ghost->mode == GHOST_MODE_FRIGHTENED)
            {
                ghost->frightened_timer = game_state->frightened_timer;
            }
        }
    }

    // Update Pac-Man
    entity_update(&game_state->pacman, game_state);

    // Control ghost movement using a tick counter
    const int GHOST_MOVEMENT_INTERVAL = 2;
    if (ghost_tick_counter % GHOST_MOVEMENT_INTERVAL == 0)
    {
        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            Ghost *ghost = &game_state->ghosts[i].specific.ghost;

            // Ensure ghosts in EXITING mode spawn at the gate even if frightened
            if (ghost->mode == GHOST_MODE_EXITING && ghost->waiting_timer == 0)
            {
                game_state->ghosts[i].position = (Vector2D){GHOST_SPAWN_GATE_X, GHOST_SPAWN_GATE_Y};
                ghost->mode = GHOST_MODE_SCATTER; // Transition to SCATTER mode
                LOG_DEBUG("Ghost exited spawn area and teleported to gate position (%f, %f).",
                          game_state->ghosts[i].position.x, game_state->ghosts[i].position.y);
            }

            // Update each ghost
            entity_update(&game_state->ghosts[i], game_state);
        }
    }

    ghost_tick_counter++;
    check_collisions(game_state);
}

void check_collisions(GameState *game_state)
{
    const float COLLISION_TOLERANCE = 0.2f; // Reduced tolerance for more precise collision detection

    // Check for collisions between Pac-Man and ghosts
    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        Ghost *ghost = &game_state->ghosts[i].specific.ghost;

        // Calculate the distance between Pac-Man and the ghost
        float dx = fabsf(game_state->ghosts[i].position.x - game_state->pacman.position.x);
        float dy = fabsf(game_state->ghosts[i].position.y - game_state->pacman.position.y);

        if (dx <= COLLISION_TOLERANCE && dy <= COLLISION_TOLERANCE)
        {
            if (ghost->mode == GHOST_MODE_FRIGHTENED)
            {
                // Ghost is frightened, Pac-Man eats the ghost
                game_state->score += GHOST_EATEN_SCORE;       // Increase score
                ghost->mode = GHOST_MODE_EATEN;               // Change ghost mode to eaten
                ghost->waiting_timer = GHOST_EATEN_WAIT_TIME; // Set wait time
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
                    return; // Exit immediately to avoid further processing
                }

                reset_level(game_state); // Reset the level instead of just Pac

                // Break out of the loop to avoid multiple life deductions in a single frame
                break;
            }
        }
    }
}
