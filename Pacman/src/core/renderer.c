#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/core/renderer.h"
#include "../../include/core/game_state.h"
#include "../../include/core/input.h"
#include "../../include/map/map.h"
#include "../../include/utils/constants.h"
#include "../../include/microzed/mzapo_parlcd.h"

// --- Static texture pointers ---
static ppm_image_t *wall_texture = NULL;
static ppm_image_t *pacman_textures[5] = {NULL};       // Right, Left, Up, Down, Closed
static ppm_image_t *ghost_red_textures[2] = {NULL};    // Right, Left
static ppm_image_t *ghost_orange_textures[2] = {NULL}; // Right, Left
static ppm_image_t *ghost_blue_textures[2] = {NULL};   // Right, Left
static ppm_image_t *ghost_pink_textures[2] = {NULL};   // Right, Left
static ppm_image_t *pellet_texture = NULL;
static ppm_image_t *power_pellet_texture = NULL;
static ppm_image_t *vulnerable_ghost_texture = NULL;
static ppm_image_t *eaten_ghost_texture = NULL;

// --- Helper: Load a texture and print error if it fails ---
static ppm_image_t *load_texture_or_warn(const char *resource_name)
{
    char path[256];
    get_resource_path(path, sizeof(path), resource_name);
    ppm_image_t *img = load_ppm(path);
    if (!img)
    {
        fprintf(stderr, "Failed to load texture: %s\n", resource_name);
    }
    return img;
}

// --- Helper: Load a pair of textures (right/left) ---
static void load_ghost_textures(const char *right, const char *left, ppm_image_t **arr)
{
    arr[0] = load_texture_or_warn(right);
    arr[1] = load_texture_or_warn(left);
}

void render_init(void)
{
    // Load main textures
    wall_texture = load_texture_or_warn("block.ppm");
    pellet_texture = load_texture_or_warn("pellet.ppm");
    power_pellet_texture = load_texture_or_warn("power-pellet.ppm");

    // Pacman textures (order: right, left, up, down, closed)
    pacman_textures[0] = load_texture_or_warn("pacman-right.ppm");
    pacman_textures[1] = load_texture_or_warn("pacman-left.ppm");
    pacman_textures[2] = load_texture_or_warn("pacman-up.ppm");
    pacman_textures[3] = load_texture_or_warn("pacman-down.ppm");
    pacman_textures[4] = load_texture_or_warn("pacman-closed.ppm");

    // Ghost textures
    load_ghost_textures("ghost-red-right.ppm", "ghost-red-left.ppm", ghost_red_textures);
    load_ghost_textures("ghost-or-right.ppm", "ghost-or-left.ppm", ghost_orange_textures);
    load_ghost_textures("ghost-blue-right.ppm", "ghost-blue-left.ppm", ghost_blue_textures);
    load_ghost_textures("ghost-pink-right.ppm", "ghost-pink-left.ppm", ghost_pink_textures);

    vulnerable_ghost_texture = load_texture_or_warn("vulnerable-ghost.ppm");
    eaten_ghost_texture = load_texture_or_warn("eyes.ppm");

    // Check for any missing essential textures
    if (!wall_texture || !pellet_texture || !power_pellet_texture)
    {
        fprintf(stderr, "Essential textures missing. Rendering may fail.\n");
    }
    for (int i = 0; i < 5; ++i)
        if (!pacman_textures[i])
            fprintf(stderr, "Pacman texture %d missing\n", i);
    for (int i = 0; i < 2; ++i)
    {
        if (!ghost_red_textures[i])
            fprintf(stderr, "Red ghost texture %d missing\n", i);
        if (!ghost_orange_textures[i])
            fprintf(stderr, "Orange ghost texture %d missing\n", i);
        if (!ghost_blue_textures[i])
            fprintf(stderr, "Blue ghost texture %d missing\n", i);
        if (!ghost_pink_textures[i])
            fprintf(stderr, "Pink ghost texture %d missing\n", i);
    }
}

void render(GameState *game_state, uint16_t *fb)
{
    static int animation_frame = 0;

    // Clear screen
    memset(fb, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

    // Calculate map offset for centering
    int map_pixel_width = game_state->map.width * TILE_SIZE;
    int map_pixel_height = game_state->map.height * TILE_SIZE;
    int offset_x = (LCD_WIDTH - map_pixel_width) / 2;
    int offset_y = (LCD_HEIGHT - map_pixel_height) / 2;
    if (offset_x < 0)
        offset_x = 0;
    if (offset_y < 0)
        offset_y = 0;

    render_map(&game_state->map, fb, offset_x, offset_y);
    render_pacman(&game_state->pacman, fb, animation_frame, offset_x, offset_y);

    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        // Optionally: bool frightened = game_state->frightened_timer > 0 && game_state->ghosts[i].specific.ghost.mode != GHOST_MODE_EATEN;
        render_ghost(&game_state->ghosts[i], fb, animation_frame, offset_x, offset_y);
    }

    render_ui(game_state, fb);
    lcd_update(fb);

    animation_frame++;
}

