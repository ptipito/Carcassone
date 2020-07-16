#include "tests/board/test_tile.h"

void test_tile_CBT_display_tile(Carc_Tile tile){
    CBT_display_tile(tile);
}

void test_tile_new_empty_tile(){
    printf("test CBT_new_empty_tile results: ");
    int result=1, i=0, j=0;
    Carc_Tile* tile = CBT_new_empty_tile();
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        if(tile->center_connexions[i]!=0
           || tile->border[i].construction != NULL
           || tile->border[i].pawn != NULL)
            result=0;

        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            if(tile->border_connexions[i][j]!=0){
                result = 0;
            }
        }
    }
    if(tile->center.construction != NULL || tile->center.pawn != NULL)
        result = 0;
    printf("%d",result && tile->rotation==CTTT_NONE);
}

void test_tile_parse_tile_file(){
    printf("test_tile_parse_tile_file: \n");
    char *tile1_path = CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_path = CBT_get_tile_file_path("cloister_path.txt");
    Carc_Tile* tile = CBT_new_tile_from_file(tile1_path);
    Carc_Tile* tile2 = CBT_new_tile_from_file(cloister_tile_path);
    printf("*******display start tile*******\n");
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_NONE);
    printf("\n*******display cloister_path tile*******\n");
    CBT_display_tile(*tile2);
    printf("cur rotation is ok: %d\n",tile2->rotation==CTTT_NONE);
    CBT_free_tile(tile);
    CBT_free_tile(tile2);
    free(tile1_path);
    free(cloister_tile_path);
}

void test_tile_turn_tile(){
    printf("test_tile_turn_tile:\n");
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
    Carc_Tile* tile = CBT_new_tile_from_file(tile_path);
    CBT_display_tile(*tile);

    printf("\tTURN CTTT_RIGHT\n");
    CBT_turn(tile,CTTT_RIGHT);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_RIGHT);

    printf("\tTURN 180\n");
    CBT_turn(tile,CTTT_UPDOWN);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_LEFT);

    printf("\tTURN CTTT_LEFT\n");
    CBT_turn(tile,CTTT_LEFT);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_UPDOWN);

    printf("\tTURN CTTT_LEFT\n");
    CBT_turn(tile,CTTT_LEFT);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_RIGHT);

    printf("\tTURN 180\n");
    CBT_turn(tile,CTTT_UPDOWN);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_LEFT);

    printf("\tTURN CTTT_NONE\n");
    CBT_turn(tile,CTTT_NONE);
    CBT_display_tile(*tile);
    printf("cur rotation is ok: %d\n",tile->rotation==CTTT_LEFT);

    CBT_free_tile(tile);
    free(tile_path);
}

void test_tile_tile_cmp(){
    printf("test_tile_cmp resutls: ");
    char* tile_path = CBT_get_tile_file_path("tile1.txt");
    Carc_Tile *t1 = CBT_new_tile_from_file(tile_path),
              *t2 = CBT_new_empty_tile();
    Carc_Tile t3 = *t1,
              t4 = *t1;
    t3.center_connexions[CTL_NORTH] = 1;
    t4.border_connexions[CTL_NORTH_EAST][CTL_EAST_NORTH] = 1;

    int test1 = CBT_tile_cmp(t1,t1);
    int test2 = CBT_tile_cmp(t1,t2);
    int test3 = CBT_tile_cmp(t1,&t3);
    int test4 = CBT_tile_cmp(t1,&t4);
    int test5 = CBT_tile_cmp(NULL,NULL);
    int test6 = CBT_tile_cmp(t1,NULL);
    printf("%d%d%d%d%d%d",test1==0,test2==1,test3==1,test4==1,test5==0,test6==1);
    CBT_free_tile(t1);
    CBT_free_tile(t2);
    free(tile_path);
}

void test_tile_CBT_new_node(){
    printf("test_CBT_new_node_results: ");
    Carc_Construction *city = CBC_new_city(1,0,CCM_NONE);
    Carc_Construction *path = CBC_new_path(1);
    Carc_Tile_Node* node = CBT_new_node(CBCT_CITY, city);

    printf("%d",node->pawn==NULL && node->node_type==CBCT_CITY && CBC_construction_cmp(node->construction, CBCT_CITY, city, CBCT_CITY)==0);

    node = CBT_new_node(CBCT_CITY, city);
    printf("%d",node->pawn==NULL && node->node_type==CBCT_CITY && CBC_construction_cmp(node->construction, CBCT_PATH, path, CBCT_PATH)==0);
}

