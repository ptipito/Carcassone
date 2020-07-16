#ifndef DEF_CARC_APP
#define DEF_CARC_APP

#include <stdlib.h>
#include <stdio.h>
#include "model/game/game.h"
#include "display/layout.h"
#include "display/details_surface.h"
#include "display/map_surface.h"
#include "control/details_controls.h"

#define APP_START_TILE "tile1.txt"

#define APP_NR_CONTROLS 8 //Correspond of the nr of values for the below enum
typedef enum {
    CCDETAILS_TURN_RIGHT, CCDETAILS_TURN_LEFT, CCDETAILS_TURN_180,
    CCDETAILS_CUR_TILE,
    CCDETAILS_PAWN_NORMAL,
    CCDETAILS_PAWN_DOUBLE,
    CCDETAILS_PAWN_ARCHITECT,
    CCDETAILS_PAWN_PIG
} Carc_Controls;

typedef struct {
    Carc_Controls id;
    SDL_Rect pos;//Contains pos, width and height of the control
} Carc_App_Control;

typedef struct {
    Carc_App_Control controls[APP_NR_CONTROLS];
    Carc_Game* game;
    Carc_Layout* window_layout;
} Carc_App;



Carc_App* Carc_App_init();
void Carc_App_free(Carc_App*);
void Carc_App_end(Carc_App*);
int Carc_App_run(Carc_App*);

#endif // DEF_CARC_APP
