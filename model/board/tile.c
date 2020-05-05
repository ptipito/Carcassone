#include "model/board/tile.h"

void CT_Free_Node(Carc_Tile_Node* node){
    if(node!=NULL){
        free(node->construction);
    }
}

Carc_Tile_Node* CT_get_node_from_loc(Carc_Tile* tile, CT_Locations loc){
    if(loc == CTL_CENTER)
        return &(tile->center);
    return &(tile->border[loc]);
}

void CT_Free_Tile(Carc_Tile* tile){
    if(tile==NULL){
        return;
    }
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_NORTH_WEST_NORTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_NORTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_NORTH_EAST_NORTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_NORTH_EAST_NORTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_EAST));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_SOUTH_EAST_EAST));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_SOUTH_EAST_SOUTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_SOUTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_SOUTH_WEST_SOUTH));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_SOUTH_WEST_WEST));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_WEST));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_NORTH_WEST_WEST));
    CT_Free_Node(CT_get_node_from_loc(tile, CTL_CENTER));
    free(tile);
}

Carc_Tile_Node CT_new_node(Carc_Construction_Type type, Carc_Construction* cons){
    Carc_Tile_Node node;
    node.node_type = type;
    node.construction = cons;

    return node;
}

Carc_Tile* CT_new_empty_tile(){
    int i=0,j=0;
    Carc_Tile* tile = malloc(sizeof(Carc_Tile));
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        tile->center_connexions[i]=0;
        tile->border[i].construction = malloc(sizeof(Carc_Construction));
        //printf("node cons %d created in %p\n",i,tile->border[i].construction);
        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            tile->border_connexions[i][j]=0;
        }
    }
    tile->center.construction = malloc(sizeof(Carc_Construction));
        //printf("node cons 12 created in %p\n",tile->center.construction);
    return tile;
}

CT_Locations CT_get_location_from_string(char* loc){
    CT_Locations result=CTL_CENTER;
    if(strcmp(loc,"C")==0){
        result = CTL_CENTER;
    }
    if(strcmp(loc,"NWW")==0){
        result = CTL_NORTH_WEST_WEST;
    }
    if(strcmp(loc,"NWN")==0){
        result = CTL_NORTH_WEST_NORTH;
    }
    if(strcmp(loc,"N")==0){
        result = CTL_NORTH;
    }
    if(strcmp(loc,"NEN")==0){
        result = CTL_NORTH_EAST_NORTH;
    }
    if(strcmp(loc,"NEE")==0){
        result = CTL_NORTH_EAST_EAST;
    }
    if(strcmp(loc,"E")==0){
        result = CTL_EAST;
    }
    if(strcmp(loc,"SEE")==0){
        result = CTL_SOUTH_EAST_EAST;
    }
    if(strcmp(loc,"SES")==0){
        result = CTL_SOUTH_EAST_SOUTH;
    }
    if(strcmp(loc,"S")==0){
        result = CTL_SOUTH;
    }
    if(strcmp(loc,"SWS")==0){
        result = CTL_SOUTH_WEST_SOUTH;
    }
    if(strcmp(loc,"SWW")==0){
        result = CTL_SOUTH_WEST_WEST;
    }
    if(strcmp(loc,"W")==0){
        result = CTL_WEST;
    }
    return result;
}

Carc_Construction_Type CT_get_node_type_from_str(char* type){
    Carc_Construction_Type result=CCT_FIELD;
    if(strcmp(type,"path")==0)
        result = CCT_PATH;
    if(strcmp(type,"path_end")==0)
        result = CCT_PATH_END;
    if(strcmp(type,"city")==0)
        result = CCT_CITY;
    if(strcmp(type,"cloister")==0)
        result = CCT_CLOISTER;
    if(strcmp(type,"garden")==0)
        result = CCT_GARDEN;
    if(strcmp(type,"field")==0)
        result = CCT_FIELD;

    return result;
}

Carc_City_Merchandise CT_parse_merchandise(char c){
    switch(c){
        case 'c':
            return CCM_CORN;
        case 'b':
            return CCM_BARREL;
        case 't':
            return CCM_TISSUE;
        default:
            return CCM_NONE;
    }
}

void CT_set_node_const(Carc_Tile* tile, CT_Locations loc, Carc_Construction* construct){
    Carc_Tile_Node* node = CT_get_node_from_loc(tile,loc);
    node->construction = construct;
}

