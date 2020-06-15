#ifndef DEF_CARC_UTILS
#define DEF_CARC_UTILS
#define FUNC_SUCCESS 0
#define FUNC_FAIL -1
#include <stdlib.h>
#include <stdio.h>

int positive_modulo(int, int);
int int_center(int, int);
int pointer_is_not_null(void*, int);
int pointer_has_not_null_value(void**, int);
int pointer_is_null(void*, int);
int pointer_has_null_value(void**, int);

#endif // DEF_CARC_UTILS
