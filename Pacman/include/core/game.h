#include "game_state.h"

// Game loop
#ifndef APO_PACMAN_GAME_H
#define APO_PACMAN_GAME_H

// Game loop and state management

// Function to run the main game loop
void run_game_loop(uint16_t *shared_fb); // <-- Fix prototype
void update_game_state(GameState *game_state);
void check_collisions(GameState *game_state);

#endif // APO_PACMAN_GAME_H