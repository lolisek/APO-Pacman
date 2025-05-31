/**
 * @file vector2d.h
 * @brief Provides a 2D vector structure and utility functions for vector operations.
 */

#ifndef APO_PACMAN_VECTOR2D_H
#define APO_PACMAN_VECTOR2D_H

#include <stdint.h>

/**
 * @brief Represents a 2D vector with floating-point coordinates.
 */
typedef struct
{
    float x; /**< X-coordinate of the vector. */
    float y; /**< Y-coordinate of the vector. */
} Vector2D;

/**
 * @brief Adds two 2D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return Vector2D The result of adding the two vectors.
 */
Vector2D vector2d_add(Vector2D a, Vector2D b);

/**
 * @brief Subtracts one 2D vector from another.
 *
 * @param a The vector to subtract from.
 * @param b The vector to subtract.
 * @return Vector2D The result of the subtraction.
 */
Vector2D vector2d_sub(Vector2D a, Vector2D b);

/**
 * @brief Scales a 2D vector by a scalar value.
 *
 * @param v The vector to scale.
 * @param scalar The scalar value to multiply the vector by.
 * @return Vector2D The scaled vector.
 */
Vector2D vector2d_scale(Vector2D v, uint16_t scalar);

/**
 * @brief Calculates the length of a 2D vector.
 *
 * @param v The vector whose length is to be calculated.
 * @return int The length of the vector.
 */
int vector2d_length(Vector2D v);

#endif // APO_PACMAN_VECTOR2D_H