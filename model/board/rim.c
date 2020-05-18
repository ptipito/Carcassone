#include "model/board/rim.h"

void CB_Rim_free_rim_elt(Carc_Rim_Element* elt){
    if(elt!=NULL){
        CBP_free_playboard_node(elt->node);
    }
    free(elt);
}

Carc_Rim* CB_Rim_new_empty(){
    Carc_Rim* rim = malloc(sizeof(Carc_Rim));
    if(rim==NULL){
        fprintf(stderr,"Couldn't allocate memory for the new empty rim");
        exit(ERR_MEM_ALLOC);
    }
    rim->first = NULL;
    return rim;
}

int CBRim_insert_playboard_node(Carc_Rim* rim, Carc_Playboard_Node* node){
    if(node==NULL)
        return -1;
    Carc_Rim_Element* new_elt = NULL;
    while(new_elt==NULL){
        new_elt = malloc(sizeof(*new_elt));
    }
    Carc_Rim_Element* current=NULL;
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
                if(CBP_Location_cmp(current->node->node_coordinates, new_elt->node->node_coordinates)==0){
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

Carc_Rim* CBRim_initiate(Carc_Playboard_Node* origin){
    Carc_Rim *rim = CB_Rim_new_empty();
    if(origin==NULL){
        return rim;
    }
    Carc_Playboard_Location loc = origin->node_coordinates;
    Carc_Playboard_Node *first_node = CBP_new_playboard_node(NULL,CBP_get_neighbor_loc(loc,CPCS_RIGHT)),
                        *snd_node = CBP_new_playboard_node(NULL,CBP_get_neighbor_loc(loc,CPCS_UP)),
                        *third_node = CBP_new_playboard_node(NULL,CBP_get_neighbor_loc(loc,CPCS_LEFT)),
                        *fourth_node = CBP_new_playboard_node(NULL,CBP_get_neighbor_loc(loc,CPCS_DOWN));

    first_node->neighbors[CPCS_LEFT] = origin;
    snd_node->neighbors[CPCS_DOWN] = origin;
    third_node->neighbors[CPCS_RIGHT] = origin;
    fourth_node->neighbors[CPCS_UP] = origin;

    CBRim_insert_playboard_node(rim,first_node);
    CBRim_insert_playboard_node(rim,snd_node);
    CBRim_insert_playboard_node(rim,third_node);
    CBRim_insert_playboard_node(rim,fourth_node);

    return rim;
}

void CBRim_remove_first(Carc_Rim* rim){
    if(rim!=NULL){
        if(rim->first!=NULL){
            Carc_Rim_Element *to_delete = rim->first;
            rim->first = rim->first->next;
            CB_Rim_free_rim_elt(to_delete);
        }
    }
}

int CBRim_remove_by_node_coordinates(Carc_Rim* rim, Carc_Playboard_Location loc){
    int removed=0;
    if(rim!=NULL){
        Carc_Rim_Element *previous=rim->first,
                       *current=rim->first;
        int done=0;
        if(rim->first==NULL){
            done=1;
        } else{
            current = current->next;
            if(CBP_Location_cmp(previous->node->node_coordinates,loc)==0){
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
            } else if(CBP_Location_cmp(current->node->node_coordinates,loc)==0){
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

void CBRim_free(Carc_Rim* rim){
    if(rim!=NULL){
        while(rim->first!=NULL){
            CBRim_remove_first(rim);
        }
    }
    free(rim);
}

void CBRim_display(Carc_Rim* rim){
    if(rim == NULL)
        printf("rim null\n");
    else{
        printf("[");
        if(rim->first==NULL){
            printf("]\n");
        } else {
            Carc_Rim_Element* current = rim->first;
            while(current!=NULL){
                printf("\n\t");
                CBP_display_playboard_node(*(current->node));
                current = current->next;
                if(current!=NULL){
                    printf(",");
                }
            }
            printf("\n]\n");
        }
    }
}

Carc_Playboard_Node* CB_Rim_find_by_location(Carc_Rim* rim, Carc_Playboard_Location node_coord){
    Carc_Playboard_Node *result=NULL;
    if(rim!=NULL && rim->first!=NULL){
        Carc_Rim_Element* current = rim->first;
        int done=0;
        while(!done){
            if(current==NULL){
                done=1;
            }else {
                if(CBP_Location_cmp(node_coord,current->node->node_coordinates)==0){
                    result = current->node;
                    done = 1;
                }
                current = current->next;
            }
        }
    }
    return result;
}