void test_tile_is_valid_loc(){
    printf("test_tile_is_valid_loc results: ");
    printf("%d",CBT_is_valid_loc(CTL_CENTER)==1);
    printf("%d",CBT_is_valid_loc(CTL_NORTH_WEST)==1);
    printf("%d",CBT_is_valid_loc(CTL_NORTH)==1);
    printf("%d",CBT_is_valid_loc(CTL_NORTH_EAST)==1);
    printf("%d",CBT_is_valid_loc(CTL_EAST_NORTH)==1);
    printf("%d",CBT_is_valid_loc(CTL_EAST)==1);
    printf("%d",CBT_is_valid_loc(CTL_EAST_SOUTH)==1);
    printf("%d",CBT_is_valid_loc(CTL_SOUTH_WEST)==1);
    printf("%d",CBT_is_valid_loc(CTL_SOUTH)==1);
    printf("%d",CBT_is_valid_loc(CTL_SOUTH_EAST)==1);
    printf("%d",CBT_is_valid_loc(CTL_WEST_SOUTH)==1);
    printf("%d",CBT_is_valid_loc(CTL_WEST)==1);
    printf("%d",CBT_is_valid_loc(CTL_WEST_NORTH)==1);

}

void test_tile_get_node_from_loc(){
    printf("test_get_node_from_loc results: ");
    char *tile_path = CBT_get_tile_file_path("tile1.txt");
    Carc_Tile *tile = CBT_new_tile_from_file(tile_path);
    Carc_Tile_Location loc = CTL_NORTH_WEST;
    Carc_Tile_Node* node = CBT_get_node_from_loc(tile,loc);
    Carc_Tile_Node* node2 = CBT_get_node_from_loc(tile,CTL_CENTER);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node2)!=0);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_NORTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_NORTH_EAST;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_EAST_NORTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_EAST;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_EAST_SOUTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_SOUTH_EAST;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_SOUTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_SOUTH_WEST;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_WEST_SOUTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_WEST;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_WEST_NORTH;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->border[loc],*node)==0);
    loc = CTL_CENTER;
    node = CBT_get_node_from_loc(tile,loc);
    printf("%d",CBT_tile_node_cmp(tile->center,*node)==0);

    //Test on null tile
    node = CBT_get_node_from_loc(NULL,CTL_CENTER);
    printf("%d",node==NULL);
    node = CBT_get_node_from_loc(NULL,CTL_NORTH);
    printf("%d",node==NULL);

    CBT_free_tile(tile);
    free(tile_path);
}

void test_tile_get_loc_from_str(){
    printf("test test_tile_get_loc_from_str results: ");
    printf("%d",CBT_get_location_from_string("NW")==CTL_NORTH_WEST);
    printf("%d",CBT_get_location_from_string("N")==CTL_NORTH);
    printf("%d",CBT_get_location_from_string("NE")==CTL_NORTH_EAST);
    printf("%d",CBT_get_location_from_string("EN")==CTL_EAST_NORTH);
    printf("%d",CBT_get_location_from_string("E")==CTL_EAST);
    printf("%d",CBT_get_location_from_string("ES")==CTL_EAST_SOUTH);
    printf("%d",CBT_get_location_from_string("SE")==CTL_SOUTH_EAST);
    printf("%d",CBT_get_location_from_string("S")==CTL_SOUTH);
    printf("%d",CBT_get_location_from_string("SW")==CTL_SOUTH_WEST);
    printf("%d",CBT_get_location_from_string("WS")==CTL_WEST_SOUTH);
    printf("%d",CBT_get_location_from_string("W")==CTL_WEST);
    printf("%d",CBT_get_location_from_string("WN")==CTL_WEST_NORTH);
    printf("%d",CBT_get_location_from_string("C")==CTL_CENTER);
    //Test for invalid strings
    printf("%d",CBT_get_location_from_string("i")==-1);
    printf("%d",CBT_get_location_from_string("")==-1);
    printf("%d",CBT_get_location_from_string("P")==-1);
}

