#include "../../include/utils/vector2d.h"
#include <math.h>

Vector2D vector2d_add(Vector2D a, Vector2D b)
{
    return (Vector2D){a.x + b.x, a.y + b.y};
}

Vector2D vector2d_sub(Vector2D a, Vector2D b)
{
    return (Vector2D){a.x - b.x, a.y - b.y};
}

Vector2D vector2d_scale(Vector2D v, int scalar)
{
    return (Vector2D){v.x * scalar, v.y * scalar};
}

int vector2d_length(Vector2D v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}