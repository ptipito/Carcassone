#include "layout.h"

Layout new_layout(){
    Layout layout;
    printf("new layout\n");
    printf("w");
    layout.window=NULL;
    printf("d");
    layout.details_surface=NULL;
    printf("m");
    layout.map_surface=NULL;
    printf("t");
    layout.map_pos.x = 0;
    layout.map_pos.y = 0;
    layout.details_pos.x = 0;
    layout.details_pos.y = 0;
    layout.tile_size = MEDIUM_TILE;
    return layout;
}

int get_tile_size_in_pixels(Tile_Size size){
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

SDL_Window* initialize(){
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

void initialize_game_layout(SDL_Window *window, Layout* layout){
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    if(screen==NULL){
        SDL_Log("Window screen couldn't be fetched: %s\n",SDL_GetError());
        carcassone_error_quit(ERR_WINDOW_SURFACE,NULL);
    }
    int screen_w=screen->w, screen_h=screen->h;
    int details_width=3*get_tile_size_in_pixels(MEDIUM_TILE);
    int map_width=screen_w-details_width;
    SDL_Rect pos;
    SDL_Surface *map_surface=NULL,
                *details_surface=NULL;

    map_surface = SDL_CreateRGBSurface(0,map_width,screen_h,32,0,0,0,0);
    details_surface = SDL_CreateRGBSurface(0,details_width,screen_h,32,0,0,0,0);
    SDL_FillRect(map_surface,NULL,SDL_MapRGB(map_surface->format,255,255,255));
    SDL_FillRect(details_surface,NULL,SDL_MapRGB(details_surface->format,0,0,0));
    pos.x=0;pos.y=0;
    SDL_BlitSurface(map_surface,NULL,screen,&pos);
    pos.y=map_width;
    SDL_BlitSurface(details_surface,NULL,screen,&pos);
    SDL_UpdateWindowSurface(window);
    printf("w");
    layout->window=window;
    layout->map_surface=map_surface;
    layout->details_surface=details_surface;
    layout->details_pos.x = screen_w - details_width;
    printf("layout: \n\twindow %p\n\tmap %p\n\tdetails %p\n\ttile size %d\n",layout->window,layout->map_surface,layout->details_surface,layout->tile_size);
    SDL_FreeSurface(screen);
}

void free_layout(Layout *layout){
    if(layout!=NULL){
        SDL_FreeSurface(layout->map_surface);
        SDL_FreeSurface(layout->details_surface);
        SDL_DestroyWindow(layout->window);
        free(layout);
    }
}

void display_grid(Layout *layout){
    int tile_size=get_tile_size_in_pixels(layout->tile_size);
    int width = layout->map_surface->w;
    int height = layout->map_surface->h;
    int i=0, cur_coord=0;

    SDL_Surface *vertical_line = SDL_CreateRGBSurface(0,1,height,32,0,0,0,0);
    SDL_Surface *horizontal_line = SDL_CreateRGBSurface(0,width,1,32,0,0,0,0);
    SDL_FillRect(vertical_line,NULL,SDL_MapRGB(vertical_line->format,0,0,0));
    SDL_FillRect(horizontal_line,NULL,SDL_MapRGB(horizontal_line->format,0,0,0));
    SDL_Rect line_pos;
    line_pos.y=0;

    //Create & Display vertical lines
    for(i=1;cur_coord<width;i++){
        cur_coord = i*tile_size;
        line_pos.x=cur_coord;
        blit_on(LP_MAP,vertical_line,NULL,&line_pos,layout);
    }
    //Create & Display hoizontal lines
    line_pos.x=0;
    cur_coord=tile_size;
    for(i=1;cur_coord<height;i++){
        cur_coord = i*tile_size;
        line_pos.y=cur_coord;
        printf("line pos %d %d\n",line_pos.x,line_pos.y);
        blit_on(LP_MAP,horizontal_line,NULL,&line_pos,layout);
    }

    SDL_UpdateWindowSurface(layout->window);

    //SDL_FreeSurface(screen);
    SDL_FreeSurface(horizontal_line);
    SDL_FreeSurface(vertical_line);
}

//Function to blit the surface on the layout surface at the dest coordinate
void blit_on(Layout_Part part, SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Layout *layout){
    SDL_Surface *screen=NULL,
                *window_screen=NULL;
    SDL_Rect screen_pos;
    switch(part){
        case LP_MAP:
            screen = layout->map_surface;
            screen_pos = layout->map_pos;
            break;
        case LP_DETAILS:
            printf("screen to details layout\n");
            screen = layout->details_surface;
            screen_pos = layout->details_pos;
            break;
    }

    SDL_BlitSurface(surface,src,screen,dest);
    //Blit the modified layout surface on the window for the modif to be visible when updating window surface
    window_screen = SDL_GetWindowSurface(layout->window);
    SDL_BlitSurface(screen,NULL,window_screen,&screen_pos);

    SDL_FreeSurface(window_screen);

}

//Functions for blit on the map/details to be prefixed with map/details
void map_insert_tile(SDL_Surface *surface, int x, int y, Layout *layout){
    SDL_Rect pos=get_slot_upper_left(x,y,layout->tile_size);
    if(pos_in_surface(pos, *(layout->map_surface)))
        blit_on(LP_MAP,surface,NULL,&pos,layout);
}

//Returns the upper left corner of the slot containing (x,y)
//(width and height potentially used later for movable map)
SDL_Rect get_slot_upper_left(int x, int y, Tile_Size tile_size){
    int block_size=get_tile_size_in_pixels(tile_size);
    SDL_Rect result;

    result.x = (x/block_size)*block_size;
    result.y = (y/block_size)*block_size;
    //TODO: handle width and hight of result once movable map implemented
    return result;
}

int pos_in_surface(SDL_Rect pos, SDL_Surface surface){
    int result=1;
    if(pos.x < 0 || pos.y < 0)
        result = 0;
    if(pos.x >= surface.w || pos.y >= surface.h)
        result = 0;

    return result;
}
