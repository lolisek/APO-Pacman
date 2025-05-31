/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  map.c      - Map management and loading

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/


#include "../../include/core/game_state.h"
#include "../../include/core/renderer.h"
#include "../../include/core/input.h"
#include "../../include/utils/logger.h"
#include <stdio.h>

void map_init(Map *map)
{
    map->width = NUM_TILES_X;
    map->height = NUM_TILES_Y;

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            map->tiles[y][x].type = TILE_EMPTY;
        }
    }
}

bool map_load_from_file(Map *map, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        LOG_ERROR("Error opening map file: %s", filename);
        return false;
    }

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            char tile_char;
            if (fscanf(file, " %c", &tile_char) != 1)
            {
                LOG_ERROR("Error reading tile at (%d, %d) in file: %s", x, y, filename);
                fclose(file);
                return false;
            }

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
                LOG_ERROR("Unknown tile type '%c' at (%d, %d) in file: %s", tile_char, x, y, filename);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    LOG_INFO("Map loaded successfully from file: %s", filename);
    return true;
}

bool map_is_walkable(const Map *map, int x, int y, EntityType entity_type)
{
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
