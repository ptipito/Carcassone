#ifndef DEF_CARC_GAME
#define DEF_CARC_GAME
#include <stdlib.h>
#include <stdio.h>
#include "model/board/carcassone_playboard.h"
#include "model/board/rim.h"

/*Contains the game representation
   - whole playboard as graph
   - the list of playable nodes
*/
typedef struct Carc_Game{
    Carc_Playboard_Origin* playboard;
    CP_Rim* playable;
} Carc_Game;

Carc_Game* CG_initiate_game(char*);
void CG_free_game(Carc_Game*);
int CG_rim_to_playboard_update_one_side(CP_Rim*, Carc_Playboard_Location, Carc_Playboard_Node*, CP_Connect_Side);
int CG_node_transfer_rim_to_playboard(Carc_Game*,Carc_Playboard_Location, Carc_Playboard_Node*);
Carc_Playboard_Node* CG_play_tile_in(Carc_Game*, Carc_Playboard_Location, Carc_Tile*);

#endif // DEF_CARC_GAME
