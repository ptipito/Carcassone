#include "model/board/constructions.h"

Carc_Construction* CC_new_city(int has_flag, int is_cathedral, Carc_City_Merchandise merch_type){
    Carc_City city;
    Carc_Construction *construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));

    city.has_flag = has_flag;
    city.is_cathedral = is_cathedral;
    city.merchandise = merch_type;

    construct->city = city;

    return construct;
}

Carc_Construction* CC_new_path(int has_lake){
    Carc_Path path;
    Carc_Construction *construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    path.has_lake = has_lake;
    construct->path = path;
    return construct;
}

Carc_Construction* CC_new_garden(){
    Carc_Garden garden;
    Carc_Construction *construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    garden.field_occupied = 0;
    garden.garden_occupied = 0;
    construct->garden = garden;
    return construct;
}

int CC_construction_cmp(Carc_Construction c1, Carc_Construction_Type type_c1, Carc_Construction c2, Carc_Construction_Type type_c2){
    if(type_c1!=type_c2){
        return 0;
    }
    switch(type_c1){
        case CCT_PATH:
            return c1.path.has_lake == c2.path.has_lake;
            break;
        case CCT_CITY:
            return (c1.city.has_flag == c2.city.has_flag
                    && c1.city.is_cathedral == c2.city.is_cathedral
                    && c1.city.merchandise == c2.city.merchandise);
            break;
        default:
            return 1;
            break;
    }
}
