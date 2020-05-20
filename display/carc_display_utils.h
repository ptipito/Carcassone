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

typedef enum Carc_Tile_Size {SMALL_TILE, MEDIUM_TILE, BIG_TILE} Carc_Tile_Size;
typedef enum Carc_Utils_View_Type {VT_CONTROL, VT_TILE, VT_PAWN} Carc_Utils_View_Type;

SDL_Rect CDUtils_get_centering_pos(SDL_Surface*, SDL_Surface*);
SDL_Window* CDUtils_initialize_window();
void CDUtils_quit_sdl(SDL_Window*);
int CDUtils_get_tile_size_in_pixels(Carc_Tile_Size);
SDL_Rect CDUtils_get_slot_upper_left(int, int, Carc_Tile_Size);
int CDUtils_pos_in_surface(SDL_Rect, SDL_Surface);
SDL_Surface* CDUtils_get_view(Carc_Utils_View_Type, char*);

#endif // DEF_CARC_DISP_UTILS
