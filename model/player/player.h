#ifndef DEF_PLAYER
#define DEF_PLAYER
#include <stdlib.h>
#include <stdio.h>
#include "model/board/tile.h"
#include "model/board/carcassone_playboard.h"
#include "model/carcassone_errors.h"

#define PLAYER_USERNAME_MAX_LEN 20
#define NB_NORMAL_PAWNS_PER_PLAYER 8
#define NB_DOUBLE_WEIGHT_PAWNS_PER_PLAYER 1
#define NB_BISHOP_PAWNS_PER_PLAYER 1
#define NB_PIG_PAWNS_PER_PLAYER 1
#define NB_ARCHITECT_PAWNS_PER_PLAYER 1

typedef enum Carc_Player_ID {PLAYER_1=1, PLAYER_2, PLAYER_3, PLAYER_4, PLAYER_5} Carc_Player_ID;
typedef enum {CPC_BLUE, CPC_YELLOW, CPC_RED, CPC_BLACK, CPC_GREEN} Carc_Player_Color;

typedef struct {
    Carc_Player_ID id;
    Carc_Player_Color color;
    char username[PLAYER_USERNAME_MAX_LEN];
    int points;
    int nb_normal_pawns;
    int nb_architect_pawns;
    int nb_bishop_pawns;
    int nb_double_weighted_pawns;
    int nb_pig_pawns;
} Carc_Player;

Carc_Player* CPP_init_player(Carc_Player_ID id, Carc_Player_Color color);
void CPP_free_player(Carc_Player*);

#endif // DEF_PLAYER
