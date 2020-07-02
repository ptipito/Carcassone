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

    //Test constructs init properly
    Carc_Macro_Construct_List* game_const = game->constructs;
    printf("%d",game_const!=NULL && game_const->construct->type==CBCT_FIELD
           && game_const->next!=NULL && game_const->next->construct->type==CBCT_PATH
           && game_const->next->next!=NULL && game_const->next->next->construct->type==CBCT_FIELD
           && game_const->next->next->next!=NULL && game_const->next->next->next->construct->type==CBCT_CITY
           && game_const->next->next->next->next==NULL);

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
    //Test game constructs updated (test only types. The deeper check is done in the test of the update_constructs function)
    printf("%d",game->constructs->construct->type==CBCT_CLOISTER
                && game->constructs->next->construct->type==CBCT_PATH
                && game->constructs->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->construct->type==CBCT_PATH
                && game->constructs->next->next->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->next->next->construct->type==CBCT_CITY
                && game->constructs->next->next->next->next->next->next==NULL
                );

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
    //Test game constructs unchanged
    printf("%d",game->constructs->construct->type==CBCT_CLOISTER
                && game->constructs->next->construct->type==CBCT_PATH
                && game->constructs->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->construct->type==CBCT_PATH
                && game->constructs->next->next->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->next->next->construct->type==CBCT_CITY
                && game->constructs->next->next->next->next->next->next==NULL
                );

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
    //Test game constructs updated (test only types. The deeper check is done in the test of the update_constructs function)
    printf("%d",game->constructs->construct->type==CBCT_CLOISTER
                && game->constructs->next->construct->type==CBCT_PATH
                && game->constructs->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->construct->type==CBCT_PATH
                && game->constructs->next->next->next->next->construct->type==CBCT_FIELD
                && game->constructs->next->next->next->next->next->construct->type==CBCT_CITY
                && game->constructs->next->next->next->next->next->next==NULL
                );

    CBP_free_playboard_node(CBP_get_neighbor(game->playboard->node,CPCS_DOWN));//To be removed when CBP_free_playboard is complete
    CGG_free_game(game);
    CBP_free_playboard_node(origin_node);
    free(start_tile_str);
    free(cloister_tile_str);
}

void test_game_play_pawn_in(){
    printf("test_game_play_pawn_in results: \n");
    char *start_tile_str=CBT_get_tile_file_path("tile1.txt");
    Carc_Player* player=CPPlayer_init_player(PLAYER_1,CPC_BLACK);
    Carc_Pawn* pawn=NULL, *pawn2=NULL;
    Carc_Tile_Location loc=CTL_CENTER;
    Carc_Game* game=CGG_initiate_game(start_tile_str,2);
    Carc_Tile* start_tile=game->playboard->node->node;
    Carc_Macro_Construct** arr_start_constructs=CBMC_get_tile_constructs_per_node(game->constructs,start_tile);

    //Test if player or tile null
    printf("Test player is null: ");
    pawn = CGG_play_pawn_in(NULL,PAWN_NORMAL,start_tile,loc,NULL,NULL);
    printf("\tTest result: %d\n",pawn==NULL);
    printf("Test tile is null: ");
    pawn = CGG_play_pawn_in(player,PAWN_NORMAL,NULL,loc,NULL,NULL);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);
    printf("Test tile_constructs is null: ");
    pawn = CGG_play_pawn_in(player,PAWN_NORMAL,start_tile,loc,NULL,NULL);
    printf("\tTest result: %d\n",pawn==NULL);
    printf("Test game is null: ");
    pawn = CGG_play_pawn_in(player,PAWN_NORMAL,start_tile,loc,arr_start_constructs,NULL);
    printf("\tTest result: %d\n",pawn==NULL);

    //Test player cannot play
    printf("Test cannot play this pawn type: \n");
    player->nb_pawns[PAWN_ARCHITECT] = 0;
    pawn = CGG_play_pawn_in(player,PAWN_ARCHITECT,start_tile,loc,arr_start_constructs,game);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);

    //Test invalid loc input
    printf("Test invalid loc input: \n");
    pawn = CGG_play_pawn_in(player,PAWN_NORMAL,start_tile,68,arr_start_constructs,game);
    printf("\tTest result: %d\n",pawn==NULL);
    CPPawn_free_pawn(pawn);

    //Test when inputs allow to play
    printf("Test when playing is ok: ");
    pawn = CGG_play_pawn_in(player, PAWN_NORMAL, start_tile, CTL_CENTER, arr_start_constructs, game);
    printf("%d",pawn!=NULL && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-1
                && start_tile->center.pawn == pawn
                && game->constructs->next->construct->nb_pawns==1
                && game->constructs->next->construct->pawns[0]==pawn
                );
    pawn = CGG_play_pawn_in(player, PAWN_NORMAL, start_tile, CTL_SOUTH_WEST, arr_start_constructs, game);
    printf("%d",pawn!=NULL && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-2
                && start_tile->border[CTL_SOUTH_WEST].pawn == pawn
                && game->constructs->construct->nb_pawns==1
                && game->constructs->construct->pawns[0]==pawn
                );

    CPPlayer_free_player(player);
    CGG_free_game(game);
    free(start_tile_str);
}

