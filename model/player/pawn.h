#ifndef DEF_CARC_PAWN
#define DEF_CARC_PAWN
#include "model/board/tile.h"
#include "model/board/carcassone_playboard.h"
#include "model/player/player.h"


typedef struct {
    Carc_Pawn_Type type;
    Carc_Player_ID owner;
    int weight;
    Carc_Playboard_Node *on_playboard_node;
    Carc_Tile_Node *on_tile_node;
} Carc_Pawn;

#endif // DEF_CARC_PAWN
