#ifndef APO_PACMAN_RENDERER_H
#define APO_PACMAN_RENDERER_H

#include "../entities/entity.h"
#include "../map/map.h"
#include "../core/game_state.h"
#include "../gui/ppm_loader.h"
#include "../microzed/mzapo_peri.h"
#include "../gui/draw_text.h"
#include "../utils/constants.h" // Use TILE_SIZE from here

#define PACMAN_ANIMATION_SPEED 4

// --- Renderer API ---

/**
 * @brief Renders the entire game state.
 * @param game_state The current game state to render.
 * @param fb The framebuffer to render to.
 */
void render(GameState *game_state, uint16_t *fb);

/**
 * @brief Renders the UI (score, lives, etc).
 */
void render_ui(const GameState *game_state, uint16_t *fb);

/**
 * @brief Renders the game map.
 * @param map The map to render.
 */
void render_map(const Map *map, uint16_t *fb, int offset_x, int offset_y);

/**
 * @brief Renders Pac-Man.
 */
void render_pacman(const Entity *pacman, uint16_t *framebuffer, int animation_frame, int offset_x, int offset_y);

/**
 * @brief Renders a ghost.
 */
void render_ghost(const Entity *ghost, uint16_t *framebuffer, int animation_frame, int offset_x, int offset_y);

#endif // APO_PACMAN_RENDERER_H