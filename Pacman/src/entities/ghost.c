#include "../../include/entities/ghost.h"
#include "../../include/entities/entity.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/constants.h"
#include "../../include/core/game_state.h"
#include <stdlib.h>
#include <stdio.h>

void ghost_init(Entity *entity, Vector2D position, GhostType type)
{
    LOG_INFO("Initializing Ghost...");
    Ghost *ghost = &entity->specific.ghost;
    ghost->mode = GHOST_MODE_SCATTER;
    ghost->type = type;
    ghost->frightened_timer = 0;

    entity_init(entity, ENTITY_TYPE_GHOST, ghost_update, ghost_render);
    entity->position = position;
    entity->direction = (Vector2D){0, 0};
    entity->speed = GHOST_SPEED;

    ghost->navigation.last_junction_pos = (Vector2D){-1, -1};
    ghost->navigation.last_junction_decision = (Vector2D){0, 0};

    LOG_DEBUG("Ghost initialized at position (%d, %d) with type %d", position.x, position.y, type);
}

Vector2D get_next_direction_towards_target(Vector2D current, Vector2D target,
                                           struct Map *map, Vector2D current_dir,
                                           GhostNavigationMemory *memory,
                                           Ghost *ghost)
{

    // print the state of the game

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

        if (!map_is_walkable(map, try_pos.x, try_pos.y) ||
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
    LOG_DEBUG("Ghost type: %d", ghost->type);
    LOG_DEBUG("Ghost frightened timer: %d", ghost->frightened_timer);
    LOG_DEBUG("Ghost target tile: (%d, %d)", ghost->target_tile.x, ghost->target_tile.y);
    LOG_DEBUG("Ghost direction: (%d, %d)", entity->direction.x, entity->direction.y);

    // Calculate the target tile based on the ghost's mode and type
    ghost->target_tile = calculate_target_tile(ghost, game_state, entity);

    // Determine the next direction toward the target tile
    Vector2D next_dir = get_next_direction_towards_target(
        entity->position,
        ghost->target_tile,
        &game_state->map,
        entity->direction,
        &ghost->navigation,
        ghost);

    LOG_DEBUG("Ghost new target tile: (%d, %d)", ghost->target_tile.x, ghost->target_tile.y);
    LOG_DEBUG("Ghost new direction: (%d, %d)", entity->direction.x, entity->direction.y);

    // Calculate the next position based on the chosen direction
    Vector2D next_pos = {
        entity->position.x + next_dir.x * entity->speed,
        entity->position.y + next_dir.y * entity->speed};

    // Check if the next position is walkable
    if (map_is_walkable(&game_state->map, next_pos.x, next_pos.y))
    {
        // Update the ghost's position and direction
        entity->position = next_pos;
        entity->direction = next_dir;
    }
    else
    {
        // Fallback: Avoid immediate reversal unless no other options exist
        Vector2D reverse_dir = {-entity->direction.x, -entity->direction.y};
        if (next_dir.x == reverse_dir.x && next_dir.y == reverse_dir.y)
        {
            // Try to find an alternative direction
            Vector2D alternative_dir = {0, 0};
            Vector2D directions[] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
            for (int i = 0; i < 4; i++)
            {
                Vector2D try_pos = {
                    entity->position.x + directions[i].x,
                    entity->position.y + directions[i].y};

                if (map_is_walkable(&game_state->map, try_pos.x, try_pos.y) &&
                    !(directions[i].x == reverse_dir.x && directions[i].y == reverse_dir.y))
                {
                    alternative_dir = directions[i];
                    break;
                }
            }

            // Use the alternative direction if found
            if (alternative_dir.x != 0 || alternative_dir.y != 0)
            {
                entity->direction = alternative_dir;
            }
            else
            {
                // If no alternative, reverse direction as a last resort
                entity->direction = reverse_dir;
            }
        }
    }
}

Vector2D calculate_target_tile(Ghost *ghost, GameState *state, Entity *ghost_entity)
{
    Entity *pacman = &state->pacman;
    Vector2D pacman_pos = pacman->position;
    Vector2D pacman_dir = pacman->direction;

    switch (ghost->mode)
    {
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
        } while (!map_is_walkable(&state->map, random_tile.x, random_tile.y));
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