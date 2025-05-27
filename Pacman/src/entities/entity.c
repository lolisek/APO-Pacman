#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, GameState *gamestate))
{
    entity->type = type;
    entity->update = update;
}

void entity_update(Entity *entity, GameState *gamestate)
{
    if (entity->update)
    {
        entity->update(entity, gamestate);
    }
}

