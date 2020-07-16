#include "tests/board/test_macro_constructions.h"


void test_macro_const_construct_new(){
    printf("test_macro_const_construct_new results: ");
    Carc_Tile_Node *node=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                   *node2=CBT_new_node(CBCT_CITY,NULL),
                   *null_node=NULL;
    Carc_Macro_Construct* mc=CBMC_new(&node);
    Carc_Macro_Construct* mc2=CBMC_new(&node2);
    printf("%d",CBMC_new(NULL)==NULL);
    printf("%d",CBMC_new(&null_node)==NULL);
    printf("%d",mc!=NULL
                && mc->type==node->node_type
                && CBC_construction_cmp(mc->construct,mc->type,node->construction,node->node_type)==0
                && mc->nb_pawns==0 && mc->pawns==NULL
                && mc->rim!=NULL && mc->rim->node==node
           );
    printf("%d",mc2!=NULL && mc2->construct==NULL);

    CBMC_free(mc);
    CBT_free_node(node);
    CBT_free_node(node2);
}

void test_macro_const_construct_list_new(){
    printf("test_macro_const_construct_list_new results: ");
    Carc_Tile_Node* n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE));
    Carc_Macro_Construct* construct=CBMC_new(&n),
                  *null_construct=NULL;
    Carc_Macro_Construct_List* l=CBMCList_new(&construct);

    printf("%d",CBMCList_new(NULL)==NULL);
    printf("%d",CBMCList_new(&null_construct)==NULL);
    printf("%d",l!=NULL && l->construct==construct && l->next==NULL);

    CBMCList_free(l);
    CBT_free_node(n);
}

void test_macro_const_construct_add_pawn(){
    printf("test_macro_const_construct_add_pawn results: ");
    Carc_Player *play1=CPPlayer_init_player(PLAYER_1,CPC_BLACK),
                *play2=CPPlayer_init_player(PLAYER_2,CPC_BLUE);
    Carc_Pawn *pawn1=CPPawn_new_pawn(play1,PAWN_NORMAL),
              *pawn2=CPPawn_new_pawn(play2,PAWN_NORMAL),
              *null_pawn=NULL;
    Carc_Tile_Node* n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE));
    Carc_Macro_Construct* construct=CBMC_new(&n);

    printf("%d\n",CBMC_add_pawn(NULL,NULL)==-2);
    printf("%d\n",CBMC_add_pawn(construct,NULL)==-2);

    printf("%d\n",CBMC_add_pawn(construct,&null_pawn)==0);
    printf("%d",CBMC_add_pawn(construct,&pawn1)==0
           && construct->nb_pawns==1
           && construct->pawns[0]==pawn1);
    printf("%d",CBMC_add_pawn(construct,&pawn1)==-3
           && construct->nb_pawns==1
           && construct->pawns[0]==pawn1);
    printf("%d",CBMC_add_pawn(construct,&pawn2)==0
           && construct->nb_pawns==2
           && construct->pawns[0]==pawn1 && construct->pawns[1]==pawn2);
    printf("%d",CBMC_add_pawn(construct,&pawn2)==-3
           && construct->nb_pawns==2
           && construct->pawns[0]==pawn1 && construct->pawns[1]==pawn2);


    CBMC_free(construct);
    CBT_free_node(n);
    CPPlayer_free_player(play1);
    CPPlayer_free_player(play2);
    CPPawn_free_pawn(pawn1);
    CPPawn_free_pawn(pawn2);
}

