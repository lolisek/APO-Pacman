#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include "../../include/microzed/mzapo_peri.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void handle_input(GameState *gamestate, bool *running) {
    static uint8_t last_knob_pos = 0;
    static int current_dir_index = 0; // Start facing right
    static int accumulated_change = 0;

    uint8_t current_knob = get_red_knob_rotation();
    int delta = (int)current_knob - (int)last_knob_pos;

    // Handle knob wrap-around (255 to 0 or vice versa)
    if (delta > KNOB_CLICK_SIZE/2) delta -= KNOB_CLICK_SIZE + 1;
    else if (delta < -KNOB_CLICK_SIZE/2) delta += KNOB_CLICK_SIZE + 1;

    accumulated_change += delta;

    // Check if we've moved enough for a full click
    if (abs(accumulated_change) >= KNOB_CLICKS_PER_TURN) {
        int clicks = accumulated_change / KNOB_CLICKS_PER_TURN;
        accumulated_change %= KNOB_CLICKS_PER_TURN; // Keep remainder

        if (clicks != 0) {
            // Change direction (1 click = 1 direction change)
            current_dir_index = (current_dir_index + (clicks > 0 ? 1 : -1)) % 4;
            if (current_dir_index < 0) current_dir_index += 4; // Fix negative
            
            gamestate->pacman.direction = directions[current_dir_index];
            
            printf("Direction changed to: (%d, %d)\n", 
                   gamestate->pacman.direction.x,
                   gamestate->pacman.direction.y);
        }
    }

    last_knob_pos = current_knob;
}