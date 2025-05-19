#ifndef APO_PACMAN_CONSTANTS_H
#define APO_PACMAN_CONSTANTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// --- Project user ---
#define USER "root"

// --- Screen configuration ---
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

// --- Map configuration ---
#define NUM_TILES_X 23
#define NUM_TILES_Y 25
#define TILE_SIZE 13

// --- Game configuration ---
#define NUM_GHOSTS 4
#define NUM_PELLETS 240
#define NUM_POWER_PELLETS 4

#define PACMAN_SPEED 1
#define GHOST_SPEED 1

#define FRIGHTENED_MODE_DURATION 5 // seconds
#define SCATTER_MODE_DURATION 7    // seconds
#define GHOST_MODE_DURATION 20     // seconds

#define PACMAN_START_X 1
#define PACMAN_START_Y 1
#define GHOST_START_X 13
#define GHOST_START_Y 15

#define PACMAN_START_LIVES 3
#define PACMAN_START_SCORE 0

// --- Input configuration ---
#define PACMAN_KNOB_SENSITIVITY 5

// --- Resource path helper ---
void get_resource_path(char *out, size_t out_size, const char *filename);

#endif // APO_PACMAN_CONSTANTS_H
