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

/**
 * @brief Loads a PPM image from a file.
 *
 * @param filename Path to the PPM file.
 * @return ppm_image_t* Pointer to the loaded PPM image, or NULL on failure.
 */
ppm_image_t *load_ppm(const char *filename);

/**
 * @brief Frees the memory allocated for a PPM image.
 *
 * @param image Pointer to the PPM image to free.
 */
void free_ppm(ppm_image_t *image);

/**
 * @brief Draws a PPM image onto the framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 * @param x X-coordinate of the top-left corner of the image.
 * @param y Y-coordinate of the top-left corner of the image.
 * @param img Pointer to the PPM image to draw.
 */
void draw_ppm_image(uint16_t *fb, int x, int y, const ppm_image_t *img);

/**
 * @brief Draws a PPM image onto the framebuffer with transparency.
 *
 * @param fb Pointer to the framebuffer.
 * @param x X-coordinate of the top-left corner of the image.
 * @param y Y-coordinate of the top-left corner of the image.
 * @param img Pointer to the PPM image to draw.
 * @param transparent_color Color to treat as transparent.
 */
void draw_ppm_image_transparent(uint16_t *fb, int x, int y, const ppm_image_t *img, uint16_t transparent_color);

#endif
// End of PPM_LOADER_H