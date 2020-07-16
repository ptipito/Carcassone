#ifndef DEF_CARC_TURN
#define DEF_CARC_TURN

#include <stdlib.h>
#include <stdio.h>
#include "model/player/player.h"
#include "model/board/tile.h"

typedef struct {
    Carc_Tile* tile;
    Carc_Player* player;
} Carc_Turn;

Carc_Turn CGT_new(Carc_Tile*, Carc_Player*);
void CGT_set_player(Carc_Turn* turn, Carc_Player*);
void CGT_set_tile(Carc_Turn* turn, Carc_Tile*);

#endif // DEF_CARC_TURN
