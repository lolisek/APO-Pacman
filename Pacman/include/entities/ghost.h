#ifndef APO_PACMAN_GHOST_H
#define APO_PACMAN_GHOST_H

#define SCATTER_DURATION 7000  // 7 seconds
#define CHASE_DURATION 20000    // 20 seconds
#define FRIGHTENED_DURATION 5000 // 5 seconds

#include "../utils/vector2d.h" // For Vector2D

// Forward declarations to avoid cyclic dependencies
struct GameState;
struct Entity;
struct Map;

// Ghost behavior modes
typedef enum
{
    GHOST_MODE_CHASE,
    GHOST_MODE_SCATTER,
    GHOST_MODE_FRIGHTENED,
    GHOST_MODE_EATEN,
    GHOST_MODE_EXITING
} GhostMode;

typedef enum
{
    GHOST_TYPE_BLINKY, // Red ghost, strategy: chase Pac
    GHOST_TYPE_PINKY,  // Pink ghost, strategy: ambush Pac
    GHOST_TYPE_INKY,   // Cyan ghost, strategy: unpredictable
    GHOST_TYPE_CLYDE   // Orange ghost, strategy: random
} GhostType;

typedef struct {
    Vector2D last_junction_pos;
    Vector2D last_junction_decision;
} GhostNavigationMemory;

// Ghost-specific structure
typedef struct
{
    GhostMode mode;       // Current behavior mode
    GhostType type;       // Type of ghost (Blinky, Pinky, Inky, Clyde)
    int frightened_timer; // Timer for frightened mode
    int waiting_timer;
    Vector2D target_tile; // Target tile for the ghost to move towards
    Vector2D starting_position;       // Starting position of the ghost
    uint64_t mode_timer;              // Added to track mode timing
    GhostNavigationMemory navigation; // Last position stored
} Ghost;

/**
 * @brief Initializes a ghost entity with its type and starting position.
 *
 * @param entity Pointer to the ghost entity.
 * @param type The type of the ghost (e.g., Blinky, Pinky, Inky, Clyde).
 */
void ghost_init(struct Entity *entity, GhostType type);

/**
 * @brief Determines the next direction for a ghost to move towards its target.
 *
 * @param current Current position of the ghost.
 * @param target Target position for the ghost.
 * @param map Pointer to the game map.
 * @param current_dir Current direction of the ghost.
 * @param memory Pointer to the ghost's navigation memory.
 * @param ghost Pointer to the ghost structure.
 * @return Vector2D The next direction for the ghost.
 */
Vector2D get_next_direction_towards_target(Vector2D current, Vector2D target,
                                           struct Map *map, Vector2D current_dir,
                                           GhostNavigationMemory *memory,
                                           Ghost *ghost);

/**
 * @brief Updates the ghost's state and position based on its mode and target.
 *
 * @param specific Pointer to the ghost-specific data.
 * @param passed_gamestate Pointer to the current game state.
 */
void ghost_update(void *specific, struct GameState *passed_gamestate);

Vector2D calculate_target_tile(Ghost *ghost, struct GameState *state, struct Entity *ghost_entity);

#endif // APO_PACMAN_GHOST_H