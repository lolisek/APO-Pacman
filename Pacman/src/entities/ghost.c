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
    Ghost *ghost = &entity->specific.ghost; // Correctly access the Ghost object
    ghost->mode = GHOST_MODE_SCATTER;
    ghost->type = type;
    ghost->frightened_timer = 0;

    entity_init(entity, ENTITY_TYPE_GHOST, ghost_update, ghost_render);
    entity->position = position;
    entity->direction = (Vector2D){0, 0};
    entity->speed = GHOST_SPEED;

    LOG_DEBUG("Ghost initialized at position (%d, %d) with type %d", position.x, position.y, type);
}

void ghost_update(void *specific, struct GameState *passed_gamestate)
{
    Entity *entity = (Entity *)specific;
    GameState *game_state = (GameState *)passed_gamestate;

    // Access the Ghost structure directly
    Ghost *ghost = &entity->specific.ghost;

    LOG_DEBUG("Updating Ghost...");
    LOG_DEBUG("Current position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Current direction: (%d, %d)", entity->direction.x, entity->direction.y);
    LOG_DEBUG("Current speed: %d", entity->speed);
    LOG_DEBUG("Current mode: %d", ghost->mode);

    // Calculate the next position based on the ghost type/mode
    ghost->target_tile = calculate_target_tile(ghost, game_state, entity);
    Vector2D next_dir = get_next_direction_towards_target(entity->position, ghost->target_tile, &game_state->map);

    entity->direction = next_dir;
    Vector2D next_pos = {
        entity->position.x + next_dir.x * entity->speed,
        entity->position.y + next_dir.y * entity->speed};

    if (map_is_walkable(&game_state->map, next_pos.x, next_pos.y))
    {
        entity->position = next_pos;
    }

    else
    {
        LOG_DEBUG("Ghost's move blocked by a wall.");
        // TODO: Implement logic for changing direction when blocked
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
                return (Vector2D){1, NUM_TILES_Y - 2}; // Scatter corner
            }
        }
        }
        break;

    case GHOST_MODE_FRIGHTENED:
    default:
        return (Vector2D){rand() % NUM_TILES_X, rand() % NUM_TILES_Y}; // Random move
    }

    return pacman_pos;
}

Vector2D get_next_direction_towards_target(Vector2D current, Vector2D target, struct Map *map)
{
    Vector2D best_dir = {0, 0};
    int min_distance = 99999;

    Vector2D directions[] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < 4; i++)
    {
        Vector2D try_pos = {
            current.x + directions[i].x,
            current.y + directions[i].y};

        if (!map_is_walkable(map, try_pos.x, try_pos.y))
            continue;

        int dx = target.x - try_pos.x;
        int dy = target.y - try_pos.y;
        int dist = dx * dx + dy * dy;

        if (dist < min_distance)
        {
            min_distance = dist;
            best_dir = directions[i];
        }
    }

    return best_dir;
}

void ghost_render(void *specific)
{
    Entity *entity = (Entity *)specific;
    Ghost *ghost = &entity->specific.ghost;

    LOG_DEBUG("Rendering Ghost...");
    LOG_DEBUG("Ghost position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Ghost mode: %d", ghost->mode);
}