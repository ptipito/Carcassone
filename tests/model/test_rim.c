#include "tests/model/test_rim.h"

void test_rim_remove_first(){
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node *playboard_first_node = CP_create_playboard(start_tile);
    CP_Rim *rim = CP_initiate_Rim(playboard_first_node);
    CP_Rim_display(rim);
    CP_Rim_remove_first(rim);
    CP_Rim_display(rim);
    CP_Rim_remove_first(rim);
    CP_Rim_remove_first(rim);
    //Test remove last node
    CP_Rim_remove_first(rim);
    CP_Rim_display(rim);
    //Test when rim is empty
    CP_Rim_remove_first(rim);
    //Test rim is null
    CP_Rim_remove_first(NULL);
    CP_free_Rim(rim);
    CP_free_playboard_node(playboard_first_node);
}

void test_rim_remove_by_node_coordinates(){
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node *playboard_first_node = CP_create_playboard(start_tile);
    CP_Rim* rim = CP_initiate_Rim(playboard_first_node);

    CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,1));
    CP_Rim_display(rim);
    CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,-1));
    CP_Rim_display(rim);
    CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(1,0));
    CP_Rim_display(rim);
    CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(-1,0));
    CP_Rim_display(rim);

    CP_free_Rim(rim);
    CP_free_playboard_node(playboard_first_node);
}

void test_rim_find_by_location(){
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node *playboard_first_node = CP_create_playboard(start_tile);
    CP_Rim* rim = CP_initiate_Rim(playboard_first_node);

    Carc_Playboard_Node* test_node;
    Carc_Playboard_Location test_loc = CP_Location_new(0,1);
    test_node = CB_Rim_find_by_location(rim, test_loc);

    printf("\t++++++++++++++++++++++++++++\n");
    printf("\ttest CB_Rim_find_by_location\n");
    printf("\t++++++++++++++++++++++++++++\n");
    printf("%d",CP_Location_cmp(test_node->node_coordinates,test_loc)==0);
    test_loc.x = 1;
    test_node = CB_Rim_find_by_location(rim, test_loc);
    printf("%d",test_node==NULL);
    test_loc.y = 0;
    test_node = CB_Rim_find_by_location(rim, test_loc);
    printf("%d",CP_Location_cmp(test_node->node_coordinates,test_loc)==0);
    test_node = CB_Rim_find_by_location(NULL, test_loc);
    printf("%d\n",test_node==NULL);

    CP_free_Rim(rim);
    CP_free_playboard_node(playboard_first_node);
}


