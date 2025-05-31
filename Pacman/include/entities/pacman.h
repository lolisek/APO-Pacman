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

/**
 * @brief Initializes the Pac-Man entity with its starting position and attributes.
 *
 * @param entity Pointer to the Pac-Man entity.
 * @param position The starting position of Pac-Man.
 */
void pacman_init(struct Entity *entity, Vector2D position);

/**
 * @brief Updates the Pac-Man entity, including movement and interactions with the map.
 *
 * @param specific Pointer to the Pac-Man-specific data.
 * @param gamestate Pointer to the current game state.
 */
void pacman_update(void *specific, struct GameState *gamestate); // Override for update

#endif // APO_PACMAN_PACMAN_H