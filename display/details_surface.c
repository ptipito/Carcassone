#include "details_surface.h"

int details_get_centering_absciss_pos(Layout* layout, SDL_Surface* s){
    int pos = int_center(layout->details_surface->w + layout->map_details_border_size, s->w);
    return pos;
}

void details_show_tile(Layout* layout, SDL_Surface* tile){
    SDL_Rect pos = get_centering_pos(layout->details_surface,tile);
    pos.x = details_get_centering_absciss_pos(layout,tile);
    blit_on(LP_DETAILS,tile,NULL,&pos,layout);
    details_show_tile_turn_controls(layout,pos);
}

void details_show_tile_turn_controls(Layout* layout, SDL_Rect tile_pos){
    SDL_Rect pos=tile_pos;
    int tile_size = get_tile_size_in_pixels(layout->tile_size),
        padding_with_tile = 5;

    SDL_Surface* control = get_view(VT_CONTROL,"turn_left.jpg");
    if(control!=NULL){
        pos.x = tile_pos.x - control->w - padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        blit_on(LP_DETAILS,control,NULL,&pos,layout);
    }

    if(control!=NULL){
        control = get_view(VT_CONTROL,"turn_right.jpg");
        pos.x = tile_pos.x + tile_size + padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        blit_on(LP_DETAILS,control,NULL,&pos,layout);
    }

    control = get_view(VT_CONTROL,"turn_180.jpg");
    if(control!=NULL){
        pos.x = tile_pos.x + int_center(tile_size,control->w);
        pos.y = tile_pos.y + tile_size + padding_with_tile;
        blit_on(LP_DETAILS,control,NULL,&pos,layout);
    }
    //Safe to free here already, as the turn controls are used nowhere else and shouldn't be erased later
    SDL_FreeSurface(control);
}