void test_macro_const_construct_add_node(){
    printf("test_macro_const_construct_add_node results: ");
    Carc_Player *play1=CPPlayer_init_player(PLAYER_1,CPC_BLACK),
                *play2=CPPlayer_init_player(PLAYER_2,CPC_BLUE);
    Carc_Pawn *pawn1=CPPawn_new_pawn(play1,PAWN_NORMAL),
              *pawn2=CPPawn_new_pawn(play2,PAWN_NORMAL);
    Carc_Tile_Node *n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n2=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n3=CBT_new_node(CBCT_PATH,CBC_new_city(0,0,CCM_NONE)),
                    *n4=CBT_new_node(CBCT_CITY,CBC_new_city(0,1,CCM_CORN)),
                    *n5=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n6=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n7=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *null_node=NULL;
    Carc_Macro_Construct* construct=CBMC_new(&n);
    n5->pawn = pawn1;
    n6->pawn = pawn2;
    n7->pawn = pawn1;

    printf("%d\n",CBMC_add_node(NULL,NULL)==-1);
    printf("%d\n",CBMC_add_node(construct,NULL)==-1);
    printf("%d\n",CBMC_add_node(construct,&null_node)==-1);

    printf("%d",CBMC_add_node(construct,&n3)==-1);
    printf("%d",CBMC_add_node(construct,&n2)==0
           && construct->nb_pawns==0
           && construct->pawns==NULL
           && construct->construct->city.is_cathedral==0
           && construct->construct->city.nb_flags==0
           && construct->construct->city.nb_merchandises==0);
    printf("%d",CBMC_add_node(construct,&n4)==0
           && construct->nb_pawns==0
           && construct->pawns==NULL
           //&& construct->construct->city.is_cathedral==1
           //&& construct->construct->city.nb_flags==0
           //&& construct->construct->city.nb_merchandises==1
           //&& construct->construct->city.merchandises[0]==CCM_CORN
           );
    printf("%d",CBMC_add_node(construct,&n5)==0
           && construct->nb_pawns==1
           && construct->pawns[0]==pawn1
           && construct->construct->city.is_cathedral==1
           && construct->construct->city.nb_flags==0
           && construct->construct->city.nb_merchandises==1
           && construct->construct->city.merchandises[0]==CCM_CORN);
    printf("%d",CBMC_add_node(construct,&n6)==0
           && construct->nb_pawns==2
           && construct->pawns[0]==pawn1 && construct->pawns[1]==pawn2
           && construct->construct->city.is_cathedral==1
           && construct->construct->city.nb_flags==0
           && construct->construct->city.nb_merchandises==1
           && construct->construct->city.merchandises[0]==CCM_CORN);
    printf("%d",CBMC_add_node(construct,&n7)==0
           && construct->nb_pawns==2
           && construct->pawns[0]==pawn1 && construct->pawns[1]==pawn2
           && construct->construct->city.is_cathedral==1
           && construct->construct->city.nb_flags==0
           && construct->construct->city.nb_merchandises==1
           && construct->construct->city.merchandises[0]==CCM_CORN);


    CBMC_free(construct);
    CBT_free_node(n);
    CBT_free_node(n2);
    CBT_free_node(n3);
    CBT_free_node(n4);
    CBT_free_node(n5);
    CBT_free_node(n6);
    CBT_free_node(n7);
    CPPlayer_free_player(play1);
    CPPlayer_free_player(play2);
    CPPawn_free_pawn(pawn1);
    CPPawn_free_pawn(pawn2);
}

void test_macro_const_node_in(){
    printf("test_macro_const_node_in results: ");
    Carc_Tile_Node *n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n2=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *null_node=NULL;
    Carc_Macro_Construct* construct=CBMC_new(&n);

    printf("%d",CBMC_node_in(NULL,NULL)==0);
    printf("%d",CBMC_node_in(construct,NULL)==0);
    printf("%d",CBMC_node_in(construct,&null_node)==0);
    printf("%d",CBMC_node_in(construct,&n)==1);
    printf("%d",CBMC_node_in(construct,&n2)==0);
    CBMC_add_node(construct,&n2);
    printf("%d",CBMC_node_in(construct,&n2)==1);

    CBMC_free(construct);
    CBT_free_node(n);
}

void test_macro_const_get_node_construct(){
    printf("test_macro_const_get_node_construct results: ");
    Carc_Tile_Node *n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n2=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n3=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *null_node=NULL;
    Carc_Macro_Construct *c1=CBMC_new(&n),
                         *c2=CBMC_new(&n2);
    Carc_Macro_Construct_List* l=CBMCList_new(&c1);
    l->next = malloc(sizeof(*(l->next)));
    l->next->construct = c2;
    l->next->next = NULL;

    printf("%d",CBMC_get_node_construct(NULL,NULL)==NULL);
    printf("%d",CBMC_get_node_construct(NULL,&n)==NULL);
    printf("%d",CBMC_get_node_construct(l,&null_node)==NULL);
    printf("%d",CBMC_get_node_construct(l,&n)==c1);
    printf("%d",CBMC_get_node_construct(l,&n2)==c2);
    printf("%d",CBMC_get_node_construct(l,&n3)==NULL);

    CBMCList_free(l);
    CBT_free_node(n);
    CBT_free_node(n2);
    CBT_free_node(n3);
}

