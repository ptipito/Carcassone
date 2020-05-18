#include "tests/model/test_rim.h"

void test_rim_new_empty(){
    printf("test_rim_new_empty results: ");
    CP_Rim *rim = CB_Rim_new_empty();
    printf("%d",rim!=NULL);
    printf("%d",rim->first==NULL);
    CP_free_Rim(rim);
}

void test_rim_insert_node(){
    printf("test_rim_insert_node results: ");
    CP_Rim *rim = CB_Rim_new_empty();
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
              *cloister_tile = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");
    Carc_Playboard_Node *node1 = CP_new_empty_playboard_node(CP_Location_new(0,1)),
                        *node2 = CP_new_playboard_node(start_tile,CP_Location_new(0,0)),
                        *node3 = CP_new_playboard_node(cloister_tile,CP_Location_new(0,0));

    printf("%d",rim!=NULL);
    printf("%d",rim->first==NULL);

    CP_Rim_insert_playboard_node(rim,node1);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && rim->first->next==NULL);
    //Test inserting same node again
    CP_Rim_insert_playboard_node(rim,node1);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && rim->first->next==NULL);
    //Test inserting new node
    CP_Rim_insert_playboard_node(rim,node2);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && CP_node_cmp(rim->first->next->node,node2)==0 && rim->first->next->next==NULL);
    //Test inserting different node in same location
    CP_Rim_insert_playboard_node(rim,node3);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && CP_node_cmp(rim->first->next->node,node2)==0 && rim->first->next->next==NULL);
    //inserting new node
    node3->node_coordinates.x = 17;
    CP_Rim_insert_playboard_node(rim,node3);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && CP_node_cmp(rim->first->next->node,node2)==0 &&
           CP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);
    //Test reinserting first node of the list
    CP_Rim_insert_playboard_node(rim,node1);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && CP_node_cmp(rim->first->next->node,node2)==0 &&
           CP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);
    //Test reinserting a node not on list edge
    CP_Rim_insert_playboard_node(rim,node2);
    printf("%d",CP_node_cmp(rim->first->node,node1)==0 && CP_node_cmp(rim->first->next->node,node2)==0 &&
           CP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);

    CP_free_Rim(rim);
}

void test_rim_remove_first(){
    printf("test_rim_remove_first results: ");
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node *playboard_first_node = CP_init_playboard(start_tile)->node;
    CP_Rim *rim = CP_initiate_Rim(playboard_first_node);
    CP_Rim_display(rim);
    printf("remove first\n");
    printf("Rim is now:\n");
    CP_Rim_remove_first(rim);
    CP_Rim_display(rim);
    printf("remove first\n");
    printf("remove first\n");
    CP_Rim_remove_first(rim);
    CP_Rim_remove_first(rim);
    //Test remove last node
    printf("remove first\n");
    CP_Rim_remove_first(rim);
    printf("Rim is now:\n");
    CP_Rim_display(rim);
    //Test when rim is empty
    CP_Rim_remove_first(rim);
    //Test rim is null
    CP_Rim_remove_first(NULL);
    CP_free_Rim(rim);
    CP_free_playboard_node(playboard_first_node);
}

void test_rim_remove_by_node_coordinates(){
    printf("test_rim_remove_by_node_coordinates results: ");
    Carc_Playboard_Node *node = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(0,1)),
                        *node2 = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(1,0)),
                        *node3 = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(0,-1)),
                        *node4 = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(-1,0));
    CP_Rim* rim = CB_Rim_new_empty();
    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,1))==0);

    CP_Rim_insert_playboard_node(rim,node);
    CP_Rim_insert_playboard_node(rim,node2);
    CP_Rim_insert_playboard_node(rim,node3);
    CP_Rim_insert_playboard_node(rim,node4);

    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,-1))==1);
    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,1))==1);
    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(1,0))==1);
    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(-1,0))==1);
    printf("%d",CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,0))==0);
    printf("%d",CP_Rim_remove_by_node_coordinates(NULL,CP_Location_new(0,0))==0);

    CP_free_Rim(rim);
}

void test_rim_find_by_location(){
    printf("CB_Rim_find_by_location results: \n");
    Carc_Tile *start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Origin *playboard_origin = CP_init_playboard(start_tile);
    Carc_Playboard_Node *playboard_first_node = playboard_origin->node;
    CP_Rim* rim = CP_initiate_Rim(playboard_first_node);

    Carc_Playboard_Node* test_node;
    Carc_Playboard_Location test_loc = CP_Location_new(0,1);
    test_node = CB_Rim_find_by_location(rim, test_loc);

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
    CP_free_playboard_node(test_node);
    CP_free_playboard(playboard_origin);
}

void test_rim_initiate(){
    Carc_Playboard_Node *node = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(0,0));
    CP_Rim *rim = CP_initiate_Rim(node);
    int success = 1;
    success = success && CP_Rim_remove_by_node_coordinates(rim,CP_Location_new(0,1));

    node->node_coordinates = CP_Location_new(12,65);
}

void test_rim_run_all(){
    test_rim_new_empty();
    printf("\n**********************************************\n");
    test_rim_insert_node();
    printf("\n**********************************************\n");
    test_rim_remove_first();
    printf("\n**********************************************\n");
    test_rim_remove_by_node_coordinates();
    printf("\n**********************************************\n");
    test_rim_find_by_location();
    printf("\n**********************************************\n");
}
