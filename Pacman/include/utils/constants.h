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

#define FRAME_DURATION_MS 30 // 30 FPS (increased from 16)
#define GAME_LOGIC_INTERVAL_MS 300

#define PELLET_SCORE 10
#define POWER_PELLET_SCORE 50

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

#define FRIGHTENED_MODE_DURATION 10
#define FRIGHTENED_GHOST_SPEED 0.5    // Reduced speed during frightened mode
#define SCATTER_MODE_DURATION 3       // seconds
#define GHOST_MODE_DURATION 20     // seconds

// #define PACMAN_START_X 12
// #define PACMAN_START_Y 18
#define PACMAN_START_X 11.0f
#define PACMAN_START_Y 18.0f

#define PACMAN_START_LIVES 3
#define PACMAN_START_SCORE 0
#define KNOB_CLICK_DELAY_MS 100
#define KNOB_ROTATION_DELAY_MS 100

#define GHOST_BLINKY_START_X 10.0f
#define GHOST_BLINKY_START_Y 11.0f
#define GHOST_PINKY_START_X 9.0f
#define GHOST_PINKY_START_Y 11.0f
#define GHOST_INKY_START_X 12.0f
#define GHOST_INKY_START_Y 11.0f
#define GHOST_CLYDE_START_X 13.0f
#define GHOST_CLYDE_START_Y 11.0f

#define GHOST_EATEN_WAIT_TIME 2 // milliseconds

#define GHOST_SPAWN_GATE_X 11.0f // Adjust to match the actual gate position in the map
#define GHOST_SPAWN_GATE_Y 9.0f  // Adjust to match the actual gate position in the map

// --- Input configuration ---
#define PACMAN_KNOB_SENSITIVITY 1

// --- Resource path helper ---
void get_resource_path(char *out, size_t out_size, const char *filename);

#endif // APO_PACMAN_CONSTANTS_H
