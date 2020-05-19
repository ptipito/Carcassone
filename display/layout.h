#ifndef DEF_LAYOUT
#define DEF_LAYOUT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "model/carcassone_errors.h"
#include "utils.h"
#define IMAGE_PATH_MAX_LEN 60
#define VIEW_TILE_FOLDER "ressources/images/"
#define VIEW_CONTROL_FOLDER "ressources/images/"
#define VIEW_PAWN_FOLDER "ressources/images/"

typedef enum Tile_Size {SMALL_TILE, MEDIUM_TILE, BIG_TILE} Tile_Size;
typedef enum View_Type {VT_CONTROL, VT_TILE, VT_PAWN} View_Type;
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
int get_centering_absciss_pos(SDL_Surface*, SDL_Surface*);
int get_centering_ordinate_pos(SDL_Surface*, SDL_Surface*);
SDL_Rect get_centering_pos(SDL_Surface*, SDL_Surface*);
SDL_Window* initialize();
int get_tile_size_in_pixels(Tile_Size);
void initialize_game_layout(SDL_Window*, Layout*);
void display_grid(Layout*);
void free_layout(Layout*);
void blit_on(Layout_Part, SDL_Surface*, SDL_Rect*, SDL_Rect*, Layout*);
SDL_Rect get_slot_upper_left(int, int, Tile_Size);
void map_insert_tile(SDL_Surface*, int, int, Layout*);
int pos_in_surface(SDL_Rect, SDL_Surface);
SDL_Surface* get_view(View_Type, char*);
void details_show_tile(Layout*, SDL_Surface*);
void details_show_tile_turn_controls(Layout* layout, SDL_Rect tile_pos);

#endif // DEF_LAYOUT
