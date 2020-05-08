#include "tests/model/test_constructions.h"

void test_new_city(){
    int i=0;
    int has_flag[6]={0,0,1,1,1,1};
    int is_cathedral[6]={0,1,0,1,1,1};
    Carc_City_Merchandise merch_type[6]={CCM_BARREL, CCM_NONE, CCM_NONE, CCM_CORN, CCM_TISSUE, CCM_NONE};
    Carc_Construction *city = NULL;

    printf("test_new_city results: ");
    for(i=0;i<6;i++){
        city = CC_new_city(has_flag[i],is_cathedral[i],merch_type[i]);
        printf("%d", city->city.has_flag==has_flag[i] && city->city.is_cathedral==is_cathedral[i] && city->city.merchandise==merch_type[i]);
    }
    free(city);
}

void test_new_path(){
    int i=0;
    int has_lake[2]={0,1};
    Carc_Construction *path = NULL;

    printf("test_new_path results: ");
    for(i=0;i<2;i++){
        path = CC_new_path(has_lake[i]);
        printf("%d", path->path.has_lake==has_lake[i]);
    }
    free(path);
}

void test_new_garden(){
    Carc_Construction *garden = CC_new_garden();
    printf("test_new_garden results: %d", garden->garden.field_occupied==0 && garden->garden.garden_occupied==0);
    free(garden);
}

void test_construction_cmp(){
    int i=0 ,j=0, result;
    int has_flag[6]={0,0,1,1,1,1};
    int is_cathedral[6]={0,1,0,1,1,1};
    Carc_City_Merchandise merch_type[6]={CCM_BARREL, CCM_NONE, CCM_NONE, CCM_CORN, CCM_TISSUE, CCM_NONE};
    Carc_Construction *city1 = NULL, *city2=NULL,
            *path1 = CC_new_path(1),
            *path2 = CC_new_path(0),
            *garden1 = CC_new_garden(),
            *garden2 = CC_new_garden();

    printf("test_construction_cmp results: ");
    for(i=0;i<6;i++){
        city1 = CC_new_city(has_flag[i],is_cathedral[i],merch_type[i]);
        for(j=i;j<6;j++){
            if(i==j){
                result = (CC_construction_cmp(city1, CCT_CITY, city1, CCT_CITY) == 0);
            } else {
                city2 = CC_new_city(has_flag[j],is_cathedral[j],merch_type[j]);
                result = (CC_construction_cmp(city1, CCT_CITY, city2, CCT_CITY) == 1);
            }
            printf("%d", result);
        }
    }
    printf("%d",CC_construction_cmp(path1,CCT_PATH, city1, CCT_CITY)==1);
    printf("%d",CC_construction_cmp(path1,CCT_PATH, garden1, CCT_GARDEN)==1);
    printf("%d",CC_construction_cmp(garden1, CCT_GARDEN, city1, CCT_CITY)==1);

    printf("%d",CC_construction_cmp(garden1, CCT_GARDEN, garden2, CCT_GARDEN)==0);
    garden1->garden.field_occupied=1;
    printf("%d",CC_construction_cmp(garden1, CCT_GARDEN, garden2, CCT_GARDEN)==0);
    garden2->garden.garden_occupied=1;
    printf("%d",CC_construction_cmp(garden1, CCT_GARDEN, garden2, CCT_GARDEN)==0);
    garden1->garden.garden_occupied=1;
    garden2->garden.field_occupied=1;
    printf("%d",CC_construction_cmp(garden1, CCT_GARDEN, garden2, CCT_GARDEN)==0);

    printf("%d",CC_construction_cmp(path1, CCT_PATH, path1, CCT_PATH)==0);
    printf("%d",CC_construction_cmp(path1, CCT_PATH, path2, CCT_PATH)==1);
    printf("%d",CC_construction_cmp(path2, CCT_PATH, path2, CCT_PATH)==0);

    printf("%d",CC_construction_cmp(NULL,CCT_CLOISTER, NULL, CCT_CLOISTER)==0);
    printf("%d",CC_construction_cmp(NULL,CCT_CLOISTER, path2, CCT_PATH_END)==1);

    free(city1);
    free(city2);
    free(path1);
    free(path2);
    free(garden1);
    free(garden2);
}

void test_construction_run_all(){
    test_new_city();
    printf("\n");
    test_new_path();
    printf("\n");
    test_new_garden();
    printf("\n");
    test_construction_cmp();
}
