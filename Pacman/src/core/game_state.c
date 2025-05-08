#include "../include/core/game_state.h"
#include "../include/entities/pacman.h"
#include <stdio.h>

void init_game_state(GameState *game_state)
{

    Pacman pacman = {0}; // Initialize Pacman
    pacman_init(&pacman, (Vector2D){PACMAN_START_X, PACMAN_START_Y});

    game_state->pacman = pacman; // Set Pac

    Ghost ghosts[NUM_GHOSTS] = {0}; // Initialize ghosts

    ghost_init(&ghosts[0], (Vector2D){GHOST_START_X, GHOST_START_Y}); // blinky
    // ghost_init(&ghosts[1], (Vector2D){GHOST_START_X + 1, GHOST_START_Y}); // pinky
    // ghost_init(&ghosts[2], (Vector2D){GHOST_START_X + 2, GHOST_START_Y}); // inky
    // ghost_init(&ghosts[3], (Vector2D){GHOST_START_X + 3, GHOST_START_Y}); // clyde

    game_state->ghosts[0] = ghosts[0]; // Set the first ghost

    game_state->score = PACMAN_START_SCORE; // Initialize score
    game_state->lives = PACMAN_START_LIVES; // Initialize lives
    game_state->frightened_timer = 0;       // Initialize frightened timer

    game_state->map = (Map){0}; // Initialize map
    map_init(&game_state->map); // Initialize the map structure
    if (!map_load_from_file(&game_state->map, "./assets/maps/level1.txt"))
    {
        fprintf(stderr, "Failed to load map\n");
        return;
    }

    game_state->game_over = false; // Set game over flag to false
}

void reset_level(GameState *game_state)
{
    // Reset Pacman position and score
    game_state->pacman.base.position = (Vector2D){PACMAN_START_X, PACMAN_START_Y};
    game_state->pacman.lives = PACMAN_START_LIVES;
    // TODO reset ghosts positions

    // Reset the map

    //

    game_state->score = PACMAN_START_SCORE;
}
void cleanup_game(GameState *game_state)
{
    // Free any allocated resources
    // For example, if you dynamically allocate memory for the map or entities,
    // you should free it here.
}
