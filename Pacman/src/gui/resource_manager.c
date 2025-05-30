#include "../../include/gui/resource_manager.h"
#include "../../include/utils/constants.h"
#include "../../include/gui/ppm_loader.h"
#include "../../include/utils/logger.h"
#include <stdlib.h>
#include <string.h>

// Global resources
static ppm_image_t *gameover_screen = NULL;
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

static ppm_image_t *load_texture_or_warn(const char *resource_name) {
    char path[256];
    get_resource_path(path, sizeof(path), resource_name);
    ppm_image_t *img = load_ppm(path);
    if (!img) {
        LOG_ERROR("Failed to load texture: %s", resource_name);
    }
    return img;
}

static void load_ghost_textures(const char *right, const char *left, ppm_image_t **arr) {
    arr[0] = load_texture_or_warn(right);
    arr[1] = load_texture_or_warn(left);
}

void init_resources() {
    // Load game over screen
    gameover_screen = load_texture_or_warn("gameover.ppm");

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
}

void cleanup_resources() {
    // Free game over screen
    if (gameover_screen) {
        free_ppm(gameover_screen);
        gameover_screen = NULL;
    }

    // Free textures
    free_ppm(wall_texture);
    free_ppm(pellet_texture);
    free_ppm(power_pellet_texture);
    free_ppm(vulnerable_ghost_texture);
    free_ppm(eaten_ghost_texture);

    for (int i = 0; i < 5; i++) {
        free_ppm(pacman_textures[i]);
    }
    for (int i = 0; i < 2; i++) {
        free_ppm(ghost_red_textures[i]);
        free_ppm(ghost_orange_textures[i]);
        free_ppm(ghost_blue_textures[i]);
        free_ppm(ghost_pink_textures[i]);
    }
}

ppm_image_t *get_gameover_screen() {
    return gameover_screen;
}

ppm_image_t *get_wall_texture() {
    return wall_texture;
}

ppm_image_t **get_pacman_textures() {
    return pacman_textures;
}

ppm_image_t **get_ghost_textures(GhostType type) {
    switch (type) {
        case GHOST_TYPE_BLINKY: return ghost_red_textures;
        case GHOST_TYPE_PINKY: return ghost_pink_textures;
        case GHOST_TYPE_INKY: return ghost_blue_textures;
        case GHOST_TYPE_CLYDE: return ghost_orange_textures;
        default: return NULL;
    }
}

ppm_image_t *get_pellet_texture() {
    return pellet_texture;
}

ppm_image_t *get_power_pellet_texture() {
    return power_pellet_texture;
}

ppm_image_t *get_vulnerable_ghost_texture() {
    return vulnerable_ghost_texture;
}

ppm_image_t *get_eaten_ghost_texture() {
    return eaten_ghost_texture;
}