void test_macro_const_add_to_list(){
    printf("test_macro_const_add_to_list results: ");
    Carc_Tile_Node *n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n2=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE));
    Carc_Macro_Construct *c1=CBMC_new(&n),
                         *c2=CBMC_new(&n2),
                         *null_construct=NULL;
    Carc_Macro_Construct_List* test_res=NULL;

    printf("%d\n",CBMCList_append(NULL,NULL)==NULL);
    printf("%d\n",CBMCList_append(NULL,&null_construct)==NULL);
    test_res = CBMCList_append(NULL,&c1);
    printf("%d",test_res!=NULL && test_res->construct==c1 && test_res->next==NULL);
    CBMCList_free(test_res);

    c1 = CBMC_new(&n);
    Carc_Macro_Construct_List *l=CBMCList_new(&c1);
    test_res = CBMCList_append(l,&c2);
    printf("%d\n", test_res!=NULL
           && test_res->construct==c2
           && test_res->next->construct==c1
           && test_res->next->next==NULL
           );
    //Test cannot add twice the same construct
    test_res = CBMCList_append(l,&c2);
    printf("%d", test_res!=NULL
           && test_res->construct==c2
           && test_res->next->construct==c1
           && test_res->next->next==NULL
           );

    CBMCList_free(l);
    CBT_free_node(n);
    CBT_free_node(n2);
}

void test_macro_const_get_tile_constructions(){
    ///TODO: add tests with Garden and Path ends when resources available
    printf("test_macro_const_get_tile_constructions results: ");
    char *start_tile_str=CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_str=CBT_get_tile_file_path("cloister_path.txt");
    Carc_Tile *start_tile=CBT_new_tile_from_file(start_tile_str),
              *cloister_tile=CBT_new_tile_from_file(cloister_tile_str);
    Carc_Macro_Construct_List* test_res=NULL;
    Carc_Tile_Node *center=CBT_get_node_from_loc(start_tile,CTL_CENTER);

    printf("%d",CBMC_get_tile_macro_constructions(NULL)==NULL);
    test_res = CBMC_get_tile_macro_constructions(start_tile);
    printf("%d",test_res!=NULL && test_res->construct->type==CBCT_FIELD
           && test_res->next!=NULL && test_res->next->construct->type==CBCT_PATH
           && test_res->next->next!=NULL && test_res->next->next->construct->type==CBCT_FIELD
           && test_res->next->next->next!=NULL && test_res->next->next->next->construct->type==CBCT_CITY
           && test_res->next->next->next->next==NULL
           && CBMC_get_node_construct(test_res,&center)==NULL
           );
    CBMCList_free(test_res);

    test_res = CBMC_get_tile_macro_constructions(cloister_tile);
    center = CBT_get_node_from_loc(cloister_tile,CTL_CENTER);
    printf("%d",test_res!=NULL
           && test_res->construct->type==CBCT_CLOISTER && test_res->construct->construct==NULL && test_res->construct->rim==NULL
           && test_res->next!=NULL && test_res->next->construct->type==CBCT_PATH && test_res->next->construct->rim!=NULL
           && test_res->next->next!=NULL && test_res->next->next->construct->type==CBCT_FIELD && test_res->next->next->construct->rim!=NULL
           && test_res->next->next->next==NULL
           && CBMC_get_node_construct(test_res,&center)==NULL
           );

    free(start_tile_str);
    free(cloister_tile_str);
    CBT_free_tile(start_tile);
    CBT_free_tile(cloister_tile);
    CBMCList_free(test_res);
}

void test_macro_const_rm_from_rim(){
    printf("test_macro_const_rm_from_rim results: ");
    Carc_Macro_Construct c_val, *c=&c_val;
    Carc_Tile_Node_List rim_val;
    Carc_Tile_Node n1_val, n2_val, n3_val, n4_val,
                *n1=&n1_val, *n2=&n2_val, *n3=&n3_val, *n4=&n4_val,
                *null_node=NULL;
    Carc_Tile_Node_List q1_val, q2_val, *q1=&q1_val, *q2=&q2_val;
    c->rim = &rim_val;
    c->rim->node = n1;
    c->rim->next = q1;
    q1->node = n2;
    q1->next = q2;
    q2->node = n3;
    q2->next = NULL;

    printf("%d",CBMC_rm_from_rim(NULL,NULL)==FUNC_FAIL);
    printf("%d",CBMC_rm_from_rim(NULL,&null_node)==FUNC_FAIL);
    printf("%d",CBMC_rm_from_rim(c,NULL)==FUNC_FAIL);
    printf("%d",CBMC_rm_from_rim(c,&null_node)==FUNC_FAIL);
    printf("%d",CBMC_rm_from_rim(c,&n4)==FUNC_SUCCESS
                && c->rim->node==n1
                && c->rim->next->node==n2
                && c->rim->next->next->node==n3
                && c->rim->next->next->next==NULL
    );
    printf("%d",CBMC_rm_from_rim(c,&n3)==FUNC_SUCCESS
                && c->rim->node==n1
                && c->rim->next->node==n2
                && c->rim->next->next==NULL
    );
    printf("%d",CBMC_rm_from_rim(c,&n1)==FUNC_SUCCESS
                && c->rim->node==n2
                && c->rim->next==NULL
    );
}

