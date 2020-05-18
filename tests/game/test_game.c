#include "tests/game/test_game.h"

void test_game_initiate(){
    printf("test_game_initiate results: ");
    char* start_tile_str = "ressources/gameset/tiles/tile1.txt";
    Carc_Game* game = CG_initiate_game(start_tile_str);

    Carc_Playboard_Origin* origin = CP_init_playboard(CT_new_tile_from_file(start_tile_str));
    printf("%d",CP_node_cmp(game->playboard->node,origin->node)==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,0))!=NULL);

    int rim_length = 0;
    CP_Rim_Element* current = game->playable->first;
    while(current!=NULL){
        rim_length++;
        current = current->next;
    }

    printf("%d",rim_length==4);

    CG_free_game(game);
}

void test_game_rim_to_playboard_update_one_side(){
    printf("test_game_initiate results: ");
    int update_result=0;
    Carc_Game* game = CG_initiate_game("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Location play_in = CP_Location_new(0,-1);
    Carc_Playboard_Node *cloister = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt"),
                                                          play_in),
                        *empty_node = CP_new_empty_playboard_node(CP_Location_new(15,26));

    update_result = CG_rim_to_playboard_update_one_side(NULL,play_in,cloister,CP_UP);
    printf("%d\n",update_result==1);
    update_result = CG_rim_to_playboard_update_one_side(game->playable,play_in,NULL,CP_UP);
    printf("%d\n",update_result==1);
    update_result = CG_rim_to_playboard_update_one_side(game->playable,CP_Location_new(15,26),cloister,CP_UP);
    printf("%d\n",update_result==2);
    update_result = CG_rim_to_playboard_update_one_side(game->playable,play_in,empty_node,CP_UP);
    printf("%d\n",update_result==3);

    update_result = CG_rim_to_playboard_update_one_side(game->playable,play_in,cloister,CP_UP);
    printf("%d",update_result==0);
    printf("%d",CB_Rim_find_by_location(game->playable,play_in)!=NULL);
    printf("%d",CP_node_cmp(cloister->neighbors[CP_UP],game->playboard->node)==0);
    printf("%d",CP_node_cmp(cloister,game->playboard->node->neighbors[CP_DOWN])==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_DOWN))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_LEFT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_RIGHT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_UP))==NULL);

    update_result = CG_rim_to_playboard_update_one_side(game->playable,play_in,cloister,CP_LEFT);
    printf("%d",update_result==0);
    printf("%d",CP_node_cmp(cloister->neighbors[CP_LEFT],NULL)==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_DOWN))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_LEFT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_RIGHT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_UP))==NULL);

    CG_free_game(game);
    CP_free_playboard_node(empty_node);
    CP_free_playboard_node(cloister);//To be removed when free_playboard is completed
}

void test_game_node_transfer_rim_to_playboard(){
    printf("test_game_initiate results:\n");
    int update_result=0;
    Carc_Game* game = CG_initiate_game("ressources/gameset/tiles/tile1.txt");
    Carc_Playboard_Location play_in = CP_Location_new(0,-1);
    Carc_Playboard_Node *cloister = CP_new_playboard_node(CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt"),
                                                          play_in),
                        *empty_node = CP_new_empty_playboard_node(CP_Location_new(15,26));

    //update_result = CG_node_transfer_rim_to_playboard(NULL,play_in,cloister);
    update_result = CG_node_transfer_rim_to_playboard(NULL,play_in,NULL);
    printf("%d\n",update_result==1);
    update_result = CG_node_transfer_rim_to_playboard(NULL,play_in,cloister);
    printf("%d\n",update_result==1);
    update_result = CG_node_transfer_rim_to_playboard(game,play_in,NULL);
    printf("%d\n",update_result==1);
    update_result = CG_node_transfer_rim_to_playboard(game,CP_Location_new(15,26),cloister);
    printf("%d\n",update_result==2);
    update_result = CG_node_transfer_rim_to_playboard(game,play_in,empty_node);
    printf("%d\n",update_result==3);

    update_result = CG_node_transfer_rim_to_playboard(game,play_in,cloister);
    printf("%d",update_result==0);
    printf("%d",CB_Rim_find_by_location(game->playable,play_in)==NULL);
    printf("%d",CP_node_cmp(cloister->neighbors[CP_UP],game->playboard->node)==0);
    printf("%d",CP_node_cmp(cloister,game->playboard->node->neighbors[CP_DOWN])==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_DOWN))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_LEFT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_RIGHT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_get_neighbor_loc(play_in,CP_UP))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,0))!=NULL);


    CG_free_game(game);
    CP_free_playboard_node(empty_node);
    CP_free_playboard_node(cloister);//To be removed when free_playboard is completed
}

