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
    int i=0;

    game->playboard = playboard_origin;
    game->playable = CBRim_initiate(playboard_origin->node);

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

int CGG_rim_to_playboard_update_one_side(Carc_Rim* rim, Carc_Playboard_Location coord, Carc_Playboard_Node* playboard_node, Carc_Playboard_Connect_Side neighbor_side){
    ///This function takes information from rim_node to duplicate it in playboard_node and perform the right rim update for the given neighbor_side.
    ///It is aimed to be used to insert a node on the playboard in the location of rim_node (which belongs to the rim).
    ///This function aims to check on one side if the neighboring node already exists either on the playboard or in the rim in order to perform
    ///the relevant updates (neighbor references for nodes as well as updating existing node in the rim to include this new neighbor expanding the
    ///rim with a new node on this side)
    if(rim==NULL || playboard_node==NULL){
        fprintf(stderr,"Wrong use of CB_rim_to_playboard_update_one_side: an input is null\n");
        return 1;
    }
    Carc_Playboard_Node* rim_node = CB_Rim_find_by_location(rim,coord);
    if(rim_node==NULL){
        fprintf(stderr,"Cannot transfer node from rim to playboard: location not in the rim\n");
        return 2;
    }
    if(CBP_Location_cmp(playboard_node->node_coordinates,coord)!=0){
        fprintf(stderr,"Wrong use of CB_rim_to_playboard_update_one_side: input node coordinates do not correspond to input location\n");
        return 3;
    }
    if(rim_node->neighbors[neighbor_side]==NULL){//The neighboring node was not yet played (i.e. does not belong to the playboard)
        Carc_Playboard_Location neighbor_loc = CBP_get_neighbor_loc(coord,neighbor_side);
        Carc_Playboard_Node* neighbor = CB_Rim_find_by_location(rim,neighbor_loc);
        if(neighbor!=NULL){
            //Update the neighboring rim node neighbors attribute to include the newly played node
            neighbor->neighbors[CBP_get_opposite_side(neighbor_side)] = playboard_node;
        } else {
            //Create a new node for the rim
            neighbor = CBP_create_rim_neigh_for(playboard_node,neighbor_side);
            CBRim_insert_playboard_node(rim,neighbor);
        }
    } else {
        //The neighbor is already on the playboard=>update its neighbors attribute
        playboard_node->neighbors[neighbor_side] = rim_node->neighbors[neighbor_side];
        playboard_node->neighbors[neighbor_side]->neighbors[CBP_get_opposite_side(neighbor_side)] = playboard_node;
    }
    return 0;
}

int CGG_node_transfer_rim_to_playboard(Carc_Game* game, Carc_Playboard_Location coord, Carc_Playboard_Node* playboard_node){
    if(game==NULL || playboard_node==NULL){
        fprintf(stderr,"Error on call CGG_node_transfer_rim_to_playboard: one argument is NULL\n");
        return 1;
    }
    if(CB_Rim_find_by_location(game->playable,coord)==NULL){
        fprintf(stderr,"Error on call CGG_node_transfer_rim_to_playboard: input location not in game's rim\n");
        return 2;
    }
    if(CBP_Location_cmp(playboard_node->node_coordinates,coord)!=0){
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
                        *playboard_node = NULL;
    //TODO combine both if in a function
    if(rim_node!=NULL){//A tile can be played in loc
        playboard_node = CBP_new_playboard_node(tile,loc);
        if(CBP_connect_is_possible(playboard_node,CPCS_UP,rim_node->neighbors[CPCS_UP])==0
           || CBP_connect_is_possible(playboard_node,CPCS_DOWN,rim_node->neighbors[CPCS_DOWN])==0
           || CBP_connect_is_possible(playboard_node,CPCS_LEFT,rim_node->neighbors[CPCS_LEFT])==0
           || CBP_connect_is_possible(playboard_node,CPCS_RIGHT,rim_node->neighbors[CPCS_RIGHT])==0){
            //The tile cannot be inserted
            CBP_free_playboard_node(playboard_node);
            return NULL;
        }
        CGG_node_transfer_rim_to_playboard(game,loc,playboard_node);

        return playboard_node;
    }
    //else loc is not playable (by definition of the game rim)
    return NULL;
}


