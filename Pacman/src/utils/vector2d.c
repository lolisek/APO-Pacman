/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  vector2d.c      - Vector2D utility functions

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/utils/vector2d.h"
#include <math.h>
#include <stdint.h>

Vector2D vector2d_add(Vector2D a, Vector2D b)
{
    return (Vector2D){a.x + b.x, a.y + b.y};
}

Vector2D vector2d_sub(Vector2D a, Vector2D b)
{
    return (Vector2D){a.x - b.x, a.y - b.y};
}

Vector2D vector2d_scale(Vector2D v, uint16_t scalar)
{
    return (Vector2D){v.x * scalar, v.y * scalar};
}

int vector2d_length(Vector2D v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}