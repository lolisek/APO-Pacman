#ifndef APO_PACMAN_RENDERER_H
#define APO_PACMAN_RENDERER_H

#include "../entities/entity.h"
#include "../map/map.h"
#include "../core/game_state.h"
#include "../gui/ppm_loader.h"
#include "../microzed/mzapo_peri.h"
#include "../gui/draw_text.h"

#define TILE_SIZE 13
#define PACMAN_ANIMATION_SPEED 4

static ppm_image_t *wall_texture = NULL;
static ppm_image_t *pacman_textures[5] = {NULL}; // Right, Left, Up, Down, Closed
static ppm_image_t *ghost_red_textures[2] = {NULL}; // Right, Left
static ppm_image_t *ghost_orange_textures[2] = {NULL}; // Right, Left
static ppm_image_t *ghost_blue_textures[2] = {NULL}; // Right, Left
static ppm_image_t *ghost_pink_textures[2] = {NULL}; // Right, Left
static ppm_image_t *pellet_texture = NULL;
static ppm_image_t *power_pellet_texture = NULL;

/**
 * @brief Initializes the rendering system.
 * Loads textures and prepares the rendering environment.
 */
void render_init(void);

/**
 * @brief Renders the entire game state.
 * @param game_state The current game state to render.
 * @param fb The framebuffer to render to.
 */
void render(GameState *game_state, uint16_t *fb);

void render_ui(const GameState *game_state, uint16_t *fb);

/**
 * @brief Renders the game map.
 * @param map The map to render.
 */
void render_map(const Map *map, uint16_t *fb, int offset_x, int offset_y);

void render_pacman(const Entity *pacman, uint16_t *framebuffer, int animation_frame, int offset_x, int offset_y);

void render_ghost(const Entity *ghost, uint16_t *framebuffer, int animation_frame, int offset_x, int offset_y);

#endif // APO_PACMAN_RENDERER_H