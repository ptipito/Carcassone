#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "display/layout.h"
#include "tests/model/test_tile.h"
#include "model/board/carcassone_playboard.h"
#include "model/board/rim.h"
#include "tests/model/test_playboard.h"
/*
This file is not yet the real main and serves as testing for SDL display.
For the time being is there no link between the model files and the functionalities available through the main
*/
void Quit(SDL_Window* window){
    if(window!=NULL){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_VIDEO)!=0){
            printf("err1\n");
        SDL_Log("Error on SDL init: %s\n",SDL_GetError());
    }
    Quit(NULL);

    SDL_Window* window = initialize();
    SDL_Event event;
    int done=0;
    Layout* window_layout = new_layout();
    initialize_game_layout(window,window_layout);

    display_grid(window_layout);

    SDL_Surface *cloister = get_view(VT_TILE,"cloister.jpg");
    if(cloister==NULL)
        printf("cloister not loaded\n");
    SDL_Rect cloister_pos; cloister_pos.x=get_tile_size_in_pixels(MEDIUM_TILE);cloister_pos.y=get_tile_size_in_pixels(MEDIUM_TILE);

    map_insert_tile(cloister,cloister_pos.x,cloister_pos.y,window_layout);
    details_show_tile(window_layout,cloister);
    SDL_UpdateWindowSurface(window_layout->window);
    while(!done){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                done=1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                map_insert_tile(cloister,event.button.x,event.button.y,window_layout);
                break;
            default:
                break;
        }

        SDL_UpdateWindowSurface(window_layout->window);
    }

    SDL_FreeSurface(cloister);
    free_layout(window_layout);
    Quit(NULL);

    return EXIT_SUCCESS;
}