void test_game_play_tile_in(){
    printf("test_game_play_tile_in results: ");
    char *start_tile_str = "ressources/gameset/tiles/tile1.txt"
         //,*cloister_tile_str = "ressources/gameset/tiles/cloister_path.txt"
         ;
    Carc_Game* game = CG_initiate_game(start_tile_str);
    Carc_Playboard_Location play_in = CP_Location_new(0,-1);
    Carc_Tile *cloister_tile = CT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt");
    Carc_Playboard_Node *origin_node = CP_new_playboard_node(CT_new_tile_from_file(start_tile_str),CP_Location_new(0,0)),
                        *curr_node=NULL;
    Carc_Playboard_Node* node_played = CG_play_tile_in(game,play_in,cloister_tile);

    printf("%d",CT_tile_cmp(game->playboard->node->node,origin_node->node)==0);
    printf("%d",CP_Location_cmp(game->playboard->node->node_coordinates,origin_node->node_coordinates)==0);
    printf("%d",game->playboard->node->neighbors[CP_DOWN]!=NULL && game->playboard->node->neighbors[CP_DOWN]==node_played);
    printf("%d",game->playboard->node->neighbors[CP_LEFT]==NULL);
    printf("%d",game->playboard->node->neighbors[CP_RIGHT]==NULL);
    printf("%d",game->playboard->node->neighbors[CP_UP]==NULL);
    //Check the neighbor was inserted correctly
    printf("\n");
    curr_node = game->playboard->node->neighbors[CP_DOWN];
    printf("%d",CT_tile_cmp(curr_node->node,cloister_tile)==0);
    printf("%d",CP_Location_cmp(curr_node->node_coordinates,play_in)==0);
    printf("%d",curr_node->neighbors[CP_UP]==game->playboard->node);
    printf("%d",curr_node->neighbors[CP_LEFT]==NULL);
    printf("%d",curr_node->neighbors[CP_RIGHT]==NULL);
    printf("%d",curr_node->neighbors[CP_DOWN]==NULL);
    //Test rim was updated properly
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,-1))!=NULL);
    //Test node was removed from rim
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,-1))==NULL);

    //Test play in a location where tiles do not connect
    printf("\n");
    play_in = CP_Location_new(0,1);
    node_played = CG_play_tile_in(game,play_in,cloister_tile);
    curr_node = game->playboard->node;
    printf("%d",node_played==NULL);
    printf("%d",curr_node->neighbors[CP_UP]==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,-1))!=NULL);

    //Test play in location not in rim
    printf("\n");
    play_in = CP_Location_new(0,1);
    node_played = CG_play_tile_in(game,play_in,cloister_tile);
    curr_node = game->playboard->node;
    printf("%d",node_played==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CP_Location_new(1,-1))!=NULL);

    CG_free_game(game);
    CP_free_playboard_node(game->playboard->node->neighbors[CP_DOWN]);//To be removed when CP_free_playboard is complete
    CP_free_playboard_node(origin_node);
}

void test_game_run_all(){
    test_game_initiate();
    printf("\n*******************************\n");
    test_game_rim_to_playboard_update_one_side();
    printf("\n*******************************\n");
    test_game_node_transfer_rim_to_playboard();
    printf("\n*******************************\n");
    test_game_play_tile_in();
    printf("\n*******************************\n");
}
