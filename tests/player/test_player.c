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

    CPPlayer_add_points(p1,5);
    printf("%d",p1->points==5);
    printf("%d",p2->points==0);
    CPPlayer_add_points(p1,-2);
    printf("%d",p1->points==3);
    printf("%d",p2->points==0);
    CPPlayer_add_points(p2,24);
    printf("%d",p1->points==3);
    printf("%d",p2->points==24);
    CPPlayer_add_points(p1,2.4);
    printf("%d",p1->points==5);
    printf("%d",p2->points==24);

    CPPlayer_free_player(p1);
    CPPlayer_free_player(p2);
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
}
