#ifndef DEF_CARC_MACRO_CONST
#define DEF_MACRO_CONST
#include "model/board/constructions.h"
#include "model/board/tile.h"
#include "model/board/carcassone_playboard.h"
#include "utils.h"

Carc_Macro_Construct* CBMC_new(Carc_Tile_Node**);
void CBMC_free(Carc_Macro_Construct*);
Carc_Macro_Construct_List* CBMCList_new(Carc_Macro_Construct**);
void CBMCList_free(Carc_Macro_Construct_List*);
int CBMCList_in(Carc_Macro_Construct_List*, Carc_Macro_Construct**);
Carc_Macro_Construct_List* CBMCList_get_pos_of(Carc_Macro_Construct_List*, Carc_Macro_Construct**);
Carc_Macro_Construct_List* CBMCList_rm(Carc_Macro_Construct_List**, Carc_Macro_Construct**);
int CBMC_add_pawn(Carc_Macro_Construct*, Carc_Pawn**);
int CBMC_add_node(Carc_Macro_Construct*, Carc_Tile_Node**);
int CBMC_node_in(Carc_Macro_Construct*, Carc_Tile_Node**);
Carc_Macro_Construct* CBMC_get_node_construct(Carc_Macro_Construct_List*, Carc_Tile_Node**);
Carc_Macro_Construct_List* CBMCList_append(Carc_Macro_Construct_List*, Carc_Macro_Construct**);
Carc_Macro_Construct_List* CBMC_get_tile_macro_constructions(Carc_Tile*);

int CBMC_rm_from_rim(Carc_Macro_Construct*, Carc_Tile_Node**);
int CBMC_rm_list_from_rim(Carc_Macro_Construct*, Carc_Tile_Node_List*);
int CBMC_transfer_rim(Carc_Macro_Construct*, Carc_Macro_Construct*);
int CBMC_enrich_with(Carc_Macro_Construct*, Carc_Macro_Construct*);
int CBMC_merge_const(Carc_Macro_Construct*, Carc_Macro_Construct**, Carc_Tile_Node_List*, Carc_Tile_Node_List*);
Carc_Macro_Construct** CBMC_get_tile_constructs_per_node(Carc_Macro_Construct_List*, Carc_Tile*);


#endif // DEF_CARC_MACRO_CONST
