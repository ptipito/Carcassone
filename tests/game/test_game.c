#include "tests/game/test_game.h"

void test_game_initiate(){
    printf("test_game_initiate results: ");
    char* start_tile_str = CBT_get_tile_file_path("tile1.txt");
    int i, boolean=1;

    //Test playboard and rim init properly
    Carc_Game* game = CGG_initiate_game(start_tile_str,2);
    Carc_Playboard_Origin* origin = CBP_init_playboard(CBT_new_tile_from_file(start_tile_str));
    printf("%d",CBP_node_cmp(game->playboard->node,origin->node)==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,0))!=NULL);
    int rim_length = 0;
    Carc_Rim_Element* current = game->playable->first;
    while(current!=NULL){
        rim_length++;
        current = current->next;
    }

    printf("%d",rim_length==4);

    //Test players init properly
    printf("%d",game->players!=NULL);
    for(i=0;i<NB_MAX_PLAYERS;i++){
        if(i==0 || i > 2){
            boolean = boolean && game->players[i]==NULL;
        } else{
            boolean = boolean && game->players[i]->id==i && game->players[i]->color==i-1;
        }
    }
    printf("%d",boolean);

    //Test creating with invalid number of players
    printf("\n");
    printf("%d\n",CGG_initiate_game(start_tile_str,1)==NULL);
    printf("%d\n",CGG_initiate_game(start_tile_str,0)==NULL);
    printf("%d\n",CGG_initiate_game(start_tile_str,NB_MAX_PLAYERS+1)==NULL);

    CGG_free_game(game);
    free(start_tile_str);
}

void test_game_rim_to_playboard_update_one_side(){
    printf("test_game_rim_to_playboard_update_one_side results: ");
    int update_result=0;
    char *tile1_path = CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_path = CBT_get_tile_file_path("cloister_path.txt");
    Carc_Game* game = CGG_initiate_game(tile1_path,2);
    Carc_Playboard_Location play_in = CBP_Location_new(0,-1);
    Carc_Playboard_Node *cloister = CBP_new_playboard_node(CBT_new_tile_from_file(cloister_tile_path),
                                                          play_in),
                        *empty_node = CBP_new_empty_playboard_node(CBP_Location_new(15,26));

    update_result = CGG_rim_to_playboard_update_one_side(NULL,play_in,&cloister,CPCS_UP);
    printf("%d\n",update_result==1);
    update_result = CGG_rim_to_playboard_update_one_side(game->playable,play_in,NULL,CPCS_UP);
    printf("%d\n",update_result==1);
    update_result = CGG_rim_to_playboard_update_one_side(game->playable,CBP_Location_new(15,26),&cloister,CPCS_UP);
    printf("%d\n",update_result==2);
    update_result = CGG_rim_to_playboard_update_one_side(game->playable,play_in,&empty_node,CPCS_UP);
    printf("%d\n",update_result==3);

    update_result = CGG_rim_to_playboard_update_one_side(game->playable,play_in,&cloister,CPCS_UP);
    printf("%d",update_result==0);
    printf("%d",CB_Rim_find_by_location(game->playable,play_in)!=NULL);
    printf("%d",CBP_node_cmp(CBP_get_neighbor(cloister,CPCS_UP),game->playboard->node)==0);


    printf("%d",CBP_node_cmp(cloister,CBP_get_neighbor(game->playboard->node,CPCS_DOWN))==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_DOWN))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_LEFT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_RIGHT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_UP))==NULL);

    update_result = CGG_rim_to_playboard_update_one_side(game->playable,play_in,&cloister,CPCS_LEFT);
    printf("%d",update_result==0);
    printf("%d",CBP_node_cmp(CBP_get_neighbor(cloister,CPCS_LEFT),NULL)==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_DOWN))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_LEFT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_RIGHT))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_UP))==NULL);

    CGG_free_game(game);
    CBP_free_playboard_node(empty_node);
    CBP_free_playboard_node(cloister);//To be removed when free_playboard is completed
    free(tile1_path);
    free(cloister_tile_path);
}

