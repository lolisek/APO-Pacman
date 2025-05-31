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
#include "../include/gui/display_scoreboard.h"
#include "../include/utils/timer.h"
#include "../include/microzed/serialize_lock.h"

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

    run_main_menu();

    serialize_unlock();
    return 0;
}
