#include "model/board/constructions.h"

Carc_Construction* CBC_new_city(int nb_flags, int is_cathedral, Carc_City_Merchandise merch_type){
    Carc_City city;
    Carc_Construction *construct = NULL;
    while(construct==NULL){
        construct = (Carc_Construction*) malloc(sizeof(Carc_Construction));
    }

    city.nb_flags = nb_flags;
    city.is_cathedral = is_cathedral;
    if(merch_type==CCM_NONE){
        city.merchandises = NULL;
        city.nb_merchandises = 0;
    }
    else{
        city.merchandises = malloc(sizeof(Carc_City_Merchandise));
        if(city.merchandises!=NULL){
            city.merchandises[0] = merch_type;
            city.nb_merchandises = 1;
        }
        else{
            fprintf(stderr,"ERROR: couldn't allocate memory for merchandise\n");
            free(construct);
            return NULL;
        }
    }

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
    int i=0, same_merchandise=1;
    if(type_c1!=type_c2){
        return 1;
    }
    if(c1==NULL || c2==NULL){
        return !(c1==c2);
    }
    switch(type_c1){
        case CBCT_PATH:
            if(c1->path.has_lake == c2->path.has_lake)
                return 0;
            else
                return 1;
        case CBCT_CITY:
            if( c1->city.nb_flags == c2->city.nb_flags
                && c1->city.is_cathedral == c2->city.is_cathedral
                && c1->city.nb_merchandises == c2->city.nb_merchandises){
                for(i=0;i<c1->city.nb_merchandises;i++){
                    if(c1->city.merchandises[i]!=c2->city.merchandises[i]){
                        same_merchandise = 0;
                        i = c1->city.nb_merchandises;
                    }
                }
                return !same_merchandise;
             } else{
                return 1;
            }
        case CBCT_GARDEN:
            //The fields in garden do not impact the equality of all gardens
            return 0;
        default:
            return 0;
    }
}

int CBC_types_connect(Carc_Construction_Type type1, Carc_Construction_Type type2){
    if(type1 >= NB_CONST_TYPES || type2 >= NB_CONST_TYPES){
        fprintf(stderr,"WARNING: try to use invalid value for the enum Carc_Construction_Type (CBC_types_connect(%d,%d))\n",type1,type2);
        return 0;
    }
    if(type1==type2){
        return 1;
    }
    if((type1==CBCT_FIELD && type2==CBCT_GARDEN)
       || (type1==CBCT_GARDEN && type2==CBCT_FIELD)){
        return 1;
    }
    if((type1==CBCT_PATH && type2==CBCT_PATH_END)
       || (type1==CBCT_PATH_END&& type2==CBCT_PATH)){
        return 1;
    }
    return 0;
}

void CBC_free_city(Carc_Construction* city){
    if(city!=NULL){
        free(city->city.merchandises);
    }
    free(city);
}

///Functions to enrich src with new_info. This shall be used for merging constructions.
///The info of the source construction (e.g. lakes, flags, etc...) is modified using
///the info in new_info (to add new lakes, flags, etc...)
int CBCCity_enrich_with(Carc_City* src, const Carc_City new_info){
    int i, res=0, max_try=5, cur_try=0, fail=-1;
    if(src==NULL){
        fprintf(stderr,"ERROR: cannot use null input for CBCCity_enrich_with\n");
        return fail;
    }
    src->is_cathedral += new_info.is_cathedral;
    src->nb_flags += new_info.nb_flags;
    for(i=0;i<new_info.nb_merchandises;i++){
        cur_try = 0;
        if(CBCCity_add_merchandise(src,new_info.merchandises[i])!=0){
            if(cur_try > max_try){
                i = new_info.nb_merchandises;
                res = fail;
            }else{
                i--;
                cur_try++;
            }
        }
    }
    return res;
}

int CBCCity_add_merchandise(Carc_City* src, Carc_City_Merchandise merch){
    Carc_City_Merchandise* for_realloc=NULL;
    if(src==NULL)
        return -1;
    if(merch!=CCM_NONE){
        for_realloc = realloc(src->merchandises,(src->nb_merchandises + 1)*sizeof(*for_realloc));
        if(for_realloc!=NULL){
            src->merchandises = for_realloc;
            src->merchandises[src->nb_merchandises] = merch;
            src->nb_merchandises++;
            return 0;
        }
        fprintf(stderr,"ERROR: couldn't allocate memory (CBCCity_add_merchandise)\n");
    }
    return -1;
}

int CBCPath_enrich_with(Carc_Path* src, const Carc_Path new_info){
    if(src==NULL){
        return -1;
    }
    src->has_lake += new_info.has_lake;
    return 0;
}
