#include "../../include/entities/pacman.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"
#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

void pacman_init(Entity *entity, Vector2D position)
{
    LOG_INFO("Initializing Pac-Man...");

    // Access the embedded Pacman structure directly
    Pacman *pacman = &entity->specific.pacman;
    pacman->lives = PACMAN_START_LIVES;
    entity_init(entity, ENTITY_TYPE_PACMAN, pacman_update, pacman_render);
    entity->position = position;
    entity->direction = (Vector2D){0, 0};
    entity->speed = PACMAN_SPEED;
}

void pacman_update(void *specific, struct GameState *passed_gamestate)
{
    Entity *entity = (Entity *)specific;
    GameState *game_state = (GameState *)passed_gamestate;

    // Predict Pac-Man's next position
    Vector2D next_position = {
        entity->position.x + entity->direction.x,
        entity->position.y + entity->direction.y};

    // Check if the next position is walkable
    if (map_is_walkable(&game_state->map, next_position.x, next_position.y, ENTITY_TYPE_PACMAN))
    {
        entity->position = next_position;

        // Check for collisions with pellets
        int tile_x = (int)(next_position.x + 0.5);
        int tile_y = (int)(next_position.y + 0.5);
        if (tile_x >= 0 && tile_x < game_state->map.width && tile_y >= 0 && tile_y < game_state->map.height)
        {
            if (game_state->map.tiles[tile_y][tile_x].type == TILE_PELLET)
            {
                game_state->score += 10;                                 // Increase score
                game_state->map.tiles[tile_y][tile_x].type = TILE_EMPTY; // Remove the pellet from the map
            }
            else if (game_state->map.tiles[tile_y][tile_x].type == TILE_POWER_PELLET)
            {
                game_state->score += 50;                                 // Increase score
                game_state->map.tiles[tile_y][tile_x].type = TILE_EMPTY; // Remove the power pellet
                game_state->frightened_timer = FRIGHTENED_MODE_DURATION; // Set frightened mode duration
            }
        }
    }
    else
    {
        LOG_DEBUG("Pac-Man's move blocked by a wall or gate.");
    }
}

void pacman_render(void *specific)
{
}
