/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  display_scoreboard.c  - Scoreboard display and management

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/gui/display_scoreboard.h"
#include "../../include/utils/constants.h"
#include "../../include/gui/ppm_loader.h"
#include "../../include/gui/draw_text.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // For qsort
#include "../../include/utils/logger.h" // Include logger for logging macros

void init_scoreboard(scoreboard_t *sb)
{
    sb->total_lines = 0;
    sb->scroll_offset = 0;
    memset(sb->lines, 0, sizeof(sb->lines));
    LOG_INFO("Initialized scoreboard");
}

int load_scores(scoreboard_t *sb)
{
    char scores_path[256];
    get_resource_path(scores_path, sizeof(scores_path), "scores.txt");
    LOG_DEBUG("Loading scores from path: %s", scores_path);

    FILE *file = fopen(scores_path, "r");
    if (!file)
    {
        LOG_ERROR("Failed to open scores file for reading");
        return 0;
    }

    sb->total_lines = 0;
    memset(sb->lines, 0, sizeof(sb->lines));

    LOG_DEBUG("Contents of scores file during loading:");
    char line[SCORE_LINE_LENGTH];
    while (fgets(line, SCORE_LINE_LENGTH, file))
    {
        LOG_DEBUG("%s", line);
    }
    rewind(file); // Reset file pointer to the beginning

    while (sb->total_lines < MAX_SCORES)
    {
        if (!fgets(sb->lines[sb->total_lines], SCORE_LINE_LENGTH, file))
        {
            break; // EOF or error
        }

        char *newline = strchr(sb->lines[sb->total_lines], '\n');
        if (newline)
            *newline = '\0';

        if (strlen(sb->lines[sb->total_lines]) > 0)
        {
            sb->total_lines++;
        }
    }

    LOG_INFO("Loaded %d scores", sb->total_lines);
    fclose(file);
    return 1;
}

int compare_scores(const void *a, const void *b)
{
    const char *line_a = (const char *)a;
    const char *line_b = (const char *)b;

    int score_a = 0, score_b = 0;
    sscanf(strrchr(line_a, ':') + 1, "%d", &score_a);
    sscanf(strrchr(line_b, ':') + 1, "%d", &score_b);

    return score_b - score_a; // Sort in descending order
}

int save_score(const char *name, int score)
{
    char scores_path[256];
    get_resource_path(scores_path, sizeof(scores_path), "scores.txt");
    LOG_DEBUG("Resolved scores path: %s", scores_path);

    scoreboard_t sb;
    init_scoreboard(&sb);
    if (!load_scores(&sb))
    {
        LOG_ERROR("Failed to load scores");
    }
    else
    {
        LOG_INFO("Loaded %d scores", sb.total_lines);
    }

    if (sb.total_lines < MAX_SCORES)
    {
        snprintf(sb.lines[sb.total_lines], SCORE_LINE_LENGTH, "%s: %d", name, score);
        sb.total_lines++;
        LOG_INFO("Added new score: %s: %d", name, score);
    }
    else
    {
        int lowest_score = 0;
        sscanf(strrchr(sb.lines[sb.total_lines - 1], ':') + 1, "%d", &lowest_score);
        LOG_DEBUG("Lowest score in scoreboard: %d", lowest_score);
        if (score > lowest_score)
        {
            snprintf(sb.lines[sb.total_lines - 1], SCORE_LINE_LENGTH, "%s: %d", name, score);
            LOG_INFO("Replaced lowest score with: %s: %d", name, score);
        }
    }

    qsort(sb.lines, sb.total_lines, SCORE_LINE_LENGTH, compare_scores);
    LOG_DEBUG("Sorted scores");

    FILE *file = fopen(scores_path, "w");
    if (!file)
    {
        LOG_ERROR("Failed to open scores file for writing");
        return -1;
    }

    for (int i = 0; i < sb.total_lines; i++)
    {
        fprintf(file, "%s\n", sb.lines[i]);
        LOG_DEBUG("Wrote score to file: %s", sb.lines[i]);
    }

    fclose(file);
    LOG_INFO("Successfully saved scores");

    // Reload scores into the system after saving
    if (!load_scores(&sb))
    {
        LOG_ERROR("Failed to reload scores after saving");
    }
    else
    {
        LOG_INFO("Reloaded %d scores after saving", sb.total_lines);
    }

    return 0;
}