void test_tile_get_node_type_from_str(){
    printf("test_tile_get_node_type_from_str results: ");
    printf("%d",CBT_get_node_type_from_str("path")==CBCT_PATH);
    printf("%d",CBT_get_node_type_from_str("path_end")==CBCT_PATH_END);
    printf("%d",CBT_get_node_type_from_str("city")==CBCT_CITY);
    printf("%d",CBT_get_node_type_from_str("cloister")==CBCT_CLOISTER);
    printf("%d",CBT_get_node_type_from_str("garden")==CBCT_GARDEN);
    printf("%d",CBT_get_node_type_from_str("field")==CBCT_FIELD);
    //Test for invalid strings
    printf("%d",CBT_get_node_type_from_str("fied")==CBCT_FIELD);
    printf("%d",CBT_get_node_type_from_str("")==CBCT_FIELD);
    printf("%d",CBT_get_node_type_from_str("hvcb")==CBCT_FIELD);
    printf("%d",CBT_get_node_type_from_str("  ")==CBCT_FIELD);
}

void test_tile_parse_merchandise_type(){
    printf("test_tile_parse_merchandise_type results: ");
    printf("%d",CBT_parse_merchandise('c')==CCM_CORN);
    printf("%d",CBT_parse_merchandise('b')==CCM_BARREL);
    printf("%d",CBT_parse_merchandise('t')==CCM_TISSUE);
    //Test invalid chars
    printf("%d",CBT_parse_merchandise(' ')==CCM_NONE);
    printf("%d",CBT_parse_merchandise('z')==CCM_NONE);
    printf("%d",CBT_parse_merchandise('d')==CCM_NONE);
    printf("%d",CBT_parse_merchandise('e')==CCM_NONE);

}

void test_tile_set_single_connexion(){
    printf("test_tile_set_single_connexion results: ");
    Carc_Tile* tile = CBT_new_empty_tile();
    CBT_set_single_connexion(tile, CTL_SOUTH, CTL_EAST);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_EAST]==1 && tile->border_connexions[CTL_EAST][CTL_SOUTH]==1);
    CBT_set_single_connexion(tile, CTL_SOUTH, CTL_NORTH);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_NORTH]==1 && tile->border_connexions[CTL_NORTH][CTL_SOUTH]==1);
    CBT_set_single_connexion(tile, CTL_NORTH_WEST, CTL_EAST_SOUTH);
    printf("%d",tile->border_connexions[CTL_NORTH_WEST][CTL_EAST_SOUTH]==1 && tile->border_connexions[CTL_EAST_SOUTH][CTL_NORTH_WEST]==1);
    CBT_set_single_connexion(tile, CTL_WEST, CTL_NORTH);
    printf("%d",tile->border_connexions[CTL_WEST][CTL_NORTH]==1 && tile->border_connexions[CTL_NORTH][CTL_WEST]==1);
    //Test with center connexion
    CBT_set_single_connexion(tile, CTL_SOUTH, CTL_CENTER);
    printf("%d",tile->center_connexions[CTL_SOUTH]==1);
    CBT_set_single_connexion(tile, CTL_CENTER, CTL_WEST_NORTH);
    printf("%d",tile->center_connexions[CTL_SOUTH]==1);

    //Test with trying to connect a node to itself
    CBT_set_single_connexion(tile, CTL_SOUTH, CTL_SOUTH);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_SOUTH]==0);
    CBT_set_single_connexion(tile, CTL_EAST, CTL_EAST);
    printf("%d",tile->border_connexions[CTL_EAST][CTL_EAST]==0);

    CBT_free_tile(tile);
}

void test_tiles_connect_in(){
    printf("test_tile_connect_in results: ");
    char *tile1_path = CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_path = CBT_get_tile_file_path("cloister_path.txt");
    Carc_Tile *tile1 = CBT_new_tile_from_file(tile1_path),
              *tile2 = CBT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");

    printf("%d",CBT_tiles_connect_in(*tile1,CTL_WEST,*tile1,CTL_EAST)==1);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_SOUTH,*tile1,CTL_NORTH)==0);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_WEST_SOUTH,*tile1,CTL_EAST_SOUTH)==1);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_WEST,*tile2,CTL_EAST)==0);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_SOUTH,*tile2,CTL_NORTH)==1);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_SOUTH_WEST,*tile2,CTL_NORTH_WEST)==1);
    printf("%d",CBT_tiles_connect_in(*tile1,CTL_NORTH,*tile2,CTL_SOUTH)==0);
    //TODO: add test with gardens

    CBT_free_tile(tile1);
    CBT_free_tile(tile2);
    free(tile1_path);
    free(cloister_tile_path);
}

