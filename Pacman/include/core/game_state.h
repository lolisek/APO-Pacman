#ifndef APO_PACMAN_GAME_STATE_H
#define APO_PACMAN_GAME_STATE_H

#ifdef __cplusplus
extern "C"
{
#endif

// --- Includes ---
#include <stdbool.h>
#include "../entities/entity.h"
#include "../utils/constants.h"
#include "../map/map.h"

    // --- Types ---

    /**
     * @brief Represents the overall state of the game.
     */
    typedef struct GameState
    {
        Entity pacman;             // Pac-Man entity
        Entity ghosts[NUM_GHOSTS]; // Array of ghost entities
        int score;                 // Current game score
        int lives;                 // Remaining lives
        int frightened_timer;      // Timer for frightened mode
        Map map;                   // Game map
        bool game_over;            // True if the game is over
    } GameState;

    // --- API ---

    /**
     * @brief Initialize the game state.
     * @param game_state Pointer to the game state to initialize.
     */
    void init_game_state(GameState *game_state);

    /**
     * @brief Reset the level to its initial state.
     * @param game_state Pointer to the game state to reset.
     */
    void reset_level(GameState *game_state);

    /**
     * @brief Clean up resources used by the game state.
     * @param game_state Pointer to the game state to clean up.
     */
    void cleanup_game(GameState *game_state);

#ifdef __cplusplus
}
#endif

#endif // APO_PACMAN_GAME_STATE_H