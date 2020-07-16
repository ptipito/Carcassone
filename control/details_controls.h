#ifndef DEF_CONTROL_DETAILS
#define DEF_CONTROL_DETAILS

#include <stdlib.h>
#include <stdio.h>
#include "display/layout.h"
#include "control/controls.h"

int CCD_turn_right(Carc_Layout, SDL_Texture*, SDL_Rect*, int);
int CCD_turn_left(Carc_Layout, SDL_Texture*, SDL_Rect*, int);
int CCD_turn_180(Carc_Layout, SDL_Texture*, SDL_Rect*, int);

#endif // DEF_CONTROL_DETAILS
