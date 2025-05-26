#include "../../include/entities/ghost.h"
#include "../../include/entities/entity.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h"
#include "../../include/core/game_state.h"
#include "../../include/utils/timer.h"
#include <stdlib.h>
#include <stdio.h>

void ghost_init(Entity *entity, GhostType type)
{
    LOG_INFO("Initializing Ghost...");
    Ghost *ghost = &entity->specific.ghost;
    ghost->mode = GHOST_MODE_SCATTER;
    ghost->type = type;
    ghost->frightened_timer = 0;
    ghost->waiting_timer = 0; // Initialize waiting timer

    entity_init(entity, ENTITY_TYPE_GHOST, ghost_update, ghost_render);
    entity->direction = (Vector2D){0, 0};
    entity->speed = GHOST_SPEED;

    ghost->navigation.last_junction_pos = (Vector2D){-1, -1};
    ghost->navigation.last_junction_decision = (Vector2D){0, 0};

    switch (type)
    {
    case GHOST_TYPE_BLINKY: // Red ghost
        entity->position = (Vector2D){GHOST_BLINKY_START_X, GHOST_BLINKY_START_Y};
        break;
    case GHOST_TYPE_PINKY: // Pink ghost
        entity->position = (Vector2D){GHOST_PINKY_START_X, GHOST_PINKY_START_Y};
        break;
    case GHOST_TYPE_INKY: // Cyan ghost
        entity->position = (Vector2D){GHOST_INKY_START_X, GHOST_INKY_START_Y};
        break;
    case GHOST_TYPE_CLYDE: // Orange ghost
        entity->position = (Vector2D){GHOST_CLYDE_START_X, GHOST_CLYDE_START_Y};
        break;
    default:
        printf("Unknown ghost type: %d\n", type);
        exit(0);
        break;
    }
    ghost->starting_position = entity->position;

    LOG_DEBUG("Ghost initialized at position (%d, %d) with type %d",
              entity->position.x, entity->position.y, ghost->type);
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
        Vector2D try_pos = {
            current.x + directions[i].x,
            current.y + directions[i].y};

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
            Vector2D try_pos = {
                current.x + possible_dirs[i].x,
                current.y + possible_dirs[i].y};

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
                    dist -= 5;
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

    LOG_DEBUG("Updating Ghost...");
    LOG_DEBUG("Ghost position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Ghost mode: %d", ghost->mode);

    if (ghost->mode == GHOST_MODE_FRIGHTENED)
    {
        entity->speed = FRIGHTENED_GHOST_SPEED; // Reduce speed during frightened mode
    }
    else
    {
        entity->speed = GHOST_SPEED; // Reset to normal speed
    }

    if (ghost->mode == GHOST_MODE_EATEN)
    {
        // Teleport ghost to spawn point
        entity->position = ghost->starting_position;
        ghost->waiting_timer = GHOST_EATEN_WAIT_TIME / FRAME_DURATION_MS; // Set wait time
        ghost->mode = GHOST_MODE_EXITING;                                 // Set mode to exiting
        LOG_DEBUG("Ghost teleported to spawn point at (%d, %d).", ghost->starting_position.x, ghost->starting_position.y);
        return;
    }

    // Handle exiting the spawn area
    if (ghost->mode == GHOST_MODE_EXITING)
    {
        if (ghost->waiting_timer > 0)
        {
            ghost->waiting_timer--; // Wait before exiting
            return;
        }

        // Move toward the gate to exit the spawn area
        Vector2D gate_position = {NUM_TILES_X / 2, NUM_TILES_Y / 2 - 1}; // Adjust to your gate position

        Vector2D next_dir = get_next_direction_towards_target(
            entity->position,
            gate_position,
            (struct Map *)&game_state->map,
            entity->direction,
            &ghost->navigation,
            ghost);

        Vector2D next_pos = {
            entity->position.x + next_dir.x,
            entity->position.y + next_dir.y};

        if (map_is_walkable(&game_state->map, next_pos.x, next_pos.y, ENTITY_TYPE_GHOST))
        {
            entity->position = next_pos;
            entity->direction = next_dir;
        }

        // Check if ghost exited the spawn area
        if (entity->position.x == gate_position.x && entity->position.y == gate_position.y)
        {
            ghost->mode = GHOST_MODE_SCATTER; // Set to SCATTER mode after exiting
            LOG_DEBUG("Ghost exited the spawn area and is now in SCATTER mode.");
        }

        return;
    }

    if (ghost->waiting_timer > 0)
    {
        // Decrease the waiting timer
        ghost->waiting_timer--;
        if (ghost->waiting_timer == 0)
        {
            LOG_DEBUG("Ghost finished waiting and can now move.");
        }
        return; // Ghost stays in place while waiting
    }

    // Handle other ghost modes (CHASE, SCATTER, FRIGHTENED)
    ghost->target_tile = calculate_target_tile(ghost, game_state, entity);

    Vector2D next_dir = get_next_direction_towards_target(
        entity->position,
        ghost->target_tile,
        (struct Map *)&game_state->map,
        entity->direction,
        &ghost->navigation,
        ghost);

    Vector2D next_pos = {
        entity->position.x + next_dir.x,
        entity->position.y + next_dir.y};

    if (map_is_walkable(&game_state->map, next_pos.x, next_pos.y, ENTITY_TYPE_GHOST))
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
            return (Vector2D){NUM_TILES_X - 2, 1};
        case GHOST_TYPE_PINKY:
            return (Vector2D){1, 1};
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
            return pacman_pos;

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

void ghost_render(void *specific)
{
    Entity *entity = (Entity *)specific;
    Ghost *ghost = &entity->specific.ghost;

    LOG_DEBUG("Rendering Ghost...");
    LOG_DEBUG("Ghost position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Ghost mode: %d", ghost->mode);
}
