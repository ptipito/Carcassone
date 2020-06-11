#include "model/board/tile.h"

char* CT_get_tile_file_path(char* tile_name){
    if(strlen(TILE_FOLDER)+strlen(tile_name) > TILE_FULL_PATH_MAX_LEN){
        fprintf(stderr,"CT_get_tile_file_path error: tile path too long for %s%s\n",TILE_FOLDER,tile_name);
        exit(ERR_STR_BUFFER_LENGTH);
    }
    char* full_path = malloc((TILE_FULL_PATH_MAX_LEN+1)*sizeof(char));//Add an extra char for '\0'
    full_path[TILE_FULL_PATH_MAX_LEN]='\0';
    strncpy(full_path,TILE_FOLDER,TILE_FULL_PATH_MAX_LEN);
    strncat(full_path,tile_name,TILE_FULL_PATH_MAX_LEN - strlen(full_path));
    return full_path;
}

void CBT_free_node(Carc_Tile_Node* node){
    if(node!=NULL){
        if(node->node_type==CBCT_CITY){
            CBC_free_city(node->construction);
        } else{
            free(node->construction);
        }
        CPPawn_free_pawn(node->pawn);
    }
    free(node);
}

Carc_Tile_Node* CBT_get_node_from_loc(Carc_Tile* tile, Carc_Tile_Location loc){
    if(tile==NULL){
        fprintf(stderr, "ERROR: cannot get node for loc %d for NULL tile\n",loc);
        return NULL;
    }
    if(CBT_is_valid_loc(loc)){
        if(loc == CTL_CENTER)
            return &(tile->center);
        return &(tile->border[loc]);
    } else{
        fprintf(stderr, "ERROR: loc %d is invalid\n",loc);
        return NULL;
    }
}

void CBT_free_tile(Carc_Tile* tile){
    if(tile!=NULL){
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_NORTH_WEST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_NORTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_NORTH_EAST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_EAST_NORTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_EAST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_EAST_SOUTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_SOUTH_EAST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_SOUTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_SOUTH_WEST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_WEST_SOUTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_WEST));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_WEST_NORTH));
        CBT_free_node(CBT_get_node_from_loc(tile, CTL_CENTER));
    }
    free(tile);
}

Carc_Tile_Location CBT_get_location_from_string(char* loc){
    Carc_Tile_Location result=-1;
    if(strcmp(loc,"C")==0){
        result = CTL_CENTER;
    }
    if(strcmp(loc,"WN")==0){
        result = CTL_WEST_NORTH;
    }
    if(strcmp(loc,"NW")==0){
        result = CTL_NORTH_WEST;
    }
    if(strcmp(loc,"N")==0){
        result = CTL_NORTH;
    }
    if(strcmp(loc,"NE")==0){
        result = CTL_NORTH_EAST;
    }
    if(strcmp(loc,"EN")==0){
        result = CTL_EAST_NORTH;
    }
    if(strcmp(loc,"E")==0){
        result = CTL_EAST;
    }
    if(strcmp(loc,"ES")==0){
        result = CTL_EAST_SOUTH;
    }
    if(strcmp(loc,"SE")==0){
        result = CTL_SOUTH_EAST;
    }
    if(strcmp(loc,"S")==0){
        result = CTL_SOUTH;
    }
    if(strcmp(loc,"SW")==0){
        result = CTL_SOUTH_WEST;
    }
    if(strcmp(loc,"WS")==0){
        result = CTL_WEST_SOUTH;
    }
    if(strcmp(loc,"W")==0){
        result = CTL_WEST;
    }
    return result;
}

int CBT_is_valid_loc(Carc_Tile_Location loc){
    return 0<= loc && loc <= CTL_CENTER;
}

Carc_Construction_Type CBT_get_node_type_from_str(char* type){
    Carc_Construction_Type result=CBCT_FIELD;
    if(strcmp(type,"path")==0)
        result = CBCT_PATH;
    if(strcmp(type,"path_end")==0)
        result = CBCT_PATH_END;
    if(strcmp(type,"city")==0)
        result = CBCT_CITY;
    if(strcmp(type,"cloister")==0)
        result = CBCT_CLOISTER;
    if(strcmp(type,"garden")==0)
        result = CBCT_GARDEN;
    if(strcmp(type,"field")==0)
        result = CBCT_FIELD;

    return result;
}

