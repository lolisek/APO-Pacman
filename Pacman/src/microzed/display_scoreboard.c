#include "display_scoreboard.h"

void init_scoreboard(scoreboard_t *sb) {
    sb->total_lines = 0;
    sb->scroll_offset = 0;
    memset(sb->lines, 0, sizeof(sb->lines));
}

int load_scores(scoreboard_t *sb) {
    FILE *file = fopen(SCORES_FILE, "r");
    if (!file) {
        perror("Failed to open scores file");
        return -1;
    }

    sb->total_lines = 0;
    while (fgets(sb->lines[sb->total_lines++], SCORE_LINE_LENGTH, file) && sb->total_lines < MAX_SCORES) {
        continue;
    }

    fclose(file);
    return 0;
}

int save_score(const char *name, int score) {
    FILE *file = fopen(SCORES_FILE, "a");
    if (!file) {
        perror("Failed to open scores file");
        return -1;
    }

    int scores_count = 1;
    while (fgets(NULL, 0, file) != NULL) {
        scores_count++;
    }

    fprintf(file, "%d. %s: %d\n", scores_count, name, score);
    fclose(file);
    return 0;
}

void draw_scoreboard(scoreboard_t *sb, uint16_t *fb, font_descriptor_t *font) {
    ppm_image_t *bgr = load_ppm("/tmp/veru/resources/scoreboardbgr.ppm");
    if (!bgr) {
        fprintf(stderr, "Failed to load scoreboard background image\n");
        return;
    }

    memcpy(fb, bgr->pixels, LCD_SIZE * sizeof(uint16_t));

    int x = 40;
    int y = 31;

    lcd_update(fb);
}

void scroll_scoreboard(scoreboard_t *sb, int direction) {
    return;
}

int handle_scoreboard_input(scoreboard_t *sb, int direction) {
    uint8_t current_knob = get_red_knob_rotation();
    int8_t delta = 0;

    if (delta > 127) delta -= 256;
    if (delta < -127) delta += 256;

    if (delta > 1) {
        return 1;
    } else if (delta < -1) {
        return 2;
    }
    
    return 0;
}