void test_tile_get_tile_file_path(){
    printf("test_tile_get_tile_file_path results: ");
    char* s = CBT_get_tile_file_path("tile1.txt");
    printf("%d",strcmp(s,"ressources/gameset/tiles/tile1.txt")==0);

    s = CBT_get_tile_file_path("cloister_path.txt");
    printf("%d",strcmp(s,"ressources/gameset/tiles/tile1.txt")!=0);
    printf("%d",strcmp(s,"ressources/gameset/tiles/cloister_path.txt")==0);

    s = CBT_get_tile_file_path("zreoti");
    printf("%d",strcmp(s,"ressources/gameset/tiles/zreoti")==0);

    free(s);
}

void test_tile_pawn_matches_node(){
    printf("test_tile_pawn_matches_node results: ");
    int i=0,j=0, k=0, nb_matches=0;
    for(i=0;i<=CBCT_FIELD;i++){
        for(j=0;j<=PAWN_PIG;j++){
            for(k=0;k<TILE_NR_BORDER_LOCATIONS;k++){
                nb_matches += CBT_node_type_matches_pawn_type(i,j,k);
            }
            nb_matches += CBT_node_type_matches_pawn_type(i,j,CTL_CENTER);
        }
    }
    printf("All tests successful: %d\n",nb_matches==231);
}

void test_tile_add_pawn(){
    printf("test_tile_add_pawn results: ");
    char* tile_start_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Tile *tile_empty=CBT_new_empty_tile(),
              *tile_start=CBT_new_tile_from_file(tile_start_str);
    Carc_Pawn *pawn=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_1,CPC_BLACK),PAWN_NORMAL),
              *architect=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_2,CPC_BLUE),PAWN_ARCHITECT);

    //Tile init without pawn tested in tile init tests
    //Test cannot add pawn on NULL tile
    printf("%d",CBT_add_pawn(pawn,NULL,CTL_CENTER)==-1);
    //Test adding pawn on empty node
    printf("%d",CBT_add_pawn(pawn,tile_empty,CTL_CENTER)==0);
    //Test adding pawn on invalid Carc_Tile_Location value
    printf("%d",CBT_add_pawn(pawn,tile_start,255)==-1);
    printf("%d",CBT_add_pawn(pawn,tile_start,-1)==-1);
    //Test adding on center
    printf("%d",CBT_add_pawn(pawn,tile_start,CTL_CENTER)==0
                && CBT_get_node_from_loc(tile_start,CTL_CENTER)->pawn!=NULL);
    //Test adding on edge
    printf("%d",CBT_add_pawn(pawn,tile_start,CTL_NORTH_EAST)==0
                && CBT_get_node_from_loc(tile_start,CTL_NORTH_EAST)->pawn!=NULL);
    //Test when player does not have pawns
    pawn->owner->nb_pawns[PAWN_NORMAL] = 0;
    printf("%d",CBT_add_pawn(pawn,tile_start,CTL_SOUTH)==-1
                && CBT_get_node_from_loc(tile_start,CTL_SOUTH)->pawn==NULL);

    //Test add pawn on a node not supporting this kind of pawn
    printf("%d",CBT_add_pawn(architect,tile_start,CTL_SOUTH)==-1
                && CBT_get_node_from_loc(tile_start,CTL_SOUTH)->pawn==NULL
                && architect->owner->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER);

    free(tile_start_str);
    CBT_free_tile(tile_empty);
    CBT_free_tile(tile_start);
    CPPlayer_free_player(pawn->owner);
    CPPawn_free_pawn(pawn);
    CPPlayer_free_player(architect->owner);
    CPPawn_free_pawn(architect);
}

