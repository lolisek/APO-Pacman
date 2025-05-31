#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../gui/ppm_loader.h"
#include "../entities/ghost.h"

/**
 * @brief Initializes all game resources by loading textures.
 */
void init_resources();

/**
 * @brief Frees all loaded game resources.
 */
void cleanup_resources();

/**
 * @brief Retrieves the game over screen texture.
 *
 * @return ppm_image_t* Pointer to the game over screen texture.
 */
ppm_image_t *get_gameover_screen();

/**
 * @brief Retrieves the wall texture.
 *
 * @return ppm_image_t* Pointer to the wall texture.
 */
ppm_image_t *get_wall_texture();

/**
 * @brief Retrieves the Pacman textures.
 *
 * @return ppm_image_t** Array of pointers to Pacman textures.
 */
ppm_image_t **get_pacman_textures();

/**
 * @brief Retrieves the textures for a specific ghost type.
 *
 * @param type The type of ghost (e.g., Blinky, Pinky, Inky, Clyde).
 * @return ppm_image_t** Array of pointers to the ghost's textures.
 */
ppm_image_t **get_ghost_textures(GhostType type);

/**
 * @brief Retrieves the pellet texture.
 *
 * @return ppm_image_t* Pointer to the pellet texture.
 */
ppm_image_t *get_pellet_texture();

/**
 * @brief Retrieves the power pellet texture.
 *
 * @return ppm_image_t* Pointer to the power pellet texture.
 */
ppm_image_t *get_power_pellet_texture();

/**
 * @brief Retrieves the vulnerable ghost texture.
 *
 * @return ppm_image_t* Pointer to the vulnerable ghost texture.
 */
ppm_image_t *get_vulnerable_ghost_texture();

/**
 * @brief Retrieves the eaten ghost texture.
 *
 * @return ppm_image_t* Pointer to the eaten ghost texture.
 */
ppm_image_t *get_eaten_ghost_texture();

/**
 * @brief Loads a texture from the given resource name and logs a warning if it fails.
 *
 * @param resource_name Name of the resource file.
 * @return ppm_image_t* Pointer to the loaded texture, or NULL on failure.
 */
ppm_image_t *load_texture_or_warn(const char *resource_name);

#endif // RESOURCE_MANAGER_H
