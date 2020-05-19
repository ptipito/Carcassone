#include "tests/model/test_rim.h"

void test_rim_new_empty(){
    printf("test_rim_new_empty results: ");
    Carc_Rim *rim = CB_Rim_new_empty();
    printf("%d",rim!=NULL);
    printf("%d",rim->first==NULL);
    CBRim_free(rim);
}

void test_rim_insert_node(){
    printf("test_rim_insert_node results: ");
    Carc_Rim *rim = CB_Rim_new_empty();
    char *tile1_path = CT_get_tile_file_path("tile1.txt"),
         *cloister_tile_path = CT_get_tile_file_path("cloister_path.txt");
    Carc_Tile *start_tile = CBT_new_tile_from_file(tile1_path),
              *cloister_tile = CBT_new_tile_from_file(cloister_tile_path);
    Carc_Playboard_Node *node1 = CBP_new_empty_playboard_node(CBP_Location_new(0,1)),
                        *node2 = CBP_new_playboard_node(start_tile,CBP_Location_new(0,0)),
                        *node3 = CBP_new_playboard_node(cloister_tile,CBP_Location_new(0,0));

    printf("%d",rim!=NULL);
    printf("%d",rim->first==NULL);

    CBRim_insert_playboard_node(rim,node1);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && rim->first->next==NULL);
    //Test inserting same node again
    CBRim_insert_playboard_node(rim,node1);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && rim->first->next==NULL);
    //Test inserting new node
    CBRim_insert_playboard_node(rim,node2);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && CBP_node_cmp(rim->first->next->node,node2)==0 && rim->first->next->next==NULL);
    //Test inserting different node in same location
    CBRim_insert_playboard_node(rim,node3);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && CBP_node_cmp(rim->first->next->node,node2)==0 && rim->first->next->next==NULL);
    //inserting new node
    node3->node_coordinates.x = 17;
    CBRim_insert_playboard_node(rim,node3);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && CBP_node_cmp(rim->first->next->node,node2)==0 &&
           CBP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);
    //Test reinserting first node of the list
    CBRim_insert_playboard_node(rim,node1);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && CBP_node_cmp(rim->first->next->node,node2)==0 &&
           CBP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);
    //Test reinserting a node not on list edge
    CBRim_insert_playboard_node(rim,node2);
    printf("%d",CBP_node_cmp(rim->first->node,node1)==0 && CBP_node_cmp(rim->first->next->node,node2)==0 &&
           CBP_node_cmp(rim->first->next->next->node,node3)==0 && rim->first->next->next->next==NULL);

    CBRim_free(rim);
    free(tile1_path);
    free(cloister_tile_path);
}

void test_rim_remove_first(){
    printf("test_rim_remove_first results: ");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile *start_tile = CBT_new_tile_from_file(tile_path);
    Carc_Playboard_Node *playboard_first_node = CBP_init_playboard(start_tile)->node;
    Carc_Rim *rim = CBRim_initiate(playboard_first_node);
    CBRim_display(rim);
    printf("remove first\n");
    printf("Rim is now:\n");
    CBRim_remove_first(rim);
    CBRim_display(rim);
    printf("remove first\n");
    printf("remove first\n");
    CBRim_remove_first(rim);
    CBRim_remove_first(rim);
    //Test remove last node
    printf("remove first\n");
    CBRim_remove_first(rim);
    printf("Rim is now:\n");
    CBRim_display(rim);
    //Test when rim is empty
    CBRim_remove_first(rim);
    //Test rim is null
    CBRim_remove_first(NULL);
    CBRim_free(rim);
    CBP_free_playboard_node(playboard_first_node);
    free(tile_path);
}

void test_rim_remove_by_node_coordinates(){
    printf("test_rim_remove_by_node_coordinates results: ");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node *node = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(0,1)),
                        *node2 = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(1,0)),
                        *node3 = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(0,-1)),
                        *node4 = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(-1,0));
    Carc_Rim* rim = CB_Rim_new_empty();
    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(0,1))==0);

    CBRim_insert_playboard_node(rim,node);
    CBRim_insert_playboard_node(rim,node2);
    CBRim_insert_playboard_node(rim,node3);
    CBRim_insert_playboard_node(rim,node4);

    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(0,-1))==1);
    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(0,1))==1);
    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(1,0))==1);
    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(-1,0))==1);
    printf("%d",CBRim_remove_by_node_coordinates(rim,CBP_Location_new(0,0))==0);
    printf("%d",CBRim_remove_by_node_coordinates(NULL,CBP_Location_new(0,0))==0);

    CBRim_free(rim);
    free(tile_path);
}

void test_rim_find_by_location(){
    printf("CB_Rim_find_by_location results: \n");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile *start_tile = CBT_new_tile_from_file(tile_path);
    Carc_Playboard_Origin *playboard_origin = CBP_init_playboard(start_tile);
    Carc_Playboard_Node *playboard_first_node = playboard_origin->node;
    Carc_Rim* rim = CBRim_initiate(playboard_first_node);

    Carc_Playboard_Node* test_node;
    Carc_Playboard_Location test_loc = CBP_Location_new(0,1);
    test_node = CB_Rim_find_by_location(rim, test_loc);

    printf("%d",CBP_Location_cmp(test_node->node_coordinates,test_loc)==0);
    test_loc.x = 1;
    test_node = CB_Rim_find_by_location(rim, test_loc);
    printf("%d",test_node==NULL);
    test_loc.y = 0;
    test_node = CB_Rim_find_by_location(rim, test_loc);
    printf("%d",CBP_Location_cmp(test_node->node_coordinates,test_loc)==0);
    test_node = CB_Rim_find_by_location(NULL, test_loc);
    printf("%d\n",test_node==NULL);

    CBRim_free(rim);
    CBP_free_playboard_node(test_node);
    CBP_free_playboard(playboard_origin);
    free(tile_path);
}

void test_rim_initiate(){
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node *node = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(0,0));
    Carc_Rim *rim = CBRim_initiate(node);
    int success = 1;
    success = success && CBRim_remove_by_node_coordinates(rim,CBP_Location_new(0,1));

    node->node_coordinates = CBP_Location_new(12,65);
    free(tile_path);
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