void test_tile_rm_pawn(){
    printf("test_tile_rm_pawn results: ");
    char* tile_start_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Tile *tile_start=CBT_new_tile_from_file(tile_start_str);
    Carc_Pawn *pawn1=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_1,CPC_BLACK),PAWN_NORMAL),
              *pawn2=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_2,CPC_BLUE),PAWN_NORMAL);

    //Test remove pawn from empty tile
    printf("%d",CBT_rm_pawn(NULL,CTL_CENTER)==-1);
    printf("%d",CBT_rm_pawn(NULL,CTL_NORTH)==-1);

    //Test remove pawn when no pawn on the node
    printf("%d",CBT_rm_pawn(tile_start,CTL_CENTER)==-1);
    printf("%d",CBT_rm_pawn(tile_start,CTL_NORTH)==-1);

    //Test remove pawn when player already at max
    tile_start->center.pawn = pawn1;
    tile_start->border[CTL_NORTH].pawn = pawn2;
    printf("%d\n",CBT_rm_pawn(tile_start,CTL_CENTER)==-1
                && tile_start->center.pawn==pawn1);
    printf("%d\n",CBT_rm_pawn(tile_start,CTL_NORTH)==-1
                && tile_start->border[CTL_NORTH].pawn==pawn2);

    //Test remove pawn when player not at max
    pawn1->owner->nb_pawns[PAWN_NORMAL]--;
    pawn2->owner->nb_pawns[PAWN_NORMAL]--;
    printf("%d",CBT_rm_pawn(tile_start,CTL_CENTER)==0
                && tile_start->center.pawn==NULL);
    printf("%d",CBT_rm_pawn(tile_start,CTL_NORTH)==0
                && tile_start->border[CTL_NORTH].pawn==NULL);

    CPPlayer_free_player(pawn1->owner);
    CPPlayer_free_player(pawn2->owner);
    CPPawn_free_pawn(pawn1);
    CPPawn_free_pawn(pawn2);
    free(tile_start_str);
    CBT_free_tile(tile_start);
}

void test_tile_list_new(){
    printf("test_tile_list_new results: ");
    Carc_Tile_Node node,
                  *pointer_to_node=&node,
                  *null_node=NULL;
    Carc_Tile_Node_List* l=CBTList_new(&pointer_to_node);

    printf("%d",CBTList_new(NULL)==NULL);
    printf("%d",CBTList_new(&null_node)==NULL);
    printf("%d",l!=NULL && l->node==pointer_to_node && l->next==NULL);

    CBTList_free(l);
}

void test_tile_list_append(){
    printf("test_tile_list_append results: ");
    Carc_Tile_Node node,
                   node2,
                  *pointer_to_node=&node,
                  *pointer_to_node2=&node2,
                  *null_node=NULL;
    Carc_Tile_Node_List* l=CBTList_new(&pointer_to_node);

    printf("%d",CBTList_append(NULL,NULL)==-1);
    printf("%d",CBTList_append(l,NULL)==-1);
    printf("%d",CBTList_append(l,&null_node)==-1);
    printf("%d",CBTList_append(l,&pointer_to_node)==-2);
    printf("%d",CBTList_append(l,&pointer_to_node2)==0);
    printf("%d",l!=NULL && l->next->node==pointer_to_node2 && l->next->next==NULL);

    CBTList_free(l);
}

void test_tile_list_append_list(){
    printf("test_tile_list_append_list results: ");
    Carc_Tile_Node node,
                   node2,
                   node3,
                   node4,
                   node5,
                  *pointer_to_node=&node,
                  *pointer_to_node2=&node2,
                  *pointer_to_node3=&node3,
                  *pointer_to_node4=&node4,
                  *pointer_to_node5=&node5;
    Carc_Tile_Node_List *l=CBTList_new(&pointer_to_node),
                        *append_single=CBTList_new(&pointer_to_node2),
                        *append_several=CBTList_new(&pointer_to_node3);
    append_several->next=CBTList_new(&pointer_to_node4);

    printf("%d",CBTList_append_list(NULL,NULL)==FUNC_FAIL);
    printf("%d",CBTList_append_list(NULL,append_single)==FUNC_FAIL);
    printf("%d",CBTList_append_list(l,NULL)==FUNC_SUCCESS && l->node==pointer_to_node && l->next==NULL);
    printf("%d",CBTList_append_list(l,l)==FUNC_FAIL);// && l->node==pointer_to_node && l->next==NULL);
    printf("%d",CBTList_append_list(l,append_single)==FUNC_SUCCESS && l->node==pointer_to_node
                && l->next->node==pointer_to_node2 && l->next->next==NULL);
    printf("%d",CBTList_append_list(l,append_several)==FUNC_SUCCESS && l->node==pointer_to_node
                && l->next->node==pointer_to_node2 && l->next->next->node==pointer_to_node3
                && l->next->next->next->node==pointer_to_node4
                && l->next->next->next->next==NULL);
    append_several->next=CBTList_new(&pointer_to_node5);
    printf("%d",CBTList_append_list(l,append_several)==FUNC_FAIL&& l->node==pointer_to_node
                && l->next->node==pointer_to_node2 && l->next->next->node==pointer_to_node3
                && l->next->next->next->node==pointer_to_node4
                && l->next->next->next->next->node==pointer_to_node5
                && l->next->next->next->next->next==NULL);
    printf("%d",CBTList_append_list(l,l)==FUNC_FAIL&& l->node==pointer_to_node
                && l->next->node==pointer_to_node2 && l->next->next->node==pointer_to_node3
                && l->next->next->next->node==pointer_to_node4
                && l->next->next->next->next->node==pointer_to_node5
                && l->next->next->next->next->next==NULL);

    CBTList_free(l);
    CBTList_free(append_several);
    CBTList_free(append_single);
}

