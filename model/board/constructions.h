#ifndef DEF_CONSTRUCTIONS
#define DEF_CONSTRUCTIONS

#include <stdlib.h>
#include <stdio.h>

#define NB_CONST_TYPES 6

typedef enum {CCM_CORN, CCM_BARREL, CCM_TISSUE, CCM_NONE} Carc_City_Merchandise;
typedef enum {CBCT_PATH, CBCT_PATH_END, CBCT_CITY, CBCT_CLOISTER, CBCT_GARDEN, CBCT_FIELD} Carc_Construction_Type;

typedef struct {
    int nb_flags;
    int is_cathedral;
    Carc_City_Merchandise* merchandises;
    int nb_merchandises;
} Carc_City;

typedef struct {
    int has_lake;
} Carc_Path;

typedef struct {
    int field_occupied;
    int garden_occupied;
} Carc_Garden;

typedef union {
    Carc_City city;
    Carc_Path path;
    Carc_Garden garden;
} Carc_Construction;

Carc_Construction* CBC_new_city(int, int, Carc_City_Merchandise);
void CBC_free_city(Carc_Construction*);
Carc_Construction* CBC_new_path(int);
Carc_Construction* CBC_new_garden();
int CBC_construction_cmp(Carc_Construction*, Carc_Construction_Type, Carc_Construction*, Carc_Construction_Type);
int CBC_types_connect(Carc_Construction_Type, Carc_Construction_Type);

int CBCCity_enrich_with(Carc_City*, const Carc_City);
int CBCCity_add_merchandise(Carc_City*, Carc_City_Merchandise);
int CBCPath_enrich_with(Carc_Path*, const Carc_Path);


#endif // DEF_CONSTRUCTIONS
