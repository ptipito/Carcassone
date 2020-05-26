#ifndef DEF_CARC_ERROR
#define DEF_CARC_ERROR
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef enum Carcassone_Error {
    ERR_SDL_INIT,
    ERR_WINDOW_INIT,
    ERR_WINDOW_SURFACE,
    ERR_MEM_ALLOC,
    ERR_STR_BUFFER_LENGTH,
    ERR_NB_PLAYERS
} Carcassone_Error;


int carcassone_error_quit(Carcassone_Error,SDL_Window*);
#endif // DEF_CARC_ERROR

