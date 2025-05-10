#ifndef DRAW_TEXT_H
#define DRAW_TEXT_H

#include <stdint.h>
#include <stdio.h>
#include "font_types.h"
#include "ppm_loader.h"
#include "mzapo_peri.h"
#include "mzapo_regs.h"
#include "mzapo_phys.h"

void draw_pixel(uint16_t *fb, int x, int y, uint16_t color);
void draw_char(uint16_t *fb, int x, int y, char ch, uint16_t color, const font_descriptor_t *font);
void draw_string(uint16_t *fb, int x, int y, const char *str, uint16_t color, const font_descriptor_t *font);

#endif // DRAW_TEXT_H
