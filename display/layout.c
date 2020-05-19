#include "layout.h"

Layout* new_layout(){
    Layout* layout = malloc(sizeof(*layout));
    layout->window = NULL;
    layout->details_surface = NULL;
    layout->map_surface = NULL;
    layout->map_details_border_size = 2;
    layout->map_pos.x = 0;
    layout->map_pos.y = 0;
    layout->details_pos.x = 0;
    layout->details_pos.y = 0;
    layout->tile_size = MEDIUM_TILE;
    return layout;
}

SDL_Rect get_centering_pos(SDL_Surface* screen, SDL_Surface* to_center_on_screen){
    SDL_Rect pos;
    pos.x = int_center(screen->w,to_center_on_screen->w);
    pos.y = int_center(screen->h,to_center_on_screen->h);
    return pos;
}

int details_get_centering_absciss_pos(Layout* layout, SDL_Surface* s){
    int pos = int_center(layout->details_surface->w + layout->map_details_border_size, s->w);
    return pos;
}

int get_tile_size_in_pixels(Tile_Size size){
    int result = 0;
    switch(size){
        case SMALL_TILE:
            result=40;
            break;
        case MEDIUM_TILE:
            result=50;
            break;
        case BIG_TILE:
            result=60;
            break;
    }
    return result;
}

SDL_Window* initialize(){
    SDL_Window *window=NULL;
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        SDL_Log("Error on SDL init: %s\n",SDL_GetError());
        carcassone_error_quit(ERR_SDL_INIT,NULL);
    }
    window = SDL_CreateWindow("Carcassone",0,30,1000,500,SDL_WINDOW_OPENGL);
    if(window==NULL){
       SDL_Log("Window couldn't be created: %s\n",SDL_GetError());
       carcassone_error_quit(ERR_WINDOW_INIT,NULL);
    }
    return window;
}

void initialize_game_layout(SDL_Window *window, Layout* layout){
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    if(screen==NULL){
        SDL_Log("Window screen couldn't be fetched: %s\n",SDL_GetError());
        carcassone_error_quit(ERR_WINDOW_SURFACE,NULL);
    }
    int screen_w=screen->w, screen_h=screen->h;
    int details_width=4*get_tile_size_in_pixels(MEDIUM_TILE);
    int map_width=screen_w-details_width;
    SDL_Rect pos, border_pos;
    SDL_Surface *map_surface=NULL,
                *details_surface=NULL;

    //Create map and details surfaces
    map_surface = SDL_CreateRGBSurface(0,map_width,screen_h,32,0,0,0,0);
    SDL_FillRect(map_surface,NULL,SDL_MapRGB(map_surface->format,255,255,255));
    details_surface = SDL_CreateRGBSurface(0,details_width,screen_h,32,0,0,0,0);
    SDL_FillRect(details_surface,NULL,SDL_MapRGB(details_surface->format,250,250,250));
    pos.x=0;pos.y=0;
    SDL_BlitSurface(map_surface,NULL,screen,&pos);
    //Draw separation border between map and details
    border_pos.x = 0;
    border_pos.y = 0;
    border_pos.w = layout->map_details_border_size;
    border_pos.h = screen_h;
    SDL_FillRect(details_surface,&border_pos,SDL_MapRGB(details_surface->format,0,0,0));
    pos.x=map_width;
    SDL_BlitSurface(details_surface,NULL,screen,&pos);
    SDL_UpdateWindowSurface(window);
    layout->window=window;
    layout->map_surface=map_surface;
    layout->details_surface=details_surface;
    layout->details_pos.x = screen_w - details_width;
}

void free_layout(Layout *layout){
    if(layout!=NULL){
        SDL_FreeSurface(layout->map_surface);
        SDL_FreeSurface(layout->details_surface);
        SDL_DestroyWindow(layout->window);
        free(layout);
    }
}

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

