#include "details_surface.h"

SDL_Rect* CDDetails_translate_x(Carc_Layout layout, SDL_Rect* pos){
    ///Translate the absciss of \pos to have the details area as coordinate referential
    pos->x += layout.details_pos.x + layout.map_details_border_size;
    return pos;
}

SDL_Rect* CDDetails_translate_y(Carc_Layout layout, SDL_Rect* pos){
    ///Translate the absciss of \pos to have the details area as coordinate referential
    pos->y += 0;//Details area starts at the top of the window
    return pos;
}

SDL_Rect* CDDetails_translate(Carc_Layout layout, SDL_Rect* pos){
    CDDetails_translate_x(layout,pos);
    CDDetails_translate_y(layout,pos);
    return pos;
}

int CDDetails_get_centering_absciss_pos(Carc_Layout* layout, SDL_Surface* s){
    int pos = int_center(layout->details_pos.w + layout->map_details_border_size, s->w);//get in the details area
    return pos;
}

SDL_Texture* CDDetails_show_tile(Carc_Layout* layout, SDL_Surface* tile){
    ///Returns a texture created from the tile surface
    SDL_Rect pos = CDUtils_get_centering_pos(layout->details_surface,tile);
    pos.x = CDDetails_get_centering_absciss_pos(layout,tile);
    pos.w = tile->w;
    pos.h = tile->h;
    CDDetails_translate(*layout,&pos);
    SDL_Texture* tile_texture=SDL_CreateTextureFromSurface(layout->renderer,tile);
    SDL_RenderCopy(layout->renderer,tile_texture,NULL,&pos);
    SDL_RenderPresent(layout->renderer);
    CDDetails_show_tile_turn_controls(layout,pos);
    return tile_texture;
}

void CDDetails_show_tile_turn_controls(Carc_Layout* layout, SDL_Rect tile_pos){
    SDL_Rect pos=tile_pos;
    int tile_size = CDUtils_get_tile_size_in_pixels(layout->tile_size),
        padding_with_tile = 5;
    SDL_Renderer* renderer=layout->renderer;

    SDL_Surface* control = CDUtils_get_view(VT_CONTROL,"turn_left.jpg");
    SDL_Texture* control_texture=NULL;
    if(control!=NULL){
        control_texture=SDL_CreateTextureFromSurface(renderer,control);
        pos.x = tile_pos.x - control->w - padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        pos.w = control->w;
        pos.h = control->h;
        SDL_RenderCopy(renderer,control_texture,NULL,&pos);
        SDL_DestroyTexture(control_texture);
    }
    SDL_FreeSurface(control);

    control = CDUtils_get_view(VT_CONTROL,"turn_right.jpg");
    if(control!=NULL){
        control_texture=SDL_CreateTextureFromSurface(renderer,control);
        pos.x = tile_pos.x + tile_size + padding_with_tile;
        pos.y = tile_pos.y - control->h/3;
        pos.w = control->w;
        pos.h = control->h;
        SDL_RenderCopy(renderer,control_texture,NULL,&pos);
        SDL_DestroyTexture(control_texture);
    }
    SDL_FreeSurface(control);

    control = CDUtils_get_view(VT_CONTROL,"turn_180.jpg");
    if(control!=NULL){
        control_texture=SDL_CreateTextureFromSurface(renderer,control);
        pos.x = tile_pos.x + int_center(tile_size,control->w);
        pos.y = tile_pos.y + tile_size + padding_with_tile;
        pos.w = control->w;
        pos.h = control->h;
        SDL_RenderCopy(renderer,control_texture,NULL,&pos);
        SDL_DestroyTexture(control_texture);
    }

    SDL_FreeSurface(control);
}

void CDDetails_blit_on(SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Carc_Layout *layout){
    CDL_blit_on(LP_DETAILS,surface,src,dest,layout);
}

