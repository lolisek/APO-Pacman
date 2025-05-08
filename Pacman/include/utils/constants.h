#ifndef APO_PACMAN_CONSTANTS_H
#define APO_PACMAN_CONSTANTS_H

#include <stdint.h>
#include <stdbool.h>

// Game-wide constants and configuration
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define TILE_SIZE 16
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

#define NUM_GHOSTS 4
#define NUM_PELLETS 240
#define NUM_POWER_PELLETS 4

#define PACMAN_SPEED 2
#define GHOST_SPEED 1

#define FRIGHTENED_MODE_DURATION 5 // Duration in seconds
#define SCATTER_MODE_DURATION 7    // Duration in seconds
#define GHOST_MODE_DURATION 20     // Duration in seconds

#define PACMAN_START_X 1
#define PACMAN_START_Y 1

#define GHOST_START_X 13
#define GHOST_START_Y 15

#define PACMAN_START_LIVES 3
#define PACMAN_START_SCORE 0

#endif // APO_PACMAN_CONSTANTS_H
