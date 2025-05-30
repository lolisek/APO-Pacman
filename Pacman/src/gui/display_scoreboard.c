#include "../../include/gui/display_scoreboard.h"
#include "../../include/utils/constants.h" // Add this include
#include "../../include/gui/ppm_loader.h"
#include <string.h>
#include <stdio.h>

void init_scoreboard(scoreboard_t *sb)
{
    sb->total_lines = 0;
    sb->scroll_offset = 0;
    memset(sb->lines, 0, sizeof(sb->lines));
}

int load_scores(scoreboard_t *sb)
{
    char scores_path[256];
    get_resource_path(scores_path, sizeof(scores_path), "scores.txt");
    FILE *file = fopen(scores_path, "r");
    if (!file)
    {
        return 0;
    }

    sb->total_lines = 0;
    memset(sb->lines, 0, sizeof(sb->lines));

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

    fprintf(stderr, "Loaded %d scores\n", sb->total_lines);
    fclose(file);
    return 0;
}

int save_score(const char *name, int score)
{
    char scores_path[256];
    get_resource_path(scores_path, sizeof(scores_path), "scores.txt");
    FILE *file = fopen(scores_path, "a");
    if (!file)
    {
        perror("Failed to open scores file");
        return -1;
    }

    fprintf(file, "%s: %d\n", name, score);
    fclose(file);
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
    const int visible_lines = 12; // Fixed number

    int lines_to_show = sb->total_lines;
    if (lines_to_show > visible_lines)
    {
        lines_to_show = visible_lines;
    }

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

        // Calculate scrollbar proportions
        float visible_ratio = (float)visible_lines / sb->total_lines;
        int scrollbar_height = LCD_HEIGHT * visible_ratio;
        int scrollbar_pos = (sb->scroll_offset * LCD_HEIGHT) / sb->total_lines;

        // draw scrollbar track
        for (int y = 0; y < LCD_HEIGHT; y++)
        {
            fb[y * LCD_WIDTH + scrollbar_x] = 0x7BEF;
        }

        // draw scrollbar thumb
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

    load_scores(sb);
    lcd_update(fb);
}

void scroll_scoreboard(scoreboard_t *sb, int direction)
{
    if (!sb)
        return;

    const int visible_lines = 12;

    // Reverse the direction of scrolling
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
        { // Exit scoreboard
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
