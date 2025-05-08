#ifndef APO_PACMAN_RENDERER_H
#define APO_PACMAN_RENDERER_H

#include "../entities/entity.h"
#include "../map/map.h"

/**
 * @brief Renders the entire game state.
 */
void render();

/**
 * @brief Renders a specific entity.
 * @param entity The entity to render.
 */
void render_entity(const Entity *entity);

/**
 * @brief Renders the game map.
 * @param map The map to render.
 */
void render_map(const Map *map);

#endif // APO_PACMAN_RENDERER_H