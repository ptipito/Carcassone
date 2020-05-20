#ifndef DEF_DETAILS_SURFACE
#define DEF_DETAILS_SURFACE
#include "display/layout.h"

int CDDetails_get_centering_absciss_pos(Carc_Layout*, SDL_Surface*);
void CDDetails_show_tile(Carc_Layout*, SDL_Surface*);
void CDDetails_show_tile_turn_controls(Carc_Layout*, SDL_Rect);
void CDDetails_blit_on(SDL_Surface*, SDL_Rect*, SDL_Rect*, Carc_Layout*);

#endif // DEF_DETAILS_SURFACE
