#ifndef DISPLAY_SCOREBOARD_H
#define DISPLAY_SCOREBOARD_H

#include "../utils/constants.h"
#include "../microzed/mzapo_parlcd.h"
#include "../microzed/mzapo_peri.h"
#include "../microzed/mzapo_phys.h"
#include "../microzed/mzapo_regs.h"
#include "../microzed/serialize_lock.h"
#include "../utils/timer.h"
#include "../utils/scoreboard.h"
#include <stdint.h>

// Ensure the scoreboard_t and font_descriptor_t types are defined
#include "../fonts/font_types.h" // Assuming scoreboard_t is defined here

/**
 * @brief Draws the scoreboard on the framebuffer.
 *
 * @param sb Pointer to the scoreboard structure.
 * @param framebuffer Pointer to the framebuffer where the scoreboard will be drawn.
 * @param font Pointer to the font descriptor used for rendering text.
 */
void draw_scoreboard(scoreboard_t *sb, uint16_t *framebuffer, const font_descriptor_t *font);

/**
 * @brief Handles scoreboard input and interaction.
 *
 * @param sb Pointer to the scoreboard structure.
 * @param framebuffer Pointer to the framebuffer for rendering updates.
 */
void handle_scoreboard(scoreboard_t *sb, uint16_t *framebuffer);

/**
 * @brief Initializes the scoreboard structure.
 *
 * @param sb Pointer to the scoreboard structure.
 */
void init_scoreboard(scoreboard_t *sb);

/**
 * @brief Loads scores from the scores file into the scoreboard structure.
 *
 * @param sb Pointer to the scoreboard structure.
 * @return int 1 if successful, 0 otherwise.
 */
int load_scores(scoreboard_t *sb);

/**
 * @brief Compares two score lines for sorting in descending order.
 *
 * @param a Pointer to the first score line.
 * @param b Pointer to the second score line.
 * @return int Difference between the scores (negative if b > a).
 */
int compare_scores(const void *a, const void *b);

/**
 * @brief Saves a new score to the scores file.
 *
 * @param name Name of the player.
 * @param score Player's score.
 * @return int 0 if successful, -1 otherwise.
 */
int save_score(const char *name, int score);

#endif // DISPLAY_SCOREBOARD_H