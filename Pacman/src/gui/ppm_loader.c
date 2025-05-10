#include "../../include/gui/ppm_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ppm_image_t *load_ppm(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }

    char header[3];
    int max_color_value;
    int width, height;
    if (fscanf(file, "%2s\n%d %d\n%d\n", header, &width, &height, &max_color_value) != 4) {
        fprintf(stderr, "Invalid PPM file format\n");
        fclose(file);
        return NULL;
    }

    if (strcmp(header, "P6") != 0) {
        fprintf(stderr, "Unsupported PPM format: %s\n", header);
        fclose(file);
        return NULL;
    }

    ppm_image_t *image = malloc(sizeof(ppm_image_t));
    if (!image) {
        perror("Failed to allocate memory for image");
        fclose(file);
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->max_color_value = max_color_value;

    image->data = malloc(image->width * image->height * 3);
    if (!image->data) {
        perror("Failed to allocate memory for image data");
        free(image);
        fclose(file);
        return NULL;
    }

    image->pixels = malloc(image->width * image->height * sizeof(uint16_t));
    if (!image->pixels) {
        perror("Failed to allocate memory for image pixels");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    int result = fread(image->data, 3, image->width * image->height, file);
    if (result != image->width * image->height) {
        fprintf(stderr, "Failed to read image data\n");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    fclose(file);
    size_t pixel_count = image->width * image->height;

    // Convert RGB to 16-bit color format (RGB565)
    for (size_t i = 0; i < pixel_count; i++) {
        uint8_t r = image->data[3 * i];
        uint8_t g = image->data[3 * i + 1];
        uint8_t b = image->data[3 * i + 2];

        uint16_t r5 = (r >> 3) & 0x1F;
        uint16_t g6 = (g >> 2) & 0x3F;
        uint16_t b5 = (b >> 3) & 0x1F;

        image->pixels[i] = (r5 << 11) | (g6 << 5) | b5;
    }

    return image;
}

void free_ppm(ppm_image_t *image) {
    if (image) {
        free(image->data);
        free(image->pixels);
        free(image);
    }
}

void draw_ppm_image(uint16_t *fb, int x, int y, const ppm_image_t *img) {
    for (int iy = 0; iy < img->height; iy++) {
        for (int ix = 0; ix < img->width; ix++) {
            int px = x + ix;
            int py = y + iy;

            if (px < 0 || px >= LCD_WIDTH || py < 0 || py >= LCD_HEIGHT) {
                continue;
            }

            fb[py * LCD_WIDTH + px] = img->pixels[iy * img->width + ix];
        }
    }
}

void draw_ppm_image_transparent(uint16_t *fb, int x, int y, const ppm_image_t *img, uint16_t transparent_color) {
    for (int iy = 0; iy < img->height; iy++) {
        for (int ix = 0; ix < img->width; ix++) {
            int px = x + ix;
            int py = y + iy;

            if (px < 0 || px >= LCD_WIDTH || py < 0 || py >= LCD_HEIGHT) {
                continue;
            }

            uint16_t color = img->pixels[iy * img->width + ix];
            if (color != transparent_color) {
                fb[py * LCD_WIDTH + px] = color;
            }
        }
    }
}

// End of PPM_LOADER_C
