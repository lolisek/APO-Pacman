#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdint.h>

#define MAX_SCORES 100
#define SCORE_LINE_LENGTH 64

/**
 * @brief Structure to represent the scoreboard.
 */
typedef struct {
    char lines[MAX_SCORES][SCORE_LINE_LENGTH]; // Array of score lines
    int total_lines;                           // Total number of lines in the scoreboard
    int scroll_offset;                         // Current scroll offset
} scoreboard_t;

#endif // SCOREBOARD_H