#include "model/player/player.h"

void test_player_init_player(){
    printf("test_player_init_player results: ");
    Carc_Player* player = CPP_init_player(PLAYER_1, CPC_BLACK);
    printf("%d",player!=NULL && player->id==PLAYER_1 && player->color==CPC_BLACK && player->points==0
                && strcmp(player->username,"Player 1")==0 && player->nb_architect_pawns==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_bishop_pawns==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_double_weighted_pawns==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_normal_pawns==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pig_pawns==NB_PIG_PAWNS_PER_PLAYER);

    player = CPP_init_player(PLAYER_4, CPC_BLACK);
    printf("%d",player!=NULL && player->id==PLAYER_4 && player->color==CPC_BLACK && player->points==0
                && strcmp(player->username,"Player 4")==0 && player->nb_architect_pawns==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_bishop_pawns==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_double_weighted_pawns==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_normal_pawns==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pig_pawns==NB_PIG_PAWNS_PER_PLAYER);
    player = CPP_init_player(PLAYER_4, CPC_GREEN);
    printf("%d",player!=NULL && player->id==PLAYER_4 && player->color==CPC_GREEN && player->points==0
                && strcmp(player->username,"Player 4")==0 && player->nb_architect_pawns==NB_ARCHITECT_PAWNS_PER_PLAYER
                && player->nb_bishop_pawns==NB_BISHOP_PAWNS_PER_PLAYER && player->nb_double_weighted_pawns==NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER
                && player->nb_normal_pawns==NB_NORMAL_PAWNS_PER_PLAYER && player->nb_pig_pawns==NB_PIG_PAWNS_PER_PLAYER);

    CPP_free_player(player);
}


