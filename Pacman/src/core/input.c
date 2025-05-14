#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include "../../include/microzed/mzapo_peri.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void handle_input(GameState *gamestate, bool *running)
{
    // TODO implement input handling for microzed
    // Random pacman movement for testing


    uint8_t current_knob = get_red_knob_rotation();

    // Map knob value so that 255-64 is the first quadrant, then 64-128, 128-192, 192-255
    int quadrant;
    if (current_knob >= 64 && current_knob < 128) {
        quadrant = 1; // Right
    } else if (current_knob >= 128 && current_knob < 192) {
        quadrant = 2; // Down
    } else if (current_knob >= 192 && current_knob < 256) {
        quadrant = 3; // Left
    } else {
        // 0-63 and 255-255 (wraps around)
        quadrant = 0; // Up
    }

    switch (quadrant)
    {
        case 0: // 255-64 (wraps around)
            gamestate->pacman.direction.x = 0;
            gamestate->pacman.direction.y = 1;   // Up
            break;
        case 1: // 64-128
            gamestate->pacman.direction.x = 1;
            gamestate->pacman.direction.y = 0;   // Right
            break;
        case 2: // 128-192
            gamestate->pacman.direction.x = 0;
            gamestate->pacman.direction.y = -1;  // Down
            break;
        case 3: // 192-255
            gamestate->pacman.direction.x = -1;
            gamestate->pacman.direction.y = 0;   // Left
            break;
        default:
            break;
    }
}
