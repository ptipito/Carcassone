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

void test_playboard_get_neighbor(){
    printf("test_playboard_get_neighbor results: ");
    Carc_Playboard_Node *empty_node = CBP_new_empty_playboard_node(CBP_Location_new(0,0)),
                        *node = CBP_new_playboard_node(NULL,CBP_Location_new(0,1));

    printf("%d",CBP_get_neighbor(NULL,CPCS_UP)==NULL);
    printf("%d",CBP_get_neighbor(empty_node,CPCS_UP)==NULL);
    printf("%d",CBP_get_neighbor(empty_node,CPCS_DOWN)==NULL);
    printf("%d",CBP_get_neighbor(empty_node,CPCS_LEFT)==NULL);
    printf("%d",CBP_get_neighbor(empty_node,CPCS_RIGHT)==NULL);
    node->neighbors[CPCS_DOWN] = malloc(sizeof(Carc_Playboard_Node**));
    *(node->neighbors[CPCS_DOWN]) = empty_node;
    printf("%d",CBP_get_neighbor(node,CPCS_DOWN)==empty_node);

    CBP_free_playboard_node(empty_node);
    CBP_free_playboard_node(node);
}

void test_playboard_set_neighbor(){
    printf("test_playboard_set_neighbor results: ");
    Carc_Playboard_Node *empty_node = CBP_new_empty_playboard_node(CBP_Location_new(0,0)),
                        *node = CBP_new_playboard_node(NULL,CBP_Location_new(0,1));

    printf("%d\n",CBP_set_neighbor(NULL,CPCS_UP,&node)==-1);
    printf("%d\n",CBP_set_neighbor(empty_node,CPCS_UP,&node)==0
                    && *(empty_node->neighbors[CPCS_UP])==node);
    node->neighbors[CPCS_DOWN] = malloc(sizeof(Carc_Playboard_Node**));
    node->neighbors[CPCS_UP] = malloc(sizeof(Carc_Playboard_Node**));
    node->neighbors[CPCS_LEFT] = malloc(sizeof(Carc_Playboard_Node**));
    node->neighbors[CPCS_RIGHT] = malloc(sizeof(Carc_Playboard_Node**));
    printf("%d",CBP_set_neighbor(node,CPCS_DOWN,&empty_node)==0
                    && *(node->neighbors[CPCS_DOWN])==empty_node);
    printf("%d",CBP_set_neighbor(node,CPCS_UP,&empty_node)==0
                    && *(node->neighbors[CPCS_UP])==empty_node);
    printf("%d",CBP_set_neighbor(node,CPCS_LEFT,&empty_node)==0
                    && *(node->neighbors[CPCS_LEFT])==empty_node);
    printf("%d",CBP_set_neighbor(node,CPCS_RIGHT,&empty_node)==0
                    && *(node->neighbors[CPCS_RIGHT])==empty_node);

    CBP_free_playboard_node(empty_node);
    CBP_free_playboard_node(node);
}

void test_playboard_is_neigh_null(){
    printf("test_playboard_set_neighbor results: ");
    Carc_Playboard_Node *empty_node = CBP_new_empty_playboard_node(CBP_Location_new(0,0)),
                        *node = CBP_new_playboard_node(NULL,CBP_Location_new(0,1));

    printf("%d",CBP_is_neighbor_null(NULL,CPCS_RIGHT)==-1);
    printf("%d",CBP_is_neighbor_null(empty_node,CPCS_RIGHT)==1);
    CBP_set_neighbor(empty_node,CPCS_RIGHT,&node);
    printf("%d",CBP_is_neighbor_null(empty_node,CPCS_RIGHT)==0);

    CBP_free_playboard_node(empty_node);
    CBP_free_playboard_node(node);
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

    char* tile_path = CBT_get_tile_file_path("tile1.txt");
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
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
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
    Carc_Playboard_Node* new_node = CBP_new_empty_playboard_node(CBP_Location_new(1,3));
    CBP_set_neighbor(n1,CPCS_UP,&new_node);
    printf("%d",CBP_node_cmp(n1,n2)==1);
    Carc_Playboard_Node* new_node2 = CBP_new_empty_playboard_node(CBP_Location_new(1,3));
    CBP_set_neighbor(n2,CPCS_UP,&new_node2);
    printf("%d",CBP_node_cmp(n1,n2)==1);
    CBP_free_playboard_node(CBP_get_neighbor(n2,CPCS_UP));
    CBP_set_neighbor(n2,CPCS_UP,n1->neighbors[CPCS_UP]);
    printf("%d",CBP_node_cmp(n1,n2)==0);
    printf("\n");
    CBP_free_playboard_node(CBP_get_neighbor(n1,CPCS_UP));
    CBP_free_playboard_node(n1);
    n2->node=NULL;//avoid double free of tile (n1 and n2 have the same pointer in "node")
    CBP_free_playboard_node(n2);
    free(tile_path);
}

void test_connect_is_possible(){
    printf("test CBP_connect_is_possible results: ");
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
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
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
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
    printf("test_create_neighbor_for results: ");
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node* start_node = CBP_new_playboard_node(CBT_new_tile_from_file(tile_path),CBP_Location_new(0,0));
    Carc_Playboard_Node* neigh1 = CBP_create_rim_neigh_for(&start_node,CPCS_UP);
    printf("%d",neigh1!=NULL && CBP_get_neighbor(neigh1,CPCS_DOWN)==start_node);
    Carc_Playboard_Node* neigh2 = CBP_create_rim_neigh_for(&start_node,CPCS_LEFT);
    printf("%d",neigh2!=NULL && CBP_get_neighbor(neigh2,CPCS_RIGHT)==start_node);
    printf("%d",neigh2!=NULL && !(CBP_get_neighbor(neigh1,CPCS_UP)==start_node));
    printf("%d",neigh2!=NULL && CBP_get_neighbor(neigh1,CPCS_DOWN)==start_node);
}

