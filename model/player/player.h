#ifndef DEF_PLAYER
#define DEF_PLAYER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "model/carcassone_errors.h"

#define PLAYER_USERNAME_MAX_LEN 20
#define NB_MAX_PLAYERS 5
#define NB_NORMAL_PAWNS_PER_PLAYER 8
#define NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER 1
#define NB_BISHOP_PAWNS_PER_PLAYER 1
#define NB_PIG_PAWNS_PER_PLAYER 1
#define NB_ARCHITECT_PAWNS_PER_PLAYER 1
#define NB_PAWN_TYPES_PER_PLAYER 5

typedef enum Carc_Player_ID {PLAYER_1=1, PLAYER_2, PLAYER_3, PLAYER_4, PLAYER_5} Carc_Player_ID;
typedef enum {CPC_BLUE, CPC_YELLOW, CPC_RED, CPC_BLACK, CPC_GREEN} Carc_Player_Color;
typedef enum Carc_Pawn_Type {PAWN_NORMAL, PAWN_BISHOP, PAWN_ARCHITECT, PAWN_DOUBLE, PAWN_PIG} Carc_Pawn_Type;

typedef struct {
    Carc_Player_ID id;
    Carc_Player_Color color;
    char username[PLAYER_USERNAME_MAX_LEN];
    int points;
    int nb_pawns[NB_PAWN_TYPES_PER_PLAYER];
} Carc_Player;

Carc_Player* CPPlayer_init_player(Carc_Player_ID id, Carc_Player_Color color);
void CPPlayer_free_player(Carc_Player*);
int CPPlayer_add_points(Carc_Player*, int);
int CPPlayer_add_to_nb_pawns(Carc_Player* player, Carc_Pawn_Type pawn_type, int delta);
int CPPlayer_get_nb_pawns(Carc_Player*, Carc_Pawn_Type);
int CPPlayer_can_play_pawn(Carc_Player*, Carc_Pawn_Type);

#endif // DEF_PLAYER
