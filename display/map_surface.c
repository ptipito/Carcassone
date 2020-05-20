#include "map_surface.h"

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
        blit_on(LP_MAP,horizontal_line,NULL,&line_pos,layout);
    }

    SDL_UpdateWindowSurface(layout->window);

    //SDL_FreeSurface(screen);
    SDL_FreeSurface(horizontal_line);
    SDL_FreeSurface(vertical_line);
}

void map_insert_tile(SDL_Surface *surface, int x, int y, Layout *layout){
    SDL_Rect pos=get_slot_upper_left(x,y,layout->tile_size);
    if(pos_in_surface(pos, *(layout->map_surface)))
        blit_on(LP_MAP,surface,NULL,&pos,layout);
}

