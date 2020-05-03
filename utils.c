#include "utils.h"

int positive_modulo(int n, int mod){
    if(mod<0)
        mod = -mod;
    return (((n % mod) + mod) % mod);
}