void test_macro_const_rm_list_from_rim(){
    printf("test_macro_const_rm_list_from_rim results: ");
    Carc_Macro_Construct c_val, *c=&c_val;
    Carc_Tile_Node_List rim_val, rm_val, *rm=&rm_val;
    Carc_Tile_Node n1_val, n2_val, n3_val, n4_val,
                *n1=&n1_val, *n2=&n2_val, *n3=&n3_val, *n4=&n4_val;
    Carc_Tile_Node_List q1_val, q2_val, *q1=&q1_val, *q2=&q2_val;
    Carc_Tile_Node_List rm_q1_val, rm_q2_val, *rm_q1=&rm_q1_val, *rm_q2=&rm_q2_val;
    c->rim = &rim_val;
    c->rim->node = n1;
    c->rim->next = q1;
    q1->node = n2;
    q1->next = q2;
    q2->node = n3;
    q2->next = NULL;
    rm->node = n2;
    rm->next = rm_q1;
    rm_q1->node = n4;
    rm_q1->next = rm_q2;
    rm_q2->node = n3;
    rm_q2->next = NULL;

    printf("%d",CBMC_rm_list_from_rim(NULL,NULL)==FUNC_FAIL);
    printf("%d",CBMC_rm_list_from_rim(c,NULL)==FUNC_SUCCESS);
    //Test when a noded to remove is not in the construct's rim
    printf("%d",CBMC_rm_list_from_rim(c,rm)==FUNC_SUCCESS
                && c->rim->node==n1
                && c->rim->next==NULL
    );
    //Test if all nodes ar in the rim
    c->rim = &rim_val;
    c->rim->node = n1;
    c->rim->next = q1;
    q1->node = n2;
    q1->next = q2;
    q2->node = n3;
    q2->next = NULL;
    rm->node = n2;
    rm->next = rm_q2;
    rm_q2->node = n3;
    rm_q2->next = NULL;
    printf("%d",CBMC_rm_list_from_rim(c,rm)==FUNC_SUCCESS
                && c->rim->node==n1
                && c->rim->next==NULL
    );
}

void test_macro_const_transfer_rim(){
    printf("test_macro_const_transfer_rim results: ");
    Carc_Macro_Construct from_val, into_val, *from=&from_val, *into=&into_val;
    Carc_Tile_Node_List rim_from_val, rim_into_val;
    Carc_Tile_Node n1_val, n2_val, n3_val, n4_val,
                *n1=&n1_val, *n2=&n2_val, *n3=&n3_val, *n4=&n4_val;
    Carc_Tile_Node_List q1_val, q2_val, q3_val, *q1=&q1_val, *q2=&q2_val, *q3=&q3_val;
    //init rims
    into->rim = &rim_into_val;
    into->rim->node = n1;
    into->rim->next = q1;
    q1->node = n2;
    q1->next = NULL;
    from->rim = &rim_from_val;
    from->rim->node = n3;
    from->rim->next = q2;
    q2->node = n4;
    q2->next = q3;
    //q3 is used to try to transfer a node in a rim that already has this node
    q3->node = n1;
    q3->next = NULL;

    //Test with null inputs
    printf("%d",CBMC_transfer_rim(NULL,NULL)==FUNC_FAIL);
    printf("%d",CBMC_transfer_rim(NULL,from)==FUNC_FAIL);
    printf("%d",CBMC_transfer_rim(into,NULL)==FUNC_SUCCESS
                && into->rim->node==n1
                && into->rim->next->node==n2
                && into->rim->next->next==NULL
           );

    //Test if no null input but one node is in both rim
    printf("%d",CBMC_transfer_rim(into,from)==FUNC_FAIL
                && into->rim->node==n1
                && into->rim->next->node==n2
                && into->rim->next->next->node==n3
                && into->rim->next->next->next->node==n4
                && into->rim->next->next->next->next==NULL
           );

    //Test when the whole rim can be successfully imported
    into->rim = &rim_into_val;
    into->rim->node = n1;
    into->rim->next = q1;
    q1->node = n2;
    q1->next = NULL;
    from->rim = &rim_from_val;
    from->rim->node = n3;
    from->rim->next = q2;
    q2->node = n4;
    q2->next = NULL;
    printf("%d",CBMC_transfer_rim(into,from)==FUNC_SUCCESS
                && into->rim->node==n1
                && into->rim->next->node==n2
                && into->rim->next->next->node==n3
                && into->rim->next->next->next->node==n4
                && into->rim->next->next->next->next==NULL
           );
}

