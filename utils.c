#include "utils.h"

int positive_modulo(int n, int mod){
    if(mod<0)
        mod = -mod;
    return (((n % mod) + mod) % mod);
}

int int_center(int x, int y){
    return (x-y)/2;
}

int pointer_is_not_null(void* p, int log_error){
    ///TO_TEST
    if(p==NULL){
        if(log_error)
            fprintf(stderr,"ERROR: input pointer is null\n");
        return 0;
    }
    return 1;
}

int pointer_is_null(void* p, int log_error){
    ///TO_TEST
    return !pointer_is_not_null(p,log_error);
}

int pointer_has_not_null_value(void** p, int log_error){
    ///TO_TEST
    if(p==NULL || *p==NULL){
        if(log_error)
            fprintf(stderr,"ERROR: input pointer is null or has null value\n");
        return 0;
    }
    return 1;
}

int pointer_has_null_value(void** p, int log_error){
    return !pointer_has_not_null_value(p,log_error);
}
