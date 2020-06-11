#include "tests/board/test_constructions.h"

void test_new_city(){
    int i=0;
    int nb_flags[6]={0,0,1,1,1,1};
    int is_cathedral[6]={0,1,0,1,1,1};
    int same_merch;
    Carc_City_Merchandise merch_type[6]={CCM_BARREL, CCM_NONE, CCM_NONE, CCM_CORN, CCM_TISSUE, CCM_NONE};
    Carc_Construction *city = NULL;

    printf("test_new_city results: ");
    for(i=0;i<6;i++){
        city = CBC_new_city(nb_flags[i],is_cathedral[i],merch_type[i]);
        same_merch = (city->city.merchandises==NULL);
        if(i!=1 && i!=2 && i!=5)
            same_merch = (city->city.merchandises[0]==merch_type[i]);
        printf("%d", city->city.nb_flags==nb_flags[i] && city->city.is_cathedral==is_cathedral[i] && same_merch);
    }
        CBC_free_city(city);
}

void test_new_path(){
    int i=0;
    int has_lake[2]={0,1};
    Carc_Construction *path = NULL;

    printf("test_new_path results: ");
    for(i=0;i<2;i++){
        path = CBC_new_path(has_lake[i]);
        printf("%d", path->path.has_lake==has_lake[i]);
        free(path);
    }
}

void test_new_garden(){
    Carc_Construction *garden = CBC_new_garden();
    printf("test_new_garden results: %d", garden->garden.field_occupied==0 && garden->garden.garden_occupied==0);
    free(garden);
}

void test_construction_cmp(){
    int i=0 ,j=0, result;
    int nb_flags[6]={0,0,1,1,1,1};
    int is_cathedral[6]={0,1,0,1,1,1};
    Carc_City_Merchandise merch_type[6]={CCM_BARREL, CCM_NONE, CCM_NONE, CCM_CORN, CCM_TISSUE, CCM_NONE};
    Carc_Construction *city1 = NULL, *city2=NULL,
            *path1 = CBC_new_path(1),
            *path2 = CBC_new_path(0),
            *garden1 = CBC_new_garden(),
            *garden2 = CBC_new_garden();

    printf("test_construction_cmp results: ");
    for(i=0;i<6;i++){
        city1 = CBC_new_city(nb_flags[i],is_cathedral[i],merch_type[i]);
        for(j=i;j<6;j++){
            if(i==j){
                result = (CBC_construction_cmp(city1, CBCT_CITY, city1, CBCT_CITY) == 0);
            } else {
                city2 = CBC_new_city(nb_flags[j],is_cathedral[j],merch_type[j]);
                result = (CBC_construction_cmp(city1, CBCT_CITY, city2, CBCT_CITY) == 1);
            }
            printf("%d", result);
        }
        //CBC_free_city(city1);
    }
    city1 = CBC_new_city(0,0,CCM_NONE);
    printf("%d",CBC_construction_cmp(path1,CBCT_PATH, city1, CBCT_CITY)==1);
    printf("%d",CBC_construction_cmp(path1,CBCT_PATH, garden1, CBCT_GARDEN)==1);
    printf("%d",CBC_construction_cmp(garden1, CBCT_GARDEN, city1, CBCT_CITY)==1);

    printf("%d",CBC_construction_cmp(garden1, CBCT_GARDEN, garden2, CBCT_GARDEN)==0);
    garden1->garden.field_occupied=1;
    printf("%d",CBC_construction_cmp(garden1, CBCT_GARDEN, garden2, CBCT_GARDEN)==0);
    garden2->garden.garden_occupied=1;
    printf("%d",CBC_construction_cmp(garden1, CBCT_GARDEN, garden2, CBCT_GARDEN)==0);
    garden1->garden.garden_occupied=1;
    garden2->garden.field_occupied=1;
    printf("%d",CBC_construction_cmp(garden1, CBCT_GARDEN, garden2, CBCT_GARDEN)==0);

    printf("%d",CBC_construction_cmp(path1, CBCT_PATH, path1, CBCT_PATH)==0);
    printf("%d",CBC_construction_cmp(path1, CBCT_PATH, path2, CBCT_PATH)==1);
    printf("%d",CBC_construction_cmp(path2, CBCT_PATH, path2, CBCT_PATH)==0);

    printf("%d",CBC_construction_cmp(NULL,CBCT_CLOISTER, NULL, CBCT_CLOISTER)==0);
    printf("%d",CBC_construction_cmp(NULL,CBCT_CLOISTER, path2, CBCT_PATH_END)==1);

    CBC_free_city(city1);
    CBC_free_city(city2);
    free(path1);
    free(path2);
    free(garden1);
    free(garden2);
}

