#include "model/board/carcassone_playboard.h"

void CBP_free_playboard_node(Carc_Playboard_Node* node){
    if(node!=NULL){
        CBT_free_tile(node->node);
        free(node);
    }
}

Carc_Playboard_Node* CBP_get_neighbor(Carc_Playboard_Node* node, Carc_Playboard_Connect_Side side){
    if(node==NULL){
        fprintf(stderr,"WARNING: tried to access neighbor of NULL node (CBP_get_neighbor)\n");
        return NULL;
    }
    Carc_Playboard_Node** neighbor_pointer = node->neighbors[side];
    if(neighbor_pointer==NULL){
        return NULL;
    }
    return (*neighbor_pointer);
}

int CBP_set_neighbor(Carc_Playboard_Node* src, Carc_Playboard_Connect_Side side, Carc_Playboard_Node** dest){
    ///Set the value of the pointer in in the neighbors array of src in position side. NOT the pointer itself.
    if(src==NULL){
        fprintf(stderr,"WARNING: tried to set neighbor of NULL node (CBP_set_neighbor)\n");
        return -1;
    }
    if(src->neighbors[side]==NULL){
        src->neighbors[side] = malloc(sizeof(*(src->neighbors[side])));
    }
    src->neighbors[side] = dest;
    return 0;
}

int CBP_is_neighbor_null(Carc_Playboard_Node* src, Carc_Playboard_Connect_Side side){
    ///Set the value of the pointer in in the neighbors array of src in position side. NOT the pointer itself.
    if(src==NULL){
        fprintf(stderr,"WARNING: tried to access neighbor of NULL node (CBP_is_neighbor_null)\n");
        return -1;
    }
    if(src->neighbors[side]==NULL || CBP_get_neighbor(src,side)==NULL){
        return 1;
    }
    return 0;
}

Carc_Playboard_Location CBP_Location_new(int x, int y){
    Carc_Playboard_Location loc;
    loc.x = x;
    loc.y = y;
    return loc;
}

