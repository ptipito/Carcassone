#include "map_surface.h"

void CDMap_display_grid(Carc_Layout *layout){
    int tile_size=CDUtils_get_tile_size_in_pixels(layout->tile_size);
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
        CDMap_blit_on(vertical_line,NULL,&line_pos,layout);
    }
    //Create & Display hoizontal lines
    line_pos.x=0;
    cur_coord=tile_size;
    for(i=1;cur_coord<height;i++){
        cur_coord = i*tile_size;
        line_pos.y=cur_coord;
        CDMap_blit_on(horizontal_line,NULL,&line_pos,layout);
    }

    SDL_UpdateWindowSurface(layout->window);

    //SDL_FreeSurface(screen);
    SDL_FreeSurface(horizontal_line);
    SDL_FreeSurface(vertical_line);
}

void CDMap_insert_tile(SDL_Surface *surface, int x, int y, Carc_Layout *layout){
    SDL_Rect pos=CDUtils_get_slot_upper_left(x,y,layout->tile_size);
    if(CDUtils_pos_in_surface(pos, *(layout->map_surface)))
        CDMap_blit_on(surface,NULL,&pos,layout);
}

void CDMap_blit_on(SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Carc_Layout *layout){
    CDL_blit_on(LP_MAP,surface,src,dest,layout);
}
