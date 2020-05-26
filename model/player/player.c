#include "model/player/player.h"

Carc_Player* CPPlayer_init_player(Carc_Player_ID id, Carc_Player_Color color){
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
    player->nb_pawns[PAWN_NORMAL] = NB_NORMAL_PAWNS_PER_PLAYER;
    player->nb_pawns[PAWN_DOUBLE] = NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER;
    player->nb_pawns[PAWN_ARCHITECT] = NB_ARCHITECT_PAWNS_PER_PLAYER;
    player->nb_pawns[PAWN_PIG] = NB_PIG_PAWNS_PER_PLAYER;
    player->nb_pawns[PAWN_BISHOP] = NB_BISHOP_PAWNS_PER_PLAYER;

    return player;
}

void CPPlayer_free_player(Carc_Player* p){
    free(p);
    p = NULL;
}

void CPPlayer_add_points(Carc_Player* player, int points){
    player->points += points;
}

int CPPlayer_add_to_nb_pawns(Carc_Player* player, Carc_Pawn_Type pawn_type, int delta){
    ///If possible, add delta to the number of pawns of type type_pawn for the player player.
    ///If the delta updates to an impossible value, the function does not modify the input player
    ///and return -1.
    int *nb_available=NULL,
        max_nb=-1;
    switch(pawn_type){
        case PAWN_NORMAL:
            nb_available = &(player->nb_pawns[PAWN_NORMAL]);
            max_nb = NB_NORMAL_PAWNS_PER_PLAYER;
            break;
        case PAWN_DOUBLE:
            nb_available = &(player->nb_pawns[PAWN_DOUBLE]);
            max_nb = NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER;
            break;
        case PAWN_ARCHITECT:
            nb_available = &(player->nb_pawns[PAWN_ARCHITECT]);
            max_nb = NB_ARCHITECT_PAWNS_PER_PLAYER;
            break;
        case PAWN_BISHOP:
            nb_available = &(player->nb_pawns[PAWN_BISHOP]);
            max_nb = NB_BISHOP_PAWNS_PER_PLAYER;
            break;
        case PAWN_PIG:
            nb_available = &(player->nb_pawns[PAWN_PIG]);
            max_nb = NB_PIG_PAWNS_PER_PLAYER;
            break;
    }

    if((*nb_available) + delta < 0){
        //fprintf(stdout,"WARNING: Player %d try decreasing nb of pawns of type %d under 0",player->id,pawn_type);
        return -1;
    } else if((*nb_available) + delta > max_nb){
        //fprintf(stdout,"WARNING: Player %d try decreasing nb of pawns of type %d above the maximum (max=%d)",
          //      player->id,pawn_type,max_nb);
        return -1;
    } else{
        (*nb_available) += delta;
        return 0;
    }
}




