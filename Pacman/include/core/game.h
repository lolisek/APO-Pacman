#ifndef APO_PACMAN_GAME_H
#define APO_PACMAN_GAME_H

#include "game_state.h"

/**
 * @brief Runs the main game loop.
 * @param shared_fb Pointer to the shared framebuffer.
 */
void run_game_loop(uint16_t *shared_fb);

/**
 * @brief Updates the game state (entities, logic, etc).
 * @param game_state Pointer to the game state.
 */
void update_game_state(GameState *game_state);

/**
 * @brief Checks for collisions (Pac-Man vs ghosts, etc).
 * @param game_state Pointer to the game state.
 */
void check_collisions(GameState *game_state);

#endif // APO_PACMAN_GAME_H