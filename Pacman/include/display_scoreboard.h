#ifndef DISPLAY_SCOREBOARD_H
#define DISPLAY_SCOREBOARD_H

#include <stdio.h>
#include <string.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "ppm_loader.h"
#include "font_types.h"
#include "mzapo_peri.h"
#include "draw_text.h"

#define SCORES_FILE "/tmp/veru/resources/scores.txt"
#define MAX_SCORES 20
#define SCORE_LINE_LENGTH 50

typedef struct {
    char lines[MAX_SCORES][SCORE_LINE_LENGTH];
    int total_lines;
    int scroll_offset;
} scoreboard_t;

void init_scoreboard(scoreboard_t *sb);
int load_scores(scoreboard_t *sb);
int save_score(const char *name, int score);
void draw_scoreboard(scoreboard_t *sb, uint16_t *fb, const font_descriptor_t *font);
void scroll_scoreboard(scoreboard_t *sb, int direction);
int handle_scoreboard_input(scoreboard_t *sb);

#endif