#ifndef DEF_CARC_GAME
#define DEF_CARC_GAME
#include <stdlib.h>
#include <stdio.h>
#include "model/board/carcassone_playboard.h"
#include "model/board/rim.h"

/*Contains the game representation
   - whole playboard as graph
   - the list of playable nodes
*/
typedef struct Carc_Game{
    Carc_Playboard_Origin* playboard;
    CP_Rim* playable;
} Carc_Game;


#endif // DEF_CARC_GAME
