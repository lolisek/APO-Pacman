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
#include "../include/gui/mzapo_peri.h"
#include "../include/gui/main_menu.h"
#include "../include/gui/display_scoreboard.h"
#include "../include/gui/custom_keyboard.h"

#include "../include/microzed/mzapo_parlcd.h"
#include "../include/microzed/mzapo_phys.h"
#include "../include/microzed/mzapo_regs.h"
#include "../include/microzed/serialize_lock.h"

#include "../include/core/game.h"

#define _BSD_SOURCE // for usleep

#define BACKGROUND_COLOR 0x0000  // Black
#define TEXT_COLOR 0xFFFF        // White
#define HIGHLIGHT_COLOR 0xF800   // Red

// // testing for game loop
// int main(int argc, char *argv[])
// {

//   // Run the game loop
//   run_game_loop();

//   return 0;
// }

int main(int argc, char *argv[]) {
  if (serialize_lock(1) <= 0) {
      printf("System is occupied\n");
      if (1) {
          printf("Waiting\n");
          serialize_lock(0);
      }
  }

  mzapo_setup();

  ppm_image_t *menu_bgr = load_ppm("/tmp/veru/resources/menu.ppm");
  if (!menu_bgr) {
      fprintf(stderr, "Failed to load menu image\n");
      return -1;
  }

  menu_state_t menu;
  init_menu(&menu);
  draw_menu(&menu);
  lcd_update(menu.framebuffer);

  scoreboard_t sb;
  init_scoreboard(&sb);
  load_scores(&sb);

  while (1) {
    int action = handle_menu_input(&menu);

    if (action == 1 || menu.selected != menu.last_selected) {
        draw_menu(&menu);
        lcd_update(menu.framebuffer);
        menu.last_selected = menu.selected;

        usleep(300000);

    } else if (action >= 2) {
        // Menu item selected
        menu.selected = action - 2;
        printf("Selected: %s\n", menu_items[action - 2]);

        // Handle selection
        if (menu.selected == 0) {
            // Start game
            printf("Starting game...\n");
            //run_game_loop();
            handle_keyboard_input(menu.framebuffer, &font_winFreeSystem14x16);

        } else if (menu.selected == 1) {
            // Show scoreboard
            printf("Showing scoreboard...\n");
            draw_scoreboard(&sb, menu.framebuffer, &font_winFreeSystem14x16);
            lcd_update(menu.framebuffer);

            while (1) {
                int sb_action = handle_scoreboard_input(&sb);

                if (sb_action == 3) { // Exit scoreboard
                    printf("Exiting scoreboard...\n");
                    break;
                }

                if (sb_action == 1 || sb_action == 2) {
                    scroll_scoreboard(&sb, sb_action == 1 ? -1 : 1);
                    draw_scoreboard(&sb, menu.framebuffer, &font_winFreeSystem14x16);
                    lcd_update(menu.framebuffer);

                    // Overscroll prevention
                    usleep(100000);
                }

                usleep(10000);
            }

            // Wait for button release
            while (blue_knob_is_pressed()) usleep(10000);

            draw_menu(&menu);
            lcd_update(menu.framebuffer);
            menu.selected = 1;

        } else if (menu.selected == 2) {
            // Exit menu
            printf("Exiting...\n");
            break;
        }
    }

    usleep(INPUT_POLL_DELAY_US);
  }

  free_ppm(menu_bgr);
  serialize_unlock();
  return 0;
}
