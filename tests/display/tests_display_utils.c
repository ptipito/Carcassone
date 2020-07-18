#include "tests/display/tests_layout.h"

void test_display_utils_get_centering_pos(){
    printf("test_display_utils_get_centering_pos result: ");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *s1=SDL_CreateRGBSurface(0,100,50,32,0,0,0,0),
                *s2=SDL_CreateRGBSurface(0,20,30,32,0,0,0,0);
    SDL_Rect pos=CDUtils_get_centering_pos(s1,s2);
    printf("%d",pos.x==40 && pos.y==10);

    s2=SDL_CreateRGBSurface(0,150,30,32,0,0,0,0);
    pos=CDUtils_get_centering_pos(s1,s2);
    printf("%d",pos.x==-25 && pos.y==10);

    s2=SDL_CreateRGBSurface(0,50,300,32,0,0,0,0);
    pos=CDUtils_get_centering_pos(s1,s2);
    printf("%d",pos.x==25 && pos.y==-125);

    s2=SDL_CreateRGBSurface(0,150,300,32,0,0,0,0);
    pos=CDUtils_get_centering_pos(s1,s2);
    printf("%d",pos.x==-25 && pos.y==-125);

    SDL_FreeSurface(s1);
    SDL_FreeSurface(s2);
    SDL_Quit();
}

void test_display_utils_get_tile_size_in_pixels(){
    printf("test_display_utils_get_tile_size_in_pixels result: ");
    printf("%d",!(CDUtils_get_tile_size_in_pixels(MEDIUM_TILE)==60));
    printf("%d",!(CDUtils_get_tile_size_in_pixels(MEDIUM_TILE)==40));
    printf("%d",CDUtils_get_tile_size_in_pixels(MEDIUM_TILE)==50);
    printf("%d",!(CDUtils_get_tile_size_in_pixels(SMALL_TILE)==50));
    printf("%d",!(CDUtils_get_tile_size_in_pixels(SMALL_TILE)==60));
    printf("%d",CDUtils_get_tile_size_in_pixels(SMALL_TILE)==40);
    printf("%d",!(CDUtils_get_tile_size_in_pixels(BIG_TILE)==40));
    printf("%d",!(CDUtils_get_tile_size_in_pixels(BIG_TILE)==50));
    printf("%d",CDUtils_get_tile_size_in_pixels(BIG_TILE)==60);
}

void test_display_utils_pos_in_surface(){
    printf("test_display_utils_pos_in_surface result: ");
    SDL_Surface *surface = SDL_CreateRGBSurface(0,80,90,32,0,0,0,0);
    SDL_Rect pos;
    pos.x = 50;
    pos.y = 100;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==0);
    pos.y = 45;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==1);
    pos.y = 90;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==0);
    pos.x = 0;
    pos.y = 0;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==1);
    pos.x = 80;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==0);
    pos.x = 1000;
    pos.y = 520;
    printf("%d",CDUtils_pos_in_surface(pos,*surface)==0);

    SDL_FreeSurface(surface);
}

void test_display_utils_get_view(){
    printf("test_display_utils_get_view result: ");
    SDL_Surface *test_surface=NULL;

    test_surface = CDUtils_get_view(VT_CONTROL,"cloister_path.jpg");
    printf("%d",test_surface==NULL);
    test_surface = CDUtils_get_view(VT_TILE,"cloister_cloister_path.jpg");
    printf("%d",test_surface!=NULL);
    test_surface = CDUtils_get_view(VT_TILE,"turn_right.jpg");
    printf("%d",test_surface==NULL);
    test_surface = CDUtils_get_view(VT_CONTROL,"turn_right.jpg");
    printf("%d",test_surface!=NULL);
    test_surface = CDUtils_get_view(VT_PAWN,"turn_right.jpg");
    printf("%d",test_surface==NULL);
    /*TODO add valid tests for pawn when pawns ressources are available*/
}

void test_display_utils_set_rect_pos(){
    printf("test_display_utils_set_rect_pos result: ");
    SDL_Rect pos;
    pos.x=0;
    pos.y=0;
    printf("%d",pos.x!=25 && pos.y!=30);
    CDUtils_set_rect_coord(&pos,25,30);
    printf("%d",pos.x==25 && pos.y==30);
    CDUtils_set_rect_coord(&pos,-60,-530);
    printf("%d",pos.x==-60 && pos.y==-530);
    CDUtils_set_rect_coord(&pos,25,-530);
    printf("%d",pos.x==25 && pos.y==-530);
    CDUtils_set_rect_coord(&pos,-25,30);
    printf("%d",pos.x==-25 && pos.y==30);
}

void test_display_utils_init_rect(){
    printf("test_display_utils_init_rect result: ");
    SDL_Rect pos;
    pos.x=0;
    pos.y=0;
    int x=25, y=85, w=20, h=60;
    pos=CDUtils_init_rect(x,y,w,h);
    printf("%d",pos.x==25 && pos.y==85 && pos.w==20 && pos.h==60);
    x=-5; y=0; w=-20; h=-60;
    pos=CDUtils_init_rect(x,y,w,h);
    printf("%d",pos.x==-5 && pos.y==0 && pos.w==-20 && pos.h==-60);
    x=0; y=-36; w=0; h=00;
    pos=CDUtils_init_rect(x,y,w,h);
    printf("%d",pos.x==0 && pos.y==-36 && pos.w==0 && pos.h==0);
}
