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
        Carcassone->controls[i].pos = CDUtils_init_rect(0,0,0,0);
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
    Carcassone->game = CGG_initiate_game(APP_START_TILE,2);//init with 2 players and change on user interaction

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
    SDL_Rect mouse_pos;

    CDMap_display_grid(display);

    SDL_Surface *cloister = CDUtils_get_view(VT_TILE,"cloister_path");
    if(cloister==NULL)
        printf("cloister not loaded\n");

    app->game->turn.tile = CBT_new_tile_from_file("cloister_path.txt");

    SDL_Rect cur_tile_pos = CDDetails_show_tile(display,cloister);
    app->controls[CCDETAILS_CUR_TILE].pos = cur_tile_pos;
    app->controls[CCDETAILS_TURN_LEFT].pos = CDDetails_show_turn_control(display,cur_tile_pos,CDTT_LEFT);
    app->controls[CCDETAILS_TURN_RIGHT].pos = CDDetails_show_turn_control(display,cur_tile_pos,CDTT_RIGHT);
    app->controls[CCDETAILS_TURN_180].pos = CDDetails_show_turn_control(display,cur_tile_pos,CDTT_UPDOWN);

    int x=0, y=0;
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
                mouse_pos = CDUtils_init_rect(x,y,0,0);
                if(CDUtils_pos_in_rect(mouse_pos,display->map_pos)){
                    CDMap_insert_tile(cloister,x,y,display,CBT_turn_type_to_degrees(game->turn.tile->rotation));
                } else{
                    x %= display->map_pos.w;
                    if(CDUtils_pos_in_rect(mouse_pos,app->controls[CCDETAILS_TURN_LEFT].pos)){
                        CCD_turn(CTTT_LEFT,*display,game,&cur_tile_pos);
                       }
                    if(CDUtils_pos_in_rect(mouse_pos,app->controls[CCDETAILS_TURN_RIGHT].pos)){
                        CCD_turn(CTTT_RIGHT,*display,game,&cur_tile_pos);
                       }
                    if(CDUtils_pos_in_rect(mouse_pos,app->controls[CCDETAILS_TURN_180].pos)){
                        CCD_turn(CTTT_UPDOWN,*display,game,&cur_tile_pos);
                       }
                }
                break;
            default:
                break;
        }
        SDL_RenderPresent(display->renderer);
    }

    SDL_FreeSurface(cloister);
    Carc_App_end(app);
    return FUNC_SUCCESS;
}
