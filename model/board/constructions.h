#ifndef DEF_CONSTRUCTIONS
#define DEF_CONSTRUCTIONS

#include <stdlib.h>
#include <stdio.h>

typedef enum {CCM_CORN, CCM_BARREL, CCM_TISSUE, CCM_NONE} Carc_City_Merchandise;
typedef enum {CCT_PATH, CCT_CITY, CCT_PATH_END, CCT_CLOISTER, CCT_GARDEN, CCT_FIELD} Carc_Construction_Type;

typedef struct {
    int has_flag : 1;
    int is_cathedral : 1;
    Carc_City_Merchandise merchandise;
} Carc_City;

typedef struct {
    int has_lake : 1;
} Carc_Path;

typedef struct {
    int field_occupied : 1;
    int garden_occupied : 1;
} Carc_Garden;

typedef union {
    Carc_City city;
    Carc_Path path;
    Carc_Garden garden;
} Carc_Construction;

Carc_Construction* CC_new_city(int, int, Carc_City_Merchandise);
Carc_Construction* CC_new_path(int);
Carc_Construction* CC_new_garden();

//TODO define pawn types


#endif // DEF_CONSTRUCTIONS