Carc_City_Merchandise CBT_parse_merchandise(char c){
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

void CBT_set_node_const(Carc_Tile* tile, Carc_Tile_Location loc, Carc_Construction* construct){
    Carc_Tile_Node* node = CBT_get_node_from_loc(tile,loc);
    node->construction = construct;
}

int  CBT_set_single_connexion(Carc_Tile* tile, Carc_Tile_Location loc, Carc_Tile_Location neighbor_loc){
    if(CBT_is_valid_loc(loc)==0 || CBT_is_valid_loc(neighbor_loc)==0){
        fprintf(stderr,"WARNING: single connexion not set as one of the locations is invalid\n");
        return -1;
    }
    if(loc == neighbor_loc){
        return -1; //no connexion to self is possible
    }
    if(loc == CTL_CENTER){
        tile->center_connexions[neighbor_loc] = 1;
    } else if(neighbor_loc == CTL_CENTER){
        tile->center_connexions[loc] = 1;
    } else {
        tile->border_connexions[loc][neighbor_loc] = 1;
        tile->border_connexions[neighbor_loc][loc] = 1;
    }
    return 0;
}

void CBT_set_node_type(Carc_Tile* tile, Carc_Tile_Location loc, Carc_Construction_Type type){
    Carc_Tile_Node* node = CBT_get_node_from_loc(tile, loc);
    if(node!=NULL)
        node->node_type=type;
}

Carc_Tile_Node* CBT_new_node(Carc_Construction_Type type, Carc_Construction* cons){
    Carc_Tile_Node* node=NULL;
    while(node==NULL){
        node = malloc(sizeof(Carc_Construction));
    }
    node->node_type = type;
    node->construction = cons;
    node->pawn = NULL;
    return node;
}

Carc_Tile* CBT_new_empty_tile(){
    int i=0, j=0;
    Carc_Tile* tile =  NULL;
    while(tile==NULL){
        tile = malloc(sizeof(*tile));
    }
    for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
        tile->center_connexions[i]=0;
        tile->border[i].construction = NULL;
        tile->border[i].node_type = -1;
        tile->border[i].pawn = NULL;
        for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
            tile->border_connexions[i][j]=0;
        }
    }
    tile->center.construction = NULL;
    tile->center.node_type = -1;
    tile->center.pawn = NULL;
    return tile;
}

