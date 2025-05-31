#ifndef APO_PACMAN_TILE_H
#define APO_PACMAN_TILE_H

#include <stdbool.h>

/**
 * @brief Enum representing the types of tiles in the game map.
 */
typedef enum
{
    TILE_EMPTY = 0,     /**< An empty tile. */
    TILE_WALL,          /**< A wall tile. */
    TILE_PELLET,        /**< A pellet tile. */
    TILE_POWER_PELLET,  /**< A power pellet tile. */
    TILE_OUT_OF_BOUNDS, /**< A tile used for ghost gates or out-of-bounds areas. */
} TileType;

/**
 * @brief Structure representing a tile in the game map.
 */
typedef struct
{
    TileType type; /**< The type of the tile (e.g., wall, pellet, etc.). */
} Tile;

#endif // APO_PACMAN_TILE_H