void test_tile_list_rm(){
    printf("test_tile_list_rm results: ");
    Carc_Tile_Node node,
                   node2,
                   node3,
                   node4,
                   node5,
                  *pointer_to_node=&node,
                  *pointer_to_node2=&node2,
                  *pointer_to_node3=&node3,
                  *pointer_to_node4=&node4,
                  *pointer_to_node5=&node5,
                  *null_node=NULL;
    Carc_Tile_Node_List *l=CBTList_new(&pointer_to_node);

    printf("%d",CBTList_rm(NULL,&pointer_to_node)==FUNC_SUCCESS);
    //Test rm node not in list
    printf("%d",CBTList_rm(&l,&pointer_to_node2)==FUNC_SUCCESS && l->node==pointer_to_node && l->next==NULL);
    //Test rm head
    printf("%d\n",CBTList_rm(&l,&pointer_to_node)==FUNC_SUCCESS && l==NULL);
    //Tests if list more than just head
    l = CBTList_new(&pointer_to_node2);//No memory leak, as the previous l was freed in the remove function
    CBTList_append(l,&pointer_to_node3);
    CBTList_append(l,&pointer_to_node4);
    CBTList_append(l,&pointer_to_node5);
    //if node is NULL
    printf("%d",CBTList_rm(&l,NULL)==FUNC_FAIL
           && l->node==pointer_to_node2 && l->next!=NULL
           && l->next->node==pointer_to_node3 && l->next->next!=NULL
           && l->next->next->node==pointer_to_node4 && l->next->next->next!=NULL
           && l->next->next->next->node==pointer_to_node5 && l->next->next->next->next==NULL
           );
    printf("%d",CBTList_rm(&l,&null_node)==FUNC_FAIL
           && l->node==pointer_to_node2 && l->next!=NULL
           && l->next->node==pointer_to_node3 && l->next->next!=NULL
           && l->next->next->node==pointer_to_node4 && l->next->next->next!=NULL
           && l->next->next->next->node==pointer_to_node5 && l->next->next->next->next==NULL
           );
    //If not not in
    printf("%d",CBTList_rm(&l,&pointer_to_node)==FUNC_SUCCESS
           && l->node==pointer_to_node2 && l->next!=NULL
           && l->next->node==pointer_to_node3 && l->next->next!=NULL
           && l->next->next->node==pointer_to_node4 && l->next->next->next!=NULL
           && l->next->next->next->node==pointer_to_node5 && l->next->next->next->next==NULL
           );
    //if node is head
    printf("%d",CBTList_rm(&l,&pointer_to_node2)==FUNC_SUCCESS
           && l->node==pointer_to_node3 && l->next!=NULL
           && l->next->node==pointer_to_node4 && l->next->next!=NULL
           && l->next->next->node==pointer_to_node5 && l->next->next->next==NULL
           );
    //if node is in tail
    printf("%d",CBTList_rm(&l,&pointer_to_node4)==FUNC_SUCCESS
           && l->node==pointer_to_node3 && l->next!=NULL
           && l->next->node==pointer_to_node5 && l->next->next==NULL
           );
    //if node is last
    printf("%d",CBTList_rm(&l,&pointer_to_node5)==FUNC_SUCCESS
           && l->node==pointer_to_node3 && l->next==NULL
           );

    CBTList_free(l);
}

