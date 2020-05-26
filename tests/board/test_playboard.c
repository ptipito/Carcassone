#include "tests/board/test_playboard.h"

void test_new_location(){
    printf("test_new_location results: ");
    Carc_Playboard_Location loc_test;
    int i,j;
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            loc_test = CBP_Location_new(i,j);
            printf("%d%d",loc_test.x==i,loc_test.y==j);
        }
    }
}

void test_location_cmp(){
    printf("test_location_cmp results: ");
    Carc_Playboard_Location loc1 = CBP_Location_new(2,3),
                            loc2 = CBP_Location_new(2,6),
                            loc3 = CBP_Location_new(2,1),
                            loc4 = CBP_Location_new(1,3),
                            loc5 = CBP_Location_new(1,6),
                            loc6 = CBP_Location_new(3,6),
                            loc7 = CBP_Location_new(3,6),
                            loc8 = CBP_Location_new(2,3);

    printf("%d",CBP_Location_cmp(loc1,loc2)==-1);
    printf("%d",CBP_Location_cmp(loc1,loc3)==1);
    printf("%d",CBP_Location_cmp(loc1,loc4)==1);
    printf("%d",CBP_Location_cmp(loc1,loc5)==1);
    printf("%d",CBP_Location_cmp(loc1,loc6)==-1);
    printf("%d",CBP_Location_cmp(loc1,loc7)==-1);
    printf("%d",CBP_Location_cmp(loc1,loc8)==0);
    printf("%d",CBP_Location_cmp(loc1,loc1)==0);
    printf("%d",CBP_Location_cmp(loc6,loc2)==1);
    printf("%d",CBP_Location_cmp(loc4,loc3)==-1);
}

void test_new_playboard_node(){
    printf("test_new_playboard_node results: ");

    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile* test_tile = CBT_new_tile_from_file(tile_path);
    Carc_Playboard_Location test_loc = CBP_Location_new(0,1);
    Carc_Playboard_Node *test_node = CBP_new_playboard_node(test_tile,test_loc),
                        *test_node2 = CBP_new_playboard_node(NULL,test_loc);

    int neigh_init_ok = 1;
    int i=0;
    while(i<CBP_MAX_NEIGHBORS && neigh_init_ok){
        neigh_init_ok = test_node->neighbors[i]==NULL;
        i++;
    }
    neigh_init_ok = (neigh_init_ok && CBT_tile_cmp(test_tile,test_node->node)==0);

    printf("%d",test_node!=NULL &&neigh_init_ok && test_node->node_coordinates.x==0 && test_node->node_coordinates.y==1);
    neigh_init_ok=1;
    i=0;
    while(i<CBP_MAX_NEIGHBORS && neigh_init_ok){
        neigh_init_ok = test_node2->neighbors[i]==NULL;
        i++;
    }

    neigh_init_ok = (neigh_init_ok && CBT_tile_cmp(NULL,test_node2->node)==0);
    printf("%d",test_node2!=NULL && neigh_init_ok && test_node2->node_coordinates.x==0 && test_node2->node_coordinates.y==1);

    CBP_free_playboard_node(test_node);
    CBP_free_playboard_node(test_node2);
    free(tile_path);
}

void test_node_cmp(){
    printf("test_node_equality results: ");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile *tile = CBT_new_tile_from_file(tile_path);
    Carc_Playboard_Node *n1 = CBP_new_playboard_node(NULL,CBP_Location_new(0,0)),
                        *n2 = CBP_new_playboard_node(tile,CBP_Location_new(0,0));

    printf("%d",CBP_node_cmp(n1,n2)==1);
    n1->node = tile;
    printf("%d",CBP_node_cmp(n1,n2)==0);
    n1->node = NULL;
    n2->node = NULL;
    printf("%d",CBP_node_cmp(n1,n2)==0);
    n1->node = tile;
    n2->node = tile;
    n1->node_coordinates = CBP_Location_new(1,2);
    printf("%d",CBP_node_cmp(n1,n2)==1);
    n2->node_coordinates = CBP_Location_new(1,2);
    n1->neighbors[CPCS_UP] = CBP_new_empty_playboard_node(CBP_Location_new(1,3));
    printf("%d",CBP_node_cmp(n1,n2)==1);
    n2->neighbors[CPCS_UP] = CBP_new_empty_playboard_node(CBP_Location_new(1,3));
    printf("%d",CBP_node_cmp(n1,n2)==1);
    CBP_free_playboard_node(n2->neighbors[CPCS_UP]);
    n2->neighbors[CPCS_UP] = n1->neighbors[CPCS_UP];
    printf("%d",CBP_node_cmp(n1,n2)==0);
    printf("\n");
    CBP_free_playboard_node(n1->neighbors[CPCS_UP]);
    CBP_free_playboard_node(n1);
    n2->node=NULL;//avoid double free of tile (n1 and n2 have the same pointer in "node")
    CBP_free_playboard_node(n2);
    free(tile_path);
}

