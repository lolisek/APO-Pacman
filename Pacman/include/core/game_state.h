#ifndef APO_PACMAN_GAME_STATE_H
#define APO_PACMAN_GAME_STATE_H

#include <stdbool.h>
#include "../entities/entity.h" // Include the full definition of Entity
#include "../utils/constants.h"
#include "../map/map.h"

/**
 * @brief Represents the overall state of the game.
 */
typedef struct GameState
{
    Entity pacman;             ///< Pac-Man entity
    Entity ghosts[NUM_GHOSTS]; ///< Array of ghost entities
    int score;                 ///< Current game score
    int lives;                 ///< Remaining lives
    int frightened_timer;      ///< Timer for frightened mode
    Map map;                   ///< Game map
    bool game_over;            ///< Flag indicating if the game is over
} GameState;

/**
 * @brief Initializes the game state.
 *
 * @param game_state Pointer to the game state to initialize.
 */
void init_game_state(GameState *game_state);

/**
 * @brief Resets the level to its initial state.
 *
 * @param game_state Pointer to the game state to reset.
 */
void reset_level(GameState *game_state);

/**
 * @brief Cleans up resources used by the game state.
 *
 * @param game_state Pointer to the game state to clean up.
 */
void cleanup_game(GameState *game_state);

#endif // APO_PACMAN_GAME_STATE_H