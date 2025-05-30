#ifndef APO_PACMAN_TILE_H
#define APO_PACMAN_TILE_H

#include <stdbool.h>

// Tile types (wall, pellet, empty, etc.)
typedef enum
{
    TILE_EMPTY = 0,
    TILE_WALL,
    TILE_PELLET,
    TILE_POWER_PELLET,
    TILE_OUT_OF_BOUNDS, // Used for ghost gates
} TileType;

// Tile structure
typedef struct
{
    TileType type; // Type of tile (wall, pellet, etc.)
} Tile;

#endif // APO_PACMAN_TILE_H