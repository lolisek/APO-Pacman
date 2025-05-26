#include "../../include/gui/main_menu.h"
#include "../../include/utils/constants.h"
#include "../../include/microzed/mzapo_peri.h"
#include "../../include/utils/timer.h"

int handle_knob_rotation(int last_pos);

const char *menu_items[MENU_ITEMS] = {
    "Play",
    "Show scoreboard",
    "Exit"};

void init_menu(menu_state_t *menu)
{
    menu->selected = 0;
    menu->last_selected = 255;
    menu->last_knob_pos = get_red_knob_rotation();
    memset(menu->framebuffer, 0, sizeof(menu->framebuffer));
}

void draw_menu(menu_state_t *menu)
{
    char path[256];
    get_resource_path(path, sizeof(path), "menu.ppm");
    ppm_image_t *menu_bgr = load_ppm(path);
    if (!menu_bgr)
    {
        fprintf(stderr, "Failed to load menu image\n");
        return;
    }
    memcpy(menu->framebuffer, menu_bgr->pixels, LCD_SIZE * sizeof(uint16_t));

    render_arrows(menu->selected, menu->framebuffer);

    lcd_update(menu->framebuffer);
}

int handle_menu_input(menu_state_t *menu)
{
    static uint32_t last_press_time = 0;
    uint32_t current_time = clock() * 1000;

    // Handle knob rotation
    int change = handle_knob_rotation(menu->last_knob_pos);

    if (change != 0)
    {
        int new_position = menu->selected - change;
        new_position = (new_position % MENU_ITEMS + MENU_ITEMS) % MENU_ITEMS;

        menu->selected = new_position;
        menu->last_knob_pos = get_red_knob_rotation();
        return 1; // Redraw needed
    }

    // Handle knob press
    if (red_knob_is_pressed())
    {
        if (current_time - last_press_time > DEBOUNCE_DELAY_MS)
        {
            last_press_time = current_time;
            return menu->selected + 2; // Return selection
        }
        // Add debounce sleep to avoid rapid re-triggering
        timer_sleep_ms(DEBOUNCE_DELAY_MS);
    }

    return 0;
}

int handle_knob_rotation(int last_pos)
{
    uint8_t current_knob = get_red_knob_rotation();
    int delta = (int)(current_knob - last_pos);

    // Handle wrap-around
    if (delta > 127)
        delta -= 256;
    if (delta < -127)
        delta += 256;

    if (abs(delta) > 3)
    {
        // Calculate clicks, rounding toward zero
        int clicks = delta / (KNOB_POSITIONS / KNOB_CLICKS_PER_TURN);

        // For small movements, just return ±1
        if (abs(clicks) == 0)
        {
            return (delta > 0) ? 1 : -1;
        }
        return clicks;
    }

    return 0;
}

void render_arrows(int selected, uint16_t *fb)
{
    char path_left[256], path_right[256];
    get_resource_path(path_left, sizeof(path_left), "arrow_left.ppm");
    get_resource_path(path_right, sizeof(path_right), "arrow_right.ppm");
    ppm_image_t *arrow_left = load_ppm(path_left);
    ppm_image_t *arrow_right = load_ppm(path_right);
    if (!arrow_left || !arrow_right)
    {
        fprintf(stderr, "Failed to load arrow images\n");
        return;
    }

    switch (selected)
    {
    case 0: // Play
        draw_ppm_image(fb, 174, 101, arrow_right);
        draw_ppm_image(fb, 292, 101, arrow_left);
        break;
    case 1: // Show scoreboard
        draw_ppm_image(fb, 72, 147, arrow_right);
        draw_ppm_image(fb, 404, 147, arrow_left);
        break;
    case 2: // Exit
        draw_ppm_image(fb, 177, 194, arrow_right);
        draw_ppm_image(fb, 294, 194, arrow_left);
        break;
    default:
        fprintf(stderr, "Invalid menu selection\n");
        break;
    }
}