Carc_Tile* CBT_new_tile_from_file(char* filename){
    FILE* tile_file = fopen(filename, "r");
    Carc_Tile* tile = CBT_new_empty_tile();
    Carc_Tile_Location loc, neighbor_loc;
    Carc_Construction_Type type;
    Carc_Construction *cur_const=NULL;
    int on_new_node_line=1,
        on_node_type_line=0,
        on_neighbors_line=0,
        node_type_len=20,
        cur_loc_len=3,
        cur_neigh_len=3,
        i=0;
    char cur_loc[20],
         node_type[3],
         cur_neighbor[3],
         cur_char = EOF;

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
            type = CBT_get_node_type_from_str(node_type);
            CBT_set_node_type(tile,loc,type);
            //Set node construction
            switch(type){
                case CBCT_CITY:
                    cur_const = CBC_new_city(0,0,CCM_NONE);
                    break;
                case CBCT_PATH:
                    cur_const = CBC_new_path(0);
                    break;
                case CBCT_GARDEN:
                    cur_const = CBC_new_garden();
                    break;
                default:
                    cur_const = NULL;
                    break;
            }
            CBT_set_node_const(tile, loc, cur_const);
            //Parse node options (if any)
            while(cur_char!='\n' && cur_char != EOF){
                cur_char = fgetc(tile_file);//on first iteration, this fast pass the first ' ' before the options list
                switch(type){
                    case CBCT_CITY:
                        switch(cur_char){
                            case ' ':
                                //separate 2 options. Do nothing
                                break;
                            case 'f':
                                cur_const->city.nb_flags = 1;
                                break;
                            case 'c':
                                cur_const->city.is_cathedral = 1;
                                break;
                            case 'm':
                                //Parse merch type
                                cur_char = fgetc(tile_file);
                                if(isalpha(cur_char)){
                                    //cur_char is supposed to be a letter representing the merchandise. Loop for parsing security
                                    cur_const->city.merchandises = malloc(sizeof(Carc_City_Merchandise));
                                    if(cur_const->city.merchandises!=NULL)
                                        cur_const->city.merchandises[0] = CBT_parse_merchandise(cur_char);
                                    else{
                                        fprintf(stderr,"ERROR: couldn't allocate memory for merch (CBT_new_tile_from_file)\n");
                                        free(cur_const);
                                        CBT_free_tile(tile);
                                        cur_char = EOF;
                                    }
                                }
                            default:
                                break;
                        }
                        break;
                    case CBCT_PATH:
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
                        neighbor_loc = CBT_get_location_from_string(cur_neighbor);
                        CBT_set_single_connexion(tile, loc, neighbor_loc);
                        i=0;//reset index for the potential next neighbor
                    }
                }
            }
        }
        //handle line change
        if(cur_char=='\n'){
            if(on_new_node_line){
                cur_loc[i] = '\0';
                loc = CBT_get_location_from_string(cur_loc);
                CBT_get_node_from_loc(tile,loc)->pawn = NULL;
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

    fclose(tile_file);
    return tile;
}

void CBT_turn(Carc_Tile *tile, CBT_Turn_Type dir){
    Carc_Tile entry_tile = *tile;
    int transposition_factor=TILE_NR_LOCATIONS_ON_ONE_EDGE,
        previous_location;
    int i=0, j=0;
    switch(dir){
        case CCT_RIGHT:
            break;
        case CCT_LEFT:
            transposition_factor = -transposition_factor;
            break;
        case CCT_UPDOWN:
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

int CBT_tiles_connect_in(Carc_Tile t1, Carc_Tile_Location t1_node_loc, Carc_Tile t2, Carc_Tile_Location t2_node_loc){
    Carc_Tile_Node *node_t1 = CBT_get_node_from_loc(&t1, t1_node_loc),
                   *node_t2 = CBT_get_node_from_loc(&t2, t2_node_loc);

    if(node_t1==NULL || node_t2==NULL)
        return 1;
    if(CBC_types_connect(node_t1->node_type,node_t2->node_type)==1)
        return 1;

    return 0;
}

int CBT_tile_node_cmp(Carc_Tile_Node n1, Carc_Tile_Node n2){
    ///Comparison on structure level, without considering pawn info. Returns 0 if equality
    return CBC_construction_cmp(n1.construction,n1.node_type,n2.construction,n2.node_type);
}

int CBT_tile_cmp(Carc_Tile* t1, Carc_Tile* t2){
    ///Comparison on structure level, without considering pawn info
    ///TODO include comparison on rotations
    int result, i=0, j=0;
    if(t1==NULL || t2 == NULL){
        result = !(t1==t2);
    } else{
        result = CBT_tile_node_cmp(t1->center,t2->center);
        while(result==0 && i<TILE_NR_BORDER_LOCATIONS){
            result = CBT_tile_node_cmp(t1->border[i],t2->border[i]);
            i++;
        }

        //Test connexions equality
        if(result==0){
            for(i=0;i<TILE_NR_BORDER_LOCATIONS;i++){
                for(j=0;j<TILE_NR_BORDER_LOCATIONS;j++){
                    if(t1->border_connexions[i][j] != t2->border_connexions[i][j]){
                        result = 1;
                        break;
                    }
                }
                if(t1->center_connexions[i] != t2->center_connexions[i]){
                    result = 1;
                    break;
                }
            }
        }
    }
    return result;
}

void CBT_display_tile(Carc_Tile tile){
    //TODO display the connections between vertical corners
    //Print north edge
    printf(" ");
    printf("%d",tile.border[CTL_NORTH_WEST].node_type);
    if(tile.border_connexions[CTL_NORTH_WEST][CTL_NORTH])
        printf("-");
    else printf(" ");
    printf("%d",tile.border[CTL_NORTH].node_type);
    if(tile.border_connexions[CTL_NORTH][CTL_NORTH_EAST])
        printf("-");
    else printf(" ");
    printf("%d\n",tile.border[CTL_NORTH_EAST].node_type);

    if(tile.border_connexions[CTL_WEST_NORTH][CTL_NORTH_WEST])
        printf("/ ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_NORTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_NORTH_EAST][CTL_EAST_NORTH])
        printf(" \\");
    else printf("  ");
    printf("\n");

    //Print north west and east corners
    printf("%d",tile.border[CTL_WEST_NORTH].node_type);
    if(tile.border_connexions[CTL_WEST_NORTH][CTL_EAST_NORTH])
        printf("-----");
    else printf("     ");
    printf("%d\n",tile.border[CTL_EAST_NORTH].node_type);
    if(tile.border_connexions[CTL_WEST_NORTH][CTL_WEST])
        printf("| ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_NORTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_EAST_NORTH][CTL_EAST])
        printf(" |");
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
    if(tile.border_connexions[CTL_WEST][CTL_WEST_SOUTH])
        printf("| ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_SOUTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_EAST][CTL_EAST_SOUTH])
        printf(" |");
    else printf(" ");
    printf("\n");

    printf("%d",tile.border[CTL_WEST_SOUTH].node_type);
    if(tile.border_connexions[CTL_WEST_SOUTH][CTL_EAST_SOUTH])
        printf("-----");
    else printf("     ");
    printf("%d\n",tile.border[CTL_EAST_SOUTH].node_type);
    if(tile.border_connexions[CTL_WEST_SOUTH][CTL_SOUTH_WEST])
        printf("\\ ");
    else printf("  ");
    printf(" ");
    if(tile.center_connexions[CTL_SOUTH])
        printf("|");
    else printf(" ");
    printf(" ");
    if(tile.border_connexions[CTL_EAST_SOUTH][CTL_SOUTH_EAST])
        printf(" /");
    else printf("  ");
    printf("\n");

    printf(" ");
    printf("%d",tile.border[CTL_SOUTH_WEST].node_type);
    if(tile.border_connexions[CTL_SOUTH_WEST][CTL_SOUTH])
        printf("-");
    else printf(" ");
    printf("%d",tile.border[CTL_SOUTH].node_type);
    if(tile.border_connexions[CTL_SOUTH][CTL_SOUTH_EAST])
        printf("-");
    else printf(" ");
    printf("%d\n",tile.border[CTL_SOUTH_EAST].node_type);

    //Pawn info
    printf("Has pawns in: ");
    int i;
    for(i=0;i<TILE_NR_LOCATIONS-1;i++){
        if(tile.border[i].pawn!=NULL){
            printf("%d ",i);
        }
    }
    if(tile.center.pawn!=NULL){
        printf("%d",CTL_CENTER);
    }
    printf("\n");
}

int CBT_node_type_matches_pawn_type(Carc_Construction_Type const_type, Carc_Pawn_Type pawn_type){
    ///Function to tell if a pawn type can be played on a construction type.
    ///Other considerations/rules such as checking if a player can legally play the architect are considered
    ///at game level, as they imply to consider the construction as a whole (all the tiles building a same city
    ///for instance) and not only a location on a tile.
    int result=0;
    switch(pawn_type){
        case PAWN_NORMAL:
            if(const_type!=CBCT_PATH_END)
                result = 1;
            //NB: the garden case is special as normal pawn can be played in the field only. This is handled in the structure Carc_Garden
            break;
        case PAWN_DOUBLE:
            if(const_type!=CBCT_PATH_END)
                result = 1;
            break;
        case PAWN_ARCHITECT:
            if(const_type==CBCT_CITY || const_type==CBCT_PATH || const_type==CBCT_PATH_END)
                result = 1;
            break;
        case PAWN_BISHOP:
            if(const_type==CBCT_CLOISTER || const_type==CBCT_GARDEN)
                result = 1;
            break;
        case PAWN_PIG:
            if(const_type==CBCT_FIELD)
                result = 1;
            break;
    }
    return result;
}

int CBT_add_pawn(Carc_Pawn* pawn, Carc_Tile* tile, Carc_Tile_Location loc){
    int res=-1;
    Carc_Tile_Node* node = CBT_get_node_from_loc(tile,loc);
    if(node==NULL){
        fprintf(stderr, "ERROR: cannot add pawn to NULL tile node\n");
    } else if(CPPlayer_can_play_pawn(pawn->owner,pawn->type)==1
              && CBT_node_type_matches_pawn_type(node->node_type,pawn->type)==1){
        res = CPPawn_play(pawn);
        if(res==0){
            node->pawn = pawn;
        }
    }
    return res;
}

int CBT_rm_pawn(Carc_Tile* tile, Carc_Tile_Location loc){
    int res=0;
    Carc_Tile_Node* node = CBT_get_node_from_loc(tile,loc);
    if(node==NULL){
        fprintf(stderr, "ERROR: cannot remove pawn from NULL tile node\n");
        res = -1;
    } else{
        res = CPPawn_send_back(node->pawn);
        //Set that no pawn on the tile
        if(res==0){
            node->pawn = NULL;
        }
    }
    return res;
}

Carc_Tile_Node_List* CBTList_new(Carc_Tile_Node** pointer_to_node){
    if(pointer_to_node==NULL || *pointer_to_node==NULL){
        fprintf(stderr,"ERROR: cannot init node list from NULL\n");
        return NULL;
    }
    Carc_Tile_Node_List* res = malloc(sizeof(*res));
    res->node = *pointer_to_node;
    res->next = NULL;
    return res;
}

void CBTList_free(Carc_Tile_Node_List* l){
    //Free the allocated memory for a list.
    //NB: freeing the tile nodes themselves is to be done when freeing the playboard
    Carc_Tile_Node_List* next=NULL;
    while(l!=NULL){
        next = l->next;
        free(l);
        l = next;
    }
}

int CBTList_append(Carc_Tile_Node_List* l, Carc_Tile_Node** n){
    int fail=-1, already_in=-2;
    Carc_Tile_Node_List* current=l, *previous=NULL;
    Carc_Tile_Node_List* append=NULL;
    if(l==NULL){
        fprintf(stderr,"ERROR: Cannot add node to NULL (CTList_add_node)\n");
        return fail;
    }
    if(n==NULL || *n==NULL){
        fprintf(stderr,"ERROR: Cannot add NULL to a node list (CTList_add_node)\n");
        return fail;
    }
    while(current!=NULL){
        if(current->node==*n){
            return already_in;
        } else{
            previous = current;
            current = current->next;
        }
    }
    append = CBTList_new(n);
    if(append==NULL){
        fprintf(stderr,"ERROR: cannot allocate memory (CTList_add_node)\n");
        return fail;
    }
    previous->next = append;
    return 0;
}

Carc_Macro_Construct* CBTMacro_Construct_new(Carc_Tile_Node** n){
    if(n==NULL || *n==NULL){
        fprintf(stderr,"ERROR: cannot initiate macro construct from NULL node (CTMacroConstruct_new)\n");
        return NULL;
    }
    Carc_Macro_Construct* construct=malloc(sizeof(*construct));
    if(construct==NULL){
        fprintf(stderr,"ERROR: couldn't allocate memory (CTMacroConstruct_new)\n");
        return NULL;
    }
    Carc_Tile_Node* node=*n;
    Carc_Construction_Type type=node->node_type;
    if(type==CBCT_GARDEN){
        //Gardens do not need to be considered as such in constructions but as fields.
        //Gardens shall be handled separately
        type = CBCT_FIELD;
    }
    construct->type = type;
    construct->construct = (node->construction==NULL ? NULL : node->construction);
    construct->pawns = NULL;
    construct->nb_pawns = 0;
    if(type==CBCT_PATH_END){
        //Path ends represent the end of a path. As such they are not part of the path rim
        construct->rim = NULL;
    }else{
        construct->rim = CBTList_new(n);
    }


    return construct;
}

void CBTMacro_Construct_free(Carc_Macro_Construct* mc){
    int i=0;
    if(mc!=NULL){
        for(i=0;i<mc->nb_pawns;i++){
            //Pawns should be freed when sent back to players. When pawns are sent back, the construct should be
            //updated accordingly. Thus this loop is in theorie useless but here for safety.
            CPPawn_free_pawn(mc->pawns[i]);
        }
        free(mc->pawns);
        CBTList_free(mc->rim);
    }
    free(mc);
}

Carc_Macro_Construct_List* CBTMacro_Construct_List_new(Carc_Macro_Construct** mc){
    if(mc==NULL || *mc==NULL){
        fprintf(stderr,"ERROR: cannot initiate list of macro constructs from NULL (CTMacro_Construct_new_list)\n");
        return NULL;
    }
    Carc_Macro_Construct_List* constructs=malloc(sizeof(*constructs));
    if(constructs==NULL){
        fprintf(stderr,"ERROR: couldn't allocate memory (CTMacro_Construct_new_list)\n");
        return NULL;
    }
    constructs->construct = *mc;
    constructs->next = NULL;

    return constructs;
}

void CBTMacro_Construct_List_free(Carc_Macro_Construct_List* l){
    Carc_Macro_Construct_List* cur=l;
    while(cur!=NULL){
        CBTMacro_Construct_free(cur->construct);
        l = cur->next;
        free(cur);
        cur = l;
    }
}

int CBTMacro_add_pawn(Carc_Macro_Construct* c, Carc_Pawn** pawn){
    ///Add a pawn to the pawns attribute of the construct, if it is not already in.
    int i, pawn_not_in=1, null_input=-2, fail=-1, success=0, pawn_already_in=-3;
    if(c==NULL || pawn==NULL){
        fprintf(stderr,"WARNING: cannot have NULL input for CBTMacro_add_pawn\n");
        return null_input;
    }
    if(*pawn==NULL){
        return success;//No pawn to add
    }
    Carc_Pawn** for_realloc=NULL;
    //Check if the pawn is already in the construct
    for(i=0;i<c->nb_pawns;i++){
        if(c->pawns[i]==*pawn){
            pawn_not_in = 0;
            i = c->nb_pawns;
        }
    }
    if(pawn_not_in){
        for_realloc = realloc(c->pawns, (c->nb_pawns + 1)*sizeof(*(c->pawns)));
        if(for_realloc==NULL){
            fprintf(stderr,"ERROR; couldn't allocate space for adding new pawn (CBTMacro_add_pawn)\n");
            return fail;
        }
        c->pawns = for_realloc;
        c->pawns[c->nb_pawns] = *pawn;
        c->nb_pawns++;
        return success;
    } else{
        return pawn_already_in;
    }
}

int CBTMacro_Construct_add_node(Carc_Macro_Construct* c, Carc_Tile_Node** n){
    ///Add a node to the rim attribute of a construct if not already in.
    if(c==NULL || n==NULL || *n==NULL){
        fprintf(stderr,"ERROR: cannot have null input in CBTMacro_Construct_add_node\n");
        return -1;
    }
    if(CBC_types_connect((*n)->node_type,c->type)==0){
        fprintf(stderr,"ERROR: cannot add node to construct if their types doesn't connect CBTMacro_Construct_add_node\n");
        return -1;
    }
    int res=CBTList_append(c->rim,n);
    if(res==0){
        //Consider adding info of the construction itself such has lakes, flags, etc...
        if(c->type==CBCT_CITY)
            CBCCity_enrich_with(&(c->construct->city),
                                (*n)->construction->city);
        if(c->type==CBCT_PATH || c->type==CBCT_PATH_END)
            CBCPath_enrich_with(&(c->construct->path),(*n)->construction->path);
        //Add pawns should be done outside this function has this function shall be executed
        //before the player is allowed to play a pawn. It is however done here to, to secure
        //unexpected inputs
        CBTMacro_add_pawn(c,&((*n)->pawn));
    }
    return res;
}

int CBTMacro_node_in(Carc_Macro_Construct* construct, Carc_Tile_Node** n){
    if(construct==NULL || n==NULL)
        return 0;
    Carc_Tile_Node_List *current=construct->rim;
    int found=0;
    while(current!=NULL){
        if(current->node==*n){
            found = 1;
            current = NULL;
        } else{
            current = current->next;
        }
    }
    return found;
}

Carc_Macro_Construct* CBTMacro_get_node_construct(Carc_Macro_Construct_List* l, Carc_Tile_Node** n){
    if(n==NULL)
        return NULL;
    Carc_Macro_Construct_List *current=l;
    Carc_Macro_Construct* res=NULL;
    while(current!=NULL){
        if(CBTMacro_node_in(current->construct,n)){
            res = current->construct;
            current = NULL;
        } else{
            current = current->next;
        }
    }
    return res;
}

Carc_Macro_Construct_List* CBTMacro_add_to_list(Carc_Macro_Construct_List* l, Carc_Macro_Construct** c){
    //Create new elt to duplicate and replace current head
    if(c==NULL || *c==NULL){
        fprintf(stderr,"ERROR: adding NULL construct to a list is forbidden (CBTMacro_add_to_list)\n");
        return NULL;
    }
    if(l==NULL){
        l = malloc(sizeof(*l));
        l->construct = *c;
        l->next = NULL;
        return l;
    }
    Carc_Macro_Construct_List* new_elt = CBTMacro_Construct_List_new(&(l->construct));
    if(new_elt==NULL){
        fprintf(stderr,"ERROR: Couldn't allocate memory (CBTMacro_add_to_list)\n");
        return NULL;
    }
    new_elt->next = l->next;
    //update head elt and insert the duplicate as second elt
    l->construct = *c;
    l->next = new_elt;
    return l;
}

Carc_Macro_Construct_List* CBTMacro_get_tile_macro_constructions(Carc_Tile* tile){
    ///TODO: handle gardens: 2 constructions needed: garden and field
    Carc_Macro_Construct_List* constructs=NULL;
    Carc_Tile_Node *cur_node, *j_node,
                   *center=CBT_get_node_from_loc(tile,CTL_CENTER);
    Carc_Tile_Node** pcur_node=&cur_node;
    Carc_Macro_Construct *j_construct=NULL,
                         *new_const=NULL;
    int i=0, j=0, added_to_const=0, center_added_to_const=0, nb_constructs=0;
    if(tile!=NULL){
        //Init lists
        cur_node = CBT_get_node_from_loc(tile,0);
        new_const = CBTMacro_Construct_new(pcur_node);
        nb_constructs++;
        constructs=CBTMacro_Construct_List_new(&new_const);

        //Walk all locations on the tile and add the nodes to the construct they belong to.
        //Create a new construct if the node doesn't belong to an existing one
        for(i=1;i<TILE_NR_BORDER_LOCATIONS;i++){
            cur_node = CBT_get_node_from_loc(tile,i);
            //Check if the node belongs to an an existing construct. Loop stops at i as we walk the locations in increasing order.
            //Hence no construct for nodes after i, since not considered.
            added_to_const = 0;
            j = 0;
            while(j<i && !added_to_const){
                if(tile->border_connexions[i][j]==1){
                    j_node = CBT_get_node_from_loc(tile, j);
                    j_construct = CBTMacro_get_node_construct(constructs,&j_node);
                    if(j_node->node_type==cur_node->node_type){
                        CBTMacro_Construct_add_node(j_construct,&cur_node);
                    }
                    added_to_const = 1;
                }
                j++;
            }
            //Create new construct if necessary
            if(!added_to_const){
                //Check if can be added to an existing construction through the center
                //E.g. a path from east to west is connected through center but has no direct connection east-west
                if(tile->center_connexions[i]==1 && center_added_to_const){
                    j_construct = CBTMacro_get_node_construct(constructs,&center);
                    CBTMacro_Construct_add_node(j_construct,&cur_node);
                } else{//A new construction is needed
                    new_const = CBTMacro_Construct_new(&cur_node);
                    nb_constructs++;
                    CBTMacro_add_to_list(constructs,&new_const);
                }
            }
            //Add center to i's constructions if linked and center is not already in a construction
            if(!center_added_to_const
               && tile->center_connexions[i]==1
               && CBC_types_connect(center->node_type,cur_node->node_type)==1){
                CBTMacro_Construct_add_node(CBTMacro_get_node_construct(constructs,pcur_node)
                                       ,&center);
                center_added_to_const = 1;
            }
        }
        //Consider stand alone center (e.g. cloister)
        if(!center_added_to_const){
            new_const = CBTMacro_Construct_new(&center);
            nb_constructs++;
            CBTMacro_add_to_list(constructs,&new_const);
        }
    }
    return constructs;
}

