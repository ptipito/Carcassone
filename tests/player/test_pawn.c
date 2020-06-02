#include "tests/player/test_pawn.h"

void test_pawn_get_weight_from_type(){
    printf("test_pawn_get_weight_from_type results: ");
    printf("%d",CPPawn_get_weight_from_type(PAWN_NORMAL)==1);
    printf("%d",CPPawn_get_weight_from_type(PAWN_DOUBLE)==2);
    printf("%d",CPPawn_get_weight_from_type(PAWN_ARCHITECT)==0);
    printf("%d",CPPawn_get_weight_from_type(PAWN_PIG)==0);
    printf("%d",CPPawn_get_weight_from_type(PAWN_BISHOP)==0);
}

void test_pawn_new_pawn(){
    printf("test_pawn_new_pawn results: ");
    Carc_Player* player=CPPlayer_init_player(PLAYER_1,CPC_YELLOW);
    Carc_Pawn* pawn=CPPawn_new_pawn(player,PAWN_NORMAL);
    printf("%d",pawn->owner!=NULL && pawn->owner==player
                && pawn->type==PAWN_NORMAL && pawn->weight==1);

    pawn = CPPawn_new_pawn(player,PAWN_DOUBLE);
    printf("%d",pawn->owner!=NULL && pawn->owner==player
                && pawn->type==PAWN_DOUBLE && pawn->weight==2);

    pawn = CPPawn_new_pawn(player,PAWN_ARCHITECT);
    printf("%d",pawn->owner!=NULL && pawn->owner==player
                && pawn->type==PAWN_ARCHITECT && pawn->weight==0);

    pawn = CPPawn_new_pawn(player,PAWN_BISHOP);
    printf("%d",pawn->owner!=NULL && pawn->owner==player
                && pawn->type==PAWN_BISHOP && pawn->weight==0);

    pawn = CPPawn_new_pawn(player,PAWN_PIG);
    printf("%d",pawn->owner!=NULL && pawn->owner==player
                && pawn->type==PAWN_PIG && pawn->weight==0);

    CPPawn_free_pawn(pawn);
    CPPlayer_free_player(player);

    //Test with NULL player
    printf("\nTest for player null --> ");
    pawn = CPPawn_new_pawn(NULL,PAWN_PIG);
    printf("\t-->Test result: %d",pawn==NULL);

}

void test_pawn_send_back(){
    printf("test_pawn_send_back results: ");
    Carc_Player* player=CPPlayer_init_player(PLAYER_1,CPC_YELLOW);
    Carc_Pawn* pawn=CPPawn_new_pawn(player,PAWN_NORMAL);

    //Test NULL input
    printf("\nTest for pawn null --> ");
    printf("\tTest result: %d",CPPawn_send_back(NULL)==-1);
    //Test nb pawn already at max
    printf("\nTest if nb pawns already at max --> ");
    printf("\tTest result: %d",CPPawn_send_back(pawn)==-1 && pawn->owner->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER);
    //Tests can send back
    printf("\nTest pawn can be sent back --> ");
    pawn->owner->nb_pawns[PAWN_NORMAL]--;
    printf("%d",CPPawn_send_back(pawn)==0 && pawn->owner->nb_pawns[PAWN_NORMAL]==NB_NORMAL_PAWNS_PER_PLAYER);
    pawn->owner->nb_pawns[PAWN_NORMAL] = 0;
    printf("%d",CPPawn_send_back(pawn)==0 && pawn->owner->nb_pawns[PAWN_NORMAL]==1);

    CPPawn_free_pawn(pawn);
    CPPlayer_free_player(player);
}

void test_pawn_play(){
    printf("test_pawn_play results: ");
    Carc_Player* player=CPPlayer_init_player(PLAYER_1,CPC_YELLOW);
    Carc_Pawn* pawn=CPPawn_new_pawn(player,PAWN_NORMAL);

    //Test NULL input
    printf("\nTest for pawn null --> ");
    printf("\tTest result: %d",CPPawn_play(NULL)==-1);
    //Test nb pawn already at 0
    printf("\nTest if player has no pawn --> ");
    pawn->owner->nb_pawns[PAWN_NORMAL] = 0;
    printf("\tTest result: %d",CPPawn_play(pawn)==-1 && pawn->owner->nb_pawns[PAWN_NORMAL]==0);

    //Tests can play
    printf("\nTest pawn can be played --> ");
    pawn->owner->nb_pawns[PAWN_NORMAL] = 1;
    printf("%d",CPPawn_play(pawn)==0 && pawn->owner->nb_pawns[PAWN_NORMAL]==0);
    pawn->owner->nb_pawns[PAWN_NORMAL] = 8;
    printf("%d",CPPawn_play(pawn)==0 && pawn->owner->nb_pawns[PAWN_NORMAL]==7);
    printf("%d",CPPawn_play(pawn)==0 && pawn->owner->nb_pawns[PAWN_NORMAL]==6);

    CPPawn_free_pawn(pawn);
    CPPlayer_free_player(player);
}

void test_pawn_run_all(){
    test_pawn_get_weight_from_type();
    printf("\n************************************\n");
    test_pawn_new_pawn();
    printf("\n************************************\n");
    test_pawn_send_back();
    printf("\n************************************\n");
    test_pawn_play();
    printf("\n************************************\n");
}
