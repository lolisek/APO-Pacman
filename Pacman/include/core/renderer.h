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
 * @brief Renders the entire game state to the framebuffer.
 *
 * @param game_state Pointer to the current game state.
 * @param fb Pointer to the framebuffer.
 */
void render(GameState *game_state, uint16_t *fb);

/**
 * @brief Renders the UI, including the score and remaining lives.
 *
 * @param game_state Pointer to the current game state.
 * @param fb Pointer to the framebuffer.
 */
void render_ui(const GameState *game_state, uint16_t *fb);

/**
 * @brief Renders the game map, including walls, pellets, and power pellets.
 *
 * @param map Pointer to the map structure.
 * @param fb Pointer to the framebuffer.
 * @param offset_x Horizontal offset for centering the map.
 * @param offset_y Vertical offset for centering the map.
 */
void render_map(const Map *map, uint16_t *fb, int offset_x, int offset_y);

/**
 * @brief Renders Pac-Man with animation based on the current frame.
 *
 * @param pacman Pointer to the Pac-Man entity.
 * @param fb Pointer to the framebuffer.
 * @param animation_frame Current animation frame.
 * @param offset_x Horizontal offset for centering the map.
 * @param offset_y Vertical offset for centering the map.
 */
void render_pacman(const Entity *pacman, uint16_t *fb, int animation_frame, int offset_x, int offset_y);

/**
 * @brief Renders a ghost with animation based on its mode and direction.
 *
 * @param ghost Pointer to the ghost entity.
 * @param fb Pointer to the framebuffer.
 * @param animation_frame Current animation frame.
 * @param offset_x Horizontal offset for centering the map.
 * @param offset_y Vertical offset for centering the map.
 */
void render_ghost(const Entity *ghost, uint16_t *fb, int animation_frame, int offset_x, int offset_y);

#endif // APO_PACMAN_RENDERER_H