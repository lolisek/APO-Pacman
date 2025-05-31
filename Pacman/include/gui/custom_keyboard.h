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

/**
 * @brief Draws the keyboard background on the framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 */
void draw_keyboard(uint16_t *fb);

/**
 * @brief Handles keyboard input and returns the corresponding action.
 *
 * @param fb Pointer to the framebuffer.
 * @param font Pointer to the font descriptor.
 * @return char* Description of the action to be taken.
 */
char *handle_keyboard_input(uint16_t *fb, const font_descriptor_t *font);

/**
 * @brief Handles the red knob input and returns the corresponding action.
 *
 * @return int Action corresponding to the red knob input.
 */
int handle_red_knob(void);

/**
 * @brief Handles the green knob input and returns the corresponding action.
 *
 * @return int Action corresponding to the green knob input.
 */
int handle_green_knob(void);

/**
 * @brief Highlights the selected key on the keyboard.
 *
 * @param fb Pointer to the framebuffer.
 * @param key_index Index of the key to highlight.
 */
void highlight_key(uint16_t *fb, int key_index);

/**
 * @brief Updates the keyboard display with the current state.
 *
 * @param fb Pointer to the framebuffer.
 * @param position Current key position.
 * @param name Current name being entered.
 * @param font Pointer to the font descriptor.
 */
void update_display(uint16_t *fb, int position, const char *name, const font_descriptor_t *font);

#endif