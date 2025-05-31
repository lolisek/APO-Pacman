#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/core/renderer.h"
#include "../../include/core/game_state.h"
#include "../../include/core/input.h"
#include "../../include/map/map.h"
#include "../../include/utils/constants.h"
#include "../../include/microzed/mzapo_parlcd.h"
#include "../../include/gui/resource_manager.h"


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
    offset_x = offset_x < 0 ? 0 : offset_x;
    offset_y = offset_y < 0 ? 0 : offset_y;

    // Render map, Pac-Man, ghosts, and UI
    render_map(&game_state->map, fb, offset_x, offset_y);
    render_pacman(&game_state->pacman, fb, animation_frame, offset_x, offset_y);

    for (int i = 0; i < NUM_GHOSTS; i++)
    {
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
    ppm_image_t **pacman_textures = get_pacman_textures();
    for (int i = 0; i < game_state->lives; i++)
    {
        draw_ppm_image(fb, 10 + i * 20, LCD_HEIGHT - 30, pacman_textures[0]);
    }
}

void render_map(const Map *map, uint16_t *fb, int offset_x, int offset_y)
{
    ppm_image_t *wall_texture = get_wall_texture();
    ppm_image_t *pellet_texture = get_pellet_texture();
    ppm_image_t *power_pellet_texture = get_power_pellet_texture();

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
    ppm_image_t **pacman_textures = get_pacman_textures();
    int direction_index = 3; // Default: Down
    if (pacman->direction.y == 0 && pacman->direction.x == 1)
    {
        direction_index = 0; // Right
    }
    else if (pacman->direction.y == 0 && pacman->direction.x == -1)
    {
        direction_index = 1; // Left
    }
    else if (pacman->direction.y == -1 && pacman->direction.x == 0)
    {
        direction_index = 2; // Up
    }

    // Alternate between open and closed mouth for animation
    ppm_image_t *texture = (animation_frame % PACMAN_ANIMATION_SPEED < PACMAN_ANIMATION_SPEED / 2)
                               ? pacman_textures[direction_index]
                               : pacman_textures[4]; // Closed mouth

    int x = offset_x + (int)(pacman->position.x * TILE_SIZE);
    int y = offset_y + (int)(pacman->position.y * TILE_SIZE);

    if (texture)
    {
        draw_ppm_image(fb, x, y, texture);
    }
}


void render_ghost(const Entity *ghost, uint16_t *fb, int animation_frame, int offset_x, int offset_y)
{
    int x = offset_x + (int)(ghost->position.x * TILE_SIZE);
    int y = offset_y + (int)(ghost->position.y * TILE_SIZE);

    ppm_image_t *texture = NULL;

    if (ghost->specific.ghost.mode == GHOST_MODE_EATEN)
    {
        texture = get_eaten_ghost_texture();
    }
    else if (ghost->specific.ghost.mode == GHOST_MODE_FRIGHTENED)
    {
        texture = get_vulnerable_ghost_texture();
    }
    else
    {
        ppm_image_t **ghost_textures = get_ghost_textures(ghost->specific.ghost.type);
        if (ghost_textures)
        {
            texture = ghost->direction.x > 0 ? ghost_textures[0] : ghost_textures[1];
        }
    }

    if (texture)
    {
        draw_ppm_image(fb, x, y, texture);
    }
}
