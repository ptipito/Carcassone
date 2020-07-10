#include "details_surface.h"

int CDDetails_get_centering_absciss_pos(Carc_Layout* layout, SDL_Surface* s){
    int pos = int_center(layout->details_surface->w + layout->map_details_border_size, s->w);
    return pos;
}

void CDDetails_show_tile(Carc_Layout* layout, SDL_Surface* tile){
    SDL_Rect pos = CDUtils_get_centering_pos(layout->details_surface,tile);
    pos.x = CDDetails_get_centering_absciss_pos(layout,tile);
    CDDetails_blit_on(tile,NULL,&pos,layout);
    CDDetails_show_tile_turn_controls(layout,pos);
}

void CDDetails_show_tile_turn_controls(Carc_Layout* layout, SDL_Rect tile_pos){
    SDL_Rect pos=tile_pos;
    int tile_size = CDUtils_get_tile_size_in_pixels(layout->tile_size),
        padding_with_tile = 5;

    SDL_Surface* control = CDUtils_get_view(VT_CONTROL,"turn_left.jpg");
    if(control!=NULL){
        pos.x = tile_pos.x - control->w - padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        CDDetails_blit_on(control,NULL,&pos,layout);
    }

    if(control!=NULL){
        control = CDUtils_get_view(VT_CONTROL,"turn_right.jpg");
        pos.x = tile_pos.x + tile_size + padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        CDDetails_blit_on(control,NULL,&pos,layout);
    }

    control = CDUtils_get_view(VT_CONTROL,"turn_180.jpg");
    if(control!=NULL){
        pos.x = tile_pos.x + int_center(tile_size,control->w);
        pos.y = tile_pos.y + tile_size + padding_with_tile;
        CDDetails_blit_on(control,NULL,&pos,layout);
    }
    //Safe to free here already, as the turn controls are used nowhere else and shouldn't be erased later
    SDL_FreeSurface(control);
}

void CDDetails_blit_on(SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Carc_Layout *layout){
    CDL_blit_on(LP_DETAILS,surface,src,dest,layout);
}
//
