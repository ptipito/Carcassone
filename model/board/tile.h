#ifndef DEF_CARC_TILE
#define DEF_CARC_TILE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include "model/board/constructions.h"
#include "utils.h"
#include "model/carcassone_errors.h"
#include "model/player/pawn.h"

#define TILE_NR_BORDER_LOCATIONS 12
#define TILE_NR_EDGES 4
#define TILE_NR_LOCATIONS_ON_ONE_EDGE 3
#define TILE_NR_LOCATIONS 3
#define TILE_FOLDER "ressources/gameset/tiles/"
#define TILE_FULL_PATH_MAX_LEN 50

//Possible locations on a tile. Each corner is represented by 2 locations to take into account city borders
typedef enum {CTL_NORTH_WEST, CTL_NORTH, CTL_NORTH_EAST,
                CTL_EAST_NORTH, CTL_EAST, CTL_EAST_SOUTH,
                CTL_SOUTH_EAST, CTL_SOUTH, CTL_SOUTH_WEST,
                CTL_WEST_SOUTH, CTL_WEST, CTL_WEST_NORTH,
                CTL_CENTER
} Carc_Tile_Location;

typedef enum {CCT_RIGHT, CCT_LEFT, CCT_UPDOWN} CBT_Turn_Type;

//Node on a tile. Each node contains a construction and is located at a tile location
typedef struct {
    Carc_Construction_Type node_type;
    Carc_Construction* construction;
    Carc_Pawn* pawn;
} Carc_Tile_Node;


typedef struct Carc_Tile {
    Carc_Tile_Location border_connexions[TILE_NR_BORDER_LOCATIONS][TILE_NR_BORDER_LOCATIONS];
    Carc_Tile_Node border[TILE_NR_BORDER_LOCATIONS];
    Carc_Tile_Node center;
    Carc_Tile_Location center_connexions[TILE_NR_BORDER_LOCATIONS];
} Carc_Tile;

Carc_Tile* CBT_new_tile_from_file(char*);
Carc_Tile* CBT_new_empty_tile();
Carc_Tile_Location CBT_get_location_from_string(char*);
Carc_Construction_Type CBT_get_node_type_from_str(char*);
void CBT_turn(Carc_Tile*, CBT_Turn_Type);
int CBT_tiles_connect_in(Carc_Tile, Carc_Tile_Location, Carc_Tile, Carc_Tile_Location);
void CBT_free_tile(Carc_Tile*);
void CBT_free_node(Carc_Tile_Node*);
int CBT_tile_cmp(Carc_Tile*, Carc_Tile*);
int CBT_tile_node_cmp(Carc_Tile_Node, Carc_Tile_Node);
void CBT_display_tile(Carc_Tile);
Carc_Tile_Node* CBT_new_node(Carc_Construction_Type, Carc_Construction*);
Carc_Tile_Node* CBT_get_node_from_loc(Carc_Tile*,Carc_Tile_Location);
Carc_City_Merchandise CBT_parse_merchandise(char);
int CBT_set_single_connexion(Carc_Tile*, Carc_Tile_Location, Carc_Tile_Location);
void CBT_set_node_const(Carc_Tile*, Carc_Tile_Location, Carc_Construction*);
void CBT_set_node_type(Carc_Tile*,Carc_Tile_Location, Carc_Construction_Type);
char* CT_get_tile_file_path(char*);
int CBT_node_type_matches_pawn_type(Carc_Construction_Type, Carc_Pawn_Type);
int CBT_add_pawn(Carc_Pawn*, Carc_Tile*, Carc_Tile_Location);
int CBT_rm_pawn(Carc_Tile*, Carc_Tile_Location);
int CBT_is_valid_loc(Carc_Tile_Location);

#endif // DEF_CARC_TILE
