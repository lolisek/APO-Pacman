#include "../../include/gui/custom_keyboard.h"
#include "../../include/utils/constants.h" // For get_resource_path
#include "../../include/utils/timer.h"     // Add this include for timer_sleep_ms

// TO DO: Add aggregation of the knob functions

void draw_keyboard(uint16_t *fb)
{
    char path[256];
    get_resource_path(path, sizeof(path), "keyboardbgr.ppm");
    ppm_image_t *keyboard_bgr = load_ppm(path);
    if (!keyboard_bgr)
    {
        fprintf(stderr, "Failed to load keyboard background image\n");
        return;
    }
    memcpy(fb, keyboard_bgr->pixels, LCD_SIZE * sizeof(uint16_t));
    free_ppm(keyboard_bgr);
}

typedef struct
{
    int position;
    int chars_written;
    char name[21];
    int last_red_action;
    int last_green_action;
} KeyboardState;

static void update_display(uint16_t *fb, int position, const char *name, const font_descriptor_t *font)
{
    draw_keyboard(fb);
    highlight_key(fb, position);
    draw_string(fb, 135, 47, name, 0xFFFF, font);
    lcd_update(fb);
}

char *handle_keyboard_input(uint16_t *fb, const font_descriptor_t *font)
{
    static const char *alphabet[27] = {
        "Q", "W", "E", "R", "T", "Y", "U", "I",
        "O", "P", "A", "S", "D", "F", "G", "H",
        "J", "K", "L", "Z", "X", "C", "V", "B",
        "N", "M", " "};

    KeyboardState state = {0};
    state.name[0] = '\0';

    static uint8_t last_red_knob = 0;
    static uint8_t last_green_knob = 0;

    // Sensitivity: how many knob steps before moving selection (higher = less sensitive)
    const int RED_KNOB_SENSITIVITY = 4;
    const int GREEN_KNOB_SENSITIVITY = 4;
    static int red_accum = 0;
    static int green_accum = 0;

    update_display(fb, state.position, state.name, font);

    while (!blue_knob_is_pressed())
    {
        // --- Less sensitive rotation handling for red knob (horizontal selection) ---
        uint8_t current_red_knob = get_red_knob_rotation();
        int red_delta = (int8_t)(current_red_knob - last_red_knob);
        last_red_knob = current_red_knob;
        red_accum += red_delta;

        if (abs(red_accum) >= RED_KNOB_SENSITIVITY)
        {
            int move = red_accum / RED_KNOB_SENSITIVITY;
            state.position = (state.position + move + 27) % 27;
            red_accum -= move * RED_KNOB_SENSITIVITY;
            update_display(fb, state.position, state.name, font);
            // No delay for fast rotation
        }

        int red_action = handle_red_knob();
        if (red_action != state.last_red_action && red_action == 3 && state.chars_written < 20)
        { // Select/add character
            state.name[state.chars_written++] = alphabet[state.position][0];
            state.name[state.chars_written] = '\0';
            update_display(fb, state.position, state.name, font);
            timer_sleep_ms(KNOB_CLICK_DELAY_MS);
        }
        state.last_red_action = red_action;

        // --- Less sensitive rotation handling for green knob (vertical selection) ---
        uint8_t current_green_knob = get_green_knob_rotation();
        int green_delta = (int8_t)(current_green_knob - last_green_knob);
        last_green_knob = current_green_knob;
        green_accum += green_delta;

        if (abs(green_accum) >= GREEN_KNOB_SENSITIVITY)
        {
            int move = green_accum / GREEN_KNOB_SENSITIVITY;
            int new_pos = state.position;
            for (int i = 0; i < abs(move); ++i)
            {
                if (move > 0)
                {
                    // Scroll up
                    if (new_pos < 8)
                    {
                        // do nothing
                    }
                    else if (new_pos < 24)
                    {
                        new_pos -= 8;
                    }
                    else if (new_pos == 24)
                    {
                        new_pos = 17;
                    }
                    else if (new_pos == 25)
                    {
                        new_pos = 18;
                    }
                    else if (new_pos == 26)
                    {
                        new_pos = 19;
                    }
                }
                else
                {
                    // Scroll down
                    if (new_pos == 17)
                    {
                        new_pos = 24;
                    }
                    else if (new_pos == 18)
                    {
                        new_pos = 25;
                    }
                    else if (new_pos == 19 || new_pos == 20 || new_pos == 21 || new_pos == 22)
                    {
                        new_pos = 26;
                    }
                    else if (new_pos > 15)
                    {
                        // do nothing
                    }
                    else
                    {
                        new_pos += 8;
                    }
                }
            }
            state.position = new_pos;
            green_accum -= move * GREEN_KNOB_SENSITIVITY;
            update_display(fb, state.position, state.name, font);
            // No delay for fast rotation
        }

        int green_action = handle_green_knob();
        if (green_action != state.last_green_action && green_action == 3 && state.chars_written > 0)
        { // Delete character
            state.name[--state.chars_written] = '\0';
            update_display(fb, state.position, state.name, font);
            timer_sleep_ms(KNOB_CLICK_DELAY_MS);
        }
        state.last_green_action = green_action;

        timer_sleep_ms((int)(KNOB_ROTATION_DELAY_MS * 0.5)); // Faster polling for smoother experience
    }

    // Return a heap-allocated copy of the name
    char *result = malloc(state.chars_written + 1);
    if (result)
        strcpy(result, state.name);
    return result;
}

