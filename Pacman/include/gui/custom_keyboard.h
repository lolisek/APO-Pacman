#ifndef CUSTOM_KEYBOARD_H
#define CUSTOM_KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#include "ppm_loader.h"
#include "../microzed/mzapo_peri.h"
#include "../fonts/font_types.h"
#include "draw_text.h"
#include "display_scoreboard.h"

void draw_keyboard(uint16_t *fb);
char *handle_keyboard_input(uint16_t *fb, const font_descriptor_t *font);
int handle_red_knob(void);
int handle_green_knob(void);
void highlight_key(uint16_t *fb, int key_index);

#endif