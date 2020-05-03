#ifndef DEF_PLAYER
#define DEF_PLAYER
#include <stdlib.h>
#include <stdio.h>
#include "model/board/tile.h"

#define MAX_PAWNS_PER_PLAYER 8

typedef enum {PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4, PLAYER_5} Carc_Player_ID;
typedef enum {BLUE, YELLOW, RED, BLACK, GREEN} Carc_Player_Color;
typedef enum {PAWN_NORMAL, PAWN_BISHOP, PAWN_ARCHITECT, PAWN_DOUBLE, PAWN_PIG} Carc_Pawn_Type;

typedef struct {
    Carc_Pawn_Type type;
    Carc_Player_ID owner;
    int weight;
    //Carc_Tile_Node *played_on;
} Carc_Pawn;


typedef struct {
    Carc_Player_ID id;
    Carc_Player_Color color;
    char username[20];
    Carc_Pawn *pawns[MAX_PAWNS_PER_PLAYER];
} Carc_Player;


#endif // DEF_PLAYER