int handle_green_knob()
{
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    static uint8_t last_knob_pos = 0;
    uint8_t current_knob = get_green_knob_rotation();
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
    if (green_knob_is_pressed())
    {
        if (current_time - last_press_time > 50)
        {
            last_press_time = current_time;
            return 3; // Return selection
        }
    } // Delete character

    return 0;
}

int handle_red_knob()
{
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    static uint8_t last_knob_pos = 0;
    uint8_t current_knob = get_red_knob_rotation();
    int8_t delta = (int8_t)(current_knob - last_knob_pos);
    last_knob_pos = current_knob;

    if (delta > 127)
        delta -= 256;
    if (delta < -127)
        delta += 256;

    if (delta < -1)
        return 1; // Scroll right
    if (delta > 1)
        return 2; // Scroll left
    if (red_knob_is_pressed())
    {
        if (current_time - last_press_time > 50)
        {
            last_press_time = current_time;
            return 3; // Return selection
        }
    } // Exit

    return 0;
}

void highlight_key(uint16_t *fb, int key_index)
{
    int x = 40;
    int y = 86;
    if (key_index < 0 || key_index > 26)
    {
        return;
    }

    char key_highlight_path[256];
    char space_highlight_path[256];
    get_resource_path(key_highlight_path, sizeof(key_highlight_path), "key_highlight.ppm");
    get_resource_path(space_highlight_path, sizeof(space_highlight_path), "space_highlight.ppm");

    ppm_image_t *key_highlight = load_ppm(key_highlight_path);
    if (!key_highlight)
    {
        fprintf(stderr, "Failed to load key highlight image\n");
        return;
    }

    ppm_image_t *space_highlight = load_ppm(space_highlight_path);
    if (!space_highlight)
    {
        fprintf(stderr, "Failed to load space highlight image\n");
        free_ppm(key_highlight);
        return;
    }

    if (key_index < 24)
    {
        x += key_index % 8 * 51;
        y += key_index / 8 * 51;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, key_highlight, 0x0000);
        free_ppm(key_highlight);
        free_ppm(space_highlight);
        return;
    }
    else if (key_index < 26)
    {
        x = 91 + (key_index - 24) * 51;
        y = 239;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, key_highlight, 0x0000);
        free_ppm(key_highlight);
        free_ppm(space_highlight);
        return;
    }
    else
    {
        x = 193;
        y = 239;

        draw_keyboard(fb);
        draw_ppm_image_transparent(fb, x, y, space_highlight, 0x0000);
        free_ppm(key_highlight);
        free_ppm(space_highlight);
        return;
    }
}
