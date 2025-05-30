#include "../../include/utils/led_manager.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"

static bool all_leds_initialized = false;

void update_leds(const GameState *game_state) {
    int total_leds = 32;

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


