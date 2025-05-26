#ifndef APO_PACMAN_PACMAN_H
#define APO_PACMAN_PACMAN_H

#include "../utils/vector2d.h"

struct GameState;

// Forward declaration of Entity to avoid circular dependency
struct Entity;

// Pac-Man-specific structure
typedef struct Pacman
{
    int lives; // Number of lives
    Vector2D buffered_direction; // Buffered direction for smoother movement
} Pacman;

// Function prototypes
void pacman_init(struct Entity *entity, Vector2D position);
void pacman_update(void *specific, struct GameState *gamestate); // Override for update
void pacman_render(void *specific);                              // Override for render

#endif // APO_PACMAN_PACMAN_H