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
    entity_init(entity, ENTITY_TYPE_PACMAN, pacman_update);
    entity->position = position;
    entity->direction = (Vector2D){0, 0};
    entity->speed = PACMAN_SPEED;
}

void pacman_update(void *specific, struct GameState *passed_gamestate)
{
    Entity *entity = (Entity *)specific;
    GameState *game_state = (GameState *)passed_gamestate;
    Pacman *pacman = &entity->specific.pacman;

    // Predict Pac-Man's next position based on the buffered direction
    Vector2D buffered_next_position = {
        entity->position.x + pacman->buffered_direction.x * entity->speed,
        entity->position.y + pacman->buffered_direction.y * entity->speed};

    // Check if the buffered direction is valid
    if (map_is_walkable(&game_state->map, (int)buffered_next_position.x, (int)buffered_next_position.y, ENTITY_TYPE_PACMAN))
    {
        // Apply the buffered direction
        entity->direction = pacman->buffered_direction;
    }
    else
    {
        LOG_DEBUG("Buffered direction blocked. Falling back to current direction.");
    }

    // Predict Pac-Man's next position based on the current direction
    Vector2D next_position = {
        entity->position.x + entity->direction.x * entity->speed,
        entity->position.y + entity->direction.y * entity->speed};

    // Check if the current direction is valid
    if (map_is_walkable(&game_state->map, (int)next_position.x, (int)next_position.y, ENTITY_TYPE_PACMAN))
    {
        // Move Pac-Man in the current direction
        entity->position = next_position;
    }
    else
    {
        LOG_DEBUG("Pac-Man's move blocked by a wall or gate.");
    }

    // Check for collisions with pellets
    int tile_x = (int)(entity->position.x + 0.5f);
    int tile_y = (int)(entity->position.y + 0.5f);
    if (tile_x >= 0 && tile_x < game_state->map.width && tile_y >= 0 && tile_y < game_state->map.height)
    {
        if (game_state->map.tiles[tile_y][tile_x].type == TILE_PELLET)
        {
            game_state->score += PELLET_SCORE;                       // Use constant for pellet score
            game_state->map.tiles[tile_y][tile_x].type = TILE_EMPTY; // Remove the pellet from the map
        }
        else if (game_state->map.tiles[tile_y][tile_x].type == TILE_POWER_PELLET)
        {
            game_state->score += POWER_PELLET_SCORE;                 // Use constant for power pellet score
            game_state->map.tiles[tile_y][tile_x].type = TILE_EMPTY; // Remove the power pellet
            game_state->frightened_timer = FRIGHTENED_MODE_DURATION; // Set frightened mode duration

            // Set all ghosts to frightened mode
            for (int i = 0; i < NUM_GHOSTS; i++)
            {
                Ghost *ghost = &game_state->ghosts[i].specific.ghost;
                if (ghost->mode != GHOST_MODE_EATEN && ghost->mode != GHOST_MODE_EXITING)
                {
                    ghost->mode = GHOST_MODE_FRIGHTENED;
                    ghost->frightened_timer = FRIGHTENED_MODE_DURATION;
                }
            }
        }
    }
}