void test_macro_const_enrich_with(){
    printf("test_macro_const_enrich_with results: ");
    Carc_Macro_Construct enrich_val, from_val, *enrich=&enrich_val, *from=&from_val;

    //Test that not connecting types do not enrich each other
    enrich->type = CBCT_CITY;
    from->type = CBCT_PATH;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);
    from->type = CBCT_FIELD;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);
    from->type = CBCT_CLOISTER;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);
    enrich->type = CBCT_PATH;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);
    from->type = CBCT_FIELD;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);
    enrich->type = CBCT_FIELD;
    from->type = CBCT_CLOISTER;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_FAIL);

    enrich->type = CBCT_CITY;
    from->type = CBCT_CITY;
    //Test null inputs
    printf("%d",CBMC_enrich_with(NULL,NULL)==FUNC_FAIL);
    printf("%d",CBMC_enrich_with(enrich,NULL)==FUNC_SUCCESS);

    //Test enrich when fine
    enrich->construct = CBC_new_city(0,1,CCM_NONE);
    from->construct = CBC_new_city(1,0,CCM_CORN);
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_SUCCESS
           && enrich->construct->city.is_cathedral==1
           && enrich->construct->city.merchandises[0]==CCM_CORN
           && enrich->construct->city.nb_merchandises==1
           && enrich->construct->city.nb_flags==1
           );
    CBC_free_city(enrich->construct);
    CBC_free_city(from->construct);
    enrich->type = CBCT_PATH;
    from->type = CBCT_PATH;
    enrich->construct = CBC_new_path(0);
    from->construct = CBC_new_path(0);
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_SUCCESS
           && enrich->construct->path.has_lake==0
           );
    free(from->construct);
    from->construct = CBC_new_path(1);
    from->type = CBCT_PATH_END;
    printf("%d",CBMC_enrich_with(enrich,from)==FUNC_SUCCESS
           && enrich->construct->path.has_lake==1
           );

    free(enrich->construct);
}

