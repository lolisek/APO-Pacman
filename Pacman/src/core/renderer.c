#include "../../include/core/renderer.h"
#include "../../include/core/game_state.h"
#include "../../include/core/input.h"
#include "../../include/map/map.h"
#include <stdio.h>

void render(GameState *game_state)
{

    // render in console for testing

    for (int y = 0; y < game_state->map.height; y++)
    {
        for (int x = 0; x < game_state->map.width; x++)
        {
            // Render Pacman if present (round the position to the nearest tile)
            if (game_state->pacman.base.position.x == x && game_state->pacman.base.position.y == y)
            {
                printf("P"); // Render Pac-Man
                continue;
            }

            switch (game_state->map.tiles[y][x].type)
            {
            case TILE_EMPTY:
                printf(" ");
                break;
            case TILE_WALL:
                printf("#");
                break;
            case TILE_PELLET:
                printf(" ");
                break;
            case TILE_POWER_PELLET:
                printf(" ");
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}

void render_entity(const Entity *entity);

void render_map(const Map *map) {}