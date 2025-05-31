#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

/**
 * @brief Initializes an entity with its type and update function.
 *
 * @param entity Pointer to the entity to initialize.
 * @param type The type of the entity (e.g., Pac-Man, Ghost).
 * @param update Function pointer for the entity's update logic.
 */
void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, GameState *gamestate))
{
    entity->type = type;
    entity->update = update;
}

/**
 * @brief Updates the entity by calling its update function.
 *
 * @param entity Pointer to the entity to update.
 * @param gamestate Pointer to the current game state.
 */
void entity_update(Entity *entity, GameState *gamestate)
{
    if (entity->update)
    {
        entity->update(entity, gamestate);
    }
}
