#include "carc_display_utils.h"

SDL_Rect CDUtils_get_centering_pos(SDL_Surface* screen, SDL_Surface* to_center_on_screen){
    SDL_Rect pos;
    pos.x = int_center(screen->w,to_center_on_screen->w);
    pos.y = int_center(screen->h,to_center_on_screen->h);
    return pos;
}

int CDUtils_get_tile_size_in_pixels(Carc_Tile_Size size){
    int result = 0;
    switch(size){
        case SMALL_TILE:
            result=40;
            break;
        case MEDIUM_TILE:
            result=50;
            break;
        case BIG_TILE:
            result=60;
            break;
    }
    return result;
}

SDL_Window* CDUtils_initialize_window(){
    SDL_Window *window=NULL;
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        SDL_Log("Error on SDL init: %s\n",SDL_GetError());
        carcassone_error_quit(ERR_SDL_INIT,NULL);
    }
    window = SDL_CreateWindow("Carcassone",0,30,1000,500,SDL_WINDOW_OPENGL);
    if(window==NULL){
       SDL_Log("Window couldn't be created: %s\n",SDL_GetError());
       carcassone_error_quit(ERR_WINDOW_INIT,NULL);
    }
    return window;
}

void CDUtils_quit_sdl(SDL_Window* window){
    if(window!=NULL){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

//Returns the upper left corner of the slot containing (x,y)
//(width and height potentially used later for movable map)
SDL_Rect CDUtils_get_slot_upper_left(int x, int y, Carc_Tile_Size tile_size){
    int block_size=CDUtils_get_tile_size_in_pixels(tile_size);
    SDL_Rect result;

    result.x = (x/block_size)*block_size;
    result.y = (y/block_size)*block_size;
    //TODO: handle width and height of result once movable map implemented
    return result;
}

int CDUtils_pos_in_surface(SDL_Rect pos, SDL_Surface surface){
    int result=1;
    if(pos.x < 0 || pos.y < 0)
        result = 0;
    if(pos.x >= surface.w || pos.y >= surface.h)
        result = 0;

    return result;
}

SDL_Surface* CDUtils_get_view(Carc_Utils_View_Type type, char* filename){
    char path_of_tile_view[IMAGE_PATH_MAX_LEN+1] = {'\0'};
    int folder_len=0;
    switch(type){
        case VT_CONTROL:
            strncat(path_of_tile_view,VIEW_CONTROL_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_CONTROL_FOLDER);
            break;
        case VT_TILE:
            strncat(path_of_tile_view,VIEW_TILE_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_TILE_FOLDER);
            break;
        case VT_PAWN:
            strncat(path_of_tile_view,VIEW_PAWN_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_PAWN_FOLDER);
            break;
    }
    strncat(path_of_tile_view,filename,IMAGE_PATH_MAX_LEN-folder_len);

    return IMG_Load(path_of_tile_view);
}


