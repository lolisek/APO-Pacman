#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, GameState *gamestate), void (*render)(void *specific))
{
    entity->type = type;
    entity->update = update;
    entity->render = render;
}

void entity_update(Entity *entity, GameState *gamestate)
{
    if (entity->update)
    {
        entity->update(entity, gamestate);
    }
}

void entity_render(Entity *entity)
{
    if (entity->render)
    {
        entity->render(entity);
    }
}