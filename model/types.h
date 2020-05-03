#ifndef DEF_TYPES
#define DEF_TYPES
#include <stdlib.h>
#include <stdio.h>
#include "tile.h"


typedef Map_Coordinates {
    int x;
    int y;
} Map_Coordinates;

typedef Map{
    Tile node;
    Map_Coordinates node_coordinates;
    Map* neighbors;
} Map;
#endif // DEF_TYPES
