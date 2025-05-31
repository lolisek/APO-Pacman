#include "../../include/entities/ghost.h"
#include "../../include/entities/entity.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h"
#include "../../include/core/game_state.h"
#include "../../include/utils/timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> // For log function

void ghost_init(Entity *entity, GhostType type)
{
    LOG_INFO("Initializing Ghost...");
    Ghost *ghost = &entity->specific.ghost;
    ghost->mode = GHOST_MODE_EXITING;
    ghost->type = type;
    ghost->frightened_timer = 0;
    ghost->waiting_timer = 0; // Initialize waiting timer

    entity_init(entity, ENTITY_TYPE_GHOST, ghost_update);
    entity->direction = (Vector2D){0, 0};
    entity->speed = GHOST_SPEED;

    ghost->navigation.last_junction_pos = (Vector2D){-1, -1};
    ghost->navigation.last_junction_decision = (Vector2D){0, 0};

    // Set the initial position and starting position based on ghost type
    switch (type)
    {
    case GHOST_TYPE_BLINKY: // Red ghost
        LOG_DEBUG("Initializing Blinky (Red Ghost)");
        entity->position = (Vector2D){GHOST_BLINKY_START_X, GHOST_BLINKY_START_Y};
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME + 2; // Set wait time for Blinky
        break;
    case GHOST_TYPE_PINKY: // Pink ghost
        LOG_DEBUG("Initializing Pinky (Pink Ghost)");
        entity->position = (Vector2D){GHOST_PINKY_START_X, GHOST_PINKY_START_Y};
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME + 4; // Set wait time for Blinky

        break;
    case GHOST_TYPE_INKY: // Cyan ghost
        LOG_DEBUG("Initializing Inky (Cyan Ghost)");
        entity->position = (Vector2D){GHOST_INKY_START_X, GHOST_INKY_START_Y};
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME + 6; // Set wait time for Blinky

        break;
    case GHOST_TYPE_CLYDE: // Orange ghost
        LOG_DEBUG("Initializing Clyde (Orange Ghost)");
        entity->position = (Vector2D){GHOST_CLYDE_START_X, GHOST_CLYDE_START_Y};
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME + 8; // Set wait time for Blinky

        break;
    default:
        LOG_ERROR("Unknown ghost type: %d", type);
        exit(1);
    }

    // Explicitly set the starting position
    ghost->starting_position = entity->position;
}


Vector2D get_next_direction_towards_target(Vector2D current, Vector2D target,
                                           struct Map *map, Vector2D current_dir,
                                           GhostNavigationMemory *memory,
                                           Ghost *ghost)
{
    Vector2D best_dir = {0, 0};
    int min_distance = 999999;
    Vector2D reverse_dir = {-current_dir.x, -current_dir.y};
    int available_dirs = 0;
    Vector2D possible_dirs[4];

    Vector2D directions[] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}}; // Right, Up, Left, Down

    for (int i = 0; i < 4; i++)
    {
        Vector2D try_pos = {current.x + directions[i].x, current.y + directions[i].y};

        if (!map_is_walkable(map, try_pos.x, try_pos.y, ENTITY_TYPE_GHOST) ||
            (directions[i].x == reverse_dir.x && directions[i].y == reverse_dir.y))
        {
            continue;
        }

        possible_dirs[available_dirs++] = directions[i];
    }

    if (available_dirs == 1)
    {
        return possible_dirs[0];
    }

    if (available_dirs > 1)
    {
        if (memory->last_junction_pos.x == current.x &&
            memory->last_junction_pos.y == current.y)
        {
            return memory->last_junction_decision;
        }

        memory->last_junction_pos = current;

        for (int i = 0; i < available_dirs; i++)
        {
            Vector2D try_pos = {current.x + possible_dirs[i].x, current.y + possible_dirs[i].y};

            int dx = target.x - try_pos.x;
            int dy = target.y - try_pos.y;
            int dist = dx * dx + dy * dy;

            // Apply ghost-specific movement biases
            switch (ghost->type)
            {
            case GHOST_TYPE_BLINKY: // Red - aggressive
                if (possible_dirs[i].x == current_dir.x &&
                    possible_dirs[i].y == current_dir.y)
                {
                    dist -= 3;
                }
                break;

            case GHOST_TYPE_PINKY: // Pink - ambush
                if (possible_dirs[i].y == -1)
                {
                    dist -= 3;
                }
                break;

            case GHOST_TYPE_INKY: // Blue - unpredictable
                if (rand() % 100 < 20)
                {
                    dist += (rand() % 10) - 5; // Random adjustment -5 to +4
                }
                break;

            case GHOST_TYPE_CLYDE: // Orange - cautious
                if (possible_dirs[i].y == 1)
                { // Prefers downward turns
                    dist -= 2;
                }
                int dx = target.x - current.x;
                int dy = target.y - current.y;
                int distance_squared = dx * dx + dy * dy;
                if (distance_squared < 64)
                { // Stronger preference when near Pac-Man
                    dist -= 5;
                }
                break;
            }

            if (dist < min_distance)
            {
                min_distance = dist;
                best_dir = possible_dirs[i];
            }
        }

        memory->last_junction_decision = best_dir;
        return best_dir;
    }

    return reverse_dir;
}


