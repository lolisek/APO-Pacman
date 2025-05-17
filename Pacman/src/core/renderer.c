#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/core/renderer.h"
#include "../../include/core/game_state.h"
#include "../../include/core/input.h"
#include "../../include/map/map.h"
#include "../../include/utils/constants.h" // Add this include

void render_init(void)
{
    char path[256];

    // Load textures and initialize rendering resources
    get_resource_path(path, sizeof(path), "block.ppm");
    wall_texture = load_ppm(path);

    get_resource_path(path, sizeof(path), "pellet.ppm");
    pellet_texture = load_ppm(path);

    get_resource_path(path, sizeof(path), "power-pellet.ppm");
    power_pellet_texture = load_ppm(path);

    if (!wall_texture || !pellet_texture || !power_pellet_texture)
    {
        fprintf(stderr, "Failed to load textures\n");
        return;
    }

    // Pacman textures
    get_resource_path(path, sizeof(path), "pacman-right.ppm");
    pacman_textures[0] = load_ppm(path);
    get_resource_path(path, sizeof(path), "pacman-left.ppm");
    pacman_textures[1] = load_ppm(path);
    get_resource_path(path, sizeof(path), "pacman-up.ppm");
    pacman_textures[2] = load_ppm(path);
    get_resource_path(path, sizeof(path), "pacman-down.ppm");
    pacman_textures[3] = load_ppm(path);
    get_resource_path(path, sizeof(path), "pacman-closed.ppm");
    pacman_textures[4] = load_ppm(path);

    if (!pacman_textures[0] || !pacman_textures[1] || !pacman_textures[2] || !pacman_textures[3] || !pacman_textures[4])
    {
        fprintf(stderr, "Failed to load Pacman textures\n");
        return;
    }

    // Ghost textures
    get_resource_path(path, sizeof(path), "ghost-red-right.ppm");
    ghost_red_textures[0] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-red-left.ppm");
    ghost_red_textures[1] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-or-right.ppm");
    ghost_orange_textures[0] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-or-left.ppm");
    ghost_orange_textures[1] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-blue-right.ppm");
    ghost_blue_textures[0] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-blue-left.ppm");
    ghost_blue_textures[1] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-pink-right.ppm");
    ghost_pink_textures[0] = load_ppm(path);
    get_resource_path(path, sizeof(path), "ghost-pink-left.ppm");
    ghost_pink_textures[1] = load_ppm(path);

    if (!ghost_red_textures[0] || !ghost_red_textures[1] ||
        !ghost_orange_textures[0] || !ghost_orange_textures[1] ||
        !ghost_blue_textures[0] || !ghost_blue_textures[1] ||
        !ghost_pink_textures[0] || !ghost_pink_textures[1])
    {
        fprintf(stderr, "Failed to load ghost textures\n");
        return;
    }
}

void render(GameState *game_state, uint16_t *fb)
{
    static int animation_frame = 0;

    // Clear screen
    memset(fb, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

    // Calculate center position
    int map_pixel_width = game_state->map.width * TILE_SIZE;
    int map_pixel_height = game_state->map.height * TILE_SIZE;
    int offset_x = (LCD_WIDTH - map_pixel_width) / 2;
    int offset_y = (LCD_HEIGHT - map_pixel_height) / 2;

    offset_x = offset_x > 0 ? offset_x : 0;
    offset_y = offset_y > 0 ? offset_y : 0;

    render_map(&game_state->map, fb, offset_x, offset_y);
    render_pacman(&game_state->pacman, fb, animation_frame, offset_x, offset_y);

    for (int i = 0; i < NUM_GHOSTS; i++)
    {
        bool frightened = game_state->frightened_timer > 0 &&
                          game_state->ghosts[i].specific.ghost.mode != GHOST_MODE_EATEN;
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

    // Center score at top
    int text_width = strlen(score_text) * 8; // Approximate width
    draw_string(fb, 10, 10, score_text, 0xFFFF, &font_winFreeSystem14x16);

    // Render lives at bottom-left
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
                if (wall_texture)
                {
                    draw_ppm_image(fb, screen_x, screen_y, wall_texture);
                }
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
    int direction_index;
    if (pacman->direction.y == 0 && pacman->direction.x == 1)
        direction_index = 0; // Right
    else if (pacman->direction.y == 0 && pacman->direction.x == -1)
        direction_index = 1; // Left
    else if (pacman->direction.y == -1 && pacman->direction.x == 0)
        direction_index = 2; // Up
    else
        direction_index = 3; // Down

    // Alternate between open and closed mouth for animation
    ppm_image_t *texture;
    if (animation_frame % PACMAN_ANIMATION_SPEED < PACMAN_ANIMATION_SPEED / 2)
    {
        texture = pacman_textures[direction_index];
    }
    else
    {
        texture = pacman_textures[4]; // Closed mouth
    }

    int x = offset_x + pacman->position.x * TILE_SIZE;
    int y = offset_y + pacman->position.y * TILE_SIZE;

    if (texture)
    {
        draw_ppm_image(fb, x, y, texture);
    }
}

void render_ghost(const Entity *ghost, uint16_t *fb, int animation_frame, int offset_x, int offset_y)
{
    int x = offset_x + ghost->position.x * TILE_SIZE;
    int y = offset_y + ghost->position.y * TILE_SIZE;

    ppm_image_t *texture = NULL;

    // Select texture based on ghost type and direction
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

    if (texture)
    {
        draw_ppm_image(fb, x, y, texture);
    }
}