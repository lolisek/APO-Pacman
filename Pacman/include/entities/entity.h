#ifndef APO_PACMAN_ENTITY_H
#define APO_PACMAN_ENTITY_H

#include <stdbool.h>
#include "../utils/vector2d.h"
#include <stdint.h>

// Forward declarations for polymorphism
struct Entity;

// Function pointer types for polymorphic behavior
typedef void (*EntityUpdateFunc)(struct Entity *entity);
typedef void (*EntityRenderFunc)(const struct Entity *entity);

// Entity types
typedef enum
{
    ENTITY_TYPE_PACMAN,
    ENTITY_TYPE_GHOST
} EntityType;

// Base structure for all entities
typedef struct Entity
{
    Vector2D position;       // Position of the entity
    Vector2D direction;      // Direction of movement
    uint16_t speed;          // Movement speed
    EntityType type;         // Type of entity
    EntityUpdateFunc update; // Function pointer for updating the entity
    EntityRenderFunc render; // Function pointer for rendering the entity
} Entity;

// Function prototypes
void entity_init(Entity *entity, EntityType type, Vector2D position, Vector2D direction, uint16_t speed);
void entity_update(Entity *entity);
void entity_render(const Entity *entity);

#endif // APO_PACMAN_ENTITY_H