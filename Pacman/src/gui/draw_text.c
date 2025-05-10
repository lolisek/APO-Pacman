#include "../../include/gui/draw_text.h"

void draw_pixel(uint16_t *fb, int x, int y, uint16_t color) {
    if (x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT) {
        return;
    }
    fb[y * LCD_WIDTH + x] = color;
}

void draw_char(uint16_t *fb, int x, int y, char ch, uint16_t color, const font_descriptor_t *font) {
    if (!fb || !font || !font->bits) {
        fprintf(stderr, "Invalid font parameters\n");
        return;
    }

    if (ch < font->firstchar || ch >= font->firstchar + font->size) {
        ch = font->defaultchar;
    }

    int char_index = ch - font->firstchar;
    int char_width = font->width ? font->width[char_index] : font->maxwidth;
    int height = font->height;

    const font_bits_t *bitmap;

    if (font->offset) {
        bitmap = &font->bits[font->offset[char_index]];
    } else {
        int words_per_line = (font->maxwidth + 15) / 16;
        bitmap = &font->bits[char_index * words_per_line * height];
    }

    for (int row = 0; row < height; row++) {
        font_bits_t row_data = *bitmap++;
        for (int col = 0; col < char_width; col++) {
            if ((row_data & (1 << (15 - col))) != 0) {
                draw_pixel(fb, x + col, y + row, color);
            }
        }
    }
}
void draw_string(uint16_t *fb, int x, int y, const char *str, uint16_t color, const font_descriptor_t *font) {
    if (!str || !font) {
        fprintf(stderr, "Invalid string or font\n");
        return;
    }

    int start_x = x;
    while (*str) {
        if (*str == '\n') {
            y += font->height + 2;
            x = start_x;
            str++;
            continue;
        }
        if (*str == '\r') {
            x = start_x;
            str++;
            continue;
        }

        draw_char(fb, x, y, *str, color, font);
        
        int char_index = *str - font->firstchar;
        int char_width = font->width ? font->width[char_index] : font->maxwidth;
        x += char_width + 1;  // Small spacing between characters
        
        str++;
    }
}