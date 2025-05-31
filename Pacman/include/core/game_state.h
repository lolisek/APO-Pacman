#ifndef APO_PACMAN_GAME_STATE_H
#define APO_PACMAN_GAME_STATE_H

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
     * @brief Initializes the game state, including Pac-Man, ghosts, and the map.
     *
     * @param game_state Pointer to the game state structure.
     */
    void init_game_state(GameState *game_state);

    /**
     * @brief Resets the level, including Pac-Man and ghost positions.
     *
     * @param game_state Pointer to the game state structure.
     */
    void reset_level(GameState *game_state);

    /**
     * @brief Clean up resources used by the game state.
     * @param game_state Pointer to the game state to clean up.
     */
    void cleanup_game(GameState *game_state);

    /**
     * @brief Updates the game state, including Pac-Man, ghosts, and collisions.
     *
     * @param game_state Pointer to the game state structure.
     */
    extern void update_game_state(GameState *game_state);

    /**
     * @brief Checks for collisions between Pac-Man and ghosts.
     *
     * @param game_state Pointer to the game state structure.
     */
    void check_collisions(GameState *game_state);

#ifdef __cplusplus
    }
#endif

#endif // APO_PACMAN_GAME_STATE_H
