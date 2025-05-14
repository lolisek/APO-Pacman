#include "../../include/core/renderer.h"
#include "../../include/core/game_state.h"
#include "../../include/core/input.h"
#include "../../include/map/map.h"
#include <stdio.h>

void render(GameState *game_state)
{

    // render in console for testing
    //while (condition)
    //{
        // frame buffer
        // lcd update
    //}
    
    
    printf("Score: %d\n", game_state->score);

    for (int y = 0; y < game_state->map.height; y++)
    {
        for (int x = 0; x < game_state->map.width; x++)
        {
            // Render Pacman if present (round the position to the nearest tile)
            if (game_state->pacman.position.x == x && game_state->pacman.position.y == y)
            {
                printf("P"); // Render Pac-Man
                continue;
            }

            // Render Ghosts if present
            bool ghost_rendered = false;
            for (int i = 0; i < NUM_GHOSTS; i++)
            {
                if (game_state->ghosts[i].position.x == x && game_state->ghosts[i].position.y == y)
                {
                    printf("G"); // Render Ghost
                    ghost_rendered = true;
                    break;
                }
            }
            if (ghost_rendered)
            {
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
                printf(".");
                break;
            case TILE_POWER_PELLET:
                printf(".");
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