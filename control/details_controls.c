#include "control/details_controls.h"


int CCD_turn_right(Carc_Layout layout, Carc_Game* game, SDL_Texture* texture, SDL_Rect* pos){
    int func_res=-1,
        tile_rot=CBT_turn_type_to_degrees(game->turn.tile->rotation),
        angle = (tile_rot + 90) % 360;
    //Query the model
    ///TO_TEST model update
    CBT_turn(game->turn.tile,CTTT_RIGHT);
    //Update the view
    SDL_SetRenderTarget(layout.renderer,texture);
    func_res = SDL_RenderCopyEx(layout.renderer,texture,NULL,pos,angle,NULL,SDL_FLIP_NONE);
    SDL_SetRenderTarget(layout.renderer,NULL);
    if(func_res==0)
        return FUNC_SUCCESS;
    return FUNC_FAIL;
}

int CCD_turn_left(Carc_Layout layout, Carc_Game* game, SDL_Texture* texture, SDL_Rect* pos){
    ///TO_TEST model update
    int func_res=-1,
        tile_rot=CBT_turn_type_to_degrees(game->turn.tile->rotation),
        angle = (tile_rot - 90) % 360;
    //Query the model
    CBT_turn(game->turn.tile,CTTT_LEFT);
    //Update the view
    SDL_SetRenderTarget(layout.renderer,texture);
    func_res = SDL_RenderCopyEx(layout.renderer,texture,NULL,pos,angle,NULL,SDL_FLIP_NONE);
    SDL_SetRenderTarget(layout.renderer,NULL);
    if(func_res==0)
        return FUNC_SUCCESS;
    return FUNC_FAIL;
}

int CCD_turn_180(Carc_Layout layout, Carc_Game* game, SDL_Texture* texture, SDL_Rect* pos){
    int func_res=-1,
        tile_rot=CBT_turn_type_to_degrees(game->turn.tile->rotation),
        angle = (tile_rot + 180) % 360;
    //Query the model
    ///TO_TEST model update
    CBT_turn(game->turn.tile,CTTT_UPDOWN);
    //Update the view
    SDL_SetRenderTarget(layout.renderer,texture);
    func_res = SDL_RenderCopyEx(layout.renderer,texture,NULL,pos,angle,NULL,SDL_FLIP_NONE);
    SDL_SetRenderTarget(layout.renderer,NULL);
    if(func_res==0)
        return FUNC_SUCCESS;
    return FUNC_FAIL;
}
