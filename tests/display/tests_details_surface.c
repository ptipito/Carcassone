#include "tests/display/tests_details_surface.h"

void test_details_surface_get_centering_absciss_pos(){
    printf("test_details_surface_get_centering_absciss_pos results: ");
    SDL_Window *window = CDUtils_initialize_window();
    SDL_Surface *s1 = SDL_CreateRGBSurface(0,25,26,32,0,0,0,0);
    Carc_Layout *layout = CDL_initialize_game_layout(window);

    printf("%d",CDDetails_get_centering_absciss_pos(layout,s1)==(layout->details_surface->w-s1->w+layout->map_details_border_size)/2);
    s1=SDL_CreateRGBSurface(0,500,260,32,0,0,0,0);
    printf("%d",CDDetails_get_centering_absciss_pos(layout,s1)==(layout->details_surface->w-s1->w+layout->map_details_border_size)/2);

    SDL_FreeSurface(s1);
    CDL_free_layout(layout);
}
