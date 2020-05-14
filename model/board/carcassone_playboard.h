#ifndef DEF_CARCASSONE_PLAYBOARD
#define DEF_CARCASSONE_PLAYBOARD
#include <stdlib.h>
#include <stdio.h>
#include "model/board/tile.h"

#define CP_MAX_NEIGHBORS TILE_NR_EDGES

typedef enum {CP_UP, CP_RIGHT, CP_DOWN, CP_LEFT} CP_Connect_Side;

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
    Carc_Playboard_Node* neighbors[CP_MAX_NEIGHBORS];//Array of neighbors from up to left (clockwise ordered)
};

//Represents the playboard as a graph. Points on the origin of the playboard
typedef struct Carc_Playboard_Origin{
    Carc_Playboard_Node* node;
} Carc_Playboard_Origin;


Carc_Playboard_Node* CP_new_playboard_node(Carc_Tile*, Carc_Playboard_Location);
int CP_connect_is_possible(Carc_Playboard_Node*, CP_Connect_Side, Carc_Playboard_Node*);
Carc_Playboard_Location CP_Location_new(int, int);
int CP_Location_cmp(Carc_Playboard_Location, Carc_Playboard_Location);
void CP_free_playboard_node(Carc_Playboard_Node*);
void CP_display_playboard_node(Carc_Playboard_Node);
Carc_Playboard_Origin* CP_init_playboard(Carc_Tile*);
CP_Connect_Side CP_get_opposite_side(CP_Connect_Side);
CP_Connect_Side CP_get_opposite_side(CP_Connect_Side);
Carc_Playboard_Location CP_get_neighbor_loc(Carc_Playboard_Location, CP_Connect_Side);
Carc_Playboard_Node* CP_create_rim_neigh_for(Carc_Playboard_Node*, CP_Connect_Side);
Carc_Playboard_Node* CP_new_empty_playboard_node(Carc_Playboard_Location);
int CP_node_cmp(Carc_Playboard_Node*, Carc_Playboard_Node*);
void CP_free_playboard(Carc_Playboard_Origin*);
int CP_node_is_empty(Carc_Playboard_Node);

#endif // DEF_CARCASSONE_PLAYBOARD
