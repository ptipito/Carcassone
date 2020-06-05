#ifndef DEF_CONSTRUCTIONS
#define DEF_CONSTRUCTIONS

#include <stdlib.h>
#include <stdio.h>

typedef enum {CCM_CORN, CCM_BARREL, CCM_TISSUE, CCM_NONE} Carc_City_Merchandise;
typedef enum {CBCT_PATH, CBCT_PATH_END, CBCT_CITY, CBCT_CLOISTER, CBCT_GARDEN, CBCT_FIELD} Carc_Construction_Type;

typedef struct {
    int has_flag;
    int is_cathedral;
    Carc_City_Merchandise merchandise;
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
Carc_Construction* CBC_new_path(int);
Carc_Construction* CBC_new_garden();

int CBC_construction_cmp(Carc_Construction*, Carc_Construction_Type, Carc_Construction*, Carc_Construction_Type);



#endif // DEF_CONSTRUCTIONS
