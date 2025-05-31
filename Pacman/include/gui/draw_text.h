#ifndef DRAW_TEXT_H
#define DRAW_TEXT_H

#include <stdint.h>
#include <stdio.h>

#include "ppm_loader.h"
#include "../microzed/mzapo_peri.h"
#include "../microzed/mzapo_regs.h"
#include "../microzed/mzapo_phys.h"
#include "../fonts/font_types.h"

/**
 * @brief Draws a single pixel on the framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param color Color of the pixel.
 */
void draw_pixel(uint16_t *fb, int x, int y, uint16_t color);

/**
 * @brief Draws a single character on the framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 * @param x X-coordinate of the top-left corner of the character.
 * @param y Y-coordinate of the top-left corner of the character.
 * @param ch Character to draw.
 * @param color Color of the character.
 * @param font Pointer to the font descriptor.
 */
void draw_char(uint16_t *fb, int x, int y, char ch, uint16_t color, const font_descriptor_t *font);

/**
 * @brief Draws a string on the framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 * @param x X-coordinate of the starting position of the string.
 * @param y Y-coordinate of the starting position of the string.
 * @param str Pointer to the null-terminated string to draw.
 * @param color Color of the string.
 * @param font Pointer to the font descriptor.
 */
void draw_string(uint16_t *fb, int x, int y, const char *str, uint16_t color, const font_descriptor_t *font);

#endif // DRAW_TEXT_H
