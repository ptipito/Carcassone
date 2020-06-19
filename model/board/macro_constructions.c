#include "model/board/macro_constructions.h"

Carc_Macro_Construct* CBMC_new(Carc_Tile_Node** n){
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

void CBMC_free(Carc_Macro_Construct* mc){
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

Carc_Macro_Construct_List* CBMCList_new(Carc_Macro_Construct** mc){
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

void CBMCList_free(Carc_Macro_Construct_List* l){
    Carc_Macro_Construct_List* cur=l;
    while(cur!=NULL){
        CBMC_free(cur->construct);
        l = cur->next;
        free(cur);
        cur = l;
    }
}

int CBMC_add_pawn(Carc_Macro_Construct* c, Carc_Pawn** pawn){
    ///Add a pawn to the pawns attribute of the construct, if it is not already in.
    int i, pawn_not_in=1, null_input=-2, fail=-1, success=0, pawn_already_in=-3;
    if(c==NULL || pawn==NULL){
        fprintf(stderr,"WARNING: cannot have NULL input for CBMC_add_pawn\n");
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
            fprintf(stderr,"ERROR; couldn't allocate space for adding new pawn (CBMC_add_pawn)\n");
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

int CBMC_add_node(Carc_Macro_Construct* c, Carc_Tile_Node** n){
    ///Add a node to the rim attribute of a construct if not already in.
    if(c==NULL || n==NULL || *n==NULL){
        fprintf(stderr,"ERROR: cannot have null input in CBMC_add_node\n");
        return -1;
    }
    if(CBC_types_connect((*n)->node_type,c->type)==0){
        fprintf(stderr,"ERROR: cannot add node to construct if their types doesn't connect CBMC_add_node\n");
        return -1;
    }
    int res=0;
    if((*n)->node_type!=CBCT_PATH_END){
        //If a node is a path end, it neither link the current node to another node of the same construct
        //nor represents a rim info -->Do not append to rim, but consider potential other info (e.g. lake)
        res = CBTList_append(c->rim,n);
    }
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
        CBMC_add_pawn(c,&((*n)->pawn));
    }
    return res;
}

int CBMC_node_in(Carc_Macro_Construct* construct, Carc_Tile_Node** n){
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

Carc_Macro_Construct* CBMC_get_node_construct(Carc_Macro_Construct_List* l, Carc_Tile_Node** n){
    if(n==NULL)
        return NULL;
    Carc_Macro_Construct_List *current=l;
    Carc_Macro_Construct* res=NULL;
    while(current!=NULL){
        if(CBMC_node_in(current->construct,n)){
            res = current->construct;
            current = NULL;
        } else{
            current = current->next;
        }
    }
    return res;
}

Carc_Macro_Construct_List* CBMCList_append(Carc_Macro_Construct_List* l, Carc_Macro_Construct** c){
    if(c==NULL || *c==NULL){
        fprintf(stderr,"ERROR: adding NULL construct to a list is forbidden (CBMCList_append)\n");
        return NULL;
    }
    if(l==NULL){
        l = malloc(sizeof(*l));
        l->construct = *c;
        l->next = NULL;
        return l;
    }
    //Create new elt to duplicate and replace current head
    Carc_Macro_Construct_List* new_elt = CBMCList_new(&(l->construct));
    if(new_elt==NULL){
        fprintf(stderr,"ERROR: Couldn't allocate memory (CBMCList_append)\n");
        return NULL;
    }
    new_elt->next = l->next;
    //update head elt and insert the duplicate as second elt
    l->construct = *c;
    l->next = new_elt;
    return l;
}

Carc_Macro_Construct_List* CBMC_get_tile_macro_constructions(Carc_Tile* tile){
    ///TODO: handle gardens
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
        new_const = CBMC_new(pcur_node);
        nb_constructs++;
        constructs=CBMCList_new(&new_const);

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
                    j_construct = CBMC_get_node_construct(constructs,&j_node);
                    if(j_node->node_type==cur_node->node_type){
                        CBMC_add_node(j_construct,&cur_node);
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
                    j_construct = CBMC_get_node_construct(constructs,&center);
                    CBMC_add_node(j_construct,&cur_node);
                } else{//A new construction is needed
                    new_const = CBMC_new(&cur_node);
                    nb_constructs++;
                    CBMCList_append(constructs,&new_const);
                }
            }
            //Add center to i's constructions if linked and center is not already in a construction
            if(!center_added_to_const
               && tile->center_connexions[i]==1
               && CBC_types_connect(center->node_type,cur_node->node_type)==1){
                CBMC_add_node(CBMC_get_node_construct(constructs,pcur_node)
                                       ,&center);
                //In case of a path end for a path cross, the path end info has to be considered for each path
                if(center->node_type!=CBCT_PATH_END)
                    center_added_to_const = 1;
            }
        }
        //Consider stand alone center (e.g. cloister)
        if(!center_added_to_const){
            new_const = CBMC_new(&center);
            nb_constructs++;
            CBMCList_append(constructs,&new_const);
        } else {
            //Rm center from rim. Why is explained below
            //Center was added to a rim but to build the rim of a construct that goes through the center
            //but center can never be part of the rim, as the rim element of a construct represents the
            //nodes on the edge of the construct that can lead to a construct extension (i.e. that are on the
            //of a tile). Center can never lead to such an extension, except during the initial build of the
            //tile constructs OR for cloisters/garden who are composed of one node only (their completion is not
            //handled through the rim.
            CBMC_rm_from_rim(CBMC_get_node_construct(constructs,&center),&center);
        }
    }
    return constructs;
}

int CBMC_rm_from_rim(Carc_Macro_Construct* c, Carc_Tile_Node** n){
    if(pointer_is_not_null(c,1)){
        return CBTList_rm(&(c->rim),n);
    }
    return FUNC_FAIL;
}

int CBMC_rm_list_from_rim(Carc_Macro_Construct* c, Carc_Tile_Node_List* rm){
    if(pointer_is_not_null(c,1)){
        return CBTList_rm_nodes(&(c->rim),rm);
    }
    return FUNC_FAIL;
}

int CBMC_transfer_rim(Carc_Macro_Construct* into, Carc_Macro_Construct* from){
    if(pointer_is_null(into,0)){
        fprintf(stderr,"ERROR: cannot transfer rim in NULL consturct\n");
        return FUNC_FAIL;
    }
    if(pointer_is_null(from,0)){
        return FUNC_SUCCESS;//No change to be performed
    }
    return CBTList_append_list(into->rim,from->rim);
}

int CBMC_enrich_with(Carc_Macro_Construct* to_enrich, Carc_Macro_Construct* new_info){
    ///Function to merge the info two constructs of a Macro Construct. Only the construct info
    ///is considered. Merging other informations (rims and pawns e.g.) is done in another function
    ///as it requires additional info.
    if(pointer_is_null(to_enrich,1)){
        return FUNC_FAIL;
    }
    if(pointer_is_null(new_info,0)){
        return FUNC_SUCCESS;//Nothing to be done
    }
    if(!CBC_types_connect(to_enrich->type,new_info->type)){
        return FUNC_FAIL;
    }
    int res=FUNC_SUCCESS;
    switch(to_enrich->type){
        case CBCT_CITY:
            res = CBCCity_enrich_with(&(to_enrich->construct->city), new_info->construct->city);
            break;
        case CBCT_PATH:
            res = CBCPath_enrich_with(&(to_enrich->construct->path), new_info->construct->path);
            break;
        case CBCT_PATH_END:
            res = CBCPath_enrich_with(&(to_enrich->construct->path), new_info->construct->path);
            break;
        case CBCT_FIELD://Nothing to be done
            break;
        default:
            fprintf(stderr,"ERROR: Try to merge non mergeable constructs of type %d (CGG_merge_constructs)\n",to_enrich->type);
            res = FUNC_FAIL;
            break;
    }
    return res;
}

int CBMC_merge_const(Carc_Macro_Construct* into, Carc_Macro_Construct** from,
                         Carc_Tile_Node_List* into_connect_nodes, Carc_Tile_Node_List* from_connect_nodes){
    ///Take information from the construct \from and enrich the construct \into with
    ///this information. Once all the \from info has been imported into \into, \from is
    ///freed (as it is now a part of \into).
    ///The nodes list are used to update the rim of into. The connecting nodes should be in the same order
    ///in both list (into_connect_nodes[0] corresponds to from_connect_nodes[0], etc...)
    int fail=FUNC_FAIL, success=FUNC_SUCCESS, res=success, i=0;
    Carc_Macro_Construct* from_construct=NULL;
    if(pointer_is_null(into,1)){
        fprintf(stderr,"ERROR: first input cannot be NULL (CGG_merge_constructs)\n");
        return fail;
    }
    if(pointer_has_null_value((void**)from,0)){ //Nothing to be done
        return success;
    }
    from_construct = *from;
    if(into==from_construct){ //Cannot merge a construct into itself
        return success;
    }
    if(!CBC_types_connect(into->type,from_construct->type)){
        fprintf(stderr,"ERROR: Construct have none matching types (CGG_merge_constructs)\n");
        return fail;
    } else{
        //Update construct info
        res = CBMC_enrich_with(into,from_construct);
        if(res==success){
            //Update pawns
            for(i=0;i<from_construct->nb_pawns;i++){
                if(CBMC_add_pawn(into,&(from_construct->pawns[i]))!=0){
                    res = fail;
                }
            }
            //Update rim
            if(!(CBMC_rm_list_from_rim(into,into_connect_nodes)==FUNC_SUCCESS
                  && CBMC_rm_list_from_rim(from_construct,from_connect_nodes)==FUNC_SUCCESS
                  && CBMC_transfer_rim(into,from_construct)==FUNC_SUCCESS)
               ){
               res = FUNC_FAIL;
            }
            //Free the now irrelevant construction
            CBMC_free(from_construct);
        }
    }
    return res;
}

