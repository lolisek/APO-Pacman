#include "../../include/entities/pacman.h"
#include <stdio.h>

void pacman_init(Pacman *pacman, Vector2D position)
{
    entity_init(&pacman->base, ENTITY_TYPE_PACMAN, position, (Vector2D){0, 0}, 2);
    pacman->lives = 3;
    pacman->score = 0;
    pacman->base.update = pacman_update;
    pacman->base.render = pacman_render;
}

void pacman_update(Entity *entity)
{
    Pacman *pacman = (Pacman *)entity;
    // Update Pac-Man's position based on direction and speed
    pacman->base.position.x += pacman->base.direction.x * pacman->base.speed;
    pacman->base.position.y += pacman->base.direction.y * pacman->base.speed;
    printf("Pac-Man updated: (%d, %d)\n", pacman->base.position.x, pacman->base.position.y);
    // TODO Implement update logic for pacman (e.g., check for collisions with pellets, ghosts, etc.)
}

void pacman_render(const Entity *entity)
{
    const Pacman *pacman = (const Pacman *)entity;
    printf("Rendering Pac-Man at (%d, %d)\n", pacman->base.position.x, pacman->base.position.y);
    // TODO Render Pac Man (e.g., draw the sprite)
}