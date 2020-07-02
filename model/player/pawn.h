#ifndef DEF_CARC_PAWN
#define DEF_CARC_PAWN
#include "model/player/player.h"
#include "utils.h"


typedef struct {
    Carc_Pawn_Type type;
    Carc_Player* owner;
    int weight;
} Carc_Pawn;


int CPPawn_get_weight_from_type(Carc_Pawn_Type);
Carc_Pawn* CPPawn_new_pawn(Carc_Player*, Carc_Pawn_Type);
void CPPawn_free_pawn(Carc_Pawn*);
int CPPawn_send_back(Carc_Pawn*);
int CPPawn_play(Carc_Pawn*);

#endif // DEF_CARC_PAWN
