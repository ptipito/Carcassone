#include "model/game/turn.h"
///TO_TEST all
Carc_Turn CGT_new(Carc_Tile* tile, Carc_Player* player){
    Carc_Turn turn;
    turn.player = player;
    turn.tile = tile;
    return turn;
}

void CGT_set_player(Carc_Turn* turn, Carc_Player* player){
    turn->player = player;
}

void CGT_set_tile(Carc_Turn* turn, Carc_Tile* tile){
    turn->tile = tile;
}