void ghost_update(void *specific, struct GameState *passed_gamestate)
{
    Entity *entity = (Entity *)specific;
    GameState *game_state = (GameState *)passed_gamestate;
    Ghost *ghost = &entity->specific.ghost;

    LOG_DEBUG("Ghost index: %ld, type: %d, mode: %d, position: (%f, %f), waiting_timer: %d",
              game_state->ghosts, ghost->type, ghost->mode, entity->position.x, entity->position.y, ghost->waiting_timer);

    // Increase ghost speed over time
    uint64_t elapsed_time = timer_get_global_elapsed_ms() / 1000;     // Convert to seconds
    float speed_multiplier = 1.0f + log(1.0f + elapsed_time) / 10.0f; // Logarithmic acceleration
    entity->speed = GHOST_SPEED * speed_multiplier;

    // Handle frightened mode
    if (ghost->mode == GHOST_MODE_FRIGHTENED)
    {
        if (ghost->frightened_timer > 0)
        {
            ghost->frightened_timer--;
        }
        else
        {
            ghost->mode = GHOST_MODE_SCATTER; // Reset to normal mode after frightened state ends
            LOG_DEBUG("Frightened mode ended for ghost index %ld, type %d.", game_state->ghosts, ghost->type);
        }
    }

    // Handle ghost movement
    if (ghost->mode == GHOST_MODE_EATEN)
    {
        // Teleport ghost to spawn point
        entity->position = ghost->starting_position;
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME; // Set wait time
        ghost->mode = GHOST_MODE_EXITING;             // Set mode to exiting
        LOG_DEBUG("Ghost index %ld teleported to spawn point at (%f, %f).", game_state->ghosts, ghost->starting_position.x, ghost->starting_position.y);
        return;
    }

    if (ghost->mode == GHOST_MODE_EXITING)
    {
        if (ghost->waiting_timer > 0)
        {
            LOG_DEBUG("Ghost index %ld waiting to exit spawn area. Remaining time: %d ms", game_state->ghosts, ghost->waiting_timer);
            ghost->waiting_timer--; // Wait before exiting
            return;
        }

        // Teleport ghost to the gate position after waiting
        entity->position = (Vector2D){GHOST_SPAWN_GATE_X, GHOST_SPAWN_GATE_Y};
        ghost->mode = GHOST_MODE_SCATTER; // Transition to SCATTER mode after exiting
        LOG_DEBUG("Ghost exited spawn area and teleported to gate position (%f, %f).", entity->position.x, entity->position.y);
        return;
    }

    // Alternate between SCATTER and CHASE modes based on the global timer
    if (ghost->mode != GHOST_MODE_FRIGHTENED && ghost->mode != GHOST_MODE_EATEN)
    {
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

    // Handle other ghost modes (CHASE, SCATTER)
    ghost->target_tile = calculate_target_tile(ghost, game_state, entity);

    Vector2D next_dir = get_next_direction_towards_target(
        entity->position,
        ghost->target_tile,
        &game_state->map,
        entity->direction,
        &ghost->navigation,
        ghost);

    Vector2D next_pos = {
        entity->position.x + next_dir.x * entity->speed,
        entity->position.y + next_dir.y * entity->speed};

    if (map_is_walkable(&game_state->map, (int)next_pos.x, (int)next_pos.y, ENTITY_TYPE_GHOST))
    {
        entity->position = next_pos;
        entity->direction = next_dir;
    }
}

Vector2D calculate_target_tile(Ghost *ghost, GameState *state, Entity *ghost_entity)
{
    Entity *pacman = &state->pacman;
    Vector2D pacman_pos = pacman->position;
    Vector2D pacman_dir = pacman->direction;

    switch (ghost->mode)
    {
    case GHOST_MODE_EATEN:
        return (Vector2D){NUM_TILES_X / 2, NUM_TILES_Y / 2};

    case GHOST_MODE_SCATTER:
        switch (ghost->type)
        {
        case GHOST_TYPE_BLINKY:
            return (Vector2D){NUM_TILES_X - 2, 1}; // Blinky goes to the top right corner
        case GHOST_TYPE_PINKY:
            return (Vector2D){1, 1}; // Pinky goes to the top left corner
        case GHOST_TYPE_INKY:
            return (Vector2D){NUM_TILES_X - 2, NUM_TILES_Y - 2};
        case GHOST_TYPE_CLYDE:
            return (Vector2D){1, NUM_TILES_Y - 2};
        }
        break;

    case GHOST_MODE_CHASE:
        switch (ghost->type)
        {
        case GHOST_TYPE_BLINKY:
        {
            return (Vector2D){
                pacman_pos.x + pacman_dir.x,
                pacman_pos.y + pacman_dir.y};
        }

        case GHOST_TYPE_PINKY:
        {
            return (Vector2D){
                pacman_pos.x + 4 * pacman_dir.x,
                pacman_pos.y + 4 * pacman_dir.y};
        }

        case GHOST_TYPE_INKY:
        {
            Entity *blinky = &state->ghosts[0];
            Vector2D vec = {
                pacman_pos.x + 2 * pacman_dir.x - blinky->position.x,
                pacman_pos.y + 2 * pacman_dir.y - blinky->position.y};
            return (Vector2D){
                blinky->position.x + 2 * vec.x,
                blinky->position.y + 2 * vec.y};
        }

        case GHOST_TYPE_CLYDE:
        {
            int dx = pacman_pos.x - ghost_entity->position.x;
            int dy = pacman_pos.y - ghost_entity->position.y;
            int distance_squared = dx * dx + dy * dy;
            if (distance_squared > 64)
            {
                return pacman_pos;
            }
            else
            {
                return (Vector2D){1, NUM_TILES_Y - 2};
            }
        }
        }
        break;

    case GHOST_MODE_FRIGHTENED:
    default:
        // Random move within walkable tiles
        Vector2D random_tile;
        do
        {
            random_tile = (Vector2D){rand() % NUM_TILES_X, rand() % NUM_TILES_Y};
        } while (!map_is_walkable(&state->map, random_tile.x, random_tile.y, ENTITY_TYPE_GHOST));
        return random_tile;
    }

    return pacman_pos;
}
