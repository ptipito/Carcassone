#include "model/game/game.h"

Carc_Game* CG_initiate_game(char* filename){
    Carc_Game *game = malloc(sizeof(Carc_Game));
    Carc_Tile *start_tile = CT_new_tile_from_file(filename);
    Carc_Playboard_Node *playboard_first_node = CP_create_playboard(start_tile);
    Carc_Playboard_Origin *playboard_origin = CP_new_playboard_origin(playboard_first_node);

    game->playboard = playboard_origin;
    game->playable = CP_initiate_Rim(playboard_first_node);

    return game;
}

int game_play_tile_in(Carc_Game* game, Carc_Playboard_Location loc, Carc_Tile* tile){
    ///1. Check if playable
    ///2. If playable:
        ///a.  for each neighbor check if nieghbor exists (NULL if in rim or not even in rim yet; valid pointer if is on the playboar)
        ///b. if neighbor not NULL, update neighbor's neighbors (equivalent to play the node on the playboard)
        ///c. if neighbor null:
            ///1. if in rim, update neigh to add the played node to its neighbors list
            ///2. else create new node, add current tile_node to its neighbor and insert the new node in the rim
    //To avoid memory leak, a node from a rim is freed when removed from the rim. This means that to transfer a node
    //from the playable rim to the playboard, a new node must be created. This node shall be freed when the playboard is.
    Carc_Playboard_Node *rim_node = CB_Rim_find_by_location(game->playable, loc),
                        *playboard_node = NULL;
    //TODO combine both if in a function
    if(rim_node!=NULL){//A tile can be played in loc
        playboard_node = CP_new_playboard_node(tile,loc);
        if(CP_connect_is_possible(playboard_node,CP_UP,rim_node->neighbors[CP_UP])==0
           || CP_connect_is_possible(playboard_node,CP_DOWN,rim_node->neighbors[CP_DOWN])==0
           || CP_connect_is_possible(playboard_node,CP_LEFT,rim_node->neighbors[CP_LEFT])==0
           || CP_connect_is_possible(playboard_node,CP_RIGHT,rim_node->neighbors[CP_RIGHT])==0){
            //The tile cannot be inserted
            CP_free_playboard_node(playboard_node);
            return 0;
        }

        //Update the rim & playboard_node neighbors references. NB: As at least one neighbor of rim_node is a
        //node on the playboard, the new playboard_node is incidentally inserted in the game playboard graph
        CP_rim_transfer_node_update_one_side(game->playable,rim_node,playboard_node,CP_UP);
        CP_rim_transfer_node_update_one_side(game->playable,rim_node,playboard_node,CP_RIGHT);
        CP_rim_transfer_node_update_one_side(game->playable,rim_node,playboard_node,CP_DOWN);
        CP_rim_transfer_node_update_one_side(game->playable,rim_node,playboard_node,CP_LEFT);

        //The location does not belong to the playboard rim anymore
        CP_Rim_remove_by_node_coordinates(game->playable,rim_node->node_coordinates);
        return 1;
    }

    //loc is not playable (by definition of the game rim)
    return 0;
}


