#ifndef APO_PACMAN_ENTITY_H
#define APO_PACMAN_ENTITY_H

#include "../utils/vector2d.h"
#include <stdint.h>
#include <stdbool.h>
#include "../entities/pacman.h"
#include "../entities/ghost.h"

struct GameState;

// Entity types
typedef enum
{
    ENTITY_TYPE_PACMAN,
    ENTITY_TYPE_GHOST
} EntityType;

// Unified Entity structure
typedef struct Entity
{
    union
    {
        Pacman pacman; // Embedded Pacman structure
        Ghost ghost;   // Embedded Ghost structure
    } specific;
    EntityType type;                                             // Type of the entity (Pacman or Ghost)
    Vector2D position;                                           // Position of the entity
    Vector2D direction;                                          // Direction of movement
    uint16_t speed;                                              // Speed of the entity
    void (*update)(void *specific, struct GameState *gamestate); // Function pointer for updating the specific entity
    void (*render)(void *specific);                              // Function pointer for rendering the specific entity
} Entity;

/**
 * @brief Initializes an entity with its type and update function.
 *
 * @param entity Pointer to the entity to initialize.
 * @param type The type of the entity (e.g., Pac-Man, Ghost).
 * @param update Function pointer for the entity's update logic.
 */
void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, struct GameState *gamestate));

/**
 * @brief Updates the entity by calling its update function.
 *
 * @param entity Pointer to the entity to update.
 * @param gamestate Pointer to the current game state.
 */
void entity_update(Entity *entity, struct GameState *gamestate);

#endif // APO_PACMAN_ENTITY_H