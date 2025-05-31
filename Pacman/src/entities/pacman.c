#include "../../include/entities/pacman.h"
#include "../../include/utils/constants.h"
#include "../../include/utils/logger.h"
#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

void pacman_init(Entity *entity, Vector2D position)
{
    LOG_INFO("Initializing Pac-Man...");

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
        entity->direction = pacman->buffered_direction; // Apply the buffered direction
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
        entity->position = next_position; // Move Pac-Man in the current direction
    }
    else
    {
        LOG_DEBUG("Pac-Man's move blocked by a wall or gate.");
    }

    // Check for collisions with pellets or power pellets
    handle_pacman_collisions(entity, game_state);
}

void handle_pacman_collisions(struct Entity *entity, struct GameState *game_state)
{
    int tile_x = (int)(entity->position.x + 0.5f);
    int tile_y = (int)(entity->position.y + 0.5f);

    if (tile_x >= 0 && tile_x < game_state->map.width && tile_y >= 0 && tile_y < game_state->map.height)
    {
        Tile *current_tile = &game_state->map.tiles[tile_y][tile_x];

        if (current_tile->type == TILE_PELLET)
        {
            game_state->score += PELLET_SCORE; // Increase score for pellet
            current_tile->type = TILE_EMPTY;   // Remove the pellet from the map
        }
        else if (current_tile->type == TILE_POWER_PELLET)
        {
            game_state->score += POWER_PELLET_SCORE; // Increase score for power pellet
            current_tile->type = TILE_EMPTY;         // Remove the power pellet
            activate_frightened_mode(game_state);    // Activate frightened mode for ghosts
        }
    }
}

void activate_frightened_mode(GameState *game_state)
{
    game_state->frightened_timer = FRIGHTENED_MODE_DURATION;

    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        Ghost *ghost = &game_state->ghosts[i].specific.ghost;

        if (ghost->mode != GHOST_MODE_EATEN && ghost->mode != GHOST_MODE_EXITING)
        {
            ghost->mode = GHOST_MODE_FRIGHTENED;
            ghost->frightened_timer = FRIGHTENED_MODE_DURATION;
        }
    }

    LOG_INFO("Frightened mode activated for all ghosts.");
}