void CT_set_single_connexion(Carc_Tile* tile, CT_Locations loc, CT_Locations neighbor_loc){
    if(loc == neighbor_loc){
        return; //no connexion to self is possible
    }
    if(loc == CTL_CENTER){
        tile->center_connexions[neighbor_loc] = 1;
        //printf("neigh of center: %d-%d=%d\n",loc,neighbor_loc,tile->center_connexions[neighbor_loc]);
    } else if(neighbor_loc == CTL_CENTER){
        tile->center_connexions[loc] = 1;
        //printf("neigh of center: %d-%d=%d\n",neighbor_loc,loc,tile->center_connexions[loc]);
    } else {
        tile->border_connexions[loc][neighbor_loc] = 1;
    }
    //printf("\t\t\tset connexion %d - %d\n",loc,neighbor_loc);
}

void CT_set_node_type(Carc_Tile* tile,CT_Locations loc, Carc_Construction_Type type){
    Carc_Tile_Node* node = CT_get_node_from_loc(tile, loc);
    node->node_type=type;
}

Carc_Tile* CT_new_tile_from_file(char* filename){
    FILE* tile_file = fopen(filename, "r");
    Carc_Tile* tile = CT_new_empty_tile();
    CT_Locations loc, neighbor_loc;
    Carc_Construction_Type type;
    Carc_Construction *cur_const=NULL;
    int on_new_node_line=1,
        on_node_type_line=0,
        on_neighbors_line=0,
        node_type_len=10,
        cur_loc_len=4,
        cur_neigh_len=4,
        i=0;
    char *cur_loc=NULL,
         *node_type=NULL,
         *cur_neighbor=NULL,
         cur_char = EOF;


    cur_loc = malloc(cur_loc_len*sizeof(char));
    node_type = malloc(cur_neigh_len*sizeof(char));
    cur_neighbor = malloc(node_type_len*sizeof(char));

    while(cur_loc==NULL){
        cur_loc = malloc(cur_loc_len*sizeof(char));
    }
    while(node_type==NULL){
        node_type = malloc(cur_neigh_len*sizeof(char));
    }
    while(cur_neighbor==NULL){
        cur_neighbor = malloc(node_type_len*sizeof(char));
    }
    for(i=0;i<cur_loc_len;i++){cur_loc[i] = '\0';}
    for(i=0;i<cur_neigh_len;i++){cur_neighbor[i] = '\0';}
    for(i=0;i<node_type_len;i++){node_type[i] = '\0';}

    i = 0;
    do{
        cur_char = fgetc(tile_file);
        if(on_new_node_line){
            if(isalpha(cur_char) && i<cur_loc_len-1){
                cur_loc[i] = cur_char;
                i++;
            }
        }
        if(on_node_type_line){
            while(cur_char !=':' && cur_char != '\n' && cur_char!=EOF){
                //Fast forward keywords in the file that are presents only for file understanding
                cur_char = fgetc(tile_file);
            }
            if(cur_char == ':')
                cur_char = fgetc(tile_file);//get to first char of the node type
            //Parse node type
            while(isalpha(cur_char)){
                if(i<node_type_len-1){//if the type is longer than supported, make sure to terminate string
                    node_type[i] = cur_char;
                    i++;
                }
                cur_char = fgetc(tile_file);
            }
            node_type[i] = '\0';
            type = CT_get_node_type_from_str(node_type);
            CT_set_node_type(tile,loc,type);
            //Set node construction
            switch(type){
                case CCT_CITY:
                    cur_const = CC_new_city(0,0,CCM_NONE);
                    break;
                case CCT_PATH:
                    cur_const = CC_new_path(0);
                    break;
                case CCT_GARDEN:
                    cur_const = CC_new_garden();
                    break;
                default:
                    cur_const = NULL;
                    break;
            }
            CT_set_node_const(tile, loc, cur_const);
            //Parse node options (if any)
            while(cur_char!='\n' && cur_char != EOF){
                cur_char = fgetc(tile_file);//on first iteration, this fast pass the first ' ' before the options list
                switch(type){
                    case CCT_CITY:
                        switch(cur_char){
                            case ' ':
                                //separate 2 options. Do nothing
                                break;
                            case 'f':
                                cur_const->city.has_flag = 1;
                                break;
                            case 'c':
                                cur_const->city.is_cathedral = 1;
                                break;
                            case 'm':
                                //Parse merch type
                                cur_char = fgetc(tile_file);
                                if(isalpha(cur_char)){
                                    //cur_char is supposed to be a letter representing the merchandise. Loop for parsing security
                                    cur_const->city.merchandise = CT_parse_merchandise(cur_char);
                                }
                            default:
                                break;
                        }
                        break;
                    case CCT_PATH:
                        switch(cur_char){
                            case 'l':
                                cur_const->path.has_lake = 1;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        //No options handled for other node types
                        break;
                }
            }
        }
        if(on_neighbors_line){
            while(cur_char!=':' && cur_char != '\n' && cur_char!=EOF){
                //Fast forward keywords in the file that are presents only for file understanding
                cur_char = fgetc(tile_file);
            }
            //parse neighbors
            if(cur_char == ':'){
                //Parse all neighbors
                while(cur_char != '\n' && cur_char != EOF){
                        cur_char = fgetc(tile_file);//get to first char of the node type
                    //Parse current neigh
                    while(isalpha(cur_char)){
                        if(i<cur_neigh_len-1){
                            cur_neighbor[i] = cur_char;
                            i++;
                        }
                        cur_char = fgetc(tile_file);
                    }
                    if(i!=0){//i=0->no neighbor selected
                        cur_neighbor[i] = '\0';
                        neighbor_loc = CT_get_location_from_string(cur_neighbor);
                        CT_set_single_connexion(tile, loc, neighbor_loc);
                        i=0;//reset index for the potential next neighbor
                    }
                }
            }
        }
        //handle line change
        if(cur_char=='\n'){
            if(on_new_node_line){
                cur_loc[i] = '\0';
                loc = CT_get_location_from_string(cur_loc);
                on_new_node_line = 0;
                on_node_type_line = 1;
                on_neighbors_line = 0;
            } else if(on_node_type_line){
                on_new_node_line = 0;
                on_node_type_line = 0;
                on_neighbors_line = 1;
            } else if(on_neighbors_line){
                on_new_node_line = 1;
                on_node_type_line = 0;
                on_neighbors_line = 0;
            }
            i=0;
        }
    }while(cur_char!=EOF);

    free(cur_loc);
    free(node_type);
    free(cur_neighbor);
    fclose(tile_file);
    return tile;
}

void CT_turn(Carc_Tile *tile, CT_Turn_Type dir){
    Carc_Tile entry_tile = *tile;
    int transposition_factor=TILE_NR_LOCATIONS_ON_ONE_EDGE,
        previous_location;
    int i=0, j=0;
    switch(dir){
        case RIGHT:
            break;
        case LEFT:
            transposition_factor = -transposition_factor;
            break;
        case UPDOWN:
            transposition_factor *= 2;
            break;
    }

    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        previous_location = positive_modulo(i - transposition_factor, TILE_NR_BORDER_LOCATIONS);
        (tile->border)[i] = entry_tile.border[previous_location];
        //printf("%d is a %d (from index %d)\n",i,(tile->border)[i].node_type,previous_location);
        (tile->center_connexions)[i] = entry_tile.center_connexions[previous_location];
        //Update connexion matrix line per line
        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            (tile->border_connexions)[i][j] =
            entry_tile.border_connexions[previous_location][positive_modulo(j - transposition_factor, TILE_NR_BORDER_LOCATIONS)];
        }
    }
}

