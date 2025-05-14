#include "../../include/gui/custom_keyboard.h"

// TO DO: Add aggregation of the knob functions

void draw_keyboard(uint16_t *fb) {
    ppm_image_t *keyboard_bgr = load_ppm("/tmp/veru/assets/resources/keyboardbgr.ppm");
    if (!keyboard_bgr) {
        fprintf(stderr, "Failed to load keyboard background image\n");
        return;
    }
    memcpy(fb, keyboard_bgr->pixels, LCD_SIZE * sizeof(uint16_t));
    free_ppm(keyboard_bgr);
}

void handle_keyboard_input(uint16_t *fb, const font_descriptor_t *font) {
    char* alphabet[27] = {
        "Q", "W", "E", "R", "T", "Y", "U", "I",
        "O", "P", "A", "S", "D", "F", "G", "H",
        "J", "K", "L", "Z", "X", "C", "V", "B",
        "N", "M", " "
    };

    char* name = malloc(21);
    if (!name) {
        fprintf(stderr, "Failed to allocate memory for name\n");
        return;
    }
    name[0] = '\0';
    int chars_written = 0;
    int position = 0;

    while (!blue_knob_is_pressed()) {
        draw_keyboard(fb);
        highlight_key(fb, position);

        int action = handle_red_knob();
        if (action == 1) { // Scroll right
            position = (position + 1) % 27;
            highlight_key(fb, position);
            draw_string(fb, 135, 47, name, 0xFFFF, font);
            lcd_update(fb);
        } else if (action == 2) { // Scroll left
            position = (position - 1 + 27) % 27;
            highlight_key(fb, position);
            draw_string(fb, 135, 47, name, 0xFFFF, font);
            lcd_update(fb);
        } else if (action == 3) { // Return selection
            if (chars_written < 20) {
                name[chars_written++] = alphabet[position][0];
                name[chars_written] = '\0';
                for (int i = 0; i < chars_written; i++) {
                    fprintf(stderr, "%c", name[i]);
                }
                fprintf(stderr, "\n");
                draw_string(fb, 135, 47, name, 0xFFFF, font);
                lcd_update(fb);
            }
            usleep(KNOB_CLICK_DELAY_MS);
        }

        action = handle_green_knob();
        if (action == 1) { // Scroll up
            if (position < 8) {
                continue;
            } else if (position < 24) {
                position -= 8;
            } else if (position == 24) {
                position = 17;
            } else if (position == 25) {
                position = 18;
            } else if (position == 26) {
                position = 19;
            }
            highlight_key(fb, position);
            draw_string(fb, 135, 47, name, 0xFFFF, font);
            lcd_update(fb);
        } else if (action == 2) { // Scroll down
            if (position == 17) {
                position = 24;
            } else if (position == 18) {
                position = 25;
            } else if (position == 19 || position == 20 || position == 21 || position == 22) {
                position = 26;
            } else if (position > 15) {
                continue;
            } else {
                position += 8;
            }
            highlight_key(fb, position);
            draw_string(fb, 135, 47, name, 0xFFFF, font);
            lcd_update(fb);
        } else if (action == 3) { 
            if (chars_written > 0) {
                name[--chars_written] = '\0';
                draw_string(fb, 135, 47, name, 0xFFFF, font);
                lcd_update(fb);
            }
            usleep(KNOB_CLICK_DELAY_MS);
        }

        usleep(KNOB_ROTATION_DELAY_MS * 1.25);
    }


}

int handle_green_knob() {
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    static uint8_t last_knob_pos = 0;
    uint8_t current_knob = get_green_knob_rotation();
    int8_t delta = (int8_t)(current_knob - last_knob_pos);
    last_knob_pos = current_knob;

    if (delta > 127) delta -= 256;
    if (delta < -127) delta += 256;

    if (delta > 1) return 1; // Scroll up
    if (delta < -1) return 2; // Scroll down
    if (green_knob_is_pressed()) {
        if (current_time - last_press_time > 50) {
            last_press_time = current_time;
            return 3;  // Return selection
        }
    } // Delete character

    return 0;
}

int handle_red_knob() {
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    static uint8_t last_knob_pos = 0;
    uint8_t current_knob = get_red_knob_rotation();
    int8_t delta = (int8_t)(current_knob - last_knob_pos);
    last_knob_pos = current_knob;

    if (delta > 127) delta -= 256;
    if (delta < -127) delta += 256;

    if (delta < -1) return 1; // Scroll right
    if (delta > 1) return 2; // Scroll left
    if (red_knob_is_pressed()) {
        if (current_time - last_press_time > 50) {
            last_press_time = current_time;
            return 3;  // Return selection
        }
    } // Exit

    return 0;
}

void highlight_key(uint16_t *fb, int key_index) {
    int x = 40;
    int y = 86;
    if (key_index < 0 || key_index > 26) {
        return;
    }

    ppm_image_t *key_highlight = load_ppm("/tmp/veru/assets/resources/key_highlight.ppm");
    if (!key_highlight) {
        fprintf(stderr, "Failed to load key highlight image\n");
        return;
    }

    ppm_image_t *space_highlight = load_ppm("/tmp/veru/assets/resources/space_highlight.ppm");
    if (!space_highlight) {
        fprintf(stderr, "Failed to load space highlight image\n");
        free_ppm(key_highlight);
        return;
    }

    if (key_index < 24) {
        x += key_index%8 * 51;
        y += key_index/8 * 51;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, key_highlight, 0x0000);
        return;
    } else if (key_index < 26) {
        x = 91 + (key_index - 24) * 51;
        y = 239;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, key_highlight, 0x0000);
        return;
    } else {
        x = 193;
        y = 239;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, space_highlight, 0x0000);
        return;
    }

    return;
}

