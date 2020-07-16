#include "app.h"

Carc_App* Carc_App_init(){
    Carc_App* Carcassone = malloc(sizeof(*Carcassone));
    if(Carcassone==NULL){
        fprintf(stderr,"ERROR: Couldn't allocate memory for the app initialization\n");
        exit(ERR_MEM_ALLOC);
    }
    //Init controls
    int i;
    for(i=0;i<APP_NR_CONTROLS;i++){
        Carcassone->controls[i].id = i;
        Carcassone->controls[i].pos = init_rect(0,0,0,0);
    }

    //Init SDL and display
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        SDL_Log("Error on SDL init: %s\n",SDL_GetError());
        CDUtils_quit_sdl(NULL);
        exit(ERR_SDL_INIT);
    }
    SDL_Window* window = CDUtils_initialize_window();
    Carcassone->window_layout = CDL_initialize_game_layout(window);///TODO: Modify to give controls as inputs to be filled

    //Init game
    char* tile_path=CBT_get_tile_file_path(APP_START_TILE);
    Carcassone->game = CGG_initiate_game(tile_path,2);//init with 2 players and change on user interaction
    free(tile_path);

    return Carcassone;
}

void Carc_App_free(Carc_App* app){
    if(pointer_is_not_null(app,0)){
        CGG_free_game(app->game);
        CDL_free_layout(app->window_layout);
    }
    free(app);
}

void Carc_App_end(Carc_App* app){
    Carc_App_free(app);
    SDL_Quit();
}

int Carc_App_run(Carc_App* app){
    SDL_Event event;
    int done=0;
    Carc_Layout* display = app->window_layout;
    Carc_Game* game = app->game;
    int tile_size = CDUtils_get_tile_size_in_pixels(display->tile_size);

    CDMap_display_grid(display);

    SDL_Surface *cloister = CDUtils_get_view(VT_TILE,"cloister.jpg");
    if(cloister==NULL)
        printf("cloister not loaded\n");
    char* cloister_path=CBT_get_tile_file_path("cloister_path.txt");
    app->game->turn.tile = CBT_new_tile_from_file(cloister_path);
    free(cloister_path);

    SDL_Texture* cur_tile_tex=CDDetails_show_tile(display,cloister);
    SDL_Rect cur_tile_pos = init_rect(display->map_pos.w + display->details_pos.w/2-tile_size/2,
                                      display->details_pos.h/2-tile_size/2,
                                      tile_size,
                                      tile_size
                                      );
    int x=0, y=0, cur_angle=0;
    while(!done){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                if(x < display->map_pos.w){
                    CDMap_insert_tile(cloister,x,y,display,cur_angle);
                } else{
                    x %= display->map_pos.w;
                    if(x >= display->details_pos.w/2-tile_size/2 - 20 - 5
                       && x<= display->details_pos.w/2-tile_size/2 - 5
                       && y >= display->details_pos.h/2 - tile_size/2 - 25/3
                       && y <= display->details_pos.h/2 - tile_size/2 - 25/3 +25
                       ){
                        cur_angle += CCD_turn_left(*display,game,cur_tile_tex,&cur_tile_pos,cur_angle);
                       }
                    if(x >= display->details_pos.w/2+tile_size/2 + 5
                       && x <= display->details_pos.w/2+tile_size/2 + 5 +20
                       && y >= display->details_pos.h/2 - tile_size/2 - 25/3
                       && y <= display->details_pos.h/2 - tile_size/2 - 25/3 +25
                       ){
                        cur_angle += CCD_turn_right(*display,game,cur_tile_tex,&cur_tile_pos,cur_angle);
                       }
                    if(x >= display->details_pos.w/2 - 20/2
                       && x <= display->details_pos.w/2+20/2
                       && y >= display->details_pos.h/2 + tile_size/2 + 5
                       && y <= display->details_pos.h/2 + tile_size/2 + 5 +20
                       ){
                        cur_angle += CCD_turn_180(*display,game,cur_tile_tex,&cur_tile_pos,cur_angle);
                       }
                }
                break;
            default:
                break;
        }
        SDL_RenderPresent(display->renderer);
    }

    SDL_FreeSurface(cloister);
    SDL_DestroyTexture(cur_tile_tex);
    Carc_App_end(app);
    return FUNC_SUCCESS;
}
