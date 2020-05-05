#ifndef DEF_CARC_TILE
#define DEF_CARC_TILE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "model/board/constructions.h"
#include "utils.h"

#define TILE_NR_BORDER_LOCATIONS 12
#define TILE_NR_EDGES 4
#define TILE_NR_LOCATIONS_ON_ONE_EDGE 3
#define TILE_NR_LOCATIONS 3

//Possible locations on a tile. Each corner is represented by 2 locations to take into account city borders
typedef enum {CTL_NORTH_WEST_NORTH, CTL_NORTH, CTL_NORTH_EAST_NORTH, CTL_NORTH_EAST_EAST, CTL_EAST,
              CTL_SOUTH_EAST_EAST, CTL_SOUTH_EAST_SOUTH, CTL_SOUTH, CTL_SOUTH_WEST_SOUTH,
              CTL_SOUTH_WEST_WEST, CTL_WEST, CTL_NORTH_WEST_WEST, CTL_CENTER
} CT_Locations;

typedef enum {RIGHT, LEFT, UPDOWN} CT_Turn_Type;

//Node on a tile. Each node contains a construction and is located at a tile location
typedef struct {
    Carc_Construction_Type node_type;
    Carc_Construction* construction;
} Carc_Tile_Node;


typedef struct Carc_Tile {
    Carc_Tile_Node border[TILE_NR_BORDER_LOCATIONS];
    CT_Locations border_connexions[TILE_NR_BORDER_LOCATIONS][TILE_NR_BORDER_LOCATIONS];
    Carc_Tile_Node center;
    CT_Locations center_connexions[TILE_NR_BORDER_LOCATIONS];
} Carc_Tile;

Carc_Tile* CT_new_tile_from_file(char*);
Carc_Tile* CT_new_empty_tile();
CT_Locations CT_get_location_from_string(char*);
Carc_Construction_Type CT_get_node_type_from_str(char*);
void CT_turn(Carc_Tile*, CT_Turn_Type);
int CT_Tiles_connect_in(Carc_Tile, CT_Locations, Carc_Tile, CT_Locations);
void CT_Free_Tile(Carc_Tile*);
int CT_tile_cmp(Carc_Tile, Carc_Tile);
int CT_tile_node_cmp(Carc_Tile_Node, Carc_Tile_Node);
void display_tile(Carc_Tile);

#endif // DEF_CARC_TILE
