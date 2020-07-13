#ifndef DEF_DETAILS_SURFACE
#define DEF_DETAILS_SURFACE
#include "display/layout.h"

SDL_Rect* CDDetails_translate_x(Carc_Layout, SDL_Rect*);
SDL_Rect* CDDetails_translate_y(Carc_Layout, SDL_Rect*);
SDL_Rect* CDDetails_translate(Carc_Layout, SDL_Rect*);
int CDDetails_get_centering_absciss_pos(Carc_Layout*, SDL_Surface*);
SDL_Texture* CDDetails_show_tile(Carc_Layout*, SDL_Surface*);
void CDDetails_show_tile_turn_controls(Carc_Layout*, SDL_Rect);
void CDDetails_blit_on(SDL_Surface*, SDL_Rect*, SDL_Rect*, Carc_Layout*);

#endif // DEF_DETAILS_SURFACE
