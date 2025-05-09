#ifndef CUSTOM_KEYBOARD_H
#define CUSTOM_KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "ppm_loader.h"
#include "mzapo_peri.h"
#include "font_types.h"
#include "draw_text.h"
#include "display_scoreboard.h"

void draw_keyboard(uint16_t *fb);
void handle_keyboard_input(uint16_t *fb, const font_descriptor_t *font);
int handle_red_knob(void);
int handle_green_knob(void);
void highlight_key(uint16_t *fb, int key_index);

#endif