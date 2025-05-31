/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  main.c      - Main entry point for the game

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
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
#include "../include/gui/main_menu.h"

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
