#ifndef DEF_LAYOUT
#define DEF_LAYOUT
#include "display/carc_display_utils.h"

typedef enum Layout_Part {LP_MAP, LP_DETAILS} Layout_Part;
typedef struct Layout{
    SDL_Window *window;
    //The window is split into two areas: the playboard (or map) and the details area (player pawns, current tile, etc...)
    SDL_Surface *map_surface,
                *details_surface;
    int map_details_border_size;
    SDL_Rect map_pos,//position of map_surface in the window
             details_pos;//position of the details surface in the window
    Tile_Size tile_size;//Current zoom display
} Layout;

Layout* new_layout();
void initialize_game_layout(SDL_Window*, Layout*);
void free_layout(Layout*);
void blit_on(Layout_Part, SDL_Surface*, SDL_Rect*, SDL_Rect*, Layout*);

#endif // DEF_LAYOUT
