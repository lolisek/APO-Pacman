/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  constants.c      - Constants and utility functions

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/utils/constants.h"
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>

void get_resource_path(char *out, size_t out_size, const char *filename)
{
    const char *user = USER;
    if (!user)
    {
        struct passwd *pw = getpwuid(getuid());
        user = pw ? pw->pw_name : "unknown";
    }
    snprintf(out, out_size, "/tmp/%s/resources/%s", user, filename);
}