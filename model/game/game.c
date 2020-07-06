#include "model/game/game.h"

Carc_Game* CGG_initiate_game(char* filename, int nb_players){
    ///Init a game struct for \nb_players and with start_tile the tile
    ///described in the file \filename
    Carc_Game* game=malloc(sizeof(Carc_Game));
    if(game==NULL){
        fprintf(stderr,"ERROR: couldn't allocate memory for new game object\n");
        carcassone_error_quit(ERR_MEM_ALLOC,NULL);
    }
    if(nb_players <= 1 || nb_players > NB_MAX_PLAYERS){
        fprintf(stderr,"ERROR: game cannot be created for %d players. This game must be played by 2 to %d players\n"
                      ,nb_players, NB_MAX_PLAYERS);
        return NULL;
    }
    Carc_Tile* start_tile=CBT_new_tile_from_file(filename);
    Carc_Playboard_Origin* playboard_origin=CBP_init_playboard(start_tile);
    Carc_Playboard_Node** pointer_on_origin_node=&(playboard_origin->node);
    int i=0;

    game->playboard = playboard_origin;
    game->playable = CBRim_initiate(pointer_on_origin_node);

    game->players[0] = NULL;//Index 0 never used. Only indices from 1 are used, so that they are aligned with player IDs
    for(i=1;i<=nb_players;i++){
        game->players[i] = CPPlayer_init_player(i,i-1);//color enum starts at 0 while player id enum starts at 1
    }
    //Set non existing players to NULL
    for(i=nb_players+1;i<=NB_MAX_PLAYERS;i++){
        game->players[i] = NULL;
    }
    //Set initial constructs
    game->constructs = CBMC_get_tile_macro_constructions(start_tile);

    return game;
}

void CGG_free_game(Carc_Game* game){
    if(game!=NULL){
        CBMCList_free(game->constructs);
        CBP_free_playboard(game->playboard);
        CBRim_free(game->playable);
        int i;
        for(i=0;i<=NB_MAX_PLAYERS;i++){
            CPPlayer_free_player(game->players[i]);
        }
    }
    free(game);
    game = NULL;
}

int CGG_rim_to_playboard_update_one_side(Carc_Rim* rim, Carc_Playboard_Location coord, Carc_Playboard_Node** playboard_node, Carc_Playboard_Connect_Side neighbor_side){
    ///This function takes information from rim_node to duplicate it in playboard_node and perform the right rim update for the given neighbor_side.
    ///It is aimed to be used to insert a node on the playboard in the location of rim_node (which belongs to the rim).
    ///This function aims to check on one side if the neighboring node already exists either on the playboard or in the rim in order to perform
    ///the relevant updates (neighbor references for nodes as well as updating existing node in the rim to include this new neighbor expanding the
    ///rim with a new node on this side)
    if(rim==NULL || playboard_node==NULL || *playboard_node==NULL){
        fprintf(stderr,"Wrong use of CB_rim_to_playboard_update_one_side: an input is null\n");
        return 1;
    }
    Carc_Playboard_Node* rim_node = CB_Rim_find_by_location(rim,coord);
    if(rim_node==NULL){
        fprintf(stderr,"Cannot transfer node from rim to playboard: location not in the rim\n");
        return 2;
    }
    if(CBP_Location_cmp((*playboard_node)->node_coordinates,coord)!=0){
        fprintf(stderr,"Wrong use of CB_rim_to_playboard_update_one_side: input node coordinates do not correspond to input location\n");
        return 3;
    }
    if(CBP_is_neighbor_null(rim_node,neighbor_side)){//The neighboring node was not yet played (i.e. does not belong to the playboard)
        Carc_Playboard_Location neighbor_loc = CBP_get_neighbor_loc(coord,neighbor_side);
        Carc_Playboard_Node* neighbor = CB_Rim_find_by_location(rim,neighbor_loc);
        if(neighbor!=NULL){
            //Update the neighboring rim node neighbors attribute to include the newly played node
            CBP_set_neighbor(neighbor,CBP_get_opposite_side(neighbor_side),playboard_node);
        } else {
            //Create a new node for the rim
            neighbor = CBP_create_rim_neigh_for(playboard_node,neighbor_side);
            CBRim_insert_playboard_node(rim,neighbor);
        }
    } else {
        //The neighbor is already on the playboard=>update its neighbors attribute
        CBP_set_neighbor(*playboard_node,neighbor_side,rim_node->neighbors[neighbor_side]);
        CBP_set_neighbor(CBP_get_neighbor(rim_node,neighbor_side),CBP_get_opposite_side(neighbor_side),playboard_node);
    }
    return 0;
}

