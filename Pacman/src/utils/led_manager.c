#include "../../include/utils/led_manager.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"

static bool all_leds_initialized = false;

#define total_leds 32

void update_leds(const GameState *game_state) {
    // If the game is not running, turn off the RGB LED
    if (!game_state || game_state->game_over)
    {
        set_rgb1(0, 0, 0); // Turn off RGB LED
        return;
    }

    // RGB LED state indication
    if (game_state->game_over)
    {
        set_rgb1(255, 0, 0); // Red for game over
    }
    else if (game_state->frightened_timer > 0)
    {
        set_rgb1(0, 0, 255); // Blue when power-up is active
    }
    else
    {
        set_rgb1(0, 255, 0); // Green for running
    }

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
        // Normal state: Turn off all LEDs
        for (int i = 0; i < total_leds; i++) {
            set_led_off(i);
        }
        all_leds_initialized = false; // Reset initialization flag
    }
}


