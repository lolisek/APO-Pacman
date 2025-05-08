#ifndef APO_PACMAN_VECTOR2D_H
#define APO_PACMAN_VECTOR2D_H

typedef struct
{
    int x;
    int y;
} Vector2D;

// Utility functions
Vector2D vector2d_add(Vector2D a, Vector2D b);
Vector2D vector2d_sub(Vector2D a, Vector2D b);
Vector2D vector2d_scale(Vector2D v, int scalar);
int vector2d_length(Vector2D v);

#endif // APO_PACMAN_VECTOR2D_H