//Function to blit the surface on the layout surface at the dest coordinate
void blit_on(Layout_Part part, SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dest, Layout *layout){
    SDL_Surface *screen=NULL,
                *window_screen=NULL;
    SDL_Rect screen_pos;
    switch(part){
        case LP_MAP:
            screen = layout->map_surface;
            screen_pos = layout->map_pos;
            break;
        case LP_DETAILS:
            screen = layout->details_surface;
            screen_pos = layout->details_pos;
            break;
    }

    SDL_BlitSurface(surface,src,screen,dest);
    //Blit the modified layout surface on the window for the modif to be visible when updating window surface
    window_screen = SDL_GetWindowSurface(layout->window);
    SDL_BlitSurface(screen,NULL,window_screen,&screen_pos);
}

//Functions to blit on the map/details to be prefixed with map/details
void map_insert_tile(SDL_Surface *surface, int x, int y, Layout *layout){
    SDL_Rect pos=get_slot_upper_left(x,y,layout->tile_size);
    if(pos_in_surface(pos, *(layout->map_surface)))
        blit_on(LP_MAP,surface,NULL,&pos,layout);
}

//Returns the upper left corner of the slot containing (x,y)
//(width and height potentially used later for movable map)
SDL_Rect get_slot_upper_left(int x, int y, Tile_Size tile_size){
    int block_size=get_tile_size_in_pixels(tile_size);
    SDL_Rect result;

    result.x = (x/block_size)*block_size;
    result.y = (y/block_size)*block_size;
    //TODO: handle width and height of result once movable map implemented
    return result;
}

int pos_in_surface(SDL_Rect pos, SDL_Surface surface){
    int result=1;
    if(pos.x < 0 || pos.y < 0)
        result = 0;
    if(pos.x >= surface.w || pos.y >= surface.h)
        result = 0;

    return result;
}

SDL_Surface* get_view(View_Type type, char* filename){
    char path_of_tile_view[IMAGE_PATH_MAX_LEN+1] = {'\0'};
    int folder_len=0;
    switch(type){
        case VT_CONTROL:
            strncat(path_of_tile_view,VIEW_CONTROL_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_CONTROL_FOLDER);
            break;
        case VT_TILE:
            strncat(path_of_tile_view,VIEW_TILE_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_TILE_FOLDER);
            break;
        case VT_PAWN:
            strncat(path_of_tile_view,VIEW_PAWN_FOLDER,IMAGE_PATH_MAX_LEN);
            folder_len = strlen(VIEW_PAWN_FOLDER);
            break;
    }
    strncat(path_of_tile_view,filename,IMAGE_PATH_MAX_LEN-folder_len);
    printf("%s\n",path_of_tile_view);
    return IMG_Load(path_of_tile_view);
}

void details_show_tile(Layout* layout, SDL_Surface* tile){
    SDL_Rect pos = get_centering_pos(layout->details_surface,tile);
    pos.x = details_get_centering_absciss_pos(layout,tile);
    blit_on(LP_DETAILS,tile,NULL,&pos,layout);
    printf("show turn controls: ");
    details_show_tile_turn_controls(layout,pos);
}

void details_show_tile_turn_controls(Layout* layout, SDL_Rect tile_pos){
    SDL_Rect pos=tile_pos;
    int tile_size = get_tile_size_in_pixels(layout->tile_size),
        padding_with_tile = 5;

    SDL_Surface* control = get_view(VT_CONTROL,"turn_left2.jpg");
    pos.x = tile_pos.x - control->w - padding_with_tile;
    pos.y += int_center(tile_size,control->h);
    blit_on(LP_DETAILS,control,NULL,&pos,layout);

    control = get_view(VT_CONTROL,"turn_right.jpg");
    pos.x = tile_pos.x + tile_size + padding_with_tile;
    pos.y = tile_pos.y + int_center(tile_size,control->h);
    blit_on(LP_DETAILS,control,NULL,&pos,layout);

    control = get_view(VT_CONTROL,"turn_1802.jpg");
    pos.x = tile_pos.x + int_center(tile_size,control->w);
    pos.y = tile_pos.y + tile_size + padding_with_tile;
    blit_on(LP_DETAILS,control,NULL,&pos,layout);
    //Safe to free here already, as the turn controls are used nowhere else and shouldn't be erased later
    SDL_FreeSurface(control);
}
