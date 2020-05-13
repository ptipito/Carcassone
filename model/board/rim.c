#include "model/board/rim.h"

void CB_Rim_free_rim_elt(CP_Rim_Element* elt){
    if(elt!=NULL){
        CP_free_playboard_node(elt->node);
    }
    free(elt);
}

CP_Rim* CB_Rim_new_empty(){
    CP_Rim* rim = malloc(sizeof(CP_Rim));
    if(rim==NULL){
        fprintf(stderr,"Couldn't allocate memory for the new empty rim");
        exit(ERR_MEM_ALLOC);
    }
    rim->first = NULL;
    return rim;
}

int CP_Rim_insert_playboard_node(CP_Rim* rim, Carc_Playboard_Node* node){
    if(node==NULL)
        return -1;
    CP_Rim_Element* new_elt = NULL;
    while(new_elt==NULL){
        new_elt = malloc(sizeof(*new_elt));
    }
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
                else if(current->next == NULL){
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
    CP_Rim *rim = CB_Rim_new_empty();
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

int CP_Rim_remove_by_node_coordinates(CP_Rim* rim, Carc_Playboard_Location loc){
    int removed=0;
    if(rim!=NULL){
        CP_Rim_Element *previous=rim->first,
                       *current=rim->first;
        int done=0;
        if(rim->first==NULL){
            done=1;
        } else{
            current = current->next;
            if(CP_Location_cmp(previous->node->node_coordinates,loc)==0){
                //If the node to remove is the first one of the list
                rim->first = current;
                done=1;
                CB_Rim_free_rim_elt(previous);
                removed = 1;
            }
        }
        while(!done){
            if(current==NULL){
                done = 1;
            } else if(CP_Location_cmp(current->node->node_coordinates,loc)==0){
                previous->next = current->next;
                CB_Rim_free_rim_elt(current);
                done=1;
                removed = 1;
            } else{
                previous = current;
                current = current->next;
            }
        }
    }
    return removed;
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
    if(rim == NULL)
        printf("rim null\n");
    else{
        printf("[");
        if(rim->first==NULL){
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


