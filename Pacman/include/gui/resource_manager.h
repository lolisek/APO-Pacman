#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../gui/ppm_loader.h"
#include "../entities/ghost.h"

// Initialize and cleanup resources
void init_resources();
void cleanup_resources();

// Accessors for resources
ppm_image_t *get_gameover_screen();
ppm_image_t *get_wall_texture();
ppm_image_t **get_pacman_textures();
ppm_image_t **get_ghost_textures(GhostType type);
ppm_image_t *get_pellet_texture();
ppm_image_t *get_power_pellet_texture();
ppm_image_t *get_vulnerable_ghost_texture();
ppm_image_t *get_eaten_ghost_texture();

#endif // RESOURCE_MANAGER_H