int CT_Tiles_connect_in(Carc_Tile t1, CT_Locations t1_node_loc, Carc_Tile t2, CT_Locations t2_node_loc){
    Carc_Tile_Node *node_t1 = CT_get_node_from_loc(&t1, t1_node_loc),
                   *node_t2 = CT_get_node_from_loc(&t2, t2_node_loc);

    if(node_t1->node_type == node_t2->node_type)
        return 1;
    if((node_t1->node_type == CCT_GARDEN && node_t2->node_type == CCT_FIELD)
       || (node_t1->node_type == CCT_FIELD && node_t2->node_type == CCT_GARDEN))
        return 1;

    return 0;
}

int CT_tile_node_cmp(Carc_Tile_Node n1, Carc_Tile_Node n2){
    return CC_construction_cmp(*(n1.construction),n1.node_type,*(n2.construction),n2.node_type);
}

int CT_tile_cmp(Carc_Tile t1, Carc_Tile t2){
    //TODO include compariosn on rotations
    int result, i=0, j=0;
    result = CT_tile_node_cmp(t1.center,t2.center);
    while(result==0 && i<TILE_NR_BORDER_LOCATIONS){
        result = CT_tile_node_cmp(t1.border[i],t2.border[i]);
        i++;
    }
    //Test connexions equality
    if(result==0){
        for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
            for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
                if(t1.border_connexions[i][j] != t2.border_connexions[i][j]){
                    result = 1;
                    break;
                }
            }
            if(t1.center_connexions[i] != t2.center_connexions[i]){
                result = 1;
                break;
            }
        }
    }
    return result;
}

