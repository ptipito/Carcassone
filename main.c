#include "display/map_surface.h"
#include "display/details_surface.h"
#include "control/details_controls.h"
#include "app.h"
/*
This file is not yet the real main and serves as testing for SDL display.
For the time being is there no link between the model files and the functionalities available through the main
*/

int main(int argc, char* argv[]){

    Carc_App* Carcassone = Carc_App_init();

    return Carc_App_run(Carcassone);
}


