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

#endif // DISPLAY_SCOREBOARD_H