/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.

  Remove next text: This line should not appear in submitted
  work and project name should be change to match real application.
  If this text is there I want 10 points subtracted from final
  evaluation.

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "../include/gui/ppm_loader.h"
#include "../include/gui/main_menu.h"
#include "../include/gui/display_scoreboard.h"
#include "../include/gui/custom_keyboard.h"

#include "../include/microzed/mzapo_parlcd.h"
#include "../include/microzed/mzapo_peri.h"
#include "../include/microzed/mzapo_phys.h"
#include "../include/microzed/mzapo_regs.h"
#include "../include/microzed/serialize_lock.h"

#include "../include/core/game.h"
#include "../include/utils/timer.h"

#define _BSD_SOURCE // for usleep

#define BACKGROUND_COLOR 0x0000  // Black
#define TEXT_COLOR 0xFFFF        // White

int main(int argc, char *argv[]) {
    if (serialize_lock(1) <= 0)
    {
        printf("System is occupied\n");
        printf("Waiting\n");
        serialize_lock(0);
    }

    mzapo_setup();

    menu_state_t menu;
    init_menu(&menu);
    draw_menu(&menu);
    lcd_update(menu.framebuffer);

    scoreboard_t sb;
    init_scoreboard(&sb);
    load_scores(&sb);

    // Initialize the global timer
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
            // Menu item selected
            menu.selected = action - 2;
            printf("Selected: %s\n", menu_items[menu.selected]);

            if (menu.selected == 0)
            {
                // Start game
                printf("Starting game...\n");
                run_game_loop(menu.framebuffer);
            }
            else if (menu.selected == 1)
            {
                // Show scoreboard
                printf("Showing scoreboard...\n");
                draw_scoreboard(&sb, menu.framebuffer, &font_winFreeSystem14x16);
                lcd_update(menu.framebuffer);

                while (1)
                {
                    int sb_action = handle_scoreboard_input(&sb);

                    if (sb_action == 3)
                    { // Exit scoreboard
                        printf("Exiting scoreboard...\n");
                        break;
                    }

                    if (sb_action == 1 || sb_action == 2)
                    {
                        scroll_scoreboard(&sb, sb_action == 1 ? -1 : 1);
                        draw_scoreboard(&sb, menu.framebuffer, &font_winFreeSystem14x16);
                        lcd_update(menu.framebuffer);
                        timer_sleep_ms(100);
                    }

                    timer_sleep_ms(10);
                }

                // Wait for button release
                while (blue_knob_is_pressed())
                    timer_sleep_ms(10);

                draw_menu(&menu);
                lcd_update(menu.framebuffer);
                menu.selected = 1;
            }
            else if (menu.selected == 2)
            {
                // Exit menu
                printf("Exiting...\n");
                break;
            }
        }

        timer_sleep_ms(INPUT_POLL_DELAY_US / 1000);
    }

    serialize_unlock();
    return 0;
}