void test_connect_is_possible(){
    printf("test CBP_connect_is_possible results: ");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile *origin_tile = CBT_new_tile_from_file(tile_path),
              *tile2 = CBT_new_tile_from_file(tile_path);


    Carc_Playboard_Location loc;
    loc.x =0;
    loc.y=1;
    Carc_Playboard_Node *origin = CBP_new_playboard_node(origin_tile,CBP_Location_new(0,0)),
                        *node1 = CBP_new_playboard_node(tile2,loc);

    printf("%d",CBP_connect_is_possible(node1,CPCS_UP,origin)==0);
    printf("%d",CBP_connect_is_possible(node1,CPCS_RIGHT,origin)==1);
    printf("%d",CBP_connect_is_possible(node1,CPCS_DOWN,origin)==0);
    printf("%d",CBP_connect_is_possible(node1,CPCS_LEFT,origin)==1);
    CBT_turn(tile2,CCT_UPDOWN);
    printf("%d",CBP_connect_is_possible(node1,CPCS_UP,origin)==1);
    printf("%d",CBP_connect_is_possible(node1,CPCS_RIGHT,origin)==1);
    printf("%d",CBP_connect_is_possible(node1,CPCS_DOWN,origin)==1);
    printf("%d",CBP_connect_is_possible(node1,CPCS_LEFT,origin)==1);

    CBT_free_tile(origin_tile);
    CBT_free_tile(tile2);
    free(tile_path);
}

void test_init_playboard(){
    printf("test_init_playboard results: ");
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Tile* start_tile = CBT_new_tile_from_file(tile_path);
    Carc_Playboard_Node* start_node = CBP_new_playboard_node(start_tile,CBP_Location_new(0,0));
    Carc_Playboard_Origin* playboard = CBP_init_playboard(start_tile);

    printf("%d",playboard != NULL && CBP_node_cmp(playboard->node,start_node)==0);
    CBP_free_playboard(playboard);
    playboard = CBP_init_playboard(NULL);
    printf("%d",playboard != NULL && CBP_node_is_empty(*(playboard->node)));
    printf("%d",playboard != NULL && CBP_node_cmp(playboard->node,start_node)==1);

    CBP_free_playboard(playboard);
    start_node->node = NULL;//Avoiding double free
    CBP_free_playboard_node(start_node);
    free(tile_path);
}

void test_get_opposite_side(){
    printf("test_get_opposite_side results: ");
    printf("%d",CBP_get_opposite_side(CPCS_UP)==CPCS_DOWN);
    printf("%d",CBP_get_opposite_side(CPCS_DOWN)==CPCS_UP);
    printf("%d",CBP_get_opposite_side(CPCS_LEFT)==CPCS_RIGHT);
    printf("%d",CBP_get_opposite_side(CPCS_RIGHT)==CPCS_LEFT);
}

void test_get_neighbor_loc(){
    printf("test_get_neighbor_loc results: ");
    Carc_Playboard_Location start_loc = CBP_Location_new(0,0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_RIGHT),CBP_Location_new(0,0))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_RIGHT),CBP_Location_new(1,0))==0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_UP),CBP_Location_new(0,0))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_RIGHT),CBP_Location_new(1,0))==0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_DOWN),CBP_Location_new(0,0))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_DOWN),CBP_Location_new(0,-1))==0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_LEFT),CBP_Location_new(0,0))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_LEFT),CBP_Location_new(-1,0))==0);

    start_loc = CBP_Location_new(5,3);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_RIGHT),CBP_Location_new(5,8))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_RIGHT),CBP_Location_new(6,3))==0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_UP),CBP_Location_new(0,0))!=0);
    printf("%d",CBP_Location_cmp(CBP_get_neighbor_loc(start_loc,CPCS_UP),CBP_Location_new(5,4))==0);
}

void test_create_neighbor_for(){
    char* tile_path = CT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node* start_node = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(0,0));
    Carc_Playboard_Node* neigh1 = CBP_create_rim_neigh_for(start_node,CPCS_UP);
    printf("test_get_neighbor_loc results: ");
    printf("%d",neigh1!=NULL && neigh1->neighbors[CPCS_DOWN]==start_node);
    Carc_Playboard_Node* neigh2 = CBP_create_rim_neigh_for(start_node,CPCS_LEFT);
    printf("%d",neigh2!=NULL && neigh2->neighbors[CPCS_RIGHT]==start_node);
    printf("%d",neigh2!=NULL && !(neigh1->neighbors[CPCS_UP]==start_node));
    printf("%d",neigh2!=NULL && neigh1->neighbors[CPCS_DOWN]==start_node);
}

void test_playboard_run_all(){
    test_new_location();
    printf("\n");
    test_location_cmp();
    printf("\n");
    test_new_playboard_node();
    printf("\n");
    test_node_cmp();
    printf("\n");
    test_connect_is_possible();
    printf("\n");
    test_init_playboard();
    printf("\n");
    test_get_opposite_side();
    printf("\n");
    test_get_neighbor_loc();
    printf("\n");
    test_create_neighbor_for();
}