void test_construction_types_connect(){
    printf("test_construction__types_connect results: ");
    int i, j, nb_success=0;
    for(i=0;i<NB_CONST_TYPES;i++){
        for(j=0;j<NB_CONST_TYPES;j++){
            nb_success += CBC_types_connect(i,j);
        }
    }
    printf("%d\n",nb_success==10);
    printf("%d\n",CBC_types_connect(NB_CONST_TYPES+1,0)==0);
    printf("%d",CBC_types_connect(0,NB_CONST_TYPES+1)==0);
}

void test_construction_add_merch(){
    printf("test_construction_add_merch results: ");
    int test_res;
    Carc_Construction* construct=CBC_new_city(0,0,CCM_NONE);
    Carc_City* city=&(construct->city);

    test_res = CBCCity_add_merchandise(NULL,CCM_NONE);
    printf("%d",test_res==-1);
    test_res = CBCCity_add_merchandise(city,CCM_NONE);
    printf("%d",test_res==-1 && city->merchandises==NULL && city->nb_merchandises==0);
    test_res = CBCCity_add_merchandise(city,CCM_CORN);
    printf("%d",test_res==0
                && city->merchandises!=NULL && city->merchandises[0]==CCM_CORN
                && city->nb_merchandises==1);
    test_res = CBCCity_add_merchandise(city,CCM_BARREL);
    printf("%d",test_res==0
                && city->merchandises!=NULL && city->merchandises[0]==CCM_CORN  && city->merchandises[1]==CCM_BARREL
                && city->nb_merchandises==2);
    test_res = CBCCity_add_merchandise(city,CCM_TISSUE);
    printf("%d",test_res==0
                && city->merchandises!=NULL && city->merchandises[0]==CCM_CORN
                && city->merchandises[1]==CCM_BARREL && city->merchandises[2]==CCM_TISSUE
                && city->nb_merchandises==3);

    CBC_free_city(construct);
}

void test_construction_city_enrich(){
    printf("test_construction_city_enrich results: ");
    Carc_Construction *src=CBC_new_city(0,0,CCM_BARREL),
                      *enrich_with=CBC_new_city(1,0,CCM_NONE);

    printf("%d",CBCCity_enrich_with(NULL,enrich_with->city)==-1);
    printf("%d",CBCCity_enrich_with(&(src->city),enrich_with->city)==0
           && src->city.is_cathedral==0 && src->city.nb_flags==1
           && src->city.nb_merchandises==1
           && src->city.merchandises[0]==CCM_BARREL);
    enrich_with->city.is_cathedral = 1;
    printf("%d",CBCCity_enrich_with(&(src->city),enrich_with->city)==0
           && src->city.is_cathedral==1 && src->city.nb_flags==2
           && src->city.nb_merchandises==1
           && src->city.merchandises[0]==CCM_BARREL);
    enrich_with->city.nb_flags = 0;
    printf("%d",CBCCity_enrich_with(&(src->city),enrich_with->city)==0
           && src->city.is_cathedral==2 && src->city.nb_flags==2
           && src->city.nb_merchandises==1
           && src->city.merchandises[0]==CCM_BARREL);
    CBCCity_add_merchandise(&(enrich_with->city),CCM_BARREL);
    printf("%d",CBCCity_enrich_with(&(src->city),enrich_with->city)==0
           && src->city.is_cathedral==3 && src->city.nb_flags==2
           && src->city.nb_merchandises==2
           && src->city.merchandises[0]==CCM_BARREL && src->city.merchandises[0]==CCM_BARREL);

    CBC_free_city(src);
    CBC_free_city(enrich_with);
}

void test_construction_path_enrich(){
    printf("test_construction_path_enrich results: ");
    Carc_Construction *src=CBC_new_path(0),
                      *enrich_with=CBC_new_path(1);

    printf("%d",CBCPath_enrich_with(NULL,enrich_with->path)==-1);
    CBCPath_enrich_with(&(src->path),enrich_with->path);
    printf("%d",src->path.has_lake==1);
    enrich_with->path.has_lake = 0;
    CBCPath_enrich_with(&(src->path),enrich_with->path);
    printf("%d",src->path.has_lake==1);
    enrich_with->path.has_lake = 5;
    CBCPath_enrich_with(&(src->path),enrich_with->path);
    printf("%d",src->path.has_lake==6);

    free(src);
    free(enrich_with);
}

void test_construction_run_all(){
    test_new_city();
    printf("\n***************************************************\n");
    test_new_path();
    printf("\n***************************************************\n");
    test_new_garden();
    printf("\n***************************************************\n");
    test_construction_cmp();
    printf("\n***************************************************\n");
    test_construction_add_merch();
    printf("\n***************************************************\n");
    test_construction_types_connect();
    printf("\n***************************************************\n");
    test_construction_city_enrich();
    printf("\n***************************************************\n");
    test_construction_path_enrich();
    printf("\n***************************************************\n");
}