void render_ui(const GameState *game_state, uint16_t *fb)
{
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "SCORE: %d", game_state->score);

    // Draw score at top-left
    draw_string(fb, 10, 10, score_text, 0xFFFF, &font_winFreeSystem14x16);

    // Draw lives at bottom-left
    for (int i = 0; i < game_state->lives; i++)
    {
        draw_ppm_image(fb, 10 + i * 20, LCD_HEIGHT - 30, pacman_textures[0]);
    }
}

void render_map(const Map *map, uint16_t *fb, int offset_x, int offset_y)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            int screen_x = offset_x + x * TILE_SIZE;
            int screen_y = offset_y + y * TILE_SIZE;

            switch (map->tiles[y][x].type)
            {
            case TILE_WALL:
            case TILE_GATE:
                if (wall_texture)
                    draw_ppm_image(fb, screen_x, screen_y, wall_texture);
                break;
            case TILE_PELLET:
                if (pellet_texture)
                {
                    int pellet_x = screen_x + (TILE_SIZE - pellet_texture->width) / 2;
                    int pellet_y = screen_y + (TILE_SIZE - pellet_texture->height) / 2;
                    draw_ppm_image_transparent(fb, pellet_x, pellet_y, pellet_texture, 0x0000);
                }
                break;
            case TILE_POWER_PELLET:
                if (power_pellet_texture)
                {
                    int pp_x = screen_x + (TILE_SIZE - power_pellet_texture->width) / 2;
                    int pp_y = screen_y + (TILE_SIZE - power_pellet_texture->height) / 2;
                    draw_ppm_image_transparent(fb, pp_x, pp_y, power_pellet_texture, 0x0000);
                }
                break;
            default:
                break;
            }
        }
    }
}

void render_pacman(const Entity *pacman, uint16_t *fb, int animation_frame, int offset_x, int offset_y)
{
    int direction_index = 3; // Default: Down
    if (pacman->direction.y == 0 && pacman->direction.x == 1)
        direction_index = 0; // Right
    else if (pacman->direction.y == 0 && pacman->direction.x == -1)
        direction_index = 1; // Left
    else if (pacman->direction.y == -1 && pacman->direction.x == 0)
        direction_index = 2; // Up

    // Alternate between open and closed mouth for animation
    ppm_image_t *texture = (animation_frame % PACMAN_ANIMATION_SPEED < PACMAN_ANIMATION_SPEED / 2)
                               ? pacman_textures[direction_index]
                               : pacman_textures[4]; // Closed mouth

    int x = offset_x + pacman->position.x * TILE_SIZE;
    int y = offset_y + pacman->position.y * TILE_SIZE;

    if (texture)
        draw_ppm_image(fb, x, y, texture);
}

void render_ghost(const Entity *ghost, uint16_t *fb, int animation_frame, int offset_x, int offset_y)
{
    int x = offset_x + ghost->position.x * TILE_SIZE;
    int y = offset_y + ghost->position.y * TILE_SIZE;

    ppm_image_t *texture = NULL;

    if (ghost->specific.ghost.mode == GHOST_MODE_EATEN)
    {
        // Render only the eyes when the ghost is eaten
        texture = eaten_ghost_texture;
    }
    else if (ghost->specific.ghost.waiting_timer > 0)
    {
        // Render the ghost as stationary while waiting
        texture = ghost_red_textures[0]; // Example: Use the red ghost texture
    }
    else if (ghost->specific.ghost.mode == GHOST_MODE_FRIGHTENED)
    {
        // Render the vulnerable ghost texture
        texture = vulnerable_ghost_texture;
    }
    else
    {
        // Render the ghost's normal texture based on its type and direction
        switch (ghost->specific.ghost.type)
        {
        case GHOST_TYPE_BLINKY:
            texture = ghost->direction.x > 0 ? ghost_red_textures[0] : ghost_red_textures[1];
            break;
        case GHOST_TYPE_PINKY:
            texture = ghost->direction.x > 0 ? ghost_pink_textures[0] : ghost_pink_textures[1];
            break;
        case GHOST_TYPE_INKY:
            texture = ghost->direction.x > 0 ? ghost_blue_textures[0] : ghost_blue_textures[1];
            break;
        case GHOST_TYPE_CLYDE:
            texture = ghost->direction.x > 0 ? ghost_orange_textures[0] : ghost_orange_textures[1];
            break;
        }
    }

    if (texture)
        draw_ppm_image(fb, x, y, texture);
}