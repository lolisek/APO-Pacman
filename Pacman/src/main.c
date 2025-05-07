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

#include "ppm_loader.h"

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "serialize_lock.h"
#include "mzapo_peri.h"

#define LCD_WIDTH  320
#define LCD_HEIGHT 480
#define LCD_SIZE   (LCD_WIDTH * LCD_HEIGHT)

#define MENU_ITEMS 3
const char *menu_items[MENU_ITEMS] = {
    "Play",
    "Show scoreboard",
    "Exit"
};

int main(int argc, char *argv[])
{

  /* Serialize execution of applications */ 

  /* Try to acquire lock the first */
  if (serialize_lock(1) <= 0) {
    printf("System is occupied\n");

    if (1) {
      printf("Waiting\n");
      /* Wait till application holding lock releases it or exits */
      serialize_lock(0);
    }
  }
  
  ppm_image_t *menu_bgr = load_ppm("/tmp/veru/resources/menu.ppm");
  if (!menu_bgr) {
    fprintf(stderr, "Failed to load menu image\n");
    return -1;
  }

  lcd_update((uint16_t*)menu_bgr->pixels);
  sleep(20);

  free_ppm(menu_bgr);





  printf("Hello world\n");

  sleep(4);

  printf("Goodbye world\n");

  /* Release the lock */
  serialize_unlock();

  return 0;
}
