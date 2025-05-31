#include "game_state.h"
#include <stdbool.h>
#include "../microzed/mzapo_peri.h"

#ifndef APO_PACMAN_INPUT_H
#define APO_PACMAN_INPUT_H

// Input handling

static const Vector2D directions[] = {
    {-1, 0}, // Left
    {0, -1}, // Down
    {1, 0},  // Right
    {0, 1}   // Up
};

/**
 * @brief Handles user input from the knobs and updates the game state accordingly.
 *
 * @param gamestate Pointer to the current game state.
 */
void handle_input(GameState *gamestate);

#endif // APO_PACMAN_INPUT_H