int CGG_node_transfer_rim_to_playboard(Carc_Game* game, Carc_Playboard_Location coord, Carc_Playboard_Node** playboard_node){
    ///Replace a node in the rim by a node on the playboard. In practise it removes the node with coordinates \coord
    ///from the \game rim and add a node in the \game playboard; updating the neighbors on the playboard and the rim
    ///and creating the new needed rim node(s)
    if(game==NULL || playboard_node==NULL || *playboard_node==NULL){
        fprintf(stderr,"Error on call CGG_node_transfer_rim_to_playboard: one argument is NULL\n");
        return 1;
    }
    if(CB_Rim_find_by_location(game->playable,coord)==NULL){
        fprintf(stderr,"Error on call CGG_node_transfer_rim_to_playboard: input location not in game's rim\n");
        return 2;
    }
    if(CBP_Location_cmp((*playboard_node)->node_coordinates,coord)!=0){
        fprintf(stderr,"Error on call CGG_node_transfer_rim_to_playboard: coordinates of input node do not correspond to input location\n");
        return 3;
    }
    int result=0;
    //Update the rim & playboard_node neighbors references. NB: As at least one neighbor of rim_node is a
    //node on the playboard, the new playboard_node is incidentally inserted in the game playboard graph
    result = CGG_rim_to_playboard_update_one_side(game->playable,coord,playboard_node,CPCS_UP);
    if(result!=0){
        return result;
    }
    result = CGG_rim_to_playboard_update_one_side(game->playable,coord,playboard_node,CPCS_RIGHT);
    if(result!=0){
        return result;
    }
    result = CGG_rim_to_playboard_update_one_side(game->playable,coord,playboard_node,CPCS_DOWN);
    if(result!=0){
        return result;
    }
    result = CGG_rim_to_playboard_update_one_side(game->playable,coord,playboard_node,CPCS_LEFT);
    if(result!=0){
        return result;
    }
    //The location does not belong to the playboard rim anymore
    result = !(CBRim_remove_by_node_coordinates(game->playable,coord));
    return result;
}

Carc_Playboard_Node* CGG_play_tile_in(Carc_Game* game, Carc_Playboard_Location loc, Carc_Tile* tile){
    //To avoid memory leak, a node from a rim is freed when removed from the rim. This means that to transfer a node
    //from the playable rim to the playboard, a new node must be created. This node shall be freed when the playboard is.
    if(tile==NULL || game==NULL){
        fprintf(stderr,"Error in CGG_play_tile_in: an input is NULL\n");
        return NULL;
    }
    if(game->playable==NULL){
        fprintf(stderr,"Error in CGG_play_tile_in: rim is NULL\n");
        return NULL;
    }
    Carc_Playboard_Node *rim_node = CB_Rim_find_by_location(game->playable, loc),
                        *playboard_node = NULL,
                        **pointer_on_playboard_node=NULL;
    //TODO combine both if in a function
    if(rim_node!=NULL){//A tile can be played in loc
        playboard_node = CBP_new_playboard_node(tile,loc);
        //encapsulate the if conditions in a func "can_insert_tile_in"
        if(CBP_connect_is_possible(playboard_node,CPCS_UP,CBP_get_neighbor(rim_node,CPCS_UP))==0
           || CBP_connect_is_possible(playboard_node,CPCS_DOWN,CBP_get_neighbor(rim_node,CPCS_DOWN))==0
           || CBP_connect_is_possible(playboard_node,CPCS_LEFT,CBP_get_neighbor(rim_node,CPCS_LEFT))==0
           || CBP_connect_is_possible(playboard_node,CPCS_RIGHT,CBP_get_neighbor(rim_node,CPCS_RIGHT))==0){
            //The tile cannot be inserted
            CBP_free_playboard_node(playboard_node);
            return NULL;
        }
        //Encapsulate if/else a Function "insert_tile"
        pointer_on_playboard_node = malloc(sizeof(*pointer_on_playboard_node));//necessary because playboard_node is a local variable. Thus do not use & to get the pointer's address
        if(pointer_on_playboard_node!=NULL){
            *pointer_on_playboard_node = playboard_node;
            CGG_node_transfer_rim_to_playboard(game,loc,pointer_on_playboard_node);
            CGG_update_game_constructs(game,playboard_node);
            free(pointer_on_playboard_node);
            return playboard_node;
        } else{
            fprintf(stderr,"ERROR: cannot allocate memory in CGG_play_tile_in\n");
            CBP_free_playboard_node(playboard_node);
            return NULL;
        }
    }
    //else loc is not playable (by definition of the game rim)
    return NULL;
}

