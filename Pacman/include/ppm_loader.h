#ifndef PPM_LOADER_H
#define PPM_LOADER_H

#include <stdint.h>

#define LCD_WIDTH  480
#define LCD_HEIGHT 320
#define LCD_SIZE   (LCD_WIDTH * LCD_HEIGHT)

 typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t max_color_value;
    uint8_t *data; // RGB888 data - loaded from PPM
    uint16_t *pixels; // RGB565 data
} ppm_image_t;

ppm_image_t *load_ppm(const char *filename);
void free_ppm(ppm_image_t *image);
void draw_ppm_image(uint16_t *fb, int x, int y, const ppm_image_t *img);
void draw_ppm_image_transparent(uint16_t *fb, int x, int y, const ppm_image_t *img, uint16_t transparent_color);

#endif
// End of PPM_LOADER_H