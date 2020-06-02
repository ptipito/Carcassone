#include "utils.h"

int positive_modulo(int n, int mod){
    if(mod<0)
        mod = -mod;
    return (((n % mod) + mod) % mod);
}

int int_center(int x, int y){
    return (x-y)/2;
}
