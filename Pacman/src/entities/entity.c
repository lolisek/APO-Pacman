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
        switch (entity->type)
        {
        case ENTITY_TYPE_PACMAN:
            entity->update(entity, gamestate);
            break;
        case ENTITY_TYPE_GHOST:
            entity->update(entity, gamestate);
            break;
        }
    }
}

void entity_render(Entity *entity)
{
    if (entity->render)
    {
        switch (entity->type)
        {
        case ENTITY_TYPE_PACMAN:
            entity->render(entity);
            break;
        case ENTITY_TYPE_GHOST:
            entity->render(entity);
            break;
        }
    }
}