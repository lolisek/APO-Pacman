#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void handle_input(GameState *gamestate)
{
    static uint8_t last_knob_pos = 0;  /**< Stores the last position of the red knob. */
    static int current_dir_index = 0;  /**< Index representing the current direction (0: Right, 1: Down, 2: Left, 3: Up). */
    static int accumulated_change = 0; /**< Accumulated knob rotation change. */

    // Get the current red knob position
    uint8_t current_knob = get_red_knob_rotation();
    int delta = (int)current_knob - (int)last_knob_pos;

    // Handle knob wrap-around (assuming 8-bit value)
    if (delta > 128)
    {
        delta -= 256;
    }
    else if (delta < -128)
    {
        delta += 256;
    }

    accumulated_change += delta;
    last_knob_pos = current_knob;

    // Only change direction when enough rotation has been accumulated
    while (abs(accumulated_change) >= (KNOB_CLICKS_PER_TURN * PACMAN_KNOB_SENSITIVITY))
    {
        int step = (accumulated_change > 0) ? 1 : -1;
        current_dir_index = (current_dir_index + step) % 4;
        if (current_dir_index < 0)
        {
            current_dir_index += 4;
        }

        // Update the buffered direction
        gamestate->pacman.specific.pacman.buffered_direction = directions[current_dir_index];

        accumulated_change -= step * KNOB_CLICKS_PER_TURN * PACMAN_KNOB_SENSITIVITY;
    }

    // Check for exit input (blue knob press)
    if (blue_knob_is_pressed())
    {
        LOG_INFO("Exit input detected. Exiting game...");
        gamestate->game_over = true;
    }
}