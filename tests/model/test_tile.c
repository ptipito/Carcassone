#include "tests/model/test_tile.h"

void test_display_tile(Carc_Tile tile){
    display_tile(tile);
}

void test_parse_tile_file(){
    Carc_Tile* tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");
    Carc_Tile* tile2 = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");
    printf("*******display start tile*******\n");
    test_display_tile(*tile);
    printf("\n*******display cloister_path tile*******\n");
    test_display_tile(*tile2);
    CT_Free_Tile(tile);
    CT_Free_Tile(tile2);
}

void test_turn_tile(){
    Carc_Tile* tile = CT_new_tile_from_file("ressources/gameset/tiles/tile1.txt");

    printf("\tTURN RIGHT\n");
    CT_turn(tile,RIGHT);
    test_display_tile(*tile);

    printf("\tTURN 180\n");
    CT_turn(tile,UPDOWN);
    test_display_tile(*tile);

    printf("\tTURN LEFT\n");
    CT_turn(tile,LEFT);
    test_display_tile(*tile);


    printf("\tTURN LEFT\n");
    CT_turn(tile,LEFT);
    test_display_tile(*tile);

    printf("\tTURN 180\n");
    CT_turn(tile,UPDOWN);
    test_display_tile(*tile);

    CT_Free_Tile(tile);
}

void test_tile_cmp(){
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
    printf("%d %d %d %d",test1==0,test2==1,test3==1,test4==1);
    CT_Free_Tile(t1);
    CT_Free_Tile(t2);

}

void test_new_empty_tile(){
    Carc_Tile *tile=CT_new_empty_tile();
    int i,j;

    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
            printf("node %d (%p)",i,&(tile->border[i]));
        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            printf("\t%d-%d = %d\n",i,j,tile->border_connexions[i][j]);
        }
            printf("\t%d-%d = %d\n",i,CTL_CENTER,tile->center_connexions[i]);
    }
    printf("Center %p\n",&(tile->center));
    CT_Free_Tile(tile);
}
