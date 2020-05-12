#include "tests/model/test_playboard.h"

void test_new_location(){
    printf("test_new_location results: ");
    Carc_Playboard_Location loc_test;
    int i,j;
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            loc_test = CP_Location_new(i,j);
            printf("%d%d",loc_test.x==i,loc_test.y==j);
        }
    }
}

void test_location_cmp(){
    printf("test_location_cmp results: ");
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
}

void test_new_playboard_node(){
    printf("test_new_playboard_node results: ");

    Carc_Tile* test_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Location test_loc = CP_Location_new(0,1);
    Carc_Playboard_Node *test_node = CP_new_playboard_node(test_tile,test_loc),
                        *test_node2 = CP_new_playboard_node(NULL,test_loc);

    int neigh_init_ok = 1;
    int i=0;
    while(i<CP_MAX_NEIGHBORS && neigh_init_ok){
        neigh_init_ok = test_node->neighbors[i]==NULL;
        i++;
    }
    neigh_init_ok = (neigh_init_ok && CT_tile_cmp(test_tile,test_node->node)==0);

    printf("%d",test_node!=NULL &&neigh_init_ok && test_node->node_coordinates.x==0 && test_node->node_coordinates.y==1);
    neigh_init_ok=1;
    i=0;
    while(i<CP_MAX_NEIGHBORS && neigh_init_ok){
        neigh_init_ok = test_node2->neighbors[i]==NULL;
        i++;
    }

    neigh_init_ok = (neigh_init_ok && CT_tile_cmp(NULL,test_node2->node)==0);
    printf("%d",test_node2!=NULL && neigh_init_ok && test_node2->node_coordinates.x==0 && test_node2->node_coordinates.y==1);

    CP_free_playboard_node(test_node);
    CP_free_playboard_node(test_node2);
}

void test_node_cmp(){
    printf("test_node_equality results: ");
    Carc_Tile *tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node *n1 = CP_new_playboard_node(NULL,CP_Location_new(0,0)),
                        *n2 = CP_new_playboard_node(tile,CP_Location_new(0,0));

    printf("%d",CP_node_cmp(n1,n2)==1);
    n1->node = tile;
    printf("%d",CP_node_cmp(n1,n2)==0);
    n1->node = NULL;
    n2->node = NULL;
    printf("%d",CP_node_cmp(n1,n2)==0);
    n1->node = tile;
    n2->node = tile;
    n1->node_coordinates = CP_Location_new(1,2);
    printf("%d",CP_node_cmp(n1,n2)==1);
    n2->node_coordinates = CP_Location_new(1,2);
    n1->neighbors[CP_UP] = CP_new_empty_playboard_node(CP_Location_new(1,3));
    printf("%d",CP_node_cmp(n1,n2)==1);
    n2->neighbors[CP_UP] = CP_new_empty_playboard_node(CP_Location_new(1,3));
    printf("%d",CP_node_cmp(n1,n2)==1);
    CP_free_playboard_node(n2->neighbors[CP_UP]);
    n2->neighbors[CP_UP] = n1->neighbors[CP_UP];
    printf("%d",CP_node_cmp(n1,n2)==0);
    printf("\n");
    CP_free_playboard_node(n1->neighbors[CP_UP]);
    CP_free_playboard_node(n1);
    n2->node=NULL;//avoid double free of tile (n1 and n2 have the same pointer in "node")
    CP_free_playboard_node(n2);
}

void test_connect_is_possible(){
    printf("test CP_connect_is_possible results: ");
    Carc_Tile *origin_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
              *tile2 = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");


    Carc_Playboard_Location loc;
    loc.x =0;
    loc.y=1;
    Carc_Playboard_Node *origin = CP_new_playboard_node(origin_tile,CP_Location_new(0,0)),
                        *node1 = CP_new_playboard_node(tile2,loc);

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

void test_init_playboard(){
    printf("test_init_playboard results: ");
    Carc_Tile* start_tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Node* start_node = CP_new_playboard_node(start_tile,CP_Location_new(0,0));
    Carc_Playboard_Origin* playboard = CP_init_playboard(start_tile);

    printf("%d",playboard != NULL && CP_node_cmp(playboard->node,start_node)==0);
    CP_free_playboard(playboard);
    playboard = CP_init_playboard(NULL);
    printf("%d",playboard != NULL && CP_node_is_empty(*(playboard->node)));
    printf("%d",playboard != NULL && CP_node_cmp(playboard->node,start_node)==1);

    CP_free_playboard(playboard);
    start_node->node = NULL;//Avoiding double free
    CP_free_playboard_node(start_node);
}

void test_get_opposite_side(){
    printf("test_get_opposite_side results: ");
    printf("%d",CP_get_opposite_side(CP_UP)==CP_DOWN);
    printf("%d",CP_get_opposite_side(CP_DOWN)==CP_UP);
    printf("%d",CP_get_opposite_side(CP_LEFT)==CP_RIGHT);
    printf("%d",CP_get_opposite_side(CP_RIGHT)==CP_LEFT);
}

void test_get_neighbor_loc(){
    printf("test_get_neighbor_loc results: ");
    Carc_Playboard_Location start_loc = CP_Location_new(0,0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_RIGHT),CP_Location_new(0,0))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_RIGHT),CP_Location_new(1,0))==0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_UP),CP_Location_new(0,0))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_RIGHT),CP_Location_new(1,0))==0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_DOWN),CP_Location_new(0,0))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_DOWN),CP_Location_new(0,-1))==0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_LEFT),CP_Location_new(0,0))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_LEFT),CP_Location_new(-1,0))==0);

    start_loc = CP_Location_new(5,3);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_RIGHT),CP_Location_new(5,8))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_RIGHT),CP_Location_new(6,3))==0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_UP),CP_Location_new(0,0))!=0);
    printf("%d",CP_Location_cmp(CP_get_neighbor_loc(start_loc,CP_UP),CP_Location_new(5,4))==0);
}

void test_create_neighbor_for(){
    Carc_Playboard_Node* start_node = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),CP_Location_new(0,0));
    Carc_Playboard_Node* neigh1 = CP_create_rim_neigh_for(start_node,CP_UP);
    printf("test_get_neighbor_loc results: ");
    printf("%d",neigh1!=NULL && neigh1->neighbors[CP_DOWN]==start_node);
    Carc_Playboard_Node* neigh2 = CP_create_rim_neigh_for(start_node,CP_LEFT);
    printf("%d",neigh2!=NULL && neigh2->neighbors[CP_RIGHT]==start_node);
    printf("%d",neigh2!=NULL && !(neigh1->neighbors[CP_UP]==start_node));
    printf("%d",neigh2!=NULL && neigh1->neighbors[CP_DOWN]==start_node);
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
