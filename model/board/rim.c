#include "model/board/rim.h"

void CB_Rim_free_rim_elt(CP_Rim_Element* elt){
    if(elt!=NULL){
        CP_free_playboard_node(elt->node);
    }
    free(elt);
}

int CP_Rim_insert_playboard_node(CP_Rim* rim, Carc_Playboard_Node* node){
    CP_Rim_Element* new_elt = malloc(sizeof(*new_elt));
    CP_Rim_Element* current=NULL;
    new_elt->node = node;
    new_elt->next = NULL;
    int done=0, in_list=0, inserted=0;

    if(rim==NULL){
        fprintf(stderr,"trying to insert node in null rim\n");
    } else{
        if(rim->first == NULL){
            rim->first = new_elt;
            inserted=1;
        } else{
            current=rim->first;
            while(!done){
                if(CP_Location_cmp(current->node->node_coordinates, new_elt->node->node_coordinates)==0){
                    //Do not insert a node already in the list
                    in_list = 1;
                    done = 1;
                }
                if(current->next == NULL){
                    done = 1;
                } else{
                    current = current->next;
                }
            }
            if(!in_list){
                current->next = new_elt;
                inserted=1;
            }
        }
    }
    return inserted;
}

CP_Rim* CP_initiate_Rim(Carc_Playboard_Node* origin){
    CP_Rim *rim = malloc(sizeof(*rim));
    rim->first = NULL;
    if(origin==NULL){
        return rim;
    }
    Carc_Playboard_Location loc = origin->node_coordinates;
    Carc_Playboard_Node *first_node = CP_new_playboard_node(NULL,CP_get_neighbor_loc(loc,CP_RIGHT)),
                        *snd_node = CP_new_playboard_node(NULL,CP_get_neighbor_loc(loc,CP_UP)),
                        *third_node = CP_new_playboard_node(NULL,CP_get_neighbor_loc(loc,CP_LEFT)),
                        *fourth_node = CP_new_playboard_node(NULL,CP_get_neighbor_loc(loc,CP_DOWN));

    first_node->neighbors[CP_LEFT] = origin;
    snd_node->neighbors[CP_DOWN] = origin;
    third_node->neighbors[CP_RIGHT] = origin;
    fourth_node->neighbors[CP_UP] = origin;

    CP_Rim_insert_playboard_node(rim,first_node);
    CP_Rim_insert_playboard_node(rim,snd_node);
    CP_Rim_insert_playboard_node(rim,third_node);
    CP_Rim_insert_playboard_node(rim,fourth_node);

    return rim;
}

void CP_Rim_remove_first(CP_Rim* rim){
    if(rim!=NULL){
        if(rim->first!=NULL){
            CP_Rim_Element *to_delete = rim->first;
            rim->first = rim->first->next;
            CB_Rim_free_rim_elt(to_delete);
        }
    }
}

void CP_Rim_remove_by_node_coordinates(CP_Rim* rim, Carc_Playboard_Location loc){
    if(rim!=NULL){
        CP_Rim_Element *previous=rim->first,
                       *current = rim->first->next;
        int done=0;
        if(previous==NULL){
            done=1;
        } else if(CP_Location_cmp(previous->node->node_coordinates,loc)==0){
            //If the node to remove is the first one of the list
            rim->first = current;
            done=1;
            CB_Rim_free_rim_elt(previous);
        }
        while(!done){
            if(current==NULL){
                done = 1;
            } else if(CP_Location_cmp(current->node->node_coordinates,loc)==0){
                previous->next = current->next;
                CB_Rim_free_rim_elt(current);
                done=1;
            } else{
                previous = current;
                current = current->next;
            }
        }
    }
}

void CP_free_Rim(CP_Rim* rim){
    if(rim!=NULL){
        while(rim->first!=NULL){
            CP_Rim_remove_first(rim);
        }
    }
    free(rim);
}

void CP_Rim_display(CP_Rim* rim){
    printf("[");
    if(rim == NULL || rim->first==NULL){
        printf("]\n");
    } else {
        CP_Rim_Element* current = rim->first;
        while(current!=NULL){
            printf("\n\t");
            CP_display_playboard_node(*(current->node));
            current = current->next;
            if(current!=NULL){
                printf(",");
            }
        }
        printf("\n]\n");
    }
}

Carc_Playboard_Node* CB_Rim_find_by_location(CP_Rim* rim, Carc_Playboard_Location node_coord){
    Carc_Playboard_Node *result=NULL;
    if(rim!=NULL && rim->first!=NULL){
        CP_Rim_Element* current = rim->first;
        int done=0;
        while(!done){
            if(current==NULL){
                done=1;
            }else {
                if(CP_Location_cmp(node_coord,current->node->node_coordinates)==0){
                    result = current->node;
                    done = 1;
                }
                current = current->next;
            }
        }
    }
    return result;
}

int CP_rim_transfer_node_update_one_side(CP_Rim* rim, Carc_Playboard_Node* rim_node, Carc_Playboard_Node* src_node, CP_Connect_Side neighbor_side){
    ///This function takes information from rim_node to duplicate it in src_node and perform the right rim update for the given neighbor_side.
    ///It is aimed to be used to insert a node on the playboard in the location of rim_node (which belongs to the rim).
    ///This function aims to check on one side if the neighboring node already exists either on the playboard or in the rim in order to perform
    ///the relevant updates (neighbor references for nodes as well as updating existing node in the rim to include this new neighbor expanding the
    ///rim with a new node on this side)
    if(rim==NULL || rim_node==NULL || src_node==NULL){
        return -1;
    }

    if(rim_node->neighbors[neighbor_side]==NULL){//The neighboring node was not yet played (i.e. has no tile)
        Carc_Playboard_Location neighbor_loc = CP_get_neighbor_loc(src_node->node_coordinates,neighbor_side);
        Carc_Playboard_Node* neighbor = CB_Rim_find_by_location(rim,neighbor_loc);
        if(neighbor!=NULL){//The neighboring node is already in the Rim => update its neighbors attribute
            neighbor->neighbors[CP_get_opposite_side(neighbor_side)] = src_node;
        } else {//Create a new node for the rim
            neighbor = CP_create_rim_neigh_for(src_node,neighbor_side);
            CP_Rim_insert_playboard_node(rim,neighbor);
        }
    } else {//The neighbor is already occupied=>update its neighbors attribute
        src_node->neighbors[neighbor_side] = rim_node->neighbors[neighbor_side];
        src_node->neighbors[neighbor_side]->neighbors[CP_get_opposite_side(neighbor_side)] = src_node;
    }
    return 0;
}

