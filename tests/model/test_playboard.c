#include "tests/model/test_playboard.h"

void test_new_location(){
    Carc_Playboard_Location loc_test;
    int i,j;
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            loc_test = CP_Location_new(i,j);
            printf("%d%d",loc_test.x==i,loc_test.y==j);
        }
    }
    printf("\n");
}

void test_location_cmp(){
    Carc_Playboard_Location loc1 = CP_Location_new(2,3),
                            loc2 = CP_Location_new(2,6),
                            loc3 = CP_Location_new(2,1),
                            loc4 = CP_Location_new(1,3),
                            loc5 = CP_Location_new(1,6),
                            loc6 = CP_Location_new(3,6),
                            loc7 = CP_Location_new(3,6),
                            loc8 = CP_Location_new(2,3);

    printf("%d",CP_Location_cmp(loc1,loc2)==-1);
    printf("%d",CP_Location_cmp(loc1,loc3)==1);
    printf("%d",CP_Location_cmp(loc1,loc4)==1);
    printf("%d",CP_Location_cmp(loc1,loc5)==1);
    printf("%d",CP_Location_cmp(loc1,loc6)==-1);
    printf("%d",CP_Location_cmp(loc1,loc7)==-1);
    printf("%d",CP_Location_cmp(loc1,loc8)==0);
    printf("%d",CP_Location_cmp(loc1,loc1)==0);
    printf("%d",CP_Location_cmp(loc6,loc2)==1);
    printf("%d",CP_Location_cmp(loc4,loc3)==-1);
    printf("\n");
}

void test_new_playboard_node(){

    Carc_Tile* test_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Location test_loc = CP_Location_new(0,1);
    Carc_Playboard_Node* test_node = CP_new_playboard_node(test_tile,test_loc);

    int neigh_init_ok = 1;
    int i=0;
    while(i<CP_MAX_NEIGHBORS && neigh_init_ok){
        neigh_init_ok = test_node->neighbors[i]==NULL;
        i++;
    }
    printf("%d %d %d %d",test_node!=NULL,neigh_init_ok, test_node->node_coordinates.x==0, test_node->node_coordinates.y==1);

    CP_free_playboard_node(test_node);
}

void test_connect_is_possible(){
    printf("test CP_connect_is_possible\n");
    Carc_Tile *origin_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
              *tile2 = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");


    Carc_Playboard_Location loc;
    loc.x =0;
    loc.y=1;
    Carc_Playboard_Node *origin = CP_create_playboard(origin_tile),
                        *node1 = CP_new_playboard_node(tile2,loc);

    printf("test results: ");
    printf("%d",CP_connect_is_possible(node1,CP_UP,origin)==0);
    printf("%d",CP_connect_is_possible(node1,CP_RIGHT,origin)==1);
    printf("%d",CP_connect_is_possible(node1,CP_DOWN,origin)==0);
    printf("%d",CP_connect_is_possible(node1,CP_LEFT,origin)==1);
    CT_turn(tile2,UPDOWN);
    printf("%d",CP_connect_is_possible(node1,CP_UP,origin)==1);
    printf("%d",CP_connect_is_possible(node1,CP_RIGHT,origin)==1);
    printf("%d",CP_connect_is_possible(node1,CP_DOWN,origin)==1);
    printf("%d",CP_connect_is_possible(node1,CP_LEFT,origin)==1);

    CT_Free_Tile(origin_tile);
    CT_Free_Tile(tile2);
}


