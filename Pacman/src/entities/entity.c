/*******************************************************************
  APO-Pacman Project for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  entity.c      - Core entity management

  Authors: Duc Minh Nguyen, Veronica Marie Ohrazdová

  License: MIT License

  Copyright (c) 2025 Duc Minh Nguyen, Veronica Marie Ohrazdová
 *******************************************************************/

#include "../../include/entities/entity.h"
#include "../../include/core/game_state.h"
#include <stdio.h>

void entity_init(Entity *entity, EntityType type, void (*update)(void *specific, GameState *gamestate))
{
    entity->type = type;
    entity->update = update;
}

void entity_update(Entity *entity, GameState *gamestate)
{
    if (entity->update)
    {
        entity->update(entity, gamestate);
    }
}
