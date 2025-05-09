#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ppm_loader.h"
#include "mzapo_peri.h"
#include "mzapo_regs.h"
#include "mzapo_phys.h"
#include "mzapo_parlcd.h"

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
    uint16_t framebuffer[LCD_SIZE];
} menu_state_t;

void init_menu(menu_state_t *menu);
void draw_menu(menu_state_t *menu);
int handle_menu_input(menu_state_t *menu);
void render_arrows(int selected, uint16_t *fb);


#endif // MAIN_MENU_H