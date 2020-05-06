#include "tests/model/test_tile.h"

void test_tile_display_tile(Carc_Tile tile){
    display_tile(tile);
}

void test_tile_parse_tile_file(){
    Carc_Tile* tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Tile* tile2 = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");
    printf("*******display start tile*******\n");
    display_tile(*tile);
    printf("\n*******display cloister_path tile*******\n");
    display_tile(*tile2);
    CT_Free_Tile(tile);
    CT_Free_Tile(tile2);
}

void test_tile_turn_tile(){
    Carc_Tile* tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    display_tile(*tile);

    printf("\tTURN RIGHT\n");
    CT_turn(tile,RIGHT);
    display_tile(*tile);

    printf("\tTURN 180\n");
    CT_turn(tile,UPDOWN);
    display_tile(*tile);

    printf("\tTURN LEFT\n");
    CT_turn(tile,LEFT);
    display_tile(*tile);


    printf("\tTURN LEFT\n");
    CT_turn(tile,LEFT);
    display_tile(*tile);

    printf("\tTURN 180\n");
    CT_turn(tile,UPDOWN);
    display_tile(*tile);

    CT_Free_Tile(tile);
}

void test_tile_tile_cmp(){
    Carc_Tile *t1 = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
              *t2 = CT_new_empty_tile();
    Carc_Tile t3 = *t1,
              t4 = *t1;
    t3.center_connexions[CTL_NORTH] = 1;
    t4.border_connexions[CTL_NORTH_EAST][CTL_EAST_NORTH] = 1;

    int test1 = CT_tile_cmp(*t1,*t1);
    int test2 = CT_tile_cmp(*t1,*t2);
    int test3 = CT_tile_cmp(*t1,t3);
    int test4 = CT_tile_cmp(*t1,t4);
    printf("test_tile_cmp resutls: %d%d%d%d",test1==0,test2==1,test3==1,test4==1);
    CT_Free_Tile(t1);
    CT_Free_Tile(t2);

}

void test_tile_new_empty_tile(){
    int result=1, i=0, j=0;
    Carc_Tile* tile = CT_new_empty_tile();
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        if(tile->center_connexions[i]!=0 || tile->border[i].construction == NULL)
            result=0;

        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            if(tile->border_connexions[i][j]!=0)
                result = 0;;
        }
    }
    if(tile->center.construction == NULL)
        result = 0;;
    printf("test CT_new_empty_tile results: %d",result);
}

void test_tile_CT_new_node(){
    Carc_Construction *city = CC_new_city(1,0,CCM_NONE);
    Carc_Construction *path = CC_new_path(1);
    Carc_Tile_Node node = CT_new_node(CCT_CITY, city);

    printf("test_CT_new_node_results: ");
    printf("%d",node.node_type == CCT_CITY && CC_construction_cmp(node.construction, CCT_CITY, city, CCT_CITY)==0);

    node = CT_new_node(CCT_CITY, city);
    printf("%d",node.node_type == CCT_CITY && CC_construction_cmp(node.construction, CCT_PATH, path, CCT_PATH)==0);
}

void test_tile_get_node_from_loc(){
    Carc_Tile* tile = CT_new_tile_from_file("ressources/gamest/tiles/tile1.txt");
    CT_Locations loc = CTL_NORTH_WEST;
    Carc_Tile_Node* node = CT_get_node_from_loc(tile,loc);
    printf("test_get_node_from_loc results: ");
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_NORTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_NORTH_EAST;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_EAST_NORTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_EAST;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_EAST_SOUTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_SOUTH_EAST;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_SOUTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_SOUTH_WEST;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_WEST_SOUTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_WEST;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_WEST_NORTH;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->border[loc],*node));
    loc = CTL_CENTER;
    node = CT_get_node_from_loc(tile,loc);
    printf("%d",CT_tile_node_cmp(tile->center,*node));
}

void test_tile_get_loc_from_str(){
    printf("test test_tile_get_loc_from_str results: ");
    printf("%d",CT_get_location_from_string("NW")==CTL_NORTH_WEST);
    printf("%d",CT_get_location_from_string("N")==CTL_NORTH);
    printf("%d",CT_get_location_from_string("NE")==CTL_NORTH_EAST);
    printf("%d",CT_get_location_from_string("EN")==CTL_EAST_NORTH);
    printf("%d",CT_get_location_from_string("E")==CTL_EAST);
    printf("%d",CT_get_location_from_string("ES")==CTL_EAST_SOUTH);
    printf("%d",CT_get_location_from_string("SE")==CTL_SOUTH_EAST);
    printf("%d",CT_get_location_from_string("S")==CTL_SOUTH);
    printf("%d",CT_get_location_from_string("SW")==CTL_SOUTH_WEST);
    printf("%d",CT_get_location_from_string("WS")==CTL_WEST_SOUTH);
    printf("%d",CT_get_location_from_string("W")==CTL_WEST);
    printf("%d",CT_get_location_from_string("WN")==CTL_WEST_NORTH);
    printf("%d",CT_get_location_from_string("C")==CTL_CENTER);
    //Test for invalid strings
    printf("%d",CT_get_location_from_string("i")==CTL_CENTER);
    printf("%d",CT_get_location_from_string("")==CTL_CENTER);
    printf("%d",CT_get_location_from_string("P")==CTL_CENTER);
}