void test_tile_list_rm_list(){
    printf("test_tile_list_rm results: ");
    Carc_Tile_Node node,
                   node2,
                   node3,
                   node4,
                   node5,
                  *pointer_to_node=&node,
                  *pointer_to_node2=&node2,
                  *pointer_to_node3=&node3,
                  *pointer_to_node4=&node4,
                  *pointer_to_node5=&node5;
    Carc_Tile_Node_List *l=CBTList_new(&pointer_to_node),
                        *rm_list=CBTList_new(&pointer_to_node),
                        *null_list=NULL;

    CBTList_append(l,&pointer_to_node2);
    CBTList_append(l,&pointer_to_node3);
    CBTList_append(l,&pointer_to_node4);

    CBTList_append(rm_list,&pointer_to_node3);
    CBTList_append(rm_list,&pointer_to_node4);
    CBTList_append(rm_list,&pointer_to_node5);

    printf("%d",CBTList_rm_nodes(NULL,rm_list)==FUNC_SUCCESS);
    printf("%d",CBTList_rm_nodes(&null_list,rm_list)==FUNC_SUCCESS);
    //Test no node to remove
    printf("%d",CBTList_rm_nodes(&l,NULL)==FUNC_SUCCESS &&
            l->node==pointer_to_node
            && l->next->node==pointer_to_node2
            && l->next->next->node==pointer_to_node3
            && l->next->next->next->node==pointer_to_node4
            && l->next->next->next->next==NULL
            );

    printf("%d",CBTList_rm_nodes(&l,rm_list)==FUNC_SUCCESS &&
            l->node==pointer_to_node2
            && l->next==NULL
            );

    CBTList_free(l);
    CBTList_free(rm_list);
}

void test_tile_path_end_is_playable(){
    printf("test_tile_path_end_is_playable results: ");
    int i;
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        printf("%d",CBT_path_end_is_playable(i)==1);
    }
    printf("%d\n",CBT_path_end_is_playable(CTL_CENTER)==0);
}

void test_tile_turn_type_to_degrees(){
    printf("test_tile_turn_type_to_degrees results: ");
    printf("%d",CBT_turn_type_to_degrees(CTTT_NONE)==0);
    printf("%d",CBT_turn_type_to_degrees(CTTT_RIGHT)==90);
    printf("%d",CBT_turn_type_to_degrees(CTTT_UPDOWN)==180);
    printf("%d",CBT_turn_type_to_degrees(CTTT_LEFT)==270);
    //Test value out of the enum
    printf("%d",CBT_turn_type_to_degrees(CTTT_NONE-1)==0);
    printf("%d",CBT_turn_type_to_degrees(CTTT_LEFT+1)==0);
}

void test_tile_run_all(){
    test_tile_parse_tile_file();
    printf("\n************************************\n");
    test_tile_turn_tile();
    printf("\n************************************\n");
    test_tile_tile_cmp();
    printf("\n************************************\n");
    test_tile_new_empty_tile();
    printf("\n************************************\n");
    test_tile_CBT_new_node();
    printf("\n************************************\n");
    test_tile_is_valid_loc();
    printf("\n************************************\n");
    test_tile_get_node_from_loc();
    printf("\n************************************\n");
    test_tile_get_loc_from_str();
    printf("\n************************************\n");
    test_tile_get_node_type_from_str();
    printf("\n************************************\n");
    test_tile_parse_merchandise_type();
    printf("\n************************************\n");
    test_tile_set_single_connexion();
    printf("\n************************************\n");
    test_tiles_connect_in();
    printf("\n************************************\n");
    test_tile_get_tile_file_path();
    printf("\n************************************\n");
    test_tile_pawn_matches_node();
    printf("\n************************************\n");
    test_tile_add_pawn();
    printf("\n************************************\n");
    test_tile_rm_pawn();
    printf("\n************************************\n");
    test_tile_list_new();
    printf("\n************************************\n");
    test_tile_list_append();
    printf("\n************************************\n");
    test_tile_list_append_list();
    printf("\n************************************\n");
    test_tile_list_rm();
    printf("\n************************************\n");
    test_tile_path_end_is_playable();
    printf("\n************************************\n");
    test_tile_turn_type_to_degrees();
    printf("\n************************************\n");
}