void test_playboard_add_pawn_in(){
    printf("test_playboard_add_pawn_in results: ");
    char* tile_start_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node *node_empty=CBP_new_empty_playboard_node(CBP_Location_new(0,2)),
              *node_start=CBP_new_playboard_node(CBT_new_tile_from_file(tile_start_str),CBP_Location_new(0,0));
    Carc_Pawn* pawn=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_1,CPC_BLACK),PAWN_NORMAL);

    //Tile init without pawn tested in tile init tests
    //Test cannot add pawn on NULL node
    printf("%d\n",CBP_add_pawn_in(pawn,NULL,CTL_CENTER)==-1);
    //Test adding pawn on empty node
    printf("%d\n",CBP_add_pawn_in(pawn,node_empty,CTL_CENTER)==-1);
    //Test adding on center
    printf("%d",CBP_add_pawn_in(pawn,node_start,CTL_CENTER)==0
                && CBT_get_node_from_loc(node_start->node,CTL_CENTER)->pawn!=NULL);
    //Test adding on edge
    printf("%d\n",CBP_add_pawn_in(pawn,node_start,CTL_NORTH_EAST)==0
                && CBT_get_node_from_loc(node_start->node,CTL_NORTH_EAST)->pawn!=NULL);
    //Test when player does not have pawns
    pawn->owner->nb_pawns[PAWN_NORMAL] = 0;
    printf("%d",CBP_add_pawn_in(pawn,node_start,CTL_SOUTH)==-1
                && CBT_get_node_from_loc(node_start->node,CTL_SOUTH)->pawn==NULL);

    free(tile_start_str);
    CBP_free_playboard_node(node_empty);
    CBP_free_playboard_node(node_start);
    CPPlayer_free_player(pawn->owner);
    CPPawn_free_pawn(pawn);
}

void test_playboard_rm_pawn_in(){
    printf("test_playboard_rm_pawn_in results: ");
    char* tile_start_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Playboard_Node *node_empty=CBP_new_empty_playboard_node(CBP_Location_new(0,2)),
              *node_start=CBP_new_playboard_node(CBT_new_tile_from_file(tile_start_str),CBP_Location_new(0,0));
    Carc_Pawn *pawn1=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_1,CPC_BLACK),PAWN_NORMAL),
              *pawn2=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_2,CPC_BLUE),PAWN_NORMAL);

    //Test remove pawn from NULL node
    printf("%d\n",CBP_rm_pawn_in(NULL,CTL_CENTER)==-1);
    printf("%d\n",CBP_rm_pawn_in(NULL,CTL_NORTH)==-1);

    //Test remove pawn from empty node
    printf("%d\n",CBP_rm_pawn_in(node_empty,CTL_CENTER)==-1);
    printf("%d\n",CBP_rm_pawn_in(node_empty,CTL_NORTH)==-1);

    //Test remove pawn when no pawn on the location for this node
    printf("%d\n",CBP_rm_pawn_in(node_start,CTL_CENTER)==-1);
    printf("%d\n",CBP_rm_pawn_in(node_start,CTL_NORTH)==-1);

    //Test remove pawn when player already at max
    node_start->node->center.pawn = pawn1;
    node_start->node->border[CTL_NORTH].pawn = pawn2;
    printf("%d\n",CBP_rm_pawn_in(node_start,CTL_CENTER)==-1
                && node_start->node->center.pawn==pawn1);
    printf("%d\n",CBP_rm_pawn_in(node_start,CTL_NORTH)==-1
                && node_start->node->border[CTL_NORTH].pawn==pawn2);

    //Test remove pawn when player not at max
    pawn1->owner->nb_pawns[PAWN_NORMAL]--;
    pawn2->owner->nb_pawns[PAWN_NORMAL]--;
    printf("%d",CBP_rm_pawn_in(node_start,CTL_CENTER)==0
                && node_start->node->center.pawn==NULL);
    printf("%d",CBP_rm_pawn_in(node_start,CTL_NORTH)==0
                && node_start->node->border[CTL_NORTH].pawn==NULL);

    free(tile_start_str);
    CBP_free_playboard_node(node_empty);
    CBP_free_playboard_node(node_start);
    CPPlayer_free_player(pawn1->owner);
    CPPlayer_free_player(pawn2->owner);
    CPPawn_free_pawn(pawn1);
    CPPawn_free_pawn(pawn2);
}

void test_playboard_run_all(){
    test_new_location();
    printf("\n***********************************************\n");
    test_location_cmp();
    printf("\n***********************************************\n");
    test_new_playboard_node();
    printf("\n***********************************************\n");
    test_node_cmp();
    printf("\n***********************************************\n");
    test_connect_is_possible();
    printf("\n***********************************************\n");
    test_init_playboard();
    printf("\n***********************************************\n");
    test_get_opposite_side();
    printf("\n***********************************************\n");
    test_get_neighbor_loc();
    printf("\n***********************************************\n");
    test_create_neighbor_for();
    printf("\n***********************************************\n");
    test_playboard_add_pawn_in();
    printf("\n***********************************************\n");
    test_playboard_rm_pawn_in();
    printf("\n***********************************************\n");
    test_playboard_get_neighbor();
    printf("\n***********************************************\n");
    test_playboard_set_neighbor();
    printf("\n***********************************************\n");
    test_playboard_is_neigh_null();
    printf("\n***********************************************\n");

}
