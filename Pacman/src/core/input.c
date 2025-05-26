#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void handle_input(GameState *gamestate, bool *running)
{
    static uint8_t last_knob_pos = 0;
    static int current_dir_index = 0; // Start facing right
    static int accumulated_change = 0;

    uint8_t current_knob = get_red_knob_rotation();
    LOG_DEBUG("Current knob position: %d, Last knob position: %d", current_knob, last_knob_pos);
    int delta = (int)current_knob - (int)last_knob_pos;

    // Handle knob wrap-around (assuming 8-bit value)
    if (delta > 128)
        delta -= 256;
    else if (delta < -128)
        delta += 256;

    accumulated_change += delta;
    last_knob_pos = current_knob;

    // Only change direction when enough rotation has been accumulated
    while (abs(accumulated_change) >= (KNOB_CLICKS_PER_TURN * PACMAN_KNOB_SENSITIVITY))
    {
        int step = (accumulated_change > 0) ? 1 : -1;
        current_dir_index = (current_dir_index + step) % 4;
        if (current_dir_index < 0)
            current_dir_index += 4;

        // Update the buffered direction
        gamestate->pacman.specific.pacman.buffered_direction = directions[current_dir_index];

        accumulated_change -= step * KNOB_CLICKS_PER_TURN * PACMAN_KNOB_SENSITIVITY;
    }
}