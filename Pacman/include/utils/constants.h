#ifndef APO_PACMAN_CONSTANTS_H
#define APO_PACMAN_CONSTANTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// --- Project user ---
/**
 * @brief Username for the project.
 */
#define USER "root"

// --- Screen configuration ---
/**
 * @brief Width of the game screen in pixels.
 */
#define SCREEN_WIDTH 480

/**
 * @brief Height of the game screen in pixels.
 */
#define SCREEN_HEIGHT 320

/**
 * @brief Duration of each frame in milliseconds (30 FPS).
 */
#define FRAME_DURATION_MS 30

/**
 * @brief Interval for game logic updates in milliseconds.
 */
#define GAME_LOGIC_INTERVAL_MS 300

/**
 * @brief Score awarded for collecting a pellet.
 */
#define PELLET_SCORE 10

/**
 * @brief Score awarded for collecting a power pellet.
 */
#define POWER_PELLET_SCORE 50

/**
 * @brief Score awarded for eating a ghost.
 */
#define GHOST_EATEN_SCORE 200

// --- Map configuration ---
/**
 * @brief Number of tiles along the X-axis of the map.
 */
#define NUM_TILES_X 23

/**
 * @brief Number of tiles along the Y-axis of the map.
 */
#define NUM_TILES_Y 25

/**
 * @brief Size of each tile in pixels.
 */
#define TILE_SIZE 13

// --- Game configuration ---
/**
 * @brief Number of ghosts in the game.
 */
#define NUM_GHOSTS 4

/**
 * @brief Total number of pellets in the game.
 */
#define NUM_PELLETS 240

/**
 * @brief Total number of power pellets in the game.
 */
#define NUM_POWER_PELLETS 4

/**
 * @brief Speed of Pacman in tiles per frame.
 */
#define PACMAN_SPEED 1

/**
 * @brief Speed of ghosts in tiles per frame.
 */
#define GHOST_SPEED 1

/**
 * @brief Duration of frightened mode in seconds.
 */
#define FRIGHTENED_MODE_DURATION 45
#define FRIGHTENED_GHOST_SPEED 0.5
#define SCATTER_MODE_DURATION 3
#define CHASE_MODE_DURATION 5
#define GHOST_MODE_DURATION 20

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

#define GHOST_EATEN_WAIT_TIME 2

#define GHOST_SPAWN_GATE_X 11.0f
#define GHOST_SPAWN_GATE_Y 9.0f
// --- Input configuration ---
#define PACMAN_KNOB_SENSITIVITY 1

/**
 * @brief Retrieves the resource path for a given filename.
 *
 * @param out Pointer to the output buffer where the resource path will be stored.
 * @param out_size Size of the output buffer.
 * @param filename Name of the resource file.
 */
void get_resource_path(char *out, size_t out_size, const char *filename);

#endif // APO_PACMAN_CONSTANTS_H
