#include "carcassone_errors.h"

int carcassone_error_quit(Carcassone_Error err, SDL_Window *window){
    switch(err){
        case ERR_SDL_INIT:
            SDL_Quit();
            break;
        case ERR_WINDOW_INIT:
            SDL_Quit();
            break;
        case ERR_WINDOW_SURFACE:
            SDL_DestroyWindow(window);
            SDL_Quit();
            break;
        default:
            SDL_Quit();
            break;
    }
    exit(err);
}
