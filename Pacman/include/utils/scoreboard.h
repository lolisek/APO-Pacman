/**
 * @file scoreboard.h
 * @brief Provides the scoreboard structure and related constants for managing game scores.
 */

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdint.h>

/**
 * @brief Maximum number of scores that can be stored in the scoreboard.
 */
#define MAX_SCORES 100

/**
 * @brief Maximum length of a single score line in the scoreboard.
 */
#define SCORE_LINE_LENGTH 64

/**
 * @brief Structure to represent the scoreboard.
 */
typedef struct {
    char lines[MAX_SCORES][SCORE_LINE_LENGTH]; /**< Array of score lines. */
    int total_lines;                           /**< Total number of lines in the scoreboard. */
    int scroll_offset;                         /**< Current scroll offset. */
} scoreboard_t;

#endif // SCOREBOARD_H