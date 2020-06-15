#include "tests/board/test_macro_constructions.h"


void test_tile_macro_construct_new(){
    printf("test_tile_macro_construct_new results: ");
    Carc_Tile_Node *node=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                   *node2=CBT_new_node(CBCT_CITY,NULL),
                   *null_node=NULL;
    Carc_Macro_Construct* mc=CBMC_new(&node);
    printf("%d",CBMC_new(NULL)==NULL);
    printf("%d",CBMC_new(&null_node)==NULL);
    printf("here: %p\n",node2);
    printf("%d",CBMC_new(&node2)==NULL);
    printf("%d",mc!=NULL
                && mc->type==node->node_type
                && CBC_construction_cmp(mc->construct,mc->type,node->construction,node->node_type)==0
                && mc->nb_pawns==0 && mc->pawns==NULL
                && mc->rim!=NULL && mc->rim->node==node
           );

    CBMC_free(mc);
    CBT_free_node(node);
    CBT_free_node(node2);
}

void test_tile_macro_construct_list_new(){
    printf("test_tile_macro_construct_list_new results: ");
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

void test_tile_macro_construct_add_pawn(){
    printf("test_tile_macro_construct_add_pawn results: ");
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

void test_tile_macro_construct_add_node(){
    printf("test_tile_macro_construct_add_node results: ");
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
           && construct->construct->city.is_cathedral==1
           && construct->construct->city.nb_flags==0
           && construct->construct->city.nb_merchandises==1
           && construct->construct->city.merchandises[0]==CCM_CORN);
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

void test_tile_macro_node_in(){
    printf("test_tile_macro_node_in results: ");
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

void test_tile_macro_get_node_construct(){
    printf("test_tile_macro_get_node_construct results: ");
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

void test_tile_macro_add_to_list(){
    printf("test_tile_macro_add_to_list results: ");
    Carc_Tile_Node *n=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE)),
                    *n2=CBT_new_node(CBCT_CITY,CBC_new_city(0,0,CCM_NONE));
    Carc_Macro_Construct *c1=CBMC_new(&n),
                         *c2=CBMC_new(&n2),
                         *null_construct=NULL;
    Carc_Macro_Construct_List *l=CBMCList_new(&c1),
                              *test_res=NULL;

    printf("%d\n",CBMCList_append(NULL,NULL)==NULL);
    printf("%d\n",CBMCList_append(NULL,&null_construct)==NULL);
    test_res = CBMCList_append(NULL,&c1);
    CBMCList_free(test_res);
    printf("%d",test_res!=NULL && test_res->construct==c1 && test_res->next==NULL);
    test_res = CBMCList_append(l,&c1);
    printf("%d", test_res!=NULL && test_res->construct==c1 && test_res->next->construct==c1
           && test_res->next->next==NULL);
    test_res = CBMCList_append(l,&c2);
    printf("%d", test_res!=NULL && test_res->construct==c2 && test_res->next->construct==c1
           && test_res->next->next->construct==c1 && test_res->next->next->next==NULL);


    CBMCList_free(l);
    CBT_free_node(n);
    CBT_free_node(n2);
}

void test_tile_macro_get_tile_constructions(){
    ///TODO: add tests with Garden and Path ends when resources available
    printf("test_tile_macro_get_tile_constructions results: ");
    char *start_tile_str=CBT_get_tile_file_path("tile1.txt"),
         *cloister_tile_str=CBT_get_tile_file_path("cloister_path.txt");
    Carc_Tile *start_tile=CBT_new_tile_from_file(start_tile_str),
              *cloister_tile=CBT_new_tile_from_file(cloister_tile_str);
    Carc_Macro_Construct_List* test_res=NULL;

    printf("%d",CBMC_get_tile_macro_constructions(NULL)==NULL);
    test_res = CBMC_get_tile_macro_constructions(start_tile);
    printf("%d",test_res!=NULL && test_res->construct->type==CBCT_FIELD
           && test_res->next!=NULL && test_res->next->construct->type==CBCT_PATH
           && test_res->next->next!=NULL && test_res->next->next->construct->type==CBCT_FIELD
           && test_res->next->next->next!=NULL && test_res->next->next->next->construct->type==CBCT_CITY
           && test_res->next->next->next->next==NULL);
    CBMCList_free(test_res);

    test_res = CBMC_get_tile_macro_constructions(cloister_tile);
    printf("%d",test_res!=NULL && test_res->construct->type==CBCT_CLOISTER
           && test_res->next!=NULL && test_res->next->construct->type==CBCT_PATH
           && test_res->next->next!=NULL && test_res->next->next->construct->type==CBCT_FIELD
           && test_res->next->next->next==NULL);

    free(start_tile_str);
    free(cloister_tile_str);
    CBT_free_tile(start_tile);
    CBT_free_tile(cloister_tile);
    CBMCList_free(test_res);
}

void test_macro_construct_run_all(){
    test_tile_macro_construct_new();
    printf("\n************************************\n");
    test_tile_macro_construct_list_new();
    printf("\n************************************\n");
    test_tile_macro_construct_add_pawn();
    printf("\n************************************\n");
    test_tile_macro_construct_add_node();
    printf("\n************************************\n");
    test_tile_macro_node_in();
    printf("\n************************************\n");
    test_tile_macro_get_node_construct();
    printf("\n************************************\n");
    test_tile_macro_add_to_list();
    printf("\n************************************\n");
    test_tile_macro_get_tile_constructions();
    printf("\n************************************\n");
}
