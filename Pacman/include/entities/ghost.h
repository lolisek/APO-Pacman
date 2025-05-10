#ifndef APO_PACMAN_GHOST_H
#define APO_PACMAN_GHOST_H

#include "../utils/vector2d.h"

struct GameState;

struct Entity;

// Ghost behavior modes
typedef enum
{
    GHOST_MODE_CHASE,
    GHOST_MODE_SCATTER,
    GHOST_MODE_FRIGHTENED,
    GHOST_MODE_EATEN
} GhostMode;

typedef enum
{
    GHOST_TYPE_BLINKY, // Red ghost, strategy: chase Pac
    GHOST_TYPE_PINKY,  // Pink ghost, strategy: ambush Pac
    GHOST_TYPE_INKY,   // Cyan ghost, strategy: unpredictable
    GHOST_TYPE_CLYDE   // Orange ghost, strategy: random
} GhostType;

// Ghost-specific structure
typedef struct
{
    GhostMode mode;       // Current behavior mode
    GhostType type;       // Type of ghost (Blinky, Pinky, Inky, Clyde)
    int frightened_timer; // Timer for frightened mode
} Ghost;

// Function prototypes
void ghost_init(struct Entity *entity, Vector2D position, GhostType type); // Initialize ghost
void ghost_update(void *specific, struct GameState *gamestate);            // Override for update
void ghost_render(void *specific);                                         // Override for render

#endif // APO_PACMAN_GHOST_H