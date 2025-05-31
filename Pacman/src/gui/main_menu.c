/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  main_menu.c      - Scoreboard display and management

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/gui/main_menu.h"
#include "../../include/gui/display_scoreboard.h"
#include "../../include/core/game.h"
#include "../../include/utils/timer.h"
#include "../../include/microzed/mzapo_parlcd.h"
#include "../../include/utils/logger.h"
#include <stdio.h>

int handle_knob_rotation(int last_pos);

void init_menu(menu_state_t *menu)
{
    menu->selected = 0;
    menu->last_selected = 255;
    menu->last_knob_pos = get_red_knob_rotation();
    memset(menu->framebuffer, 0, sizeof(menu->framebuffer));
}

void draw_menu(menu_state_t *menu)
{
    char path[256];
    get_resource_path(path, sizeof(path), "menu.ppm");
    ppm_image_t *menu_bgr = load_ppm(path);
    if (!menu_bgr)
    {
        fprintf(stderr, "Failed to load menu image\n");
        return;
    }
    memcpy(menu->framebuffer, menu_bgr->pixels, LCD_SIZE * sizeof(uint16_t));

    render_arrows(menu->selected, menu->framebuffer);

    lcd_update(menu->framebuffer);
}

int handle_menu_input(menu_state_t *menu)
{
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    int change = handle_knob_rotation(menu->last_knob_pos);

    if (change != 0)
    {
        int new_position = menu->selected + change;
        new_position = (new_position % MENU_ITEMS + MENU_ITEMS) % MENU_ITEMS;

        menu->selected = new_position;
        menu->last_knob_pos = get_red_knob_rotation();
        return 1;
    }

    if (red_knob_is_pressed())
    {
        if (current_time - last_press_time > DEBOUNCE_DELAY_MS)
        {
            last_press_time = current_time;
            return menu->selected + 2;
        }
    }

    return 0;
}

int handle_knob_rotation(int last_pos)
{
    uint8_t current_knob = get_red_knob_rotation();
    int delta = (int)(current_knob - last_pos);

    if (delta > 127)
        delta -= 256;
    if (delta < -127)
        delta += 256;

    if (abs(delta) > 3)
    {
        int clicks = delta / (KNOB_POSITIONS / KNOB_CLICKS_PER_TURN);

        if (abs(clicks) == 0)
        {
            return (delta > 0) ? 1 : -1;
        }
        return clicks;
    }

    return 0;
}

void render_arrows(int selected, uint16_t *fb)
{
    char path_left[256], path_right[256];
    get_resource_path(path_left, sizeof(path_left), "arrow_left.ppm");
    get_resource_path(path_right, sizeof(path_right), "arrow_right.ppm");
    ppm_image_t *arrow_left = load_ppm(path_left);
    ppm_image_t *arrow_right = load_ppm(path_right);
    if (!arrow_left || !arrow_right)
    {
        fprintf(stderr, "Failed to load arrow images\n");
        return;
    }

    switch (selected)
    {
    case 0:
        draw_ppm_image(fb, 174, 101, arrow_right);
        draw_ppm_image(fb, 292, 101, arrow_left);
        break;
    case 1:
        draw_ppm_image(fb, 72, 147, arrow_right);
        draw_ppm_image(fb, 404, 147, arrow_left);
        break;
    case 2:
        draw_ppm_image(fb, 177, 194, arrow_right);
        draw_ppm_image(fb, 294, 194, arrow_left);
        break;
    default:
        fprintf(stderr, "Invalid menu selection\n");
        break;
    }
}

void run_main_menu()
{
    menu_state_t menu;
    init_menu(&menu);
    draw_menu(&menu);
    lcd_update(menu.framebuffer);

    scoreboard_t sb;
    init_scoreboard(&sb);
    load_scores(&sb);

    timer_init_global();

    while (1)
    {
        int action = handle_menu_input(&menu);

        if (action == 1 || menu.selected != menu.last_selected)
        {
            draw_menu(&menu);
            lcd_update(menu.framebuffer);
            menu.last_selected = menu.selected;
            timer_sleep_ms(300);
        }
        else if (action >= 2)
        {
            menu.selected = action - 2;

            if (menu.selected == 0)
            {
                LOG_INFO("Starting game...");
                run_game_loop(menu.framebuffer);
                draw_menu(&menu);
                lcd_update(menu.framebuffer);
            }
            else if (menu.selected == 1)
            {
                LOG_INFO("Showing scoreboard...");
                load_scores(&sb);
                handle_scoreboard(&sb, menu.framebuffer);
                draw_menu(&menu);
                lcd_update(menu.framebuffer);
            }
            else if (menu.selected == 2)
            {
                LOG_INFO("Exiting...");
                break;
            }
        }

        timer_sleep_ms(INPUT_POLL_DELAY_US / 1000);
    }
}