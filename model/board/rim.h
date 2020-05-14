#ifndef DEF_CARC_RIM
#define DEF_CARC_RIM
#include <stdlib.h>
#include <stdio.h>
#include "model/board/constructions.h"
#include "model/board/carcassone_playboard.h"
#include "model/carcassone_errors.h"

//Used to keep a list of playable nodes at each time
typedef struct CP_Rim_Element CP_Rim_Element;
struct CP_Rim_Element{
    Carc_Playboard_Node* node;
    CP_Rim_Element* next;
};

typedef struct CP_Rim{
    CP_Rim_Element* first;
} CP_Rim;

CP_Rim* CB_Rim_new_empty();
int CP_Rim_insert_playboard_node(CP_Rim*, Carc_Playboard_Node*);
CP_Rim* CP_initiate_Rim(Carc_Playboard_Node*);
void CP_Rim_remove_first(CP_Rim*);
int CP_Rim_remove_by_node_coordinates(CP_Rim*, Carc_Playboard_Location);
void CP_free_Rim(CP_Rim*);
void CP_Rim_display(CP_Rim*);
Carc_Playboard_Node* CB_Rim_find_by_location(CP_Rim*, Carc_Playboard_Location);


#endif // DEF_CARC_RIM