void draw_scoreboard(scoreboard_t *sb, uint16_t *fb, const font_descriptor_t *font)
{
    if (!sb || !fb || !font)
        return;

    char bgr_path[256];
    get_resource_path(bgr_path, sizeof(bgr_path), "scoreboardbgr.ppm");
    ppm_image_t *bgr = load_ppm(bgr_path);
    if (bgr)
    {
        memcpy(fb, bgr->pixels, LCD_SIZE * sizeof(uint16_t));
        free_ppm(bgr);
    }
    else
    {
        memset(fb, 0x0000, LCD_SIZE * sizeof(uint16_t));
    }

    const int start_x = 50;
    const int start_y = 30;
    const int line_height = font->height + 4;
    const int visible_lines = 12;

    int lines_to_show = sb->total_lines > visible_lines ? visible_lines : sb->total_lines;

    for (int i = 0; i < lines_to_show; i++)
    {
        int line_idx = i + sb->scroll_offset;
        if (line_idx >= sb->total_lines)
            break;

        draw_string(fb, start_x, start_y + i * line_height,
                    sb->lines[line_idx], 0xFFFF, font);
    }

    if (sb->total_lines > visible_lines)
    {
        int scrollbar_width = 10;
        int scrollbar_x = LCD_WIDTH - scrollbar_width;

        float visible_ratio = (float)visible_lines / sb->total_lines;
        int scrollbar_height = LCD_HEIGHT * visible_ratio;
        int scrollbar_pos = (sb->scroll_offset * LCD_HEIGHT) / sb->total_lines;

        for (int y = 0; y < LCD_HEIGHT; y++)
        {
            fb[y * LCD_WIDTH + scrollbar_x] = 0x7BEF;
        }

        for (int y = scrollbar_pos; y < scrollbar_pos + scrollbar_height; y++)
        {
            if (y >= LCD_HEIGHT)
                break;
            for (int x = scrollbar_x; x < scrollbar_x + scrollbar_width; x++)
            {
                fb[y * LCD_WIDTH + x] = 0xFFFF;
            }
        }
    }

    lcd_update(fb);
}

void scroll_scoreboard(scoreboard_t *sb, int direction)
{
    if (!sb)
        return;

    const int visible_lines = 12;

    sb->scroll_offset -= direction;

    if (sb->scroll_offset < 0)
    {
        sb->scroll_offset = 0;
    }
    else if (sb->scroll_offset > sb->total_lines - visible_lines)
    {
        sb->scroll_offset = sb->total_lines - visible_lines;
    }

    if (sb->total_lines <= visible_lines)
    {
        sb->scroll_offset = 0;
    }
}

int handle_scoreboard_input(scoreboard_t *sb)
{
    static uint8_t last_knob_pos = 0;
    uint8_t current_knob = get_red_knob_rotation();
    int8_t delta = (int8_t)(current_knob - last_knob_pos);
    last_knob_pos = current_knob;

    if (delta > 127)
        delta -= 256;
    if (delta < -127)
        delta += 256;

    if (delta > 1)
        return 1; // Scroll up
    if (delta < -1)
        return 2; // Scroll down
    if (blue_knob_is_pressed())
        return 3; // Exit

    return 0;
}

void handle_scoreboard(scoreboard_t *sb, uint16_t *framebuffer)
{
    if (!sb)
    {
        fprintf(stderr, "DEBUG: handle_scoreboard called with NULL scoreboard pointer\n");
        return;
    }

    draw_scoreboard(sb, framebuffer, &font_winFreeSystem14x16);
    lcd_update(framebuffer);

    while (1)
    {
        int sb_action = handle_scoreboard_input(sb);

        if (sb_action == 3)
        {
            fprintf(stderr, "DEBUG: Exiting scoreboard\n");
            break;
        }

        if (sb_action == 1 || sb_action == 2)
        {
            scroll_scoreboard(sb, sb_action == 1 ? -1 : 1);
            draw_scoreboard(sb, framebuffer, &font_winFreeSystem14x16);
            lcd_update(framebuffer);
            timer_sleep_ms(100);
        }

        timer_sleep_ms(10);
    }

    while (blue_knob_is_pressed())
        timer_sleep_ms(10);

    fprintf(stderr, "DEBUG: Scoreboard interaction ended\n");
}
