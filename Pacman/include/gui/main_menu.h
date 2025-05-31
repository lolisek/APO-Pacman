#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../gui/ppm_loader.h"
#include "../microzed/mzapo_peri.h"
#include "../microzed/mzapo_regs.h"
#include "../microzed/mzapo_phys.h"
#include "../microzed/mzapo_parlcd.h"
#include "../utils/constants.h"
#include "../gui/display_scoreboard.h"

#define ARROW_SIZE 24
#define ARROW_SHAFT_WIDTH 6
#define ARROW_HEAD_LENGTH 10
#define ARROW_COLOR 0xF7A0 // Yellow

#define DEBOUNCE_DELAY_MS 50
#define INPUT_POLL_DELAY_US 10000  // 10ms
#define SCROLL_DELAY INPUT_POLL_DELAY_US * 100

#define MENU_ITEMS 3
extern const char *menu_items[MENU_ITEMS];

typedef struct {
    int selected;
    uint8_t last_knob_pos;
    uint8_t last_selected;
    uint8_t knob_accumulator;  // For tracking partial clicks
    uint16_t framebuffer[LCD_SIZE];
} menu_state_t;

/**
 * @brief Handles knob rotation and calculates the change in position.
 *
 * @param last_pos Last knob position.
 * @return int Change in position (positive for clockwise, negative for counterclockwise).
 */
int handle_knob_rotation(int last_pos);

/**
 * @brief Initializes the main menu state.
 *
 * @param menu Pointer to the menu state structure.
 */
void init_menu(menu_state_t *menu);

/**
 * @brief Draws the main menu on the framebuffer.
 *
 * @param menu Pointer to the menu state structure.
 */
void draw_menu(menu_state_t *menu);

/**
 * @brief Handles user input for the main menu.
 *
 * @param menu Pointer to the menu state structure.
 * @return int Action code (1 for redraw, 2+ for menu selection).
 */
int handle_menu_input(menu_state_t *menu);

/**
 * @brief Renders arrows on the menu to indicate the selected item.
 *
 * @param selected Index of the selected menu item.
 * @param fb Pointer to the framebuffer.
 */
void render_arrows(int selected, uint16_t *fb);

/**
 * @brief Runs the main menu loop, handling user input and actions.
 */
void run_main_menu();

#endif // MAIN_MENU_H