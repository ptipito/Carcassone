#include "model/player/player.h"

Carc_Player* CPP_init_player(Carc_Player_ID id, Carc_Player_Color color){
    Carc_Player* player = malloc(sizeof(*player));
    if(player==NULL){
        fprintf(stderr,"Couldn't allocate memory for player %d\n",id);
        carcassone_error_quit(ERR_MEM_ALLOC,NULL);
    }

    char* username_base="Player ";
    char player_ID[2]={'\0'};
    player->id = id;
    player->color = color;
    player->points = 0;
    strncat(player->username,username_base,PLAYER_USERNAME_MAX_LEN-1);
    itoa(id,player_ID,10);
    strncat(player->username, player_ID,PLAYER_USERNAME_MAX_LEN - strlen(player->username) - 1);
    player->nb_normal_pawns = NB_NORMAL_PAWNS_PER_PLAYER;
    player->nb_double_weighted_pawns = NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER;
    player->nb_architect_pawns = NB_ARCHITECT_PAWNS_PER_PLAYER;
    player->nb_pig_pawns = NB_PIG_PAWNS_PER_PLAYER;
    player->nb_bishop_pawns = NB_BISHOP_PAWNS_PER_PLAYER;

    return player;
}

void CPP_free_player(Carc_Player* p){
    free(p);
    p = NULL;
}
