#include "model/player/player.h"

void test_player_init_player(){
    printf("test_player_init_player results: ");
    Carc_Player* player = CPPlayer_init_player(PLAYER_1, CPC_BLACK);
    printf("%d",player!=NULL && player->id==PLAYER_1 && player->color==CPC_BLACK && player->points==0
                && strcmp(player->username,"Player 1")==0 && player->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);

    player = CPPlayer_init_player(PLAYER_4, CPC_BLACK);
    printf("%d",player!=NULL && player->id==PLAYER_4 && player->color==CPC_BLACK && player->points==0
                && strcmp(player->username,"Player 4")==0 && player->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);
    player = CPPlayer_init_player(PLAYER_4, CPC_GREEN);
    printf("%d",player!=NULL && player->id==PLAYER_4 && player->color==CPC_GREEN && player->points==0
                && strcmp(player->username,"Player 4")==0 && player->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);
    player = CPPlayer_init_player(PLAYER_3, 25);
    printf("%d",player!=NULL && player->id==PLAYER_3 && player->color==25 && player->points==0
                && strcmp(player->username,"Player 3")==0 && player->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);

    CPPlayer_free_player(player);
}

void test_player_add_points(){
    printf("test_player_add_points results: ");
    Carc_Player *p1=CPPlayer_init_player(PLAYER_1,CPC_BLUE),
                *p2=CPPlayer_init_player(PLAYER_2,CPC_RED);

    printf("%d",CPPlayer_add_points(p1,5)==0 && p1->points==5);
    printf("%d",p2->points==0);
    printf("%d",CPPlayer_add_points(p1,-2)==0 && p1->points==3);
    printf("%d",p2->points==0);
    CPPlayer_add_points(p2,24);
    printf("%d",p2->points==24);
    printf("%d",CPPlayer_add_points(p1,2.4)==0 && p1->points==5);
    printf("%d",p2->points==24);

    CPPlayer_free_player(p1);
    CPPlayer_free_player(p2);

    //Test null input
    printf("\nTest null input --> ");
    printf("\tTest result: %d",CPPlayer_add_points(NULL,25)==-1);
}

void test_player_add_to_nb_pawns(){
    printf("test_player_add_to_nb_pawns results: ");
    Carc_Player *p1=CPPlayer_init_player(PLAYER_1,CPC_BLUE),
                *p2=CPPlayer_init_player(PLAYER_2,CPC_RED);

    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_NORMAL,1)==-1 && p1->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_NORMAL,-1)==0 && p1->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_NORMAL,-NB_NORMAL_PAWNS_PER_PLAYER-1)==-1
                && p1->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_NORMAL,-(NB_NORMAL_PAWNS_PER_PLAYER-1))==0
                && p1->nb_pawns[PAWN_NORMAL]==0);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_NORMAL,1)==0 && p1->nb_pawns[PAWN_NORMAL]==1);

    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_ARCHITECT,1)==-1 && p1->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_ARCHITECT,-1)==0 && p1->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_ARCHITECT,-NB_ARCHITECT_PAWNS_PER_PLAYER)==-1
                && p1->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER-1);

    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_DOUBLE,1)==-1 && p1->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_DOUBLE,-1)==0 && p1->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_DOUBLE,-NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER)==-1
                && p1->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER-1);

    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_BISHOP,1)==-1 && p1->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_BISHOP,-1)==0 && p1->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_BISHOP,-NB_BISHOP_PAWNS_PER_PLAYER)==-1
                && p1->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER-1);

    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_PIG,1)==-1 && p1->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_PIG,-1)==0 && p1->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER-1);
    printf("%d",CPPlayer_add_to_nb_pawns(p1,PAWN_PIG,-NB_PIG_PAWNS_PER_PLAYER)==-1
                && p1->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER-1);

    printf("%d",p2->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER);
    printf("%d",p2->nb_pawns[PAWN_ARCHITECT]==NB_ARCHITECT_PAWNS_PER_PLAYER);
    printf("%d",p2->nb_pawns[PAWN_BISHOP]==NB_BISHOP_PAWNS_PER_PLAYER);
    printf("%d",p2->nb_pawns[PAWN_DOUBLE]==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER);
    printf("%d",p2->nb_pawns[PAWN_PIG]==NB_PIG_PAWNS_PER_PLAYER);

    CPPlayer_free_player(p1);
    CPPlayer_free_player(p2);

    //Test null input
    printf("\nTest null input --> ");
    printf("\tTest result: %d",CPPlayer_add_to_nb_pawns(NULL,PAWN_NORMAL,2)==-1);
}

void test_player_get_nb_pawns(){
    printf("test_player_get_nb_pawns results: ");
    Carc_Player* p = CPPlayer_init_player(PLAYER_1,CPC_BLACK);
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_ARCHITECT)==NB_ARCHITECT_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_NORMAL)==NB_NORMAL_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_DOUBLE)==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_PIG)==NB_PIG_PAWNS_PER_PLAYER);
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_BISHOP)==NB_BISHOP_PAWNS_PER_PLAYER);

    p->nb_pawns[PAWN_NORMAL]=3;
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_NORMAL)==3);

    p->nb_pawns[PAWN_NORMAL]=-3;
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_NORMAL)==-3);

    p->nb_pawns[PAWN_NORMAL]=9;
    printf("%d",CPPlayer_get_nb_pawns(p,PAWN_NORMAL)==9);

    CPPlayer_free_player(p);
    }

void test_player_can_play_pawn(){
    printf("test_player_can_play_pawn results: ");
    Carc_Player* p = CPPlayer_init_player(PLAYER_1,CPC_BLACK);
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_ARCHITECT)==1);
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_NORMAL)==1);
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_DOUBLE)==1);
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_PIG)==1);
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_BISHOP)==1);

    p->nb_pawns[PAWN_NORMAL] = 0;
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_NORMAL)==0);

    p->nb_pawns[PAWN_NORMAL] = -5;
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_NORMAL)==0);

    p->nb_pawns[PAWN_NORMAL] = 1;
    printf("%d",CPPlayer_can_play_pawn(p,PAWN_NORMAL)==1);

    CPPlayer_free_player(p);
}

void test_player_run_all(){
    test_player_init_player();
    printf("\n");
    test_player_add_points();
    printf("\n");
    test_player_add_to_nb_pawns();
    printf("\n");
    test_player_get_nb_pawns();
    printf("\n");
    test_player_can_play_pawn();
    printf("\n");
}
