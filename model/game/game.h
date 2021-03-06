#ifndef DEF_CARC_GAME
#define DEF_CARC_GAME
#include <stdlib.h>
#include <stdio.h>
#include "model/carcassone_errors.h"
#include "model/board/carcassone_playboard.h"
#include "model/board/rim.h"
#include "model/board/macro_constructions.h"
#include "model/player/player.h"

/*Contains the game representation
   - whole playboard as graph
   - the list of playable nodes
*/
typedef struct Carc_Game{
    Carc_Playboard_Origin* playboard;
    Carc_Rim* playable;
    Carc_Macro_Construct_List* constructs;
    Carc_Player* players[NB_MAX_PLAYERS+1];//index 0 should remain NULL. Other indices are aligned with player id (starting from 1).
} Carc_Game;

Carc_Game* CGG_initiate_game(char*, int);
void CGG_free_game(Carc_Game*);
int CGG_rim_to_playboard_update_one_side(Carc_Rim*, Carc_Playboard_Location, Carc_Playboard_Node**, Carc_Playboard_Connect_Side);
int CGG_node_transfer_rim_to_playboard(Carc_Game*,Carc_Playboard_Location, Carc_Playboard_Node**);
Carc_Playboard_Node* CGG_play_tile_in(Carc_Game*, Carc_Playboard_Location, Carc_Tile*);
Carc_Pawn* CGG_play_pawn_in(Carc_Player*, Carc_Pawn_Type, Carc_Playboard_Node*, Carc_Tile_Location, Carc_Macro_Construct**, Carc_Game*);
int CGG_can_play_pawn_in(Carc_Pawn*, Carc_Playboard_Node*, Carc_Macro_Construct*, Carc_Tile_Location);
int CGG_merge_constructs_with_neighbor(Carc_Game*, Carc_Playboard_Node*, Carc_Playboard_Connect_Side, Carc_Macro_Construct_List*);
int CGG_update_game_constructs(Carc_Game*, Carc_Playboard_Node*);

#endif // DEF_CARC_GAME
