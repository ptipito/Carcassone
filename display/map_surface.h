#ifndef DEF_MAP_SURFACE
#define DEF_MAP_SURFACE
#include "display/layout.h"

void CDMap_display_grid(Carc_Layout*);
void CDMap_insert_tile(SDL_Surface*, int, int, Carc_Layout*);
void CDMap_blit_on(SDL_Surface*, SDL_Rect*, SDL_Rect*, Carc_Layout*);

#endif // DEF_MAP_SURFACE

