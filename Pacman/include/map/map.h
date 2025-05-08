#ifndef APO_PACMAN_MAP_H
#define APO_PACMAN_MAP_H

#include "../map/tile.h"
#include "../utils/constants.h"

// Map structure
typedef struct
{
    Tile tiles[NUM_TILES_Y][NUM_TILES_X]; // 2D array of tiles
} Map;

// Function prototypes
void map_init(Map *map);
bool map_load_from_file(Map *map, const char *filename);
void map_render(const Map *map);
bool map_is_walkable(const Map *map, int x, int y);

#endif // APO_PACMAN_MAP_H
