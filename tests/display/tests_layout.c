#include "tests/display/tests_layout.h"

void test_layout_new_layout(){
    printf("test_layout_new_layout result: ");
    Carc_Layout *layout=CDL_new_layout();
    printf("%d",layout!=NULL);
    printf("%d",layout->window==NULL);
    printf("%d",layout->map_surface==NULL);
    printf("%d",layout->details_surface==NULL);
    printf("%d",layout->map_pos.x==0);
    printf("%d",layout->map_pos.y==0);
    printf("%d",layout->details_pos.x==0);
    printf("%d",layout->details_pos.y==0);
    printf("%d",layout->tile_size==MEDIUM_TILE);

}

void test_layout_initialize_game_layout(){
    printf("test_layout_new_layout result: ");
    SDL_Window* window=CDUtils_initialize_window();
    Carc_Layout *layout=CDL_initialize_game_layout(window);
    printf("%d",layout!=NULL);
    printf("%d",layout->window==window);
    printf("%d",layout->map_surface!=NULL);
    printf("%d",layout->details_surface!=NULL);
    printf("%d",layout->map_pos.x==0);
    printf("%d",layout->map_pos.y==0);
    printf("%d",layout->details_pos.x==layout->map_surface->w);
    printf("%d",layout->details_pos.y==0);
    printf("%d",layout->tile_size==MEDIUM_TILE);
    CDUtils_quit_sdl(window);
}
