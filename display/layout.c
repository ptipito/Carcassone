#include "layout.h"

Carc_Layout* CDL_new_layout(){
    Carc_Layout* layout = malloc(sizeof(*layout));
    layout->window = NULL;
    layout->renderer = NULL;
    layout->details_texture = NULL;
    layout->map_texture = NULL;
    layout->details_surface = NULL;
    layout->map_surface = NULL;
    layout->map_details_border_size = 2;
    layout->map_pos.x = 0;
    layout->map_pos.y = 0;
    layout->details_pos.x = 0;
    layout->details_pos.y = 0;
    layout->tile_size = MEDIUM_TILE;
    return layout;
}

Carc_Layout* CDL_initialize_game_layout(SDL_Window *window){
    Carc_Layout* layout=CDL_new_layout();
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    if(screen==NULL){
        SDL_Log("Window screen couldn't be fetched: %s\n",SDL_GetError());
        carcassone_error_quit(ERR_WINDOW_SURFACE,NULL);
    }
    int screen_w=screen->w, screen_h=screen->h;
    int details_width=4*CDUtils_get_tile_size_in_pixels(MEDIUM_TILE);
    int map_width=screen_w-details_width;
    SDL_Rect pos, border_pos;
    SDL_Surface *map_surface=NULL,
                *details_surface=NULL;
    SDL_Texture *map_texture=NULL,
                *details_texture=NULL;

    //Create map and details surfaces
    map_surface = SDL_CreateRGBSurface(0,map_width,screen_h,32,0,0,0,0);
    SDL_FillRect(map_surface,NULL,SDL_MapRGB(map_surface->format,255,255,255));
    details_surface = SDL_CreateRGBSurface(0,details_width,screen_h,32,0,0,0,0);
    SDL_FillRect(details_surface,NULL,SDL_MapRGB(details_surface->format,250,250,250));
    //Draw separation border between map and details
    border_pos.x = 0;
    border_pos.y = 0;
    border_pos.w = layout->map_details_border_size;
    border_pos.h = screen_h;
    SDL_FillRect(details_surface,&border_pos,SDL_MapRGB(details_surface->format,0,0,0));
    SDL_UpdateWindowSurface(window);
    layout->window = window;
    layout->map_surface = map_surface;
    layout->details_surface = details_surface;
    ///NEW CODE
    layout->renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_TARGETTEXTURE);
    map_texture = SDL_CreateTextureFromSurface(layout->renderer,map_surface);
    details_texture = SDL_CreateTextureFromSurface(layout->renderer,details_surface);
    layout->map_texture = map_texture;
    layout->details_texture = details_texture;
    layout->map_pos.x = 0;
    layout->map_pos.y = 0;
    layout->map_pos.w = map_width;
    layout->map_pos.h = map_surface->h;
    layout->details_pos.x = layout->map_pos.w;
    layout->details_pos.y = 0;
    layout->details_pos.w = details_width;
    layout->details_pos.h = map_surface->h;

    //Draw
    SDL_SetRenderDrawColor(layout->renderer,255,255,255,0);
    SDL_RenderClear(layout->renderer);
    SDL_RenderCopy(layout->renderer,layout->map_texture,NULL,&(layout->map_pos));
    SDL_RenderCopy(layout->renderer,layout->details_texture,NULL,&(layout->details_pos));
    SDL_RenderPresent(layout->renderer);

    return layout;
}

void CDL_free_layout(Carc_Layout *layout){
    if(layout!=NULL){
        SDL_FreeSurface(layout->map_surface);
        SDL_FreeSurface(layout->details_surface);
        SDL_DestroyTexture(layout->map_texture);
        SDL_DestroyTexture(layout->details_texture);
        SDL_DestroyRenderer(layout->renderer);
        SDL_DestroyWindow(layout->window);
        free(layout);
    }
}

//Function to blit the surface on the layout surface at the dest coordinate
void CDL_blit_on(Carc_Layout_Part part, SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Carc_Layout *layout){
    SDL_Surface *screen=NULL,
                *window_screen=NULL;
    SDL_Rect screen_pos;
    switch(part){
        case LP_MAP:
            screen = layout->map_surface;
            screen_pos = layout->map_pos;
            break;
        case LP_DETAILS:
            screen = layout->details_surface;
            screen_pos = layout->details_pos;
            break;
    }

    SDL_BlitSurface(surface,src,screen,dest);
    //Blit the modified layout surface on the window for the modif to be visible when updating window surface
    window_screen = SDL_GetWindowSurface(layout->window);
    SDL_BlitSurface(screen,NULL,window_screen,&screen_pos);
}

