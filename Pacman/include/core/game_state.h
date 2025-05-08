#ifndef APO_PACMAN_GAME_STATE_H
#define APO_PACMAN_GAME_STATE_H

#include <stdbool.h>
#include "entities/entity.h"
#include "utils/constants.h"
#include "map/map.h"

/**
 * @brief Represents the overall state of the game.
 */
typedef struct
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
 */
void init_game_state();

/**
 * @brief Resets the level to its initial state.
 */
void reset_level();

/**
 * @brief Cleans up resources used by the game state.
 */
void cleanup_game();

#endif // APO_PACMAN_GAME_STATE_H