void display_tile(Carc_Tile tile){
    //Print north edge
    printf(" ");
    printf("%d",tile.border[CTL_NORTH_WEST_NORTH].node_type);
    if(tile.border_connexions[CTL_NORTH_WEST_NORTH][CTL_NORTH])
        printf("-");
    else printf(" ");
    printf("%d",tile.border[CTL_NORTH].node_type);
    if(tile.border_connexions[CTL_NORTH][CTL_NORTH_EAST_NORTH])
        printf("-");
    else printf(" ");
    printf("%d\n",tile.border[CTL_NORTH_EAST_NORTH].node_type);

    if(tile.border_connexions[CTL_NORTH_WEST_WEST][CTL_NORTH_WEST_NORTH])
        printf("/ ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_NORTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_NORTH_EAST_NORTH][CTL_NORTH_EAST_EAST])
        printf(" \\");
    else printf("  ");
    printf("\n");

    //Print north west and east corners
    printf("%d",tile.border[CTL_NORTH_WEST_WEST].node_type);
    if(tile.border_connexions[CTL_NORTH_WEST_WEST][CTL_NORTH_EAST_EAST])
        printf("-----");
    else printf("     ");
    printf("%d\n",tile.border[CTL_NORTH_EAST_EAST].node_type);
    printf(" ");
    if(tile.border_connexions[CTL_NORTH_WEST_WEST][CTL_WEST])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.center_connexions[CTL_NORTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_NORTH_EAST_EAST][CTL_EAST])
        printf("|");
    else printf(" ");
    printf(" ");
    printf("\n");

    //print center line
    printf("%d",tile.border[CTL_WEST].node_type);
    if(tile.center_connexions[CTL_WEST])
        printf("--");
    else printf("  ");
    printf("%d",tile.center.node_type);
    if(tile.center_connexions[CTL_EAST])
        printf("--");
    else printf("  ");
    printf("%d\n",tile.border[CTL_EAST].node_type);
    printf(" ");
    if(tile.border_connexions[CTL_WEST][CTL_SOUTH_WEST_WEST])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.center_connexions[CTL_SOUTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_EAST][CTL_SOUTH_EAST_EAST])
        printf("|");
    else printf(" ");
    printf("\n");

    printf("%d",tile.border[CTL_SOUTH_WEST_WEST].node_type);
    if(tile.border_connexions[CTL_SOUTH_WEST_WEST][CTL_SOUTH_EAST_EAST])
        printf("-----");
    else printf("     ");
    printf("%d\n",tile.border[CTL_SOUTH_EAST_EAST].node_type);
    if(tile.border_connexions[CTL_SOUTH_WEST_WEST][CTL_SOUTH_WEST_SOUTH])
        printf("\\ ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_SOUTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_SOUTH_EAST_EAST][CTL_SOUTH_EAST_SOUTH])
        printf(" /");
    else printf("  ");
    printf("\n");

    printf(" ");
    printf("%d",tile.border[CTL_SOUTH_WEST_SOUTH].node_type);
    if(tile.border_connexions[CTL_SOUTH_WEST_SOUTH][CTL_SOUTH])
        printf("-");
    else printf(" ");
    printf("%d",tile.border[CTL_SOUTH].node_type);
    if(tile.border_connexions[CTL_SOUTH][CTL_SOUTH_EAST_SOUTH])
        printf("-");
    else printf(" ");
    printf("%d\n",tile.border[CTL_SOUTH_EAST_SOUTH].node_type);
}
