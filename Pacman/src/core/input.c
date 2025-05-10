#include "../../include/core/input.h"
#include "../../include/core/game_state.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void handle_input(GameState *gamestate, bool *running)
{
    // TODO implement input handling for microzed
    // Random pacman movement for testing

    int random_direction = rand() % 4; // Random number between 0 and 3
    switch (random_direction)
    {
    case 0:
        gamestate->pacman.direction.x = 0;
        gamestate->pacman.direction.y = -1; // Move up
        break;
    case 1:

        gamestate->pacman.direction.x = 0;
        gamestate->pacman.direction.y = 1; // Move down
        break;
    case 2:

        gamestate->pacman.direction.x = -1; // Move left
        gamestate->pacman.direction.y = 0;
        break;
    case 3:
        gamestate->pacman.direction.x = 1; // Move right
        gamestate->pacman.direction.y = 0;
        break;

    default:
        gamestate->pacman.direction.x = 0;
        gamestate->pacman.direction.y = 0; // No movement
        break;
    }
}