void test_macro_const_merge(){
    printf("test_macro_const_merge results: ");
    char *start_tile_path=CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_path=CBT_get_tile_file_path("cloister_path.txt");
    Carc_Playboard_Node *start_node=CBP_new_playboard_node(CBT_new_tile_from_file(start_tile_path),CBP_Location_new(0,0)),
                        *cloister_node=CBP_new_playboard_node(CBT_new_tile_from_file(cloister_tile_path),CBP_Location_new(0,0));
    CBT_turn(cloister_node->node,CTTT_RIGHT);
    Carc_Tile_Node *start_path_connect = CBT_get_node_from_loc(start_node->node,CTL_EAST),
                    *cloister_path_connect = CBT_get_node_from_loc(cloister_node->node,CTL_WEST);
    //Add info on cloister_path to check all operations of the merge
    cloister_path_connect->construction->path.has_lake = 3;

    Carc_Macro_Construct_List *start_constructs=CBMC_get_tile_macro_constructions(start_node->node),
                              *cloister_constructs=CBMC_get_tile_macro_constructions(cloister_node->node);
    Carc_Macro_Construct *start_path=start_constructs->next->construct,
                         *cloister_path=cloister_constructs->next->construct,
                         *null_construct=NULL;
    //Tests null inputs
    printf("%d",CBMC_merge_const(NULL,NULL,NULL,NULL)==FUNC_FAIL);
    printf("%d",CBMC_merge_const(start_path,NULL,NULL,NULL)==FUNC_SUCCESS);
    printf("%d",CBMC_merge_const(start_path,&null_construct,NULL,NULL)==FUNC_SUCCESS);
    printf("%d\n",CBMC_merge_const(start_path,&null_construct,NULL,NULL)==FUNC_SUCCESS);



    Carc_Tile_Node_List *into_connect=CBTList_new(&start_path_connect),
                        *from_connect=CBTList_new(&cloister_path_connect);
    Carc_Pawn *pawn=CPPawn_new_pawn(CPPlayer_init_player(PLAYER_1,CPC_BLACK),PAWN_NORMAL);
    CBMC_add_pawn(cloister_path,&pawn);
    printf("%d",CBMC_merge_const(start_path,&cloister_path,into_connect,from_connect)==FUNC_SUCCESS
                && start_path->construct->path.has_lake==3
                && start_path->nb_pawns==1
                && start_path->pawns[0]==pawn
                && start_path->rim->node==CBT_get_node_from_loc(start_node->node,CTL_WEST)
                && start_path->rim->next==NULL
                );


    //Test merging fields by adding cloister tile under start_tile to test if rim extension is working properly
    Carc_Tile_Node *south=CBT_get_node_from_loc(start_node->node,CTL_SOUTH),
                    *north=CBT_get_node_from_loc(cloister_node->node,CTL_NORTH);
    Carc_Macro_Construct *start_field=CBMC_get_node_construct(start_constructs,&south),
                         *cloister_field=CBMC_get_node_construct(cloister_constructs,&north);
    Carc_Tile_Node_List *start_connect=CBP_get_edge_nodes(start_node,CPCS_DOWN),
                        *cloister_connect=CBP_get_edge_nodes(cloister_node,CPCS_UP);
    int test_res=CBMC_merge_const(start_field,&cloister_field,start_connect,cloister_connect);
    printf("%d",test_res==FUNC_SUCCESS && start_field->construct==NULL
                && start_field->nb_pawns==0
                && start_field->rim->node==CBT_get_node_from_loc(start_node->node,CTL_EAST_SOUTH)
                && start_field->rim->next->node==CBT_get_node_from_loc(start_node->node,CTL_WEST_SOUTH)
                && start_field->rim->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_EAST_NORTH)
                && start_field->rim->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_EAST)
                && start_field->rim->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_EAST_SOUTH)
                && start_field->rim->next->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_SOUTH_EAST)
                && start_field->rim->next->next->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_SOUTH)
                && start_field->rim->next->next->next->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_SOUTH_WEST)
                && start_field->rim->next->next->next->next->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_WEST_SOUTH)
                && start_field->rim->next->next->next->next->next->next->next->next->next->node==CBT_get_node_from_loc(cloister_node->node,CTL_WEST_NORTH)
                && start_field->rim->next->next->next->next->next->next->next->next->next->next==NULL
                );

    //Tests merging a construct with itself
    Carc_Tile_Node *west_node=CBT_get_node_from_loc(start_node->node,CTL_WEST);
    Carc_Tile_Node_List *into_same_connect=into_connect,
                        *from_same_connect=CBTList_new(&west_node);
    Carc_Macro_Construct start_path2=*start_path,
                         *p_start_path_2=&start_path2,
                         *city=start_constructs->next->next->next->construct;

    printf("%d",CBMC_merge_const(p_start_path_2,&p_start_path_2,into_same_connect,from_same_connect)==FUNC_SUCCESS
                  && p_start_path_2->rim==NULL
           );
    into_same_connect=CBTList_new(&(city->rim->node));
    printf("%d\n",CBMC_merge_const(city,&city,into_same_connect,from_connect)==FUNC_SUCCESS
                  && city->rim->node==CBT_get_node_from_loc(start_node->node,CTL_NORTH)
                  && city->rim->next->node==CBT_get_node_from_loc(start_node->node,CTL_NORTH_EAST)
                  && city->rim->next->next==NULL
           );

    CBTList_free(into_connect);
    CBTList_free(from_connect);
    CBTList_free(into_same_connect);
    CBTList_free(from_same_connect);
    CBP_free_playboard_node(start_node);
    CPPlayer_free_player(pawn->owner);
    CPPawn_free_pawn(pawn);
    CBP_free_playboard_node(cloister_node);
    free(start_tile_path);
    free(cloister_tile_path);
    CBMCList_free(cloister_constructs);
    start_path->rim = NULL;//Avoid issue on freeing because rim already freed on the test with start_path2
    CBMCList_free(start_constructs);
}