void test_game_merge_constructs_with_neighbor(){
    printf("test_game_merge_constructs_with_neighbor results: \n");
    char *cloister_path=CBT_get_tile_file_path("cloister_path.txt"),
    *start_tile_path=CBT_get_tile_file_path("tile1.txt");
    Carc_Game* game=CGG_initiate_game(start_tile_path,3);
    Carc_Tile* tile_cloister=CBT_new_tile_from_file(cloister_path);
    Carc_Macro_Construct_List* cloister_constructs=CBMC_get_tile_macro_constructions(tile_cloister);

    Carc_Playboard_Node* cloister=CBP_new_playboard_node(tile_cloister,CBP_Location_new(0,-1));
    cloister->neighbors[CPCS_UP] = &(game->playboard->node);

    //Test merge with null neighbor
    int test_res=CGG_merge_constructs_with_neighbor(NULL,cloister,CPCS_UP,cloister_constructs);
    printf("%d",test_res==FUNC_FAIL);
    test_res=CGG_merge_constructs_with_neighbor(game,NULL,CPCS_UP,NULL);
    printf("%d",test_res==FUNC_FAIL);
    test_res=CGG_merge_constructs_with_neighbor(game,cloister,CPCS_UP,NULL);
    printf("%d",test_res==FUNC_FAIL);
    //Test merges properly with a good tile as neighbor and 1 construct on the edge
    test_res=CGG_merge_constructs_with_neighbor(game,cloister,CPCS_UP,cloister_constructs);
    int rim_is_right = 1;
    Carc_Tile_Node_List* cur=game->constructs->construct->rim;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur==NULL);
    printf("%d",test_res==FUNC_SUCCESS
           && game->constructs->construct->type==CBCT_FIELD
           && rim_is_right
           //other constructs are untouched
           && game->constructs->next->construct->type==CBCT_PATH
           && game->constructs->next->next->construct->type==CBCT_FIELD
           && game->constructs->next->next->next->construct->type==CBCT_CITY
           && game->constructs->next->next->next->next==NULL
           );

    //Test if neighhbor is null (nothing changed)
    test_res = CGG_merge_constructs_with_neighbor(game,cloister,CPCS_DOWN,cloister_constructs);
    printf("%d",test_res==FUNC_SUCCESS
           && game->constructs->construct->type==CBCT_FIELD
           && game->constructs->next->construct->type==CBCT_PATH
           && game->constructs->next->next->construct->type==CBCT_FIELD
           && game->constructs->next->next->next->construct->type==CBCT_CITY
           && game->constructs->next->next->next->next==NULL
           );

    //Test merging cloister_path with starting tile on path side. (Merge 2 fields of starting tile into one thanks to field of cloister_path)
    Carc_Tile* tile_cloister2=CBT_new_tile_from_file(cloister_path);
    CBT_turn(tile_cloister2,CCT_RIGHT);
    Carc_Macro_Construct_List* cloister_constructs2=CBMC_get_tile_macro_constructions(tile_cloister2);
    Carc_Playboard_Node* cloister2=CBP_new_playboard_node(tile_cloister2,CBP_Location_new(1,0));
    cloister2->neighbors[CPCS_LEFT] = &(game->playboard->node);


    test_res = CGG_merge_constructs_with_neighbor(game,cloister2,CPCS_LEFT,cloister_constructs2);


    cur=game->constructs->next->construct->rim;//The extended field rim
    rim_is_right = 1;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_NORTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_NORTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister2->node->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(cloister->node->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur==NULL);

    cur=game->constructs->construct->rim;//The rim of the path construction
    int path_rim_is_right=(cur->node==&(game->playboard->node->node->border[CTL_WEST]) && cur->next==NULL);

    printf("%d",test_res==FUNC_SUCCESS
           && rim_is_right
           && path_rim_is_right
           && game->constructs->construct->type==CBCT_PATH
           && game->constructs->next->construct->type==CBCT_FIELD
           && game->constructs->next->next->construct->type==CBCT_CITY
           && game->constructs->next->next->next==NULL
           );

    //Avoid double free by removing from tile constructs the merrged constructions
    Carc_Tile_Node* n=CBT_get_node_from_loc(tile_cloister,CTL_NORTH);
    Carc_Macro_Construct* c=CBMC_get_node_construct(cloister_constructs,&n);
    CBMCList_rm(&cloister_constructs,&c);
    n=CBT_get_node_from_loc(tile_cloister2,CTL_NORTH);
    c=CBMC_get_node_construct(cloister_constructs2,&n);
    CBMCList_rm(&cloister_constructs2,&c);
    n=CBT_get_node_from_loc(tile_cloister2,CTL_WEST);
    c=CBMC_get_node_construct(cloister_constructs2,&n);
    CBMCList_rm(&cloister_constructs2,&c);
    //Free operations can be performed without double free
    CGG_free_game(game);
    CBMCList_free(cloister_constructs);
    free(cloister_path);
    free(start_tile_path);
    CBP_free_playboard_node(cloister);
    CBP_free_playboard_node(cloister2);
}

