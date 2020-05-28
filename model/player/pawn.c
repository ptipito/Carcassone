#include "model/player/pawn.h"

int CPPawn_get_weight_from_type(Carc_Pawn_Type type){
    int weight=1;
    switch(type){
        case PAWN_NORMAL:
            weight = 1;
            break;
        case PAWN_DOUBLE:
            weight = 2;
            break;
        default:
            weight = 0;
            break;
    }
    return weight;
}

Carc_Pawn* CPPawn_new_pawn(Carc_Player* player, Carc_Pawn_Type type){
    if(player==NULL){
        fprintf(stderr,"ERROR: Can't create pawn for NULL player\n");
        return NULL;
    }
    Carc_Pawn* pawn = malloc(sizeof(*pawn));
    if(pawn==NULL){
        fprintf(stderr,"ERROR: couldn't allocate memory for pawn of type %d for player %d\n",
                        type,player->id);
        return NULL;
    }
    pawn->owner = player;
    pawn->type = type;
    pawn->weight = CPPawn_get_weight_from_type(type);

    return pawn;
}

void CPPawn_free_pawn(Carc_Pawn* pawn){
    free(pawn);
}

int CPPawn_send_back(Carc_Pawn* pawn){
    ///Send a pawn back into its player's hand
    int res=0;
    if(pawn==NULL){
        fprintf(stderr, "ERROR: cannot send NULL pawnd back to a player \n");
        res = -1;
    } else{
        res = CPPlayer_add_to_nb_pawns(pawn->owner,pawn->type,1);
        if(res!=0){
            fprintf(stderr, "ERROR: cannot send pawn of type %d back to player %d (max pawns already reached)\n",
                            pawn->type,pawn->owner->id);
            res = -1;
        } else{
            CPPawn_free_pawn(pawn);
        }
    }
    return res;
}

int CPPawn_play(Carc_Pawn* pawn){
    int res=-1;
    if(pawn==NULL){
        fprintf(stderr,"ERROR: cannot play a pawn NULL. (CPPawn_play)\n");
        return res;
    }
    if(CPPlayer_get_nb_pawns(pawn->owner,pawn->type)>0){
        CPPlayer_add_to_nb_pawns(pawn->owner,pawn->type,-1);
        res = 0;
    } else {
        fprintf(stderr,"ERROR: cannot play a pawn of type %d for player %d, because the player does not have any available pawn of type %d. (CPPawn_play)\n"
                , pawn->type,pawn->owner->id,pawn->type);
    }
    return res;
}
