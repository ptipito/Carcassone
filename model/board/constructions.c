#include "model/board/constructions.h"

Carc_Construction* CBC_new_city(int has_flag, int is_cathedral, Carc_City_Merchandise merch_type){
    Carc_City city;
    Carc_Construction *construct = NULL;
    while(construct==NULL){
        construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    }

    city.has_flag = has_flag;
    city.is_cathedral = is_cathedral;
    city.merchandise = merch_type;

    construct->city = city;

    return construct;
}

Carc_Construction* CBC_new_path(int has_lake){
    Carc_Path path;
    Carc_Construction *construct = NULL;
    while(construct==NULL){
        construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    }
    path.has_lake = has_lake;
    construct->path = path;
    return construct;
}

Carc_Construction* CBC_new_garden(){
    Carc_Garden garden;
    Carc_Construction *construct = NULL;
    while(construct==NULL){
        construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    }
    garden.field_occupied = 0;
    garden.garden_occupied = 0;
    construct->garden = garden;
    return construct;
}

int CBC_construction_cmp(Carc_Construction* c1, Carc_Construction_Type type_c1, Carc_Construction* c2, Carc_Construction_Type type_c2){
    if(type_c1!=type_c2){
        return 1;
    }
    if(c1==NULL || c2==NULL){
        return !(c1==c2);
    }
    switch(type_c1){
        case CCBT_PATH:
            if(c1->path.has_lake == c2->path.has_lake)
                return 0;
            else
                return 1;
        case CCBT_CITY:
            if( c1->city.has_flag == c2->city.has_flag
                && c1->city.is_cathedral == c2->city.is_cathedral
                && c1->city.merchandise == c2->city.merchandise){
                return 0;
            } else{
                return 1;
            }
        case CCBT_GARDEN:
            //The fields in garden do not impact the equality of all gardens
            return 0;
        default:
            return 0;
    }
}
