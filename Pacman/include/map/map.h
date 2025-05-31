#ifndef APO_PACMAN_MAP_H
#define APO_PACMAN_MAP_H

#include "../map/tile.h"
#include "../utils/constants.h"

// Map structure
typedef struct
{
    int width;                            // Width of the map in tiles
    int height;                           // Height of the map in tiles
    Tile tiles[NUM_TILES_Y][NUM_TILES_X]; // 2D array of tiles
} Map;

/**
 * @brief Initializes the map with default values.
 *
 * @param map Pointer to the Map structure.
 */
void map_init(Map *map);

/**
 * @brief Loads a map from a file.
 *
 * @param map Pointer to the Map structure.
 * @param filename Path to the map file.
 * @return true if the map was loaded successfully, false otherwise.
 */
bool map_load_from_file(Map *map, const char *filename);

/**
 * @brief Checks if a tile at the given coordinates is walkable.
 *
 * @param map Pointer to the Map structure.
 * @param x X-coordinate of the tile.
 * @param y Y-coordinate of the tile.
 * @param entity_type Type of the entity attempting to walk.
 * @return true if the tile is walkable, false otherwise.
 */
bool map_is_walkable(const Map *map, int x, int y,
                     EntityType entity_type); // Check if a tile is walkable for a specific entity type

#endif // APO_PACMAN_MAP_H