void test_game_node_transfer_rim_to_playboard(){
    printf("test_game_node_transfer_rim_to_playboard results:\n");
    int update_result=0;
    char* tile1_path = CBT_get_tile_file_path("tile1.txt");
    Carc_Game* game = CGG_initiate_game(tile1_path,2);
    Carc_Playboard_Location play_in = CBP_Location_new(0,-1);
    Carc_Playboard_Node *cloister = CBP_new_playboard_node(CBT_new_tile_from_file("ressources/gameset/tiles/cloister_path.txt"),
                                                          play_in),
                        *empty_node = CBP_new_empty_playboard_node(CBP_Location_new(15,26));

    update_result = CGG_node_transfer_rim_to_playboard(NULL,play_in,NULL);
    printf("%d\n",update_result==1);
    update_result = CGG_node_transfer_rim_to_playboard(NULL,play_in,&cloister);
    printf("%d\n",update_result==1);
    update_result = CGG_node_transfer_rim_to_playboard(game,play_in,NULL);
    printf("%d\n",update_result==1);
    update_result = CGG_node_transfer_rim_to_playboard(game,CBP_Location_new(15,26),&cloister);
    printf("%d\n",update_result==2);
    update_result = CGG_node_transfer_rim_to_playboard(game,play_in,&empty_node);
    printf("%d\n",update_result==3);

    update_result = CGG_node_transfer_rim_to_playboard(game,play_in,&cloister);
    printf("%d",update_result==0);
    printf("%d",CB_Rim_find_by_location(game->playable,play_in)==NULL);
    printf("%d",CBP_node_cmp(CBP_get_neighbor(cloister,CPCS_UP),game->playboard->node)==0);
    printf("%d",CBP_node_cmp(cloister,CBP_get_neighbor(game->playboard->node,CPCS_DOWN))==0);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_DOWN))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_LEFT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_RIGHT))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_get_neighbor_loc(play_in,CPCS_UP))==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,0))!=NULL);


    CGG_free_game(game);
    CBP_free_playboard_node(empty_node);
    CBP_free_playboard_node(cloister);//To be removed when free_playboard is completed
    free(tile1_path);
}

