#include "../../include/core/game_state.h"
#include "../../include/core/renderer.h"
#include "../../include/core/input.h"
#include <stdio.h>

// Function prototypes
void map_init(Map *map)
{
    // Initialize the map dimensions
    map->width = NUM_TILES_X;
    map->height = NUM_TILES_Y;

    // Initialize the tiles in the map
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            map->tiles[y][x].type = TILE_EMPTY; // Set all tiles to empty by default
        }
    }
}
bool map_load_from_file(Map *map, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening map file: %s\n", filename);
        return false;
    }

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            char tile_char;
            fscanf(file, " %c", &tile_char); // Read a character from the file

            switch (tile_char)
            {
            case '.':
                map->tiles[y][x].type = TILE_EMPTY;
                break;
            case '#':
                map->tiles[y][x].type = TILE_WALL;
                break;
            case '*':
                map->tiles[y][x].type = TILE_PELLET;
                break;
            case '@':
                map->tiles[y][x].type = TILE_POWER_PELLET;
                break;
            case '=':
                map->tiles[y][x].type = TILE_OUT_OF_BOUNDS; // Used for ghost gates
                break;
            default:
                fprintf(stderr, "Unknown tile type: %c\n", tile_char);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return true;
}
void map_render(const Map *map)
{
    // Render the map to the console or screen
    // TODO implement rendering logic for screen
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            switch (map->tiles[y][x].type)
            {
            case TILE_EMPTY:
                printf(" "); // Empty tile
                break;
            case TILE_WALL:
                printf("#"); // Wall tile
                break;
            case TILE_PELLET:
                printf("*"); // Pellet tile
                break;
            case TILE_POWER_PELLET:
                printf("o"); // Power pellet tile
                break;
            default:
                printf("?"); // Unknown tile type
                break;
            }
        }
        printf("\n");
    }
};
bool map_is_walkable(const Map *map, int x, int y, EntityType entity_type)
{
    // Check if the tile at (x, y) is walkable
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
    {
        return false; // Out of bounds
    }

    TileType tile_type = map->tiles[y][x].type;
    if (tile_type == TILE_WALL)
    {
        return false; // Walls are not walkable
    }

    return true; // All other tiles are walkable
}
