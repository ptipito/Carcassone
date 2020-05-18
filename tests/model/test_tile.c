#include "tests/model/test_tile.h"

void test_tile_CBT_display_tile(Carc_Tile tile){
    CBT_display_tile(tile);
}

void test_tile_new_empty_tile(){
    printf("test CBT_new_empty_tile results: ");
    int result=1, i=0, j=0;
    Carc_Tile* tile = CBT_new_empty_tile();
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        if(tile->center_connexions[i]!=0 || tile->border[i].construction != NULL)
            result=0;

        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            if(tile->border_connexions[i][j]!=0){
                result = 0;
            }
        }
    }
    if(tile->center.construction != NULL)
        result = 0;
    printf("%d",result);
}

void test_tile_parse_tile_file(){
    printf("test_tile_parse_tile_file: \n");
    Carc_Tile* tile = CBT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Tile* tile2 = CBT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");
    printf("*******display start tile*******\n");
    CBT_display_tile(*tile);
    printf("\n*******display cloister_path tile*******\n");
    CBT_display_tile(*tile2);
    CBT_free_tile(tile);
    CBT_free_tile(tile2);
}

void test_tile_turn_tile(){
    printf("test_tile_turn_tile:\n");
    Carc_Tile* tile = CBT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    CBT_display_tile(*tile);

    printf("\tTURN CCT_RIGHT\n");
    CBT_turn(tile,CCT_RIGHT);
    CBT_display_tile(*tile);

    printf("\tTURN 180\n");
    CBT_turn(tile,CCT_UPDOWN);
    CBT_display_tile(*tile);

    printf("\tTURN CCT_LEFT\n");
    CBT_turn(tile,CCT_LEFT);
    CBT_display_tile(*tile);

    printf("\tTURN CCT_LEFT\n");
    CBT_turn(tile,CCT_LEFT);
    CBT_display_tile(*tile);

    printf("\tTURN 180\n");
    CBT_turn(tile,CCT_UPDOWN);
    CBT_display_tile(*tile);

    CBT_free_tile(tile);
}

void test_tile_tile_cmp(){
    printf("test_tile_cmp resutls: ");
    Carc_Tile *t1 = CBT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
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

}

void test_tile_CBT_new_node(){
    printf("test_CBT_new_node_results: ");
    Carc_Construction *city = CBC_new_city(1,0,CCM_NONE);
    Carc_Construction *path = CBC_new_path(1);
    Carc_Tile_Node* node = CBT_new_node(CCBT_CITY, city);

    printf("%d",node->node_type == CCBT_CITY && CBC_construction_cmp(node->construction, CCBT_CITY, city, CCBT_CITY)==0);

    node = CBT_new_node(CCBT_CITY, city);
    printf("%d",node->node_type == CCBT_CITY && CBC_construction_cmp(node->construction, CCBT_PATH, path, CCBT_PATH)==0);
}

void test_tile_get_node_from_loc(){
    printf("test_get_node_from_loc results: ");
    Carc_Tile* tile = CBT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
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

    CBT_free_tile(tile);
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
    printf("%d",CBT_get_location_from_string("i")==CTL_CENTER);
    printf("%d",CBT_get_location_from_string("")==CTL_CENTER);
    printf("%d",CBT_get_location_from_string("P")==CTL_CENTER);
}

void test_tile_get_node_type_from_str(){
    printf("test_tile_get_node_type_from_str results: ");
    printf("%d",CBT_get_node_type_from_str("path")==CCBT_PATH);
    printf("%d",CBT_get_node_type_from_str("path_end")==CCBT_PATH_END);
    printf("%d",CBT_get_node_type_from_str("city")==CCBT_CITY);
    printf("%d",CBT_get_node_type_from_str("cloister")==CCBT_CLOISTER);
    printf("%d",CBT_get_node_type_from_str("garden")==CCBT_GARDEN);
    printf("%d",CBT_get_node_type_from_str("field")==CCBT_FIELD);
    //Test for invalid strings
    printf("%d",CBT_get_node_type_from_str("fied")==CCBT_FIELD);
    printf("%d",CBT_get_node_type_from_str("")==CCBT_FIELD);
    printf("%d",CBT_get_node_type_from_str("hvcb")==CCBT_FIELD);
    printf("%d",CBT_get_node_type_from_str("  ")==CCBT_FIELD);
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
    Carc_Tile *tile1 = CBT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
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
}

void test_tile_run_all(){
    test_tile_parse_tile_file();
    printf("\n");
    test_tile_turn_tile();
    printf("\n");
    test_tile_tile_cmp();
    printf("\n");
    test_tile_new_empty_tile();
    printf("\n");
    test_tile_CBT_new_node();
    printf("\n");
    test_tile_get_node_from_loc();
    printf("\n");
    test_tile_get_loc_from_str();
    printf("\n");
    test_tile_get_node_type_from_str();
    printf("\n");
    test_tile_parse_merchandise_type();
    printf("\n");
    test_tile_set_single_connexion();
    printf("\n");
    test_tiles_connect_in();
}
