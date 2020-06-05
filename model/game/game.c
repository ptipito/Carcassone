#include "model/game/game.h"

Carc_Game* CGG_initiate_game(char* filename, int nb_players){
    Carc_Game *game = malloc(sizeof(Carc_Game));
    if(game==NULL){
        fprintf(stderr,"ERROR: couldn't allocate memory for new game object\n");
        carcassone_error_quit(ERR_MEM_ALLOC,NULL);
    }
    if(nb_players <= 1 || nb_players > NB_MAX_PLAYERS){
        fprintf(stderr,"ERROR: game cannot be created for %d players. This game must be played by 2 to %d players\n"
                      ,nb_players, NB_MAX_PLAYERS);
        return NULL;
    }
    Carc_Tile *start_tile = CBT_new_tile_from_file(filename);
    Carc_Playboard_Origin *playboard_origin = CBP_init_playboard(start_tile);
    Carc_Playboard_Node** pointer_on_origin_node = &(playboard_origin->node);
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

    return game;
}

void CGG_free_game(Carc_Game* game){
    if(game!=NULL){
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
        if(CBP_connect_is_possible(playboard_node,CPCS_UP,CBP_get_neighbor(rim_node,CPCS_UP))==0
           || CBP_connect_is_possible(playboard_node,CPCS_DOWN,CBP_get_neighbor(rim_node,CPCS_DOWN))==0
           || CBP_connect_is_possible(playboard_node,CPCS_LEFT,CBP_get_neighbor(rim_node,CPCS_LEFT))==0
           || CBP_connect_is_possible(playboard_node,CPCS_RIGHT,CBP_get_neighbor(rim_node,CPCS_RIGHT))==0){
            //The tile cannot be inserted
            CBP_free_playboard_node(playboard_node);
            return NULL;
        }
        pointer_on_playboard_node = malloc(sizeof(*pointer_on_playboard_node));
        if(pointer_on_playboard_node!=NULL){
            *pointer_on_playboard_node = playboard_node;
            CGG_node_transfer_rim_to_playboard(game,loc,pointer_on_playboard_node);
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

Carc_Pawn* CGG_play_pawn_in(Carc_Player* player, Carc_Pawn_Type pawn_type, Carc_Tile* tile, Carc_Tile_Location loc_on_tile){
    Carc_Pawn* pawn=NULL;
    if(tile==NULL){
        fprintf(stderr,"ERROR: cannot have null tile input in CGG_play_pawn_in\n");
    } else if(CPPlayer_can_play_pawn(player,pawn_type)==1){
        pawn = CPPawn_new_pawn(player,pawn_type);
        if(CBT_add_pawn(pawn, tile, loc_on_tile)!=0){
            //The pawn can't be played
            CPPawn_free_pawn(pawn);
            pawn = NULL;
        }
    }
    return pawn;
}

int CGG_can_play_pawn_in(Carc_Pawn* pawn, Carc_Tile* tile, Carc_Tile_Location loc){
    ///Function to consider if a pawn can be played in a given location. It is based on
    ///CBT_node_type_matches_pawn_type and add special rules, such as is the global
    ///construction already occupied or when to play the architect
    int allowed_to_play=0;
    if(tile==NULL){
        fprintf(stderr,"ERROR: cannot play pawn on null tile (CGG_can_play_pawn_in)\n");
        return allowed_to_play;
    }
    Carc_Tile_Node* node=CBT_get_node_from_loc(tile,loc);
    if(CBT_node_type_matches_pawn_type(node->node_type,pawn->type)==1
       && CPPlayer_can_play_pawn(pawn->owner,pawn->type)==1){
        ///TODO: complete once the construction at global level are implemented
        ///Add check if construction occupied
        ///Add architect rule
        allowed_to_play = 1;
    }
    return allowed_to_play;
}