void test_macro_const_list_rm(){
    printf("test_macro_const_list_rm results: ");
    Carc_Macro_Construct c1, c2, c3, *pc1=&c1, *pc2=&c2, *pc3=&c3, *null_const=NULL;
    Carc_Macro_Construct_List *test_list=CBMCList_new(&pc1), *test_result=NULL, *null_list=NULL;
    CBMCList_append(test_list,&pc2);
    CBMCList_append(test_list,&pc3);

    printf("%d",CBMCList_rm(NULL,&pc1)==NULL);
    printf("%d",CBMCList_rm(&null_list,&pc1)==NULL);
    printf("%d",CBMCList_rm(&test_list,NULL)==NULL);
    printf("%d",CBMCList_rm(&test_list,&null_const)==NULL);

    test_result = CBMCList_rm(&test_list,&pc2);
    printf("%d",test_result!=NULL
           && test_result->construct==pc2
           && test_result->next==NULL
           && test_list->construct==pc3
           && test_list->next->construct==pc1
           && test_list->next->next==NULL
           );
    test_result = NULL;
    CBMCList_free(test_result);

    test_result = CBMCList_rm(&test_list,&pc1);
    printf("%d",test_result!=NULL
           && test_result->construct==pc1
           && test_result->next==NULL
           && test_list->construct==pc3
           && test_list->next==NULL
           );
    test_result = NULL;
    CBMCList_free(test_result);

    test_result = CBMCList_rm(&test_list,&pc3);
    printf("%d",test_result!=NULL
           && test_result->construct==pc3
           && test_result->next==NULL
           && test_list==NULL
           );
    test_result = NULL;
    CBMCList_free(test_result);

    CBMCList_free(test_list);
}

void test_macro_const_list_is_in(){
    printf("test_macro_const_list_is_in results: ");
    Carc_Macro_Construct c1, c2, c3, *pc1=&c1, *pc2=&c2, *pc3=&c3, *null_const=NULL;
    Carc_Macro_Construct_List *test_list=CBMCList_new(&pc1);
    CBMCList_append(test_list,&pc3);

    printf("%d",CBMCList_in(NULL,NULL)==0);
    printf("%d",CBMCList_in(test_list,NULL)==0);
    printf("%d",CBMCList_in(test_list,&null_const)==0);
    printf("%d",CBMCList_in(test_list,&pc2)==0);
    printf("%d",CBMCList_in(test_list,&pc1)==1);
    printf("%d",CBMCList_in(test_list,&pc3)==1);

    //Free memory
    Carc_Macro_Construct_List *cur=test_list, *prev=NULL;;
    while(cur!=NULL){
        prev = cur;
        cur = cur->next;
        free(prev);
    }
}

void test_macro_get_tile_constructs_per_node(){
    printf("CBMC_get_tile_constructs_per_node results: ");
    char* str_start_tile=CBT_get_tile_file_path("tile1.txt");
    Carc_Tile* tile=CBT_new_tile_from_file(str_start_tile);
    Carc_Macro_Construct_List *test_list=CBMC_get_tile_macro_constructions(tile);
    //Test null inputs
    printf("%d\n",CBMC_get_tile_constructs_per_node(NULL,NULL)==NULL);
    printf("%d\n",CBMC_get_tile_constructs_per_node(NULL,tile)==NULL);
    printf("%d\n",CBMC_get_tile_constructs_per_node(test_list,NULL)==NULL);

    //Test output veracity
    Carc_Macro_Construct** test_res=CBMC_get_tile_constructs_per_node(test_list,tile);
    printf("%d",test_res[CTL_NORTH_WEST]==test_list->next->next->next->construct);
    printf("%d",test_res[CTL_NORTH]==test_list->next->next->next->construct);
    printf("%d",test_res[CTL_NORTH_EAST]==test_list->next->next->next->construct);
    printf("%d",test_res[CTL_WEST_NORTH]==test_list->next->next->construct);
    printf("%d",test_res[CTL_EAST_NORTH]==test_list->next->next->construct);
    printf("%d",test_res[CTL_WEST]==test_list->next->construct);
    printf("%d",test_res[CTL_CENTER]==test_list->next->construct);
    printf("%d",test_res[CTL_EAST]==test_list->next->construct);
    printf("%d",test_res[CTL_WEST_SOUTH]==test_list->construct);
    printf("%d",test_res[CTL_EAST_SOUTH]==test_list->construct);
    printf("%d",test_res[CTL_SOUTH_EAST]==test_list->construct);
    printf("%d",test_res[CTL_SOUTH]==test_list->construct);
    printf("%d",test_res[CTL_SOUTH_WEST]==test_list->construct);

    free(test_res);
    CBT_free_tile(tile);
    CBMCList_free(test_list);
    free(str_start_tile);
}