void test_game_play_tile_in(){
    printf("test_game_play_tile_in results: ");
    char *start_tile_str = CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_str = CBT_get_tile_file_path("cloister_path.txt");
    Carc_Game* game = CGG_initiate_game(start_tile_str,2);
    Carc_Playboard_Location play_in = CBP_Location_new(0,-1);
    Carc_Tile* cloister_tile = CBT_new_tile_from_file(cloister_tile_str);
    Carc_Playboard_Node *origin_node = CBP_new_playboard_node(CBT_new_tile_from_file(start_tile_str),CBP_Location_new(0,0)),
                        *curr_node=NULL;
    Carc_Playboard_Node* node_played = CGG_play_tile_in(game,play_in,cloister_tile);

    printf("%d",CBT_tile_cmp(game->playboard->node->node,origin_node->node)==0);
    printf("%d",CBP_Location_cmp(game->playboard->node->node_coordinates,origin_node->node_coordinates)==0);
    printf("%d",CBP_get_neighbor(game->playboard->node,CPCS_DOWN)!=NULL && CBP_get_neighbor(game->playboard->node,CPCS_DOWN)==node_played);
    printf("%d",CBP_get_neighbor(game->playboard->node,CPCS_LEFT)==NULL);
    printf("%d",CBP_get_neighbor(game->playboard->node,CPCS_RIGHT)==NULL);
    printf("%d",CBP_get_neighbor(game->playboard->node,CPCS_UP)==NULL);
    //Check the neighbor was inserted correctly
    printf("\n");
    curr_node = CBP_get_neighbor(game->playboard->node,CPCS_DOWN);
    printf("%d",CBT_tile_cmp(curr_node->node,cloister_tile)==0);
    printf("%d",CBP_Location_cmp(curr_node->node_coordinates,play_in)==0);
    printf("%d",CBP_get_neighbor(curr_node,CPCS_UP)==game->playboard->node);
    printf("%d",CBP_get_neighbor(curr_node,CPCS_LEFT)==NULL);
    printf("%d",CBP_get_neighbor(curr_node,CPCS_RIGHT)==NULL);
    printf("%d",CBP_get_neighbor(curr_node,CPCS_DOWN)==NULL);
    //Test rim was updated properly
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,-1))!=NULL);
    //Test node was removed from rim
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,-1))==NULL);

    //Test play in a location where tiles do not connect
    printf("\n");
    play_in = CBP_Location_new(0,1);
    node_played = CGG_play_tile_in(game,play_in,cloister_tile);
    curr_node = game->playboard->node;
    printf("%d",node_played==NULL);
    printf("%d",CBP_get_neighbor(curr_node,CPCS_UP)==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,-1))!=NULL);

    //Test play in location not in rim
    printf("\n");
    play_in = CBP_Location_new(0,1);
    node_played = CGG_play_tile_in(game,play_in,cloister_tile);
    curr_node = game->playboard->node;
    printf("%d",node_played==NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,0))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(0,-2))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(-1,-1))!=NULL);
    printf("%d",CB_Rim_find_by_location(game->playable,CBP_Location_new(1,-1))!=NULL);

    CBP_free_playboard_node(CBP_get_neighbor(game->playboard->node,CPCS_DOWN));//To be removed when CBP_free_playboard is complete
    CGG_free_game(game);
    CBP_free_playboard_node(origin_node);
    free(start_tile_str);
    free(cloister_tile_str);
}

void test_game_play_pawn_in(){
    printf("test_game_play_pawn_in results: \n");
    char *start_tile_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Tile* start_tile=CBT_new_tile_from_file(start_tile_str);
    Carc_Player* player=CPPlayer_init_player(PLAYER_1,CPC_BLACK);
    Carc_Pawn* pawn=NULL;
    Carc_Tile_Location loc=CTL_CENTER;
    printf("loc=%d",loc);
    //Test if player or tile null
    printf("Test player is null: ");
    pawn = CGG_play_pawn_in(NULL,PAWN_NORMAL,start_tile,loc);
    printf("\tTest result: %d\n",pawn==NULL);
    printf("Test tile is null: ");
    pawn = CGG_play_pawn_in(NULL,PAWN_NORMAL,start_tile,loc);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);

    //Test player cannot play
    printf("Test cannot play this pawn type: \n");
    player->nb_pawns[PAWN_ARCHITECT] = 0;
    pawn = CGG_play_pawn_in(player,PAWN_ARCHITECT,start_tile,loc);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);

    //Test invalid loc input
    printf("Test invalid loc input: \n");
    pawn = CGG_play_pawn_in(player,PAWN_NORMAL,start_tile,68);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);

    //Test when inputs allow to play
    printf("Test when playing is ok: ");
    pawn = CGG_play_pawn_in(player, PAWN_NORMAL, start_tile, CTL_CENTER);
    printf("%d",pawn!=NULL && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-1
                && start_tile->center.pawn == pawn);
    CPPawn_free_pawn(pawn);
    pawn = CGG_play_pawn_in(player, PAWN_NORMAL, start_tile, CTL_SOUTH_WEST);
    printf("%d",pawn!=NULL && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-2
                && start_tile->border[CTL_SOUTH_WEST].pawn == pawn);
    CPPawn_free_pawn(pawn);

    CPPlayer_free_player(player);
    free(start_tile_str);
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
    test_game_play_pawn_in();
    printf("\n*******************************\n");
}