void test_tile_get_node_type_from_str(){
    printf("test_tile_get_node_type_from_str results: ");
    printf("%d",CT_get_node_type_from_str("path")==CCT_PATH);
    printf("%d",CT_get_node_type_from_str("path_end")==CCT_PATH_END);
    printf("%d",CT_get_node_type_from_str("city")==CCT_CITY);
    printf("%d",CT_get_node_type_from_str("cloister")==CCT_CLOISTER);
    printf("%d",CT_get_node_type_from_str("garden")==CCT_GARDEN);
    printf("%d",CT_get_node_type_from_str("field")==CCT_FIELD);
    //Test for invalid strings
    printf("%d",CT_get_node_type_from_str("fied")==CCT_FIELD);
    printf("%d",CT_get_node_type_from_str("")==CCT_FIELD);
    printf("%d",CT_get_node_type_from_str("hvcb")==CCT_FIELD);
    printf("%d",CT_get_node_type_from_str("  ")==CCT_FIELD);
}

void test_tile_parse_merchandise_type(){
    printf("test_tile_parse_merchandise_type results: ");
    printf("%d",CT_parse_merchandise('c')==CCM_CORN);
    printf("%d",CT_parse_merchandise('b')==CCM_BARREL);
    printf("%d",CT_parse_merchandise('t')==CCM_TISSUE);
    //Test invalid chars
    printf("%d",CT_parse_merchandise(' ')==CCM_NONE);
    printf("%d",CT_parse_merchandise('z')==CCM_NONE);
    printf("%d",CT_parse_merchandise('d')==CCM_NONE);
    printf("%d",CT_parse_merchandise('e')==CCM_NONE);

}

void test_tile_set_single_connexion(){
    printf("test_tile_set_single_connexion results: ");
    Carc_Tile* tile = CT_new_empty_tile();
    CT_set_single_connexion(tile, CTL_SOUTH, CTL_EAST);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_EAST]==1 && tile->border_connexions[CTL_EAST][CTL_SOUTH]==1);
    CT_set_single_connexion(tile, CTL_SOUTH, CTL_NORTH);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_NORTH]==1 && tile->border_connexions[CTL_NORTH][CTL_SOUTH]==1);
    CT_set_single_connexion(tile, CTL_NORTH_WEST, CTL_EAST_SOUTH);
    printf("%d",tile->border_connexions[CTL_NORTH_WEST][CTL_EAST_SOUTH]==1 && tile->border_connexions[CTL_EAST_SOUTH][CTL_NORTH_WEST]==1);
    CT_set_single_connexion(tile, CTL_WEST, CTL_NORTH);
    printf("%d",tile->border_connexions[CTL_WEST][CTL_NORTH]==1 && tile->border_connexions[CTL_NORTH][CTL_WEST]==1);
    //Test with center connexion
    CT_set_single_connexion(tile, CTL_SOUTH, CTL_CENTER);
    printf("%d",tile->center_connexions[CTL_SOUTH]==1);
    CT_set_single_connexion(tile, CTL_CENTER, CTL_WEST_NORTH);
    printf("%d",tile->center_connexions[CTL_SOUTH]==1);

    //Test with trying to connect a node to itself
    CT_set_single_connexion(tile, CTL_SOUTH, CTL_SOUTH);
    printf("%d",tile->border_connexions[CTL_SOUTH][CTL_SOUTH]==0);
    CT_set_single_connexion(tile, CTL_EAST, CTL_EAST);
    printf("%d",tile->border_connexions[CTL_EAST][CTL_EAST]==0);

    CT_Free_Tile(tile);
}

void test_tiles_connect_in(){
    Carc_Tile *tile1 = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt"),
              *tile2 = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");

    printf("test_tile_connect_in results: ");
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_WEST,*tile1,CTL_EAST)==1);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_SOUTH,*tile1,CTL_NORTH)==0);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_WEST_SOUTH,*tile1,CTL_EAST_SOUTH)==1);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_WEST,*tile2,CTL_EAST)==0);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_SOUTH,*tile2,CTL_NORTH)==1);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_SOUTH_WEST,*tile2,CTL_NORTH_WEST)==1);
    printf("%d",CT_Tiles_connect_in(*tile1,CTL_NORTH,*tile2,CTL_SOUTH)==0);
    //TODO: add test with gardens

    CT_Free_Tile(tile1);
    CT_Free_Tile(tile2);
}

void test_tile_cmp(){
    Carc_Tile *tile1 = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Tile *tile2 = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");

    printf("test_tile_cmp results: ");
    printf("%d",CT_tile_cmp(*tile1,*tile1)==0);
    printf("%d",CT_tile_cmp(*tile2,*tile2)==0);
    printf("%d",CT_tile_cmp(*tile2,*tile1)==1);
    printf("%d",CT_tile_cmp(*tile1,*tile2)==1);

    CT_Free_Tile(tile1);
    CT_Free_Tile(tile2);
}
