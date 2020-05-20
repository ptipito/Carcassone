#ifndef DEF_CARC_DISP_UTILS
#define DEF_CARC_DISP_UTILS
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

SDL_Rect get_centering_pos(SDL_Surface*, SDL_Surface*);
SDL_Window* initialize_window();
void Quit(SDL_Window*);
int get_tile_size_in_pixels(Tile_Size);
SDL_Rect get_slot_upper_left(int, int, Tile_Size);
int pos_in_surface(SDL_Rect, SDL_Surface);
SDL_Surface* get_view(View_Type, char*);

#endif // DEF_CARC_DISP_UTILS
