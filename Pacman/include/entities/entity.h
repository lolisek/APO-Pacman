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

// Function prototypes
void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, struct GameState *gamestate));
void entity_update(Entity *entity, struct GameState *gamestate);

#endif // APO_PACMAN_ENTITY_H