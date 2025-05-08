#include "../include/core/game.h"
#include "../include/core/renderer.h"
#include "../include/utils/timer.h"
#include "../include/core/input.h"
#include <stdio.h>

void run_game_loop()
{
    GameState game_state;
    init_game_state(&game_state);

    // Main game loop

    Timer frame_timer;
    int running = 1;

    while (running)
    {
        timer_start(&frame_timer);

        // Handle input
        handle_input(&game_state, &running);

        // Update game state
        update_game_state(&game_state);

        // Render the game
        render(&game_state);

        // Control frame rate (e.g., 60 FPS)
        timer_stop(&frame_timer);
        uint64_t elapsed_ms = timer_get_elapsed_ms(&frame_timer);
        if (elapsed_ms < 16)
        {
            timer_sleep_ms(16 - elapsed_ms);
        }
    }
}

void update_game_state(GameState *game_state)
{
    // Update game state logic here
    // For example, update Pacman and ghost positions, check for collisions, etc.
    entity_update((Entity *)&game_state->pacman);

    // entity_update((Entity *)&game_state->ghosts[0]);

    // handle collisions with pellets, ghosts, etc.

    check_collisions(game_state);

    printf("Pacman position: (%d, %d)\n", game_state->pacman.base.position.x, game_state->pacman.base.position.y);
}

void check_collisions(GameState *game_state)
{
    // Check for collisions between Pacman and walls
    if (!map_is_walkable(&game_state->map, game_state->pacman.base.position.x, game_state->pacman.base.position.y))
    {
        // Handle collision with wall
        // Reset Pacman's position or prevent movement
        game_state->pacman.base.position.x -= game_state->pacman.base.direction.x * game_state->pacman.base.speed;
        game_state->pacman.base.position.y -= game_state->pacman.base.direction.y * game_state->pacman.base.speed;
    }

    // // Check for collisions between Pacman and ghosts
    // for (int i = 0; i < NUM_GHOSTS; i++)
    // {
    //     if (game_state->ghosts[i].base.position.x == game_state->pacman.base.position.x &&
    //         game_state->ghosts[i].base.position.y == game_state->pacman.base.position.y)
    //     {
    //         // Handle collision with ghost
    //         if (game_state->ghosts[i].mode == GHOST_MODE_FRIGHTENED)
    //         {
    //             // Ghost is frightened, Pacman eats the ghost
    //             game_state->score += 200; // Increase score
    //             game_state->ghosts[i].mode = GHOST_MODE_EATEN; // Change ghost mode to eaten
    //         }
    //         else
    //         {
    //             // Pacman loses a life
    //             game_state->lives--;
    //             if (game_state->lives <= 0)
    //             {
    //                 game_state->game_over = true; // Game over
    //             }
    //         }

    //         // Reset Pacman's position
    //         game_state->pacman.base.position.x = PACMAN_START_X;
    //         game_state->pacman.base.position.y = PACMAN_START_Y;
    //         // Reset ghost positions
    //         for (int j = 0; j < NUM_GHOSTS; j++)
    //         {
    //             game_state->ghosts[j].base.position.x = GHOST_START_X;
    //             game_state->ghosts[j].base.position.y = GHOST_START_Y;
    //         }

    //         // Reset frightened timer
    //         game_state->frightened_timer = 0;

    //     }
    // }
    // // Check for collisions with pellets
    // for (int y = 0; y < game_state->map.height; y++)
    // {
    //     for (int x = 0; x < game_state->map.width; x++)
    //     {
    //         if (game_state->map.tiles[y][x].type == TILE_PELLET)
    //         {
    //             if (game_state->pacman.base.position.x == x && game_state->pacman.base.position.y == y)
    //             {
    //                 // Pacman eats the pellet
    //                 game_state->score += 10; // Increase score
    //                 game_state->map.tiles[y][x].type = TILE_EMPTY; // Remove the pellet from the map
    //             }
    //         }
    //     }
    // }
}