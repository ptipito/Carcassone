#ifndef DEF_CARCASSONE_PLAYBOARD
#define DEF_CARCASSONE_PLAYBOARD
#include <stdlib.h>
#include <stdio.h>
#include "model/board/tile.h"

#define CBP_MAX_NEIGHBORS TILE_NR_EDGES

typedef enum {CPCS_UP, CPCS_RIGHT, CPCS_DOWN, CPCS_LEFT} Carc_Playboard_Connect_Side;

//Coordinates of a Tile on the playboard. The starting tile is considered the origin (0,0).
typedef struct Carc_Playboard_Location {
    int x;
    int y;
} Carc_Playboard_Location;

//A node of the playboard graph
typedef struct Carc_Playboard_Node Carc_Playboard_Node;
struct Carc_Playboard_Node {
    Carc_Tile* node;
    Carc_Playboard_Location node_coordinates;//Needed for display of the tile on the map
    Carc_Playboard_Node** neighbors[CBP_MAX_NEIGHBORS];//Array of neighbors from up to left (clockwise ordered)
};

//Represents the playboard as a graph. Points on the origin of the playboard
typedef struct Carc_Playboard_Origin{
    Carc_Playboard_Node* node;
} Carc_Playboard_Origin;
/*
//A structure to consider constructions from at playboard level and not at tile or tile node level.
//It is, it gather in a same construct the nodes of a same construct at macro level, i.e. over several tiles. E.g. a
//same city over three different tiles.
//This avoids walking the same graph again and again and recompute all information of a construction at macro level.
typedef struct {
    Carc_Construction_Type type;
    Carc_Construction construct;
    Carc_Pawn** pawns;
    Carc_Tile_Node_List* rim;
} Carc_Playboard_Construct;
*/


Carc_Playboard_Node* CBP_new_playboard_node(Carc_Tile*, Carc_Playboard_Location);
Carc_Playboard_Node* CBP_get_neighbor(Carc_Playboard_Node*, Carc_Playboard_Connect_Side);
int CBP_set_neighbor(Carc_Playboard_Node*, Carc_Playboard_Connect_Side, Carc_Playboard_Node**);
int CBP_is_neighbor_null(Carc_Playboard_Node*, Carc_Playboard_Connect_Side);
int CBP_connect_is_possible(Carc_Playboard_Node*, Carc_Playboard_Connect_Side, Carc_Playboard_Node*);
Carc_Playboard_Location CBP_Location_new(int, int);
int CBP_Location_cmp(Carc_Playboard_Location, Carc_Playboard_Location);
void CBP_free_playboard_node(Carc_Playboard_Node*);
void CBP_display_playboard_node(Carc_Playboard_Node);
Carc_Playboard_Origin* CBP_init_playboard(Carc_Tile*);
Carc_Playboard_Connect_Side CBP_get_opposite_side(Carc_Playboard_Connect_Side);
Carc_Playboard_Location CBP_get_neighbor_loc(Carc_Playboard_Location, Carc_Playboard_Connect_Side);
Carc_Playboard_Node* CBP_create_rim_neigh_for(Carc_Playboard_Node**, Carc_Playboard_Connect_Side);
Carc_Playboard_Node* CBP_new_empty_playboard_node(Carc_Playboard_Location);
int CBP_node_cmp(Carc_Playboard_Node*, Carc_Playboard_Node*);
void CBP_free_playboard(Carc_Playboard_Origin*);
int CBP_node_is_empty(Carc_Playboard_Node);
int CBP_add_pawn_in(Carc_Pawn*, Carc_Playboard_Node*, Carc_Tile_Location);
int CBP_rm_pawn_in(Carc_Playboard_Node*, Carc_Tile_Location);

#endif // DEF_CARCASSONE_PLAYBOARD
