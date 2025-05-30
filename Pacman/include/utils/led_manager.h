#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include "../core/game_state.h"

/**
 * @brief Updates the LED state based on the current game state.
 * @param game_state The current game state.
 */
void update_leds(const GameState *game_state);

#endif // LED_MANAGER_H