void test_macro_player_has_pawns_in(){
    printf("test_macro_player_has_pawns_in results: ");
    Carc_Macro_Construct c_val, *c=&c_val;
    c->nb_pawns=0;
    c->pawns=NULL;
    Carc_Player *pl1=CPPlayer_init_player(PLAYER_1,CPC_BLACK),
                *pl2=CPPlayer_init_player(PLAYER_2,CPC_BLUE);
    Carc_Pawn *pa1=CPPawn_new_pawn(pl1,PAWN_NORMAL),
              *pa2=CPPawn_new_pawn(pl1,PAWN_NORMAL),
              *pa3=CPPawn_new_pawn(pl2,PAWN_NORMAL);
    //Test null inputs
    printf("%d",CBMC_player_has_pawns_in(NULL,pl1,PAWN_NORMAL)==0);
    printf("%d",CBMC_player_has_pawns_in(c,NULL,PAWN_NORMAL)==0);

    //Test output veracity
    printf("%d",CBMC_player_has_pawns_in(c,pl1,PAWN_NORMAL)==0);
    CBMC_add_pawn(c,&pa1);
    printf("%d",CBMC_player_has_pawns_in(c,pl1,PAWN_NORMAL)==1);
    printf("%d",CBMC_player_has_pawns_in(c,pl2,PAWN_NORMAL)==0);
    printf("%d",CBMC_player_has_pawns_in(c,pl1,PAWN_ARCHITECT)==0);
    CBMC_add_pawn(c,&pa2);
    CBMC_add_pawn(c,&pa3);
    printf("%d",CBMC_player_has_pawns_in(c,pl1,PAWN_NORMAL)==2);
    printf("%d",CBMC_player_has_pawns_in(c,pl2,PAWN_NORMAL)==1);
    printf("%d",CBMC_player_has_pawns_in(c,pl1,PAWN_PIG)==0);

    CPPlayer_free_player(pl1);
    CPPlayer_free_player(pl2);
    CPPawn_free_pawn(pa1);
    CPPawn_free_pawn(pa2);
    CPPawn_free_pawn(pa3);
}

void test_macro_has_no_pawns(){
    printf("test_macro_has_no_pawns results: ");
    Carc_Macro_Construct c;
    c.nb_pawns=0;
    c.pawns=NULL;
    Carc_Pawn pawn_val, *pawn=&pawn_val;

    printf("%d",CBMC_has_no_pawns(c)==1);
    c.nb_pawns=2;
    printf("%d",CBMC_has_no_pawns(c)==1);
    c.nb_pawns=0;//To avoid bug when adding pawn on next line
    CBMC_add_pawn(&c,&pawn);
    c.nb_pawns=0;
    printf("%d",CBMC_has_no_pawns(c)==1);
    c.nb_pawns=1;
    printf("%d",CBMC_has_no_pawns(c)==0);
}


void test_macro_construct_run_all(){
    test_macro_const_construct_new();
    printf("\n************************************\n");
    test_macro_const_construct_list_new();
    printf("\n************************************\n");
    test_macro_const_construct_add_pawn();
    printf("\n************************************\n");
    test_macro_const_construct_add_node();
    printf("\n************************************\n");
    test_macro_const_node_in();
    printf("\n************************************\n");
    test_macro_const_get_node_construct();
    printf("\n************************************\n");
    test_macro_const_add_to_list();
    printf("\n************************************\n");
    test_macro_const_get_tile_constructions();
    printf("\n************************************\n");
    test_macro_const_rm_from_rim();
    printf("\n************************************\n");
    test_macro_const_rm_list_from_rim();
    printf("\n************************************\n");
    test_macro_const_transfer_rim();
    printf("\n************************************\n");
    test_macro_const_enrich_with();
    printf("\n************************************\n");
    test_macro_const_list_rm();
    printf("\n************************************\n");
    test_macro_const_merge();
    printf("\n************************************\n");
    test_macro_const_list_is_in();
    printf("\n************************************\n");
    test_macro_has_no_pawns();
    printf("\n************************************\n");
}
