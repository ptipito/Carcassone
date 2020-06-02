#ifndef DEF_CARC_RIM
#define DEF_CARC_RIM
#include <stdlib.h>
#include <stdio.h>
#include "model/board/constructions.h"
#include "model/board/carcassone_playboard.h"
#include "model/carcassone_errors.h"

//Used to keep a list of playable nodes at each time
typedef struct Carc_Rim_Element Carc_Rim_Element;
struct Carc_Rim_Element{
    Carc_Playboard_Node* node;
    Carc_Rim_Element* next;
};

typedef struct Carc_Rim{
    Carc_Rim_Element* first;
} Carc_Rim;

Carc_Rim* CB_Rim_new_empty();
int CBRim_insert_playboard_node(Carc_Rim*, Carc_Playboard_Node*);
Carc_Rim* CBRim_initiate(Carc_Playboard_Node**);
void CBRim_remove_first(Carc_Rim*);
int CBRim_remove_by_node_coordinates(Carc_Rim*, Carc_Playboard_Location);
void CBRim_free(Carc_Rim*);
void CBRim_display(Carc_Rim*);
Carc_Playboard_Node* CB_Rim_find_by_location(Carc_Rim*, Carc_Playboard_Location);


#endif // DEF_CARC_RIM
