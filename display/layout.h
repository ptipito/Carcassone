#ifndef DEF_LAYOUT
#define DEF_LAYOUT
#include "display/carc_display_utils.h"

typedef enum Carc_Layout_Part {LP_MAP, LP_DETAILS} Carc_Layout_Part;
typedef struct Carc_Layout{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *map_texture,
                *details_texture;
    //The window is split into two areas: the playboard (or map) and the details area (player pawns, current tile, etc...)
    SDL_Surface *map_surface,
                *details_surface;
    SDL_Rect map_pos,
             details_pos;
    int map_details_border_size;//Border size between both surfaces. The border is drawn on details_surface
    Carc_Tile_Size tile_size;//Current zoom display
} Carc_Layout;

Carc_Layout* CDL_new_layout();
Carc_Layout* CDL_initialize_game_layout(SDL_Window*);
void CDL_free_layout(Carc_Layout*);
void CDL_blit_on(Carc_Layout_Part, SDL_Surface*, SDL_Rect*, SDL_Rect*, Carc_Layout*);

#endif // DEF_LAYOUT
