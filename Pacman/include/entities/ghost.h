#ifndef APO_PACMAN_GHOST_H
#define APO_PACMAN_GHOST_H

#include "./entity.h"

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
    Entity base;          // Base entity structure
    GhostMode mode;       // Current behavior mode
    GhostType type;       // Type of ghost (Blinky, Pinky, Inky, Clyde)
    int frightened_timer; // Timer for frightened mode
} Ghost;

// Function prototypes
void ghost_init(Ghost *ghost, Vector2D position);
void ghost_update(Entity *entity);       // Override for update
void ghost_render(const Entity *entity); // Override for render

#endif // APO_PACMAN_GHOST_H