#include "control/details_controls.h"

///TODO: replace SDL_Texture by having a filename in tile Struct
int CCD_turn(Carc_Tile_Turn_Type dir, Carc_Layout layout, Carc_Game* game, SDL_Rect* pos){
    int func_res=FUNC_FAIL,
        tile_rot=CBT_turn_type_to_degrees(game->turn.tile->rotation),
        angle = (tile_rot + CBT_turn_type_to_degrees(dir)) % 360;
    //Query and update the model
    if(CBT_turn(game->turn.tile,dir)==FUNC_SUCCESS){
        //Update the view
        SDL_Surface* surface=CDUtils_get_view(VT_TILE,game->playboard->node->node->name);
        SDL_Texture* texture=SDL_CreateTextureFromSurface(layout.renderer,surface);
        SDL_SetRenderTarget(layout.renderer,texture);
        func_res = SDL_RenderCopyEx(layout.renderer,texture,NULL,pos,angle,NULL,SDL_FLIP_NONE);
        SDL_SetRenderTarget(layout.renderer,NULL);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        if(func_res==0)
            return FUNC_SUCCESS;
    }
    return FUNC_FAIL;
}

