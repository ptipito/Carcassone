#ifndef DEF_DETAILS_SURFACE
#define DEF_DETAILS_SURFACE
#include "display/layout.h"

#define TURN_SURFACE_PADDING_WITH_TILE 5

typedef enum {CDTT_LEFT, CDTT_RIGHT, CDTT_UPDOWN} Display_Details_Turn_Type;

SDL_Rect* CDDetails_translate_x(Carc_Layout, SDL_Rect*);
SDL_Rect* CDDetails_translate_y(Carc_Layout, SDL_Rect*);
SDL_Rect* CDDetails_translate(Carc_Layout, SDL_Rect*);
int CDDetails_get_centering_absciss_pos(Carc_Layout*, SDL_Surface*);
SDL_Rect CDDetails_show_tile(Carc_Layout*, SDL_Surface*);
SDL_Rect CDDetails_show_turn_control(Carc_Layout*, SDL_Rect, Display_Details_Turn_Type);
void CDDetails_blit_on(SDL_Surface*, SDL_Rect*, SDL_Rect*, Carc_Layout*);

#endif // DEF_DETAILS_SURFACE
