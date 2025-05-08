#ifndef APO_PACMAN_PACMAN_H
#define APO_PACMAN_PACMAN_H

#include <stdbool.h>
#include "../entities/entity.h"
#include "../entities/ghost.h"

// Pac-Man-specific structure
typedef struct
{
    Entity base; // Base entity structure
    int lives;   // Number of lives
    int score;   // Current score
} Pacman;

// Function prototypes
void pacman_init(Pacman *pacman, Vector2D position);
void pacman_update(Entity *entity);       // Override for update
void pacman_render(const Entity *entity); // Override for render

#endif // APO_PACMAN_PACMAN_H