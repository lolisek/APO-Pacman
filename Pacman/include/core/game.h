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
 * @brief Handles the game over sequence, including rendering the game over screen
 *        and asking the player if they want to save their score.
 * @param shared_fb Pointer to the shared framebuffer.
 * @param score The player's final score.
 */
void handle_game_over(uint16_t *shared_fb, int score);

/**
 * @brief Initializes game resources, such as preloading textures.
 */
void init_game_resources();

/**
 * @brief Cleans up game resources, freeing allocated memory.
 */
void cleanup_game_resources();

/**
 * @brief Spawns a random pellet on an empty tile in the game map.
 *
 * @param game_state Pointer to the game state structure.
 */
void spawn_random_pellet(GameState *game_state);

/**
 * @brief Spawns a random power pellet on an empty tile in the game map.
 *
 * @param game_state Pointer to the game state structure.
 */
void spawn_random_power_pellet(GameState *game_state);

/**
 * @brief Main game loop that updates the game state and handles pellet spawning.
 *
 * @param arg Unused argument for thread compatibility.
 * @return void* Always returns NULL.
 */
void *game_loop(void *arg);

/**
 * @brief Render loop that updates the framebuffer and displays the game.
 *
 * @param arg Pointer to the shared framebuffer.
 * @return void* Always returns NULL.
 */
void *render_loop(void *arg);

/**
 * @brief Cleans up the game state by resetting all entities and variables.
 *
 * @param game_state Pointer to the game state structure.
 */
void cleanup_game(GameState *game_state);

#endif // APO_PACMAN_GAME_H