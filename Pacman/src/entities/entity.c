#include "../../include/entities/entity.h"
#include <stdio.h>

void entity_init(Entity *entity, EntityType type, Vector2D position, Vector2D direction, int speed)
{
    entity->position = position;
    entity->direction = direction;
    entity->speed = speed;
    entity->type = type;
    entity->update = NULL; // To be set by derived types
    entity->render = NULL; // To be set by derived types
}

void entity_update(Entity *entity)
{
    if (entity->update)
    {
        entity->update(entity);
    }
}

void entity_render(const Entity *entity)
{
    if (entity->render)
    {
        entity->render(entity);
    }
}