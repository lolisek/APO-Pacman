#ifndef DISPLAY_SCOREBOARD_H
#define DISPLAY_SCOREBOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font_types.h"
#include "ppm_loader.h"
#include "mzapo_peri.h"

#define MAX_SCORES 100
#define SCORE_LINE_LENGTH 64
#define SCORES_FILE "/tmp/veru/scores.txt"

typedef struct {
    char lines[MAX_SCORES][SCORE_LINE_LENGTH];
    int total_lines;
    int scroll_offset;
} scoreboard_t;

void init_scoreboard(scoreboard_t *sb);
int load_scores(scoreboard_t *sb);
int save_score(const char *name, int score);
void draw_scoreboard(scoreboard_t *sb, uint16_t *fb, font_descriptor_t *font);
void scroll_scoreboard(scoreboard_t *sb, int direction);
int handle_scoreboard_input(scoreboard_t *sb, int direction);

#endif