void test_game_update_constructs(){
    printf("test_game_update_constructs results: ");
    char *cloister_path=CBT_get_tile_file_path("cloister_path.txt"),
         *start_tile_path=CBT_get_tile_file_path("tile1.txt");
    Carc_Game* game=CGG_initiate_game(start_tile_path,3);
    Carc_Tile *tile_cloister=CBT_new_tile_from_file(cloister_path),
              *start_tile=game->playboard->node->node;
    Carc_Playboard_Node* node_cloister=CBP_new_playboard_node(tile_cloister,CBP_Location_new(0,-1));
    node_cloister->neighbors[CPCS_UP] = &(game->playboard->node);

    //Test null inputs
    printf("%d",CGG_update_game_constructs(NULL,node_cloister)==FUNC_FAIL);
    printf("%d",CGG_update_game_constructs(game,NULL)==FUNC_SUCCESS);

    //Test merge when played with only one neighbor
    int test_res=CGG_update_game_constructs(game,node_cloister),
        rim_is_right = 1;
    Carc_Tile_Node_List* cur=game->constructs->next->next->construct->rim;//The merged field
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(node_cloister->node->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(start_tile->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(start_tile->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur==NULL);
    printf("%d",test_res==FUNC_SUCCESS
                && game->constructs->construct->type==CBCT_CLOISTER
                && game->constructs->next->construct->type==CBCT_PATH
                    && game->constructs->next->construct->rim->node==CBT_get_node_from_loc(tile_cloister,CTL_SOUTH)
                    && game->constructs->next->construct->rim->next==NULL
                && game->constructs->next->next->construct->type==CBCT_FIELD && rim_is_right
                && game->constructs->next->next->next->construct->type==CBCT_PATH
                    && game->constructs->next->next->next->construct->rim->node==CBT_get_node_from_loc(start_tile,CTL_EAST)
                    && game->constructs->next->next->next->construct->rim->next->node==CBT_get_node_from_loc(start_tile,CTL_WEST)
                    && game->constructs->next->next->next->construct->rim->next->next==NULL
                && game->constructs->next->next->next->next->construct->type==CBCT_FIELD
                    && game->constructs->next->next->next->next->construct->rim->node==CBT_get_node_from_loc(start_tile,CTL_EAST_NORTH)
                    && game->constructs->next->next->next->next->construct->rim->next->node==CBT_get_node_from_loc(start_tile,CTL_WEST_NORTH)
                    && game->constructs->next->next->next->next->construct->rim->next->next==NULL
                && game->constructs->next->next->next->next->next->construct->type==CBCT_CITY
                    && game->constructs->next->next->next->next->next->construct->rim->node==CBT_get_node_from_loc(start_tile,CTL_NORTH_WEST)
                    && game->constructs->next->next->next->next->next->construct->rim->next->node==CBT_get_node_from_loc(start_tile,CTL_NORTH)
                    && game->constructs->next->next->next->next->next->construct->rim->next->next->node==CBT_get_node_from_loc(start_tile,CTL_NORTH_EAST)
                    && game->constructs->next->next->next->next->next->construct->rim->next->next->next==NULL
                && game->constructs->next->next->next->next->next->next==NULL
           );


    //Test when several neighbors
    Carc_Tile *tile_cloister2=CBT_new_tile_from_file(cloister_path),
              *tile_cloister3=CBT_new_tile_from_file(cloister_path);
    CBT_turn(tile_cloister2,CCT_RIGHT);
    Carc_Playboard_Node *node_cloister2=CBP_new_playboard_node(tile_cloister2,CBP_Location_new(1,0)),
                        *node_cloister3=CBP_new_playboard_node(tile_cloister3,CBP_Location_new(1,-1));
    node_cloister2->neighbors[CPCS_LEFT] = &(game->playboard->node);
    node_cloister3->neighbors[CPCS_LEFT] = &(node_cloister);
    node_cloister3->neighbors[CPCS_UP] = &(node_cloister2);
    //Prepare 2nd neighbor
    CGG_update_game_constructs(game,node_cloister2);//Do not display test result as the case is the same as the previous one
    //Test the merger happened properly on both side
    test_res = CGG_update_game_constructs(game,node_cloister3);
    //Check the field rim
    cur = game->constructs->next->next->construct->rim;//The extended field rim
    rim_is_right = 1;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister3->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister3->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister3->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister3->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister3->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_NORTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_NORTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_EAST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister2->border[CTL_EAST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister->border[CTL_SOUTH_EAST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister->border[CTL_SOUTH_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister->border[CTL_WEST]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(tile_cloister->border[CTL_WEST_NORTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur->node==&(game->playboard->node->node->border[CTL_WEST_SOUTH]));
    cur = cur->next;
    rim_is_right = (rim_is_right && cur==NULL);

    printf("%d",test_res==FUNC_SUCCESS
                && game->constructs->construct->type==CBCT_CLOISTER
                && game->constructs->next->construct->type==CBCT_PATH
                    && game->constructs->next->construct->rim->node==CBT_get_node_from_loc(tile_cloister3,CTL_SOUTH)
                    && game->constructs->next->construct->rim->next==NULL
                && game->constructs->next->next->construct->type==CBCT_FIELD && rim_is_right
                && game->constructs->next->next->next->construct->type==CBCT_CLOISTER
                && game->constructs->next->next->next->next->construct->type==CBCT_PATH
                    && game->constructs->next->next->next->next->construct->rim->node==CBT_get_node_from_loc(start_tile,CTL_WEST)
                    && game->constructs->next->next->next->next->construct->rim->next==NULL
                && game->constructs->next->next->next->next->next->construct->type==CBCT_CLOISTER
                && game->constructs->next->next->next->next->next->next->construct->type==CBCT_PATH
                    && game->constructs->next->next->next->next->next->next->construct->rim->node==CBT_get_node_from_loc(tile_cloister,CTL_SOUTH)
                    && game->constructs->next->next->next->next->next->next->construct->rim->next==NULL
                && game->constructs->next->next->next->next->next->next->next->construct->type==CBCT_CITY
                    && game->constructs->next->next->next->next->next->next->next->construct->rim->node==CBT_get_node_from_loc(start_tile,CTL_NORTH_WEST)
                    && game->constructs->next->next->next->next->next->next->next->construct->rim->next->node==CBT_get_node_from_loc(start_tile,CTL_NORTH)
                    && game->constructs->next->next->next->next->next->next->next->construct->rim->next->next->node==CBT_get_node_from_loc(start_tile,CTL_NORTH_EAST)
                    && game->constructs->next->next->next->next->next->next->next->construct->rim->next->next->next==NULL
                && game->constructs->next->next->next->next->next->next->next->next==NULL
           );


    ///TODO add a test with 2 neighbors without merging their constructions when resources are available (e.g. one has a city, the other a path)

    CGG_free_game(game);
    free(cloister_path);
    free(start_tile_path);
    CBP_free_playboard_node(node_cloister);
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
    test_game_merge_constructs_with_neighbor();
    printf("\n*******************************\n");
    test_game_update_constructs();
    printf("\n*******************************\n");
}
