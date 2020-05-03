#ifndef DEF_LAYOUT
#define DEF_LAYOUT
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "model/carcassone_errors.h"

typedef enum Tile_Size {SMALL_TILE, MEDIUM_TILE, BIG_TILE} Tile_Size;
typedef struct Layout{
    SDL_Window *window;
    SDL_Surface *map_surface,
                *details_surface;
    SDL_Rect map_pos,
             details_pos;
    Tile_Size tile_size;
} Layout;
typedef enum Layout_Part {LP_MAP, LP_DETAILS} Layout_Part;

Layout new_layout();
int get_tile_size_in_pixels(Tile_Size);
void initialize_game_layout(SDL_Window*, Layout*);
void display_grid(Layout*);
void free_layout(Layout*);
void blit_on(Layout_Part, SDL_Surface*, SDL_Rect*, SDL_Rect*, Layout*);
SDL_Rect get_slot_upper_left(int, int, Tile_Size);
void map_insert_tile(SDL_Surface*, int, int, Layout*);
int pos_in_surface(SDL_Rect, SDL_Surface);

#endif // DEF_LAYOUT