Carc_Pawn* CGG_play_pawn_in(Carc_Player* player, Carc_Pawn_Type pawn_type, Carc_Playboard_Node* node, Carc_Tile_Location loc_on_tile,
                            Carc_Macro_Construct* tile_constructs[TILE_NR_LOCATIONS], Carc_Game* game){
    if(pointer_is_not_null(node,1) && pointer_is_not_null(tile_constructs,1)
       && pointer_is_not_null(game,1)
       ){
        Carc_Tile* tile=node->node;
        Carc_Pawn* pawn=CPPawn_new_pawn(player,pawn_type);
        Carc_Macro_Construct* construct_in_loc=tile_constructs[loc_on_tile];
        if(CGG_can_play_pawn_in(pawn,node,construct_in_loc,loc_on_tile)){
            if(CBT_add_pawn(pawn, tile, loc_on_tile)==FUNC_SUCCESS){
                //Using CBMC_get_node_construct could fail since the rim is already altered (so possibly not on the current tile anymore) when a pawn is played
                CBMC_add_pawn(construct_in_loc,&pawn);
            } else{
                //The pawn can't be played
                CPPawn_free_pawn(pawn);
                pawn = NULL;
            }
        } else{
            //The pawn can't be played
            CPPawn_free_pawn(pawn);
            pawn = NULL;
        }
        return pawn;
    }
    return NULL;
}

int CGG_can_play_pawn_in(Carc_Pawn* pawn, Carc_Playboard_Node* node, Carc_Macro_Construct* c, Carc_Tile_Location loc){
    ///Function to consider if a pawn can be played in a given tile location. It is based on
    ///CBT_node_type_matches_pawn_type and add special rules, such as is the global
    ///construction already occupied or when to play the architect
    int allowed_to_play=0;
    if(pointer_is_null(node,1) || pointer_is_null(c,1) || pointer_is_null(pawn,0)){
        return 0;
    }
    Carc_Tile_Node* tile_node=CBT_get_node_from_loc(node->node,loc);
    if(tile_node!=NULL
       && CBT_node_type_matches_pawn_type(tile_node->node_type,pawn->type,loc)
       && CPPlayer_can_play_pawn(pawn->owner,pawn->type)
       ){
        allowed_to_play = 1;
        switch(pawn->type){
            case PAWN_ARCHITECT:
                if(!CBMC_player_has_pawns_in(c,pawn->owner,PAWN_NORMAL))
                    allowed_to_play = 0;
                break;
            case PAWN_PIG:
                if(!CBMC_player_has_pawns_in(c,pawn->owner,PAWN_NORMAL))
                    allowed_to_play = 0;
                break;
            case PAWN_NORMAL:
                if(!CBMC_has_no_pawns(*c))
                    allowed_to_play = 0;
                break;
            case PAWN_DOUBLE:
                if(!CBMC_has_no_pawns(*c))
                    allowed_to_play = 0;
                break;
            case PAWN_BISHOP:
                if(!CBMC_has_no_pawns(*c))
                    allowed_to_play = 0;
                break;
        }
    }
    return allowed_to_play;
}

