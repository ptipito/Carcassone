#include "model/board/carcassone_playboard.h"

void CP_free_playboard_node(Carc_Playboard_Node* node){
    if(node!=NULL){
        CT_Free_Tile(node->node);
    }
    free(node);
}

Carc_Playboard_Location CP_Location_new(int x, int y){
    Carc_Playboard_Location loc;
    loc.x = x;
    loc.y = y;
    return loc;
}

int CP_Location_cmp(Carc_Playboard_Location l1,Carc_Playboard_Location l2){
    int result;
    if(l1.x<l2.x){
        result = -1;
    } else if(l1.x == l2.x){
        if(l1.y<l2.y){
            result = -1;
        } else if(l1.y == l2.y){
            result = 0;
        }
        else{
            result = 1;
        }
    } else {
        result = 1;
    }
    return result;
}

Carc_Playboard_Node* CP_create_playboard(Carc_Tile* start_tile){
    int i=0;
    Carc_Playboard_Node *playboard_origin = malloc(sizeof(Carc_Playboard_Node));
    playboard_origin->node = start_tile;
    for(i=0;i<CP_MAX_NEIGHBORS;i++){
        playboard_origin->neighbors[i]=NULL;
    }
    playboard_origin->node_coordinates.x = 0;
    playboard_origin->node_coordinates.y = 0;

    return playboard_origin;
}

Carc_Playboard_Node* CP_new_playboard_node(Carc_Tile* tile, Carc_Playboard_Location coordinates){
    int i=0;
    Carc_Playboard_Node *playboard_node = malloc(sizeof(Carc_Playboard_Node));
    playboard_node->node = tile;
    for(i=0;i<CP_MAX_NEIGHBORS;i++){
        playboard_node->neighbors[i]=NULL;
    }
    playboard_node->node_coordinates = coordinates;
    return playboard_node;
}

Carc_Playboard_Node* CP_new_empty_playboard_node(Carc_Playboard_Location coordinates){
    return CP_new_playboard_node(NULL,coordinates);
}

int CP_node_is_empty(Carc_Playboard_Node node){
    return (node.node==NULL);
}

int CP_connect_is_possible(Carc_Playboard_Node* src, CP_Connect_Side connect_side, Carc_Playboard_Node* to_connect){
    int is_possible=1;
    int src_locations[TILE_NR_LOCATIONS_ON_ONE_EDGE] = {CTL_CENTER},
        neigh_locations[TILE_NR_LOCATIONS_ON_ONE_EDGE] = {CTL_CENTER};//3 is the nr of locations on an edge. To be refactored with a more robust way than magic nr
        int i=0;
    if(src==NULL || to_connect==NULL
       || CP_node_is_empty(*src) || CP_node_is_empty(*to_connect)){
        return is_possible; //No node => no constraints => "connection" is ok
    }

    switch(connect_side){
        //TODO: ADAPT the below computation with TILE_NR_LOCATIONS_ON_ONE_EDGE
        case CP_UP:
            src_locations[0] = CTL_NORTH_WEST_NORTH;
            neigh_locations[0] = CTL_SOUTH_WEST_SOUTH;
            src_locations[1] = CTL_NORTH;
            neigh_locations[1] = CTL_SOUTH;
            src_locations[2] = CTL_NORTH_EAST_NORTH;
            neigh_locations[2] = CTL_SOUTH_EAST_SOUTH;
            break;
        case CP_RIGHT:
            src_locations[0] = CTL_NORTH_EAST_EAST;
            neigh_locations[0] = CTL_NORTH_WEST_WEST;
            src_locations[1] = CTL_EAST;
            neigh_locations[1] = CTL_WEST;
            src_locations[2] = CTL_SOUTH_EAST_EAST;
            neigh_locations[2] = CTL_SOUTH_WEST_WEST;
            break;
        case CP_DOWN:
            src_locations[0] = CTL_SOUTH_WEST_SOUTH;
            neigh_locations[0] = CTL_NORTH_WEST_NORTH;
            src_locations[1] = CTL_SOUTH;
            neigh_locations[1] = CTL_NORTH;
            src_locations[2] = CTL_SOUTH_EAST_SOUTH;
            neigh_locations[2] = CTL_NORTH_EAST_NORTH;
            break;
        case CP_LEFT:
            src_locations[0] = CTL_NORTH_WEST_WEST;
            neigh_locations[0] = CTL_NORTH_EAST_EAST;
            src_locations[1] = CTL_WEST;
            neigh_locations[1] = CTL_EAST;
            src_locations[2] = CTL_SOUTH_WEST_WEST;
            neigh_locations[2] = CTL_SOUTH_EAST_EAST;
            break;
    }
    while(is_possible && (i < TILE_NR_LOCATIONS_ON_ONE_EDGE)){
        if(CT_Tiles_connect_in(*(src->node),src_locations[i],*(to_connect->node),neigh_locations[i])==0){
            is_possible=0;
        }
        i++;
    }
    return is_possible;
}

void CP_display_playboard_node(Carc_Playboard_Node node){
    printf("(%d,%d)",node.node_coordinates.x,node.node_coordinates.y);
}

Carc_Playboard_Origin* CP_new_playboard_origin(Carc_Playboard_Node* origin_node){
    printf("attribute dynamic memory for origin playboard\n");
    Carc_Playboard_Origin *origin = malloc(sizeof(*origin));
    printf("attribute node to origin\n");
    origin->node = origin_node;

    return origin;
}

void CP_free_playboard(Carc_Playboard_Origin* origin){
    //TODO walk the graph to free all nodes
    if(origin!=NULL){
        CP_free_playboard_node(origin->node);
    }
    free(origin);
}

CP_Connect_Side CP_get_opposite_side(CP_Connect_Side side){
    CP_Connect_Side opp_side;
    switch(side){
        case CP_UP:
            opp_side = CP_DOWN;
        case CP_DOWN:
            opp_side =CP_UP;
        case CP_LEFT:
            opp_side = CP_RIGHT;
        case CP_RIGHT:
            opp_side = CP_LEFT;
    }
    return opp_side;
}

Carc_Playboard_Location CP_get_neighbor_loc(Carc_Playboard_Location src, CP_Connect_Side neighbor_side){
     Carc_Playboard_Location new_location;
     switch(neighbor_side){
        case CP_UP:
            new_location.x = src.x;
            new_location.y = src.y+1;
            break;
        case CP_DOWN:
            new_location.x = src.x;
            new_location.y = src.y-1;
            break;
        case CP_RIGHT:
            new_location.x = src.x+1;
            new_location.y = src.y;
            break;
        case CP_LEFT:
            new_location.x = src.x-1;
            new_location.y = src.y;
            break;
     }
     return new_location;
}

Carc_Playboard_Node* CP_create_neigh_for(Carc_Playboard_Node* src_node, CP_Connect_Side neighbor_side){
    Carc_Playboard_Location neighbor_loc = CP_get_neighbor_loc(src_node->node_coordinates,neighbor_side);
    Carc_Playboard_Node* neighbor = CP_new_empty_playboard_node(neighbor_loc);
    //src_node is a neighbor of this new empty node
    neighbor->neighbors[CP_get_opposite_side(neighbor_side)] = src_node;
    return neighbor;
}

