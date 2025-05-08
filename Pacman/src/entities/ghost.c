#include "../../include/entities/ghost.h"
#include <stdio.h>

void ghost_init(Ghost *ghost, Vector2D position)
{
    entity_init(&ghost->base, ENTITY_TYPE_GHOST, position, (Vector2D){0, 0}, 1);
    ghost->mode = GHOST_MODE_SCATTER;
    ghost->frightened_timer = 0;
    ghost->base.update = ghost_update;
    ghost->base.render = ghost_render;

    // TODO implement ghost-types (blinky, pinky, inky, clyde)
}

void ghost_update(Entity *entity)
{
    Ghost *ghost = (Ghost *)entity;
    // Update ghost's position based on direction and speed
    ghost->base.position.x += ghost->base.direction.x * ghost->base.speed;
    ghost->base.position.y += ghost->base.direction.y * ghost->base.speed;
    printf("Ghost updated: (%d, %d)\n", ghost->base.position.x, ghost->base.position.y);
    // TODO Implement ghost behavior based on mode
}

void ghost_render(const Entity *entity)
{
    const Ghost *ghost = (const Ghost *)entity;

    printf("Rendering Ghost at (%d, %d)\n", ghost->base.position.x, ghost->base.position.y);
    // TODO Render ghost based on its current mode
}