int CGG_merge_constructs_with_neighbor(Carc_Game* game, Carc_Playboard_Node* node, Carc_Playboard_Connect_Side neigh_side,
                                       Carc_Macro_Construct_List* tile_constructs){
    ///Merge the construction(s) present on the edge of node and on the edge of its neigh on side \neigh_side. The node
    ///construct are provided as input \tile_constructs to avoid recreating the list of constructs with different pointer references
    if(pointer_is_null(game,1) || pointer_is_null(node,1) || pointer_is_null(tile_constructs,1)){
        return FUNC_FAIL;
    }

    int res=FUNC_SUCCESS, cpt=0;
    Carc_Playboard_Node* neighbor=CBP_get_neighbor(node,neigh_side);
    Carc_Tile_Node_List *node_connect_points=CBP_get_edge_nodes(node,neigh_side),
                        *cur_node=NULL;
    Carc_Macro_Construct *cur_construct=NULL;
    if(pointer_is_null(neighbor,0)){
        //If no neighbors, no merger is possible.
        //Adding the constructs that could not be merged shall be done in the parent function
        return FUNC_SUCCESS;
    }

    Carc_Tile_Node_List *neighbor_connect_points=CBP_get_edge_nodes(neighbor,CBP_get_opposite_side(neigh_side)),
                        *neigh_cur_node=neighbor_connect_points,
                        *cur_construct_rim=NULL,
                        *neigh_cur_construct_rim=NULL;
    Carc_Macro_Construct *next_construct=NULL,
                         *neigh_cur_construct=NULL;
    Carc_Macro_Construct_List* neigh_cur_construct_pos=NULL;
    //Get the constructs on the edge to node
    if(pointer_is_not_null(node_connect_points,1) && pointer_is_not_null(neighbor_connect_points,1)){
        cur_node = node_connect_points;
        while(cpt<TILE_NR_LOCATIONS_ON_ONE_EDGE){
            cpt++;
            cur_construct_rim = CBTList_new(&(cur_node->node));
            cur_construct = CBMC_get_node_construct(tile_constructs,&(cur_node->node));
            next_construct = CBMC_get_node_construct(tile_constructs,&(cur_node->next->node));
            neigh_cur_construct_rim = CBTList_new(&(neigh_cur_node->node));
            neigh_cur_construct = CBMC_get_node_construct(game->constructs,&(neigh_cur_node->node));
            while(next_construct==cur_construct && cpt<TILE_NR_LOCATIONS_ON_ONE_EDGE){
                //Extend the current construct to its limit to reach the next construct on the next iteration of the cpt loop
                cur_construct = next_construct;
                cur_node = cur_node->next;
                CBTList_append(cur_construct_rim,&(cur_node->node));//build the connection points list
                neigh_cur_node = neigh_cur_node->next;
                CBTList_append(neigh_cur_construct_rim,&(neigh_cur_node->node));//build the connection points list
                next_construct = (cur_node==NULL ? NULL : CBMC_get_node_construct(tile_constructs,&(cur_node->node)));
                cpt++;
            }
            //Merging (as below) the neighboring construction into the construction of the new node has 2 advantages:
            //    -Avoiding merging twice the same structure od the new node (e.g. consider a tile with a field separated by a path only
            //      on one edge. The field would be considered twice by the above loop. Concrete example: the tile cloister_path)
            //    - Allowing to merge two constructions already existing in the game without complicated considerations. Same example as
            //      above. Let's consider merging the path of cloister_path with the path of the starting tile. The 2 fields of the starting
            //      tile are now bounded by the one of cloister_path. Merging the new tile in existing constructions would make hard to
            //      detect this link, as opposed with merging the neighbor in the new tile.
            neigh_cur_construct_pos = CBMCList_rm(&(game->constructs),&neigh_cur_construct);//Rm before merge as the construct is freed after the mergure
            CBMC_merge_const(cur_construct,&neigh_cur_construct,cur_construct_rim,neigh_cur_construct_rim);
            CBMCList_append(game->constructs,&cur_construct);//Keep track of the mergure's result
            neigh_cur_construct_pos->construct = NULL;//Avoid double free
            CBMCList_free(neigh_cur_construct_pos);

            CBTList_free(cur_construct_rim);
            CBTList_free(neigh_cur_construct_rim);
            //set for next iteration
            if(cpt<TILE_NR_LOCATIONS_ON_ONE_EDGE && node!=NULL && neigh_cur_node!=NULL){
                cur_node = cur_node->next;
                neigh_cur_node = neigh_cur_node->next;
                cur_construct = next_construct;
            }
            //Error case handling
            if(node==NULL || neigh_cur_node==NULL){
                if(cpt<TILE_NR_LOCATIONS_ON_ONE_EDGE){
                    fprintf(stderr,"Warning: Null node found in a construct. All following nodes discarded\n");
                    cpt = TILE_NR_LOCATIONS_ON_ONE_EDGE;
                    res = FUNC_FAIL;
                }
            }
        }
    }

    return res;
}

int CGG_update_game_constructs(Carc_Game* game, Carc_Playboard_Node* node){
    ///Update the game constructs attribute of \game to incorporate the constructs
    ///of the newly played node \node.
    if(pointer_is_null(game,1))
        return FUNC_FAIL;
    if(pointer_is_null(node,0))
        return FUNC_SUCCESS;
    int success=1, i=0, j=0;
    Carc_Playboard_Node* cur_neighbor=NULL;
    Carc_Tile_Node* cur_tile_node=NULL;
    Carc_Macro_Construct* cur_construct=NULL;
    Carc_Macro_Construct_List *node_constructs=CBMC_get_tile_macro_constructions(node->node),
                              *cur_for_iteration=NULL;
    //First merge the tile constructions to the playboard constructions (for the sides where there are neighbors)
    for(i=0;i<CBP_MAX_NEIGHBORS;i++){
        cur_neighbor = CBP_get_neighbor(node,i);
        if(cur_neighbor!=NULL){
            success = (success && (CGG_merge_constructs_with_neighbor(game,node,i,node_constructs)==FUNC_SUCCESS));
        }
    }
    //Consider the sides without neighbors. Now that the sides with neighbors have been merged, 2 cases for each tile node:
    for(i=0;i<CBP_MAX_NEIGHBORS;i++){
        cur_neighbor = CBP_get_neighbor(node,i);
        if(cur_neighbor==NULL){
            for(j=0;j<TILE_NR_LOCATIONS_ON_ONE_EDGE;j++){
                cur_tile_node = CBT_get_node_from_loc(node->node,i*TILE_NR_LOCATIONS_ON_ONE_EDGE + j);
                cur_construct = CBMC_get_node_construct(game->constructs,&cur_tile_node);
                if(cur_construct==NULL){
                    //The construct is not in the game constructs and has to be added
                    cur_construct = CBMC_get_node_construct(node_constructs,&cur_tile_node);
                    CBMCList_append(game->constructs,&cur_construct);
                }
                //else the construct is in the game constructs and the rim has already been adjusted to the rim
                //of the tile construct during the merge in the first loop.
            }
        }
    }
    //Consider stand alone center constructs (for instance cloisters)
    //This also catches edge cases of construction on the edge not added to the game constructs (shouldn't happen but, if so, will avoid memory leak)
    cur_for_iteration = node_constructs;
    while(cur_for_iteration!=NULL){
        if(!CBMCList_in(game->constructs,&(cur_for_iteration->construct))){
            CBMCList_append(game->constructs,&(cur_for_iteration->construct));
        }
        cur_for_iteration = cur_for_iteration->next;
    }
    //No need to (and DO NOT) free node_constructs as all constructs are in game->constructs for further use
    if(success)
        return FUNC_SUCCESS;
    return FUNC_FAIL;
}