int CBP_Location_cmp(Carc_Playboard_Location l1,Carc_Playboard_Location l2){
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

int CBP_node_cmp(Carc_Playboard_Node* n1,Carc_Playboard_Node* n2){
    int equal=0, different=1, i=0, same_neigh=1;
    if(n1==NULL || n2==NULL){
        if(n1==n2)
            return equal;
        else
            return different;
    } else{
        if(CBP_Location_cmp(n1->node_coordinates,n2->node_coordinates)!=0){
            return different;
        }
        if(CBT_tile_cmp(n1->node,n2->node)!=0)
            return different;
        while(i<CBP_MAX_NEIGHBORS && same_neigh){
            same_neigh = (CBP_get_neighbor(n1,i)==CBP_get_neighbor(n2,i));
            if(!same_neigh){
                return different;
            }
            i++;
        }
    }
    return equal;
}

Carc_Playboard_Node* CBP_new_playboard_node(Carc_Tile* tile, Carc_Playboard_Location coordinates){
    int i=0;
    Carc_Playboard_Node *playboard_node = NULL;
    while(playboard_node==NULL){
        playboard_node = malloc(sizeof(Carc_Playboard_Node));
    }
    playboard_node->node = tile;
    for(i=0;i<CBP_MAX_NEIGHBORS;i++){
        playboard_node->neighbors[i]=NULL;
    }
    playboard_node->node_coordinates = coordinates;
    return playboard_node;
}

Carc_Playboard_Node* CBP_new_empty_playboard_node(Carc_Playboard_Location coordinates){
    return CBP_new_playboard_node(NULL,coordinates);
}

int CBP_node_is_empty(Carc_Playboard_Node node){
    return (node.node==NULL);
}

int CBP_connect_is_possible(Carc_Playboard_Node* src, Carc_Playboard_Connect_Side connect_side, Carc_Playboard_Node* to_connect){
    int is_possible=1;
    int src_locations[TILE_NR_LOCATIONS_ON_ONE_EDGE] = {CTL_CENTER},
        neigh_locations[TILE_NR_LOCATIONS_ON_ONE_EDGE] = {CTL_CENTER};//3 is the nr of locations on an edge. To be refactored with a more robust way than magic nr
        int i=0;
    if(src==NULL || to_connect==NULL
       || CBP_node_is_empty(*src) || CBP_node_is_empty(*to_connect)){
        return is_possible; //No node => no constraints => "connection" is ok
    }

    switch(connect_side){
        case CPCS_UP:
            src_locations[0] = CTL_NORTH_WEST;
            neigh_locations[0] = CTL_SOUTH_WEST;
            src_locations[1] = CTL_NORTH;
            neigh_locations[1] = CTL_SOUTH;
            src_locations[2] = CTL_NORTH_EAST;
            neigh_locations[2] = CTL_SOUTH_EAST;
            break;
        case CPCS_RIGHT:
            src_locations[0] = CTL_EAST_NORTH;
            neigh_locations[0] = CTL_WEST_NORTH;
            src_locations[1] = CTL_EAST;
            neigh_locations[1] = CTL_WEST;
            src_locations[2] = CTL_EAST_SOUTH;
            neigh_locations[2] = CTL_WEST_SOUTH;
            break;
        case CPCS_DOWN:
            src_locations[0] = CTL_SOUTH_WEST;
            neigh_locations[0] = CTL_NORTH_WEST;
            src_locations[1] = CTL_SOUTH;
            neigh_locations[1] = CTL_NORTH;
            src_locations[2] = CTL_SOUTH_EAST;
            neigh_locations[2] = CTL_NORTH_EAST;
            break;
        case CPCS_LEFT:
            src_locations[0] = CTL_WEST_NORTH;
            neigh_locations[0] = CTL_EAST_NORTH;
            src_locations[1] = CTL_WEST;
            neigh_locations[1] = CTL_EAST;
            src_locations[2] = CTL_WEST_SOUTH;
            neigh_locations[2] = CTL_EAST_SOUTH;
            break;
    }
    while(is_possible && (i < TILE_NR_LOCATIONS_ON_ONE_EDGE)){
        if(CBT_tiles_connect_in(*(src->node),src_locations[i],*(to_connect->node),neigh_locations[i])==0){
            is_possible=0;
        }
        i++;
    }
    return is_possible;
}

void CBP_display_playboard_node(Carc_Playboard_Node node){
    printf("(%d,%d)",node.node_coordinates.x,node.node_coordinates.y);
}

Carc_Playboard_Origin* CBP_init_playboard(Carc_Tile* start_tile){
    Carc_Playboard_Origin *origin = NULL;
    while(origin==NULL){
        origin = malloc(sizeof(*origin));
    }
    origin->node = CBP_new_playboard_node(start_tile,CBP_Location_new(0,0));

    return origin;
}

void CBP_free_playboard(Carc_Playboard_Origin* origin){
    //TODO walk the graph to free all nodes
    if(origin!=NULL){
        CBP_free_playboard_node(origin->node);
    }
    free(origin);
}

Carc_Playboard_Connect_Side CBP_get_opposite_side(Carc_Playboard_Connect_Side side){
    Carc_Playboard_Connect_Side opp_side;
    switch(side){
        case CPCS_UP:
            opp_side = CPCS_DOWN;
            break;
        case CPCS_DOWN:
            opp_side =CPCS_UP;
            break;
        case CPCS_LEFT:
            opp_side = CPCS_RIGHT;
            break;
        case CPCS_RIGHT:
            opp_side = CPCS_LEFT;
            break;
    }
    return opp_side;
}

Carc_Playboard_Location CBP_get_neighbor_loc(Carc_Playboard_Location src, Carc_Playboard_Connect_Side neighbor_side){
     Carc_Playboard_Location new_location;
     switch(neighbor_side){
        case CPCS_UP:
            new_location.x = src.x;
            new_location.y = src.y+1;
            break;
        case CPCS_DOWN:
            new_location.x = src.x;
            new_location.y = src.y-1;
            break;
        case CPCS_RIGHT:
            new_location.x = src.x+1;
            new_location.y = src.y;
            break;
        case CPCS_LEFT:
            new_location.x = src.x-1;
            new_location.y = src.y;
            break;
     }
     return new_location;
}

Carc_Playboard_Node* CBP_create_rim_neigh_for(Carc_Playboard_Node** src_node, Carc_Playboard_Connect_Side neighbor_side){
    ///Create a new empty node. This new node has in its neighbors the src_node. However the created node being
    ///empty, it is not referenced in the neighbors attribute of src_node. This function aims to update the rim
    ///when a new node is played on the playboard. The returned node should be added to a rim.
    Carc_Playboard_Location neighbor_loc = CBP_get_neighbor_loc((*src_node)->node_coordinates,neighbor_side);
    Carc_Playboard_Node* neighbor = CBP_new_empty_playboard_node(neighbor_loc);
    //src_node is a neighbor of this new empty node
    CBP_set_neighbor(neighbor,CBP_get_opposite_side(neighbor_side),src_node);
    return neighbor;
}

int CBP_add_pawn_in(Carc_Pawn* pawn, Carc_Playboard_Node* node, Carc_Tile_Location loc_in_tile){
    int res=0;
    if(node==NULL){
        res = -1;
        fprintf(stderr, "ERROR: cannot add pawn to NULL playboard node\n");
    } else{
        res = CBT_add_pawn(pawn, node->node,loc_in_tile);
    }
    return res;
}

int CBP_rm_pawn_in(Carc_Playboard_Node* node, Carc_Tile_Location loc_in_tile){
    int res=0;
    if(node==NULL){
        res = -1;
        fprintf(stderr, "ERROR: cannot remove pawn from NULL playboard node\n");
    } else{
        res = CBT_rm_pawn(node->node,loc_in_tile);
    }
    return res;
}

Carc_Tile_Node_List* CBP_get_edge_nodes(Carc_Playboard_Node* node, Carc_Playboard_Connect_Side edge){
    if(pointer_is_null(node,0) || pointer_is_null(node->node,0)){
        return NULL;
    }
    Carc_Tile_Node* cur_tile_node=NULL;
    Carc_Tile_Node_List* node_list=NULL;
    //Get node of merging side
    switch(edge){
        case CPCS_UP:
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_NORTH_WEST);
            node_list = CBTList_new(&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_NORTH);
            CBTList_append(node_list,&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_NORTH_EAST);
            CBTList_append(node_list,&cur_tile_node);
            break;
        case CPCS_RIGHT:
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_EAST_NORTH);
            node_list = CBTList_new(&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_EAST);
            CBTList_append(node_list,&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_EAST_SOUTH);
            CBTList_append(node_list,&cur_tile_node);
            break;
        case CPCS_DOWN:
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_SOUTH_WEST);
            node_list = CBTList_new(&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_SOUTH);
            CBTList_append(node_list,&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_SOUTH_EAST);
            CBTList_append(node_list,&cur_tile_node);
            break;
        case CPCS_LEFT:
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_WEST_NORTH);
            node_list = CBTList_new(&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_WEST);
            CBTList_append(node_list,&cur_tile_node);
            cur_tile_node = CBT_get_node_from_loc(node->node,CTL_WEST_SOUTH);
            CBTList_append(node_list,&cur_tile_node);
            break;
        default:
            fprintf(stderr,"WARNING: Invalid value for enum Carc_Playboard_Connect_Side: %d. Discraded\n",edge);
            break;
    }
    return node_list;
}


