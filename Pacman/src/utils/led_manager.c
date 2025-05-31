/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  led_manager.c      - LED management and updates

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/utils/led_manager.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"

#define total_leds 32

void update_leds(const GameState *game_state) {
    // If the game is not running, turn off the RGB LED
    if (!game_state || game_state->game_over)
    {
        set_rgb1(0, 0, 0); // Turn off RGB LED
        set_all_leds_off(); // Turn off all LEDs
        return;
    }

    // RGB LED state indication
    if (game_state->frightened_timer > 0)
    {
        set_rgb1(0, 0, 255); // Blue when power-up is active
    }
    else
    {
        set_rgb1(0, 255, 0); // Green for running
    }

    // LED behavior during the game
    if (game_state->frightened_timer > 0) {
        // Frightened state: Start with all LEDs on and turn them off one by one
        int leds_off = ((FRIGHTENED_MODE_DURATION - game_state->frightened_timer) * total_leds) / FRIGHTENED_MODE_DURATION;

        LOG_DEBUG("Frightened mode active. Remaining time: %d ms, LEDs off: %d", game_state->frightened_timer, leds_off);

        for (int i = 0; i < total_leds; i++) {
            if (i < leds_off) {
                set_led_off(i); // Turn off LED
            } else {
                set_led_on(i); // Keep LED on
            }
        }
    } else {
        // Normal state: Start with all LEDs on and progressively turn them off as lives decrease
        int leds_off = ((PACMAN_START_LIVES - game_state->lives) * total_leds) / PACMAN_START_LIVES;

        LOG_DEBUG("Normal state. Lives remaining: %d, LEDs off: %d", game_state->lives, leds_off);

        for (int i = 0; i < total_leds; i++) {
            if (i < leds_off)
            {
                set_led_off(i); // Turn off LED
            }
            else
            {
                set_led_on(i); // Keep LED on
            }
        }
    }
}


