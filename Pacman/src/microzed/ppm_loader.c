#include "ppm_loader.h"
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

    image->pixels = malloc(image->width * image->height * sizeof(struct pixel));
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

    // Convert RGB to 16-bit color format (RGB565)
    for (unsigned i = 0; i < width * height; i++) {
        image->pixels[i].r = image->data[i*3] >> 3;
        image->pixels[i].g = image->data[i*3+1] >> 2;
        image->pixels[i].b = image->data[i*3+2] >> 3;
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

void draw_ppm_image(uint16_t *lcd_buffer, const ppm_image_t *image, 
    int x, int y, int lcd_width, int lcd_height) {
    if (!image || !lcd_buffer) return;

    const uint16_t *src_pixels = (const uint16_t *)image->pixels;

    for (int j = 0; j < image->height; j++) {
        int dest_y = y + j;
        if (dest_y < 0 || dest_y >= lcd_height) continue;

        for (int i = 0; i < image->width; i++) {
            int dest_x = x + i;
            if (dest_x >= 0 && dest_x < lcd_width) {
                int src_pos = j * image->width + i;
                int dest_pos = dest_y * lcd_width + dest_x;
                
                lcd_buffer[dest_pos] = src_pixels[src_pos];
            }
        }
    }
}
// End of PPM_LOADER_C
