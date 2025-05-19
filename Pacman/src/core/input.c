#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include "../../include/microzed/mzapo_peri.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void handle_input(GameState *gamestate, bool *running) {
    static uint8_t last_knob_pos = 0;
    static int current_dir_index = 0; // Start facing right
    static int accumulated_change = 0;

    // Increase this multiplier to make the knob less sensitive (e.g., 2 or 3)
    const int SENSITIVITY_FACTOR = 5;

    uint8_t current_knob = get_red_knob_rotation();
    int delta = (int)current_knob - (int)last_knob_pos;

    // Handle knob wrap-around (assuming 8-bit value)
    if (delta > 128)
        delta -= 256;
    else if (delta < -128)
        delta += 256;

    // Apply sensitivity factor: require more rotation before registering a change
    accumulated_change += delta;

    last_knob_pos = current_knob;

    // Only change direction when enough rotation has been accumulated
    while (abs(accumulated_change) >= (KNOB_CLICKS_PER_TURN * SENSITIVITY_FACTOR))
    {
        int step = (accumulated_change > 0) ? 1 : -1;
        current_dir_index = (current_dir_index + step) % 4;
        if (current_dir_index < 0)
            current_dir_index += 4;

        gamestate->pacman.direction = directions[current_dir_index];

        printf("Direction changed to: (%d, %d)\n",
               gamestate->pacman.direction.x,
               gamestate->pacman.direction.y);

        accumulated_change -= step * KNOB_CLICKS_PER_TURN * SENSITIVITY_FACTOR;
    }
}