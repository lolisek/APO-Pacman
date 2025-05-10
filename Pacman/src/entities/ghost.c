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

    // Access the embedded Ghost structure directly
    // Ghost *ghost = &entity->specific.ghost;

    LOG_DEBUG("Updating Ghost...");
    LOG_DEBUG("Current position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Current direction: (%d, %d)", entity->direction.x, entity->direction.y);
    LOG_DEBUG("Current speed: %d", entity->speed);
    LOG_DEBUG("Current mode: %d", ghost->mode);

    // Calculate the next position based on the ghost type/mode
    // For simplicity, we'll just move in random directions for now

    entity->direction.x = (rand() % 3) - 1; // Random direction (-1, 0, 1)
    entity->direction.y = (rand() % 3) - 1; // Random direction (-1, 0, 1)
    if (entity->direction.x == 0 && entity->direction.y == 0)
    {
        entity->direction.x = 1; // Default to moving right if no direction is set
    }
    LOG_DEBUG("New direction: (%d, %d)", entity->direction.x, entity->direction.y);

    // Predict Ghost's next position
    Vector2D next_position = {
        entity->position.x + entity->direction.x * entity->speed,
        entity->position.y + entity->direction.y * entity->speed};

    // Check if the next position is walkable
    if (map_is_walkable(&game_state->map, next_position.x, next_position.y))
    {
        // Update Ghost's position
        entity->position = next_position;
    }
    else
    {
        LOG_DEBUG("Ghost's move blocked by a wall.");
        // TODO: Implement logic for changing direction when blocked
    }

    LOG_INFO("Ghost update complete.");
}

void ghost_render(void *specific)
{
    Entity *entity = (Entity *)specific;
    Ghost *ghost = &entity->specific.ghost; // Correctly access the Ghost object

    LOG_DEBUG("Rendering Ghost...");
    LOG_DEBUG("Ghost position: (%d, %d)", entity->position.x, entity->position.y);
    LOG_DEBUG("Ghost mode: %d", ghost->mode);

    // Render Ghost (e.g., draw the sprite on the screen)
    printf("Rendering Ghost at (%d, %d) in mode %d\n",
           entity->position.x, entity->position.y, ghost->mode);

    LOG_INFO("Ghost rendered.");
}