#include "map_surface.h"

void CDMap_display_grid(Carc_Layout *layout){
    int tile_size=CDUtils_get_tile_size_in_pixels(layout->tile_size);
    int width = layout->map_surface->w;
    int height = layout->map_surface->h;
    int start_x=layout->map_pos.x;
    int start_y=layout->map_pos.y;
    int i=0, cur_coord=0;

    SDL_SetRenderTarget(layout->renderer,layout->map_texture);

    SDL_SetRenderDrawColor(layout->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    //Create & Display vertical lines
    for(i=0;cur_coord<width;i++){
        cur_coord = i*tile_size;
        SDL_RenderDrawLine(layout->renderer,cur_coord,start_y,cur_coord,height);
    }

    //Create & Display horizontal lines
    cur_coord=tile_size;
    for(i=0;cur_coord<height;i++){
        cur_coord = i*tile_size;
        SDL_RenderDrawLine(layout->renderer,start_x,cur_coord,start_x+width,cur_coord);
    }

    SDL_RenderPresent(layout->renderer);
}

void CDMap_insert_tile(SDL_Surface *surface, int x, int y, Carc_Layout *layout, int rot_angle){
    SDL_Rect pos=CDUtils_get_slot_upper_left(x,y,layout->tile_size);
    pos.w = surface->w;
    pos.h = surface->h;
    surface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(layout->renderer,surface);
    if(CDUtils_pos_in_rect(pos, layout->map_pos)){
        SDL_SetRenderTarget(layout->renderer,layout->map_texture);
        SDL_RenderCopyEx(layout->renderer, texture, NULL, &pos,rot_angle,NULL,SDL_FLIP_NONE);
    }
    SDL_RenderPresent(layout->renderer);
    SDL_DestroyTexture(texture);
}

void CDMap_blit_on(SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Carc_Layout *layout){
    CDL_blit_on(LP_MAP,surface,src,dest